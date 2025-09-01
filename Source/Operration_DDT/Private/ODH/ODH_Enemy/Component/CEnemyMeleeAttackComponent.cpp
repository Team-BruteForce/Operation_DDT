// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

UCEnemyMeleeAttackComponent::UCEnemyMeleeAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	// 기본값 설정
	bIsAttackActive = false;
	AttackCooldown = 1.0f;
	LastAttackTime = 0.0f;
	
	// 근접 공격 콜리전 생성
	MeleeAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeAttackCollision"));
	MeleeAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeleeAttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	MeleeAttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeleeAttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void UCEnemyMeleeAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// 콜리전 오버랩 이벤트 바인딩
	if (MeleeAttackCollision)
	{
		MeleeAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &UCEnemyMeleeAttackComponent::OnMeleeAttackOverlap);
	}
}

void UCEnemyMeleeAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCEnemyMeleeAttackComponent::ActivateMeleeAttack()
{
	if (MeleeAttackCollision)
	{
		MeleeAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		bIsAttackActive = true;
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("Melee Attack Activated!"));
		}
		
		// 디버그 시각화 (개발 중에만 사용)
		DrawDebugBox(GetWorld(), MeleeAttackCollision->GetComponentLocation(), MeleeAttackCollision->GetScaledBoxExtent(), 
			MeleeAttackCollision->GetComponentRotation().Quaternion(), FColor::Red, false, 0.5f);
	}
}

void UCEnemyMeleeAttackComponent::DeactivateMeleeAttack()
{
	if (MeleeAttackCollision)
	{
		MeleeAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bIsAttackActive = false;
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("Melee Attack Deactivated!"));
		}
	}
}

float UCEnemyMeleeAttackComponent::GetMeleeDamage() const
{
	// 소유자 액터에서 StatusComponent를 찾아서 공격력 반환
	AActor* Owner = GetOwner();
	if (Owner)
	{
		UCEnemyStatusComponent* StatusComponent = Owner->FindComponentByClass<UCEnemyStatusComponent>();
		if (StatusComponent)
		{
			return StatusComponent->GetAttackPower();
		}
	}
	
	// StatusComponent를 찾을 수 없는 경우 기본값 반환
	return 20.0f;
}

void UCEnemyMeleeAttackComponent::OnMeleeAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsAttackActive || !OtherActor)
		return;

	// 자기 자신은 제외
	if (OtherActor == GetOwner())
		return;

	// 쿨다운 체크
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastAttackTime < AttackCooldown)
		return;

	// 플레이어인지 확인 (Pawn 타입)
	if (OtherActor->IsA<APawn>())
	{
		// 공격 이벤트 발생
		OnMeleeAttackHit.Broadcast(OtherActor);
		
		// 쿨다운 업데이트
		LastAttackTime = CurrentTime;
		
		// 디버그 출력
		if (GEngine)
		{
			float Damage = GetMeleeDamage();
			FString DebugMessage = FString::Printf(TEXT("Melee Attack Hit: %s with %.1f damage!"), *OtherActor->GetName(), Damage);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMessage);
		}
	}
}
