// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h"
#include "ODH/ODH_AIController/CSkeletonEnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

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

	// 약점 콜리전 (WeekPointSocket)
	WeakPointCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeakPointCollision"));
	WeakPointCollision->SetupAttachment(GetMesh(), TEXT("WeekPointSocket"));
	WeakPointCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WeakPointCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	WeakPointCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeakPointCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	WeakPointCollision->SetBoxExtent(FVector(16.9f, 14.27f, 8.0f));

	// 오른손 콜리전 (Hand_R_Collision)
	MeleeAttackCollisionR = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeAttackCollisionR"));
	MeleeAttackCollisionR->SetupAttachment(GetMesh(), TEXT("Hand_R_Collision"));
	MeleeAttackCollisionR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeleeAttackCollisionR->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	MeleeAttackCollisionR->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeleeAttackCollisionR->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	MeleeAttackCollisionR->SetBoxExtent(FVector(20.0f, 5.0f, 7.0f));

	// 왼손 콜리전 (Hand_L_Collision)
	MeleeAttackCollisionL = CreateDefaultSubobject<UBoxComponent>(TEXT("MeleeAttackCollisionL"));
	MeleeAttackCollisionL->SetupAttachment(GetMesh(), TEXT("Hand_L_Collision"));
	MeleeAttackCollisionL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeleeAttackCollisionL->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	MeleeAttackCollisionL->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	MeleeAttackCollisionL->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	MeleeAttackCollisionL->SetBoxExtent(FVector(20.0f, 5.0f, 7.0f));

	// 콤보 공격 마지막 콜리전
	ComboAttackLastCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ComboAttackLastCollision"));
	ComboAttackLastCollision->SetupAttachment(GetMesh()); // 메쉬의 직접 자식으로 설정
	ComboAttackLastCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ComboAttackLastCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	ComboAttackLastCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ComboAttackLastCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ComboAttackLastCollision->SetBoxExtent(FVector(25.0f, 8.0f, 10.0f));
	ComboAttackLastCollision->SetRelativeLocation(FVector::ZeroVector); // 메쉬 중심에 위치
	ComboAttackLastCollision->ComponentTags.Add(TEXT("ComboLast"));

	// 바디 대미지용 L 소켓들
	UpperLTakeDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("UpperLTakeDamageCollision"));
	UpperLTakeDamageCollision->SetupAttachment(GetMesh(), TEXT("UpperLTakeDamageSocket"));
	UpperLTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UpperLTakeDamageCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	UpperLTakeDamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	UpperLTakeDamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	UpperLTakeDamageCollision->SetBoxExtent(FVector(17.9f, 4.7f, 4.7f));
	UpperLTakeDamageCollision->SetRelativeLocation(FVector(12,0,0));

	LowerLTakeDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LowerLTakeDamageCollision"));
	LowerLTakeDamageCollision->SetupAttachment(GetMesh(), TEXT("LowerLTakeDamageSocket"));
	LowerLTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LowerLTakeDamageCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	LowerLTakeDamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LowerLTakeDamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	LowerLTakeDamageCollision->SetBoxExtent(FVector(16.6f, 6.0f, 6.0f));
	LowerLTakeDamageCollision->SetRelativeLocation(FVector(4,0,0));

	HandLTakeDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HandLTakeDamageCollision"));
	HandLTakeDamageCollision->SetupAttachment(GetMesh(), TEXT("HandLTakeDamageSocket"));
	HandLTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HandLTakeDamageCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	HandLTakeDamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HandLTakeDamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	HandLTakeDamageCollision->SetBoxExtent(FVector(10.0f, 4.1f, 7.5f));
	HandLTakeDamageCollision->SetRelativeLocation(FVector(10,0,0));

	CalfLTakeDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CalfLTakeDamageCollision"));
	CalfLTakeDamageCollision->SetupAttachment(GetMesh(), TEXT("CalfLTakeDamageSocket"));
	CalfLTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CalfLTakeDamageCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CalfLTakeDamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CalfLTakeDamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	CalfLTakeDamageCollision->SetBoxExtent(FVector(22.2f, 4.3f, 4.2f));
	CalfLTakeDamageCollision->SetRelativeLocation(FVector(-22,0,0));

	FootLTakeDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("FootLTakeDamageCollision"));
	FootLTakeDamageCollision->SetupAttachment(GetMesh(), TEXT("FootLTakeDamageSocket"));
	FootLTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FootLTakeDamageCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	FootLTakeDamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	FootLTakeDamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	FootLTakeDamageCollision->SetBoxExtent(FVector(4.4f, 12.8f, 5.1f));
	FootLTakeDamageCollision->SetRelativeLocation(FVector(-3,9,0));

	// 바디 대미지용 R 소켓들
	UpperRTakeDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("UpperRTakeDamageCollision"));
	UpperRTakeDamageCollision->SetupAttachment(GetMesh(), TEXT("UpperRTakeDamageSocket"));
	UpperRTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UpperRTakeDamageCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	UpperRTakeDamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	UpperRTakeDamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	UpperRTakeDamageCollision->SetBoxExtent(FVector(17.9f, 4.7f, 4.7f));
	UpperRTakeDamageCollision->SetRelativeLocation(FVector(-13,0,0));

	LowerRTakeDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("LowerRTakeDamageCollision"));
	LowerRTakeDamageCollision->SetupAttachment(GetMesh(), TEXT("LowerRTakeDamageSocket"));
	LowerRTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	LowerRTakeDamageCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	LowerRTakeDamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	LowerRTakeDamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	LowerRTakeDamageCollision->SetBoxExtent(FVector(16.6f, 6.0f, 6.0f));
	LowerRTakeDamageCollision->SetRelativeLocation(FVector(-15,0,0));

	HandRTakeDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HandRTakeDamageCollision"));
	HandRTakeDamageCollision->SetupAttachment(GetMesh(), TEXT("HandRTakeDamageSocket"));
	HandRTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HandRTakeDamageCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	HandRTakeDamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HandRTakeDamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	HandRTakeDamageCollision->SetBoxExtent(FVector(9.1f, 5.0f, 7.7f));
	HandRTakeDamageCollision->SetRelativeLocation(FVector(-8,0,0));

	CalfRTakeDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CalfRTakeDamageCollision"));
	CalfRTakeDamageCollision->SetupAttachment(GetMesh(), TEXT("CalfRTakeDamageSocket"));
	CalfRTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CalfRTakeDamageCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	CalfRTakeDamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CalfRTakeDamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	CalfRTakeDamageCollision->SetBoxExtent(FVector(20.9f, 4.5f, 3.9f));
	CalfRTakeDamageCollision->SetRelativeLocation(FVector(23,0,0));

	FootRTakeDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("FootRTakeDamageCollision"));
	FootRTakeDamageCollision->SetupAttachment(GetMesh(), TEXT("FootRTakeDamageSocket"));
	FootRTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FootRTakeDamageCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	FootRTakeDamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	FootRTakeDamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	FootRTakeDamageCollision->SetBoxExtent(FVector(8.0f, 5.0f, 4.0f));

	// 중앙부 추가: Pelvis / Bust
	PelvisTakeDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("PelvisTakeDamageCollision"));
	PelvisTakeDamageCollision->SetupAttachment(GetMesh(), TEXT("PelvisTakeDamageSocket"));
	PelvisTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PelvisTakeDamageCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	PelvisTakeDamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	PelvisTakeDamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	PelvisTakeDamageCollision->SetBoxExtent(FVector(15.0f, 6.4f, 15.6f));

	BustTakeDamageCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BustTakeDamageCollision"));
	BustTakeDamageCollision->SetupAttachment(GetMesh(), TEXT("BustTakeDamageSocket"));
	BustTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BustTakeDamageCollision->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	BustTakeDamageCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BustTakeDamageCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	BustTakeDamageCollision->SetBoxExtent(FVector(25.2f, 10.8f, 11.7f));
	BustTakeDamageCollision->SetRelativeLocation(FVector(0,3,0));
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

	// 소켓 콜리전 오버랩 이벤트 바인딩
	if (WeakPointCollision)
	{
		WeakPointCollision->OnComponentBeginOverlap.AddDynamic(this, &ACSkeletonEnemy::OnWeakPointOverlap);
	}
	if (MeleeAttackCollisionR)
	{
		MeleeAttackCollisionR->OnComponentBeginOverlap.AddDynamic(this, &ACSkeletonEnemy::OnMeleeAttackOverlap);
	}
	if (MeleeAttackCollisionL)
	{
		MeleeAttackCollisionL->OnComponentBeginOverlap.AddDynamic(this, &ACSkeletonEnemy::OnMeleeAttackOverlap);
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

	// 전투 중 피격 시 NoHitTime 초기화
	if (AController* C = GetController())
	{
		if (AAIController* AI = Cast<AAIController>(C))
		{
			if (UBlackboardComponent* BB = AI->GetBlackboardComponent())
			{
				const FName KeyIsInCombat = TEXT("IsInCombat");
				const FName KeyNoHitTime = TEXT("NoHitTime");
				const bool bInCombat = BB->GetValueAsBool(KeyIsInCombat);
				if (bInCombat)
				{
					BB->SetValueAsFloat(KeyNoHitTime, 0.0f);
				}
			}
		}
	}

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
void ACSkeletonEnemy::PlayComboAttack()
{
	if (MeleeAttackComponent)
	{
		MeleeAttackComponent->ActivateMeleeAttack();
		bIsMeleeAttacking = true;
		
		// 콤보 공격 쿨다운 설정 (빠른 연속 공격)
		AttackCooldown = 0.3f;
		
		// 기존 타이머 클리어 후 재설정
		GetWorldTimerManager().ClearTimer(MeleeAttackTimerHandle);
		FTimerDelegate ClearMelee;
		ClearMelee.BindLambda([this]()
		{
			bIsMeleeAttacking = false;
		});
		GetWorldTimerManager().SetTimer(MeleeAttackTimerHandle, ClearMelee, 0.6f, false);
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Skeleton Enemy Combo Attack!"));
		}

		// 콜리전 활성화/비활성화는 애니메이션 노티파이로 처리
	}
}

