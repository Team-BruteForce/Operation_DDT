// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h"
#include "ODH/ODH_AIController/CSkeletonEnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BrainComponent.h"

// Sets default values
ACSkeletonEnemy::ACSkeletonEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// AI 컨트롤러 클래스 설정
	AIControllerClass = ACSkeletonEnemyAIController::StaticClass();

	// 스테이터스 컴포넌트 생성
	StatusComponent = CreateDefaultSubobject<UCEnemyStatusComponent>(TEXT("StatusComponent"));
	
	// 근접 공격 컴포넌트 생성
	MeleeAttackComponent = CreateDefaultSubobject<UCEnemyMeleeAttackComponent>(TEXT("MeleeAttackComponent"));

	// 근접 공격 콜리전 생성 (Mesh의 자식으로 설정)
	MeleeAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeAttackCollision"));
	MeleeAttackCollision->SetupAttachment(GetMesh());
	MeleeAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeleeAttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	MeleeAttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeleeAttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
	// 콜리전 위치 및 크기 설정 (Mesh 기준 상대 위치)
	MeleeAttackCollision->SetRelativeLocation(FVector(50.0f, 0.0f, 0.0f)); // Mesh 앞쪽
	MeleeAttackCollision->SetRelativeRotation(FRotator::ZeroRotator);
	MeleeAttackCollision->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f)); // 공격 범위
}

// Called when the game starts or when spawned
void ACSkeletonEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// 타이머 핸들 초기화
	DeathTimerHandle.Invalidate();
	
	// Skeleton Enemy 전용 스테이터스 설정
	if (StatusComponent)
	{
		// Skeleton Enemy는 체력이 높고 방어력이 높음
		StatusComponent->SetMaxHealth(120.0f);
		StatusComponent->SetCurrentHealth(120.0f);
		StatusComponent->SetAttackPower(25.0f);
		StatusComponent->SetDefensePower(8.0f);
		
		// 사망 이벤트 바인딩
		StatusComponent->OnDeath.AddDynamic(this, &ACSkeletonEnemy::OnDeath);
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, 
				FString::Printf(TEXT("Skeleton Enemy Spawned - Health: %.0f, Attack: %.0f"), 
				StatusComponent->GetHealthPercent() * 100, StatusComponent->GetAttackPower()));
		}
	}
	
	// 근접 공격 컴포넌트의 히트 이벤트에 바인딩
	if (MeleeAttackComponent)
	{
		MeleeAttackComponent->OnMeleeAttackHit.AddDynamic(this, &ACSkeletonEnemy::OnMeleeAttackHit);
	}

	// 근접 공격 콜리전 오버랩 이벤트 바인딩
	if (MeleeAttackCollision)
	{
		MeleeAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ACSkeletonEnemy::OnMeleeAttackOverlap);
	}

	// 낙하/회전 연출은 사용하지 않음
}

void ACSkeletonEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 타이머 정리
	if (GetWorldTimerManager().IsTimerActive(DeathTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(DeathTimerHandle);
	}
	
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ACSkeletonEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 애니메이션에서 처리하므로 코드 기반 이동 연출 업데이트 제거
}

// Called to bind functionality to input
void ACSkeletonEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// IDamageable 인터페이스 구현
void ACSkeletonEnemy::TakeDamage_Implementation(float DamageAmount)
{
	if (StatusComponent)
	{
		StatusComponent->TakeDamage(DamageAmount);
	}
}

bool ACSkeletonEnemy::CanTakeDamage_Implementation() const
{
	if (StatusComponent)
	{
		return !StatusComponent->IsDead();
	}
	return false;
}

bool ACSkeletonEnemy::IsDead_Implementation() const
{
	if (StatusComponent)
	{
		return StatusComponent->IsDead();
	}
	return false;
}

// UGameplayStatics::ApplyDamage를 위한 TakeDamage 오버라이드
float ACSkeletonEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// 이미 사망한 경우 데미지를 받지 않음
	if (StatusComponent && StatusComponent->IsDead())
	{
		return 0.0f;
	}

	// IDamageable 인터페이스의 TakeDamage_Implementation 호출
	TakeDamage_Implementation(DamageAmount);

	// 디버그 출력
	if (GEngine)
	{
		float CurrentHealth = StatusComponent ? StatusComponent->GetCurrentHealth() : 0.0f;
		float HealthPercent = StatusComponent ? StatusComponent->GetHealthPercent() * 100.0f : 0.0f;
		
		FString DebugMessage = FString::Printf(TEXT("Skeleton Enemy took %.1f damage from %s! Health: %.1f (%.1f%%)"), 
			DamageAmount, DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"), CurrentHealth, HealthPercent);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, DebugMessage);
	}

	return DamageAmount;
}