void ACSkeletonEnemy::PlayDashAttack()
{
	if (MeleeAttackComponent)
	{
		MeleeAttackComponent->ActivateMeleeAttack();
		bIsMeleeAttacking = true;
		
		// 돌진 공격 쿨다운 설정 (강력한 단발 공격)
		AttackCooldown = 0.6f;
		
		// 기존 타이머 클리어 후 재설정
		GetWorldTimerManager().ClearTimer(MeleeAttackTimerHandle);
		FTimerDelegate ClearMelee;
		ClearMelee.BindLambda([this]()
		{
			bIsMeleeAttacking = false;
		});
		GetWorldTimerManager().SetTimer(MeleeAttackTimerHandle, ClearMelee, 0.6f, false);
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Skeleton Enemy Dash Attack!"));
		}

		// 콜리전 활성화/비활성화는 애니메이션 노티파이로 처리
	}
}

void ACSkeletonEnemy::EnableComboCollisions()
{
	if (MeleeAttackCollisionR)
	{
		MeleeAttackCollisionR->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	if (MeleeAttackCollisionL)
	{
		MeleeAttackCollisionL->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	
	// 디버그 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Combo Collisions Enabled"));
	}
}

void ACSkeletonEnemy::DisableComboCollisions()
{
	if (MeleeAttackCollisionR)
	{
		MeleeAttackCollisionR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (MeleeAttackCollisionL)
	{
		MeleeAttackCollisionL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (ComboAttackLastCollision)
	{
		ComboAttackLastCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	// 쿨다운 초기화 (다음 공격을 위해)
	LastHitTime = 0.0f;
	
	// 디버그 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Combo Collisions Disabled - Cooldown Reset"));
	}
}

void ACSkeletonEnemy::DisableAllCollisions()
{
	// 약점 콜리전 비활성화
	if (WeakPointCollision)
	{
		WeakPointCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	// 공격 콜리전 비활성화
	if (MeleeAttackCollisionR)
	{
		MeleeAttackCollisionR->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (MeleeAttackCollisionL)
	{
		MeleeAttackCollisionL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (ComboAttackLastCollision)
	{
		ComboAttackLastCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	// 모든 피격 콜리전 비활성화
	if (UpperLTakeDamageCollision)
	{
		UpperLTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (LowerLTakeDamageCollision)
	{
		LowerLTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (HandLTakeDamageCollision)
	{
		HandLTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (CalfLTakeDamageCollision)
	{
		CalfLTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (FootLTakeDamageCollision)
	{
		FootLTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	if (UpperRTakeDamageCollision)
	{
		UpperRTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (LowerRTakeDamageCollision)
	{
		LowerRTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (HandRTakeDamageCollision)
	{
		HandRTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (CalfRTakeDamageCollision)
	{
		CalfRTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (FootRTakeDamageCollision)
	{
		FootRTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	if (PelvisTakeDamageCollision)
	{
		PelvisTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (BustTakeDamageCollision)
	{
		BustTakeDamageCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	// 디버그 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("All Collisions Disabled"));
	}
}

void ACSkeletonEnemy::EnableLastComboCollision()
{
	if (ComboAttackLastCollision)
	{
		ComboAttackLastCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	
	// 디버그 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Magenta, TEXT("Last Combo Collision Enabled"));
	}
}


void ACSkeletonEnemy::OnDeath()
{
	// 사망 시 처리 로직
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Skeleton Enemy Died! Will be removed in 3 seconds..."));
	}

	// 모든 콜리전 비활성화 (사망 시 충돌 방지)
	DisableAllCollisions();

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

	// 쿨다운 체크
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastHitTime < AttackCooldown)
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
		
		// 쿨다운 업데이트
		LastHitTime = CurrentTime;
		
		// 디버그 출력
		if (GEngine)
		{
			FString DebugMessage = FString::Printf(TEXT("Skeleton Enemy Melee Attack Overlap: %s with %.1f damage!"), *OtherActor->GetName(), Damage);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMessage);
		}
	}
}

void ACSkeletonEnemy::OnWeakPointOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this)
		return;

	// 디버그 출력: 약점 오버랩 감지
	if (GEngine)
	{
		const FString OtherName = OtherActor ? OtherActor->GetName() : TEXT("None");
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Cyan,
			FString::Printf(TEXT("[WeakPoint Overlap] by: %s"), *OtherName)
		);
	}

	// 플레이어 투사체/무기 등으로부터의 데미지를 2배로 적용
	// 기존 방식: UGameplayStatics::ApplyDamage 사용
	if (StatusComponent && !StatusComponent->IsDead())
	{
		float BaseDamage = StatusComponent->GetAttackPower();
		float DamageToApply = BaseDamage * 2.0f;
		AController* InstigatorController = GetInstigatorController();
		UGameplayStatics::ApplyDamage(this, DamageToApply, InstigatorController, OtherActor, nullptr);
	}
}

// 이동/낙하 연출 관련 로직은 애니메이션으로 대체