// IGenericTeamAgentInterface 구현
FGenericTeamId ACSkeletonEnemy::GetGenericTeamId() const
{
	return FGenericTeamId(1); // Team ID 1 (적 팀)
}

// 공격 함수들
void ACSkeletonEnemy::PlayMeleeAttack()
{
	if (MeleeAttackComponent)
	{
		MeleeAttackComponent->ActivateMeleeAttack();
		bIsMeleeAttacking = true;
		
		// 기존 타이머 클리어 후 재설정
		GetWorldTimerManager().ClearTimer(MeleeAttackTimerHandle);
		FTimerDelegate ClearMelee;
		ClearMelee.BindLambda([this]()
		{
			bIsMeleeAttacking = false;
			
			// 근접 공격 콜리전 비활성화
			if (MeleeAttackCollision)
			{
				MeleeAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		});
		GetWorldTimerManager().SetTimer(MeleeAttackTimerHandle, ClearMelee, 0.6f, false);
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Skeleton Enemy Melee Attack!"));
		}

		// 근접 공격 콜리전 활성화
		if (MeleeAttackCollision)
		{
			MeleeAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}

		// 이동 연출은 애니메이션에서 처리
	}
}

void ACSkeletonEnemy::OnDeath()
{
	// 사망 시 처리 로직
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Skeleton Enemy Died! Will be removed in 3 seconds..."));
	}

	// 이동/AI 즉시 정지
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->DisableMovement();
		MoveComp->SetMovementMode(MOVE_None);
	}
	if (AController* C = GetController())
	{
		if (AAIController* AI = Cast<AAIController>(C))
		{
			AI->StopMovement();
			if (UBrainComponent* Brain = AI->GetBrainComponent())
			{
				Brain->StopLogic(TEXT("Dead"));
			}
		}
	}
	
	// 기존 타이머가 있다면 클리어
	GetWorldTimerManager().ClearTimer(DeathTimerHandle);
	
	FTimerDelegate DestroySelf;
	DestroySelf.BindLambda([this]()
	{
		if (IsValid(this))
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Skeleton Enemy removed from game!"));
			}
			Destroy();
		}
	});
	GetWorldTimerManager().SetTimer(DeathTimerHandle, DestroySelf, 3.0f, false);
	
	// 사망 애니메이션 재생, 사망 효과 등 추가 가능
	// 예: PlayDeathAnimation();

	// 진행 중이던 시각 이동 종료 및 원복 (애니메이션 처리로 불필요)
}

void ACSkeletonEnemy::OnMeleeAttackHit(AActor* HitActor)
{
	// 근접 공격이 플레이어에게 히트했을 때의 처리
	if (HitActor && HitActor->IsA<APawn>())
	{
		if (GEngine)
		{
			float Damage = MeleeAttackComponent->GetMeleeDamage();
			FString DebugMessage = FString::Printf(TEXT("Skeleton Enemy Melee Attack Hit: %s with %.1f damage!"), *HitActor->GetName(), Damage);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, DebugMessage);
		}
		
		// 여기에 플레이어에게 데미지를 주는 로직 추가
		// 예: HitActor->TakeDamage(MeleeAttackComponent->GetMeleeDamage());
	}
}

void ACSkeletonEnemy::OnMeleeAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!MeleeAttackComponent || !MeleeAttackComponent->IsAttackActive() || !OtherActor)
		return;

	// 자기 자신은 제외
	if (OtherActor == this)
		return;

	// 플레이어인지 확인
	if (OtherActor->IsA<APawn>())
	{
		// 공격 이벤트 발생
		MeleeAttackComponent->OnMeleeAttackHit.Broadcast(OtherActor);

		// 실제 데미지 적용
		float Damage = MeleeAttackComponent->GetMeleeDamage();
		AController* InstigatorController = GetInstigatorController();
		UGameplayStatics::ApplyDamage(OtherActor, Damage, InstigatorController, this, nullptr);
		
		// 디버그 출력
		if (GEngine)
		{
			FString DebugMessage = FString::Printf(TEXT("Skeleton Enemy Melee Attack Overlap: %s with %.1f damage!"), *OtherActor->GetName(), Damage);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMessage);
		}
	}
}

// 이동/낙하 연출 관련 로직은 애니메이션으로 대체
