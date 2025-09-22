// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemy.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h"
#include "ODH/Component/CEnemyProjectileComp.h"
#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemyRangedATKManager.h"
#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerHitEffectPoolManager.h"
#include "ODH/ODH_Enemy/Projectile/CFlowerProjectile.h"
#include "ODH/ODH_AIController/CFlowerEnemyAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "Player/CPlayerBullet.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyHealthBarComponent.h"
#include "Player/DDTPlayer.h"
#include "../../UMG/Public/Components/WidgetComponent.h"

// Sets default values
ACFlowerEnemy::ACFlowerEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// AI 컨트롤러 클래스 설정
	AIControllerClass = ACFlowerEnemyAIController::StaticClass();

	// 스테이터스 컴포넌트 생성
	StatusComponent = CreateDefaultSubobject<UCEnemyStatusComponent>(TEXT("StatusComponent"));
	
	// 근접 공격 컴포넌트 생성
	MeleeAttackComponent = CreateDefaultSubobject<UCEnemyMeleeAttackComponent>(TEXT("MeleeAttackComponent"));


	// 애로우 컴포넌트 생성 (원거리 공격 발사 위치)
	ProjectileSpawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnArrow"));
	ProjectileSpawnArrow->SetupAttachment(RootComponent);
	ProjectileSpawnArrow->SetArrowColor(FLinearColor::Green);
	ProjectileSpawnArrow->SetArrowLength(50.0f);
	ProjectileSpawnArrow->SetArrowSize(2.0f);
	// 기본 위치는 전방 80cm (블루프린트에서 자유롭게 변경 가능)
	ProjectileSpawnArrow->SetRelativeLocation(FVector(80.0f, 0.0f, 0.0f));

	CreateDamageCollisions();
	BindDamageCollisionEvents();
}

// Called when the game starts or when spawned
void ACFlowerEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// 타이머 핸들 초기화
	DeathTimerHandle.Invalidate();
	
	// Flower Enemy 전용 스테이터스 설정
	if (StatusComponent)
	{
		// Flower Enemy는 중간 체력과 공격력을 가짐
		StatusComponent->SetMaxHealth(MaxHP);
		StatusComponent->SetCurrentHealth(MaxHP);
		StatusComponent->SetAttackPower(25.0f);
		StatusComponent->SetDefensePower(0.0f);
		
		// 사망 이벤트 바인딩
		StatusComponent->OnDeath.AddDynamic(this, &ACFlowerEnemy::OnDeath);
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, 
				FString::Printf(TEXT("Flower Enemy Spawned - Health: %.0f, Attack: %.0f"), 
				StatusComponent->GetHealthPercent() * 100, StatusComponent->GetAttackPower()));
		}
	}
	
	// 근접 공격 컴포넌트의 히트 이벤트에 바인딩
	if (MeleeAttackComponent)
	{
		MeleeAttackComponent->OnMeleeAttackHit.AddDynamic(this, &ACFlowerEnemy::OnMeleeAttackHit);
	}

	// 낙하 타임라인 델리게이트 바인딩 (커브가 있는 경우에만 재생됨)
	if (FallCurve)
	{
		FOnTimelineFloat UpdateDelegate;
		UpdateDelegate.BindUFunction(this, FName("OnFallTimelineUpdate"));
		FallTimeline.AddInterpFloat(FallCurve, UpdateDelegate);

		FOnTimelineEvent FinishedDelegate;
		FinishedDelegate.BindUFunction(this, FName("OnFallTimelineFinished"));
		FallTimeline.SetTimelineFinishedFunc(FinishedDelegate);

		// 커브 X축이 0..1이라고 가정하고 Duration에 맞추어 재생 속도를 설정
		if (FallDuration > 0.0f)
		{
			FallTimeline.SetPlayRate(1.0f / FallDuration);
		}
	}

	// 헤드 콜리전 초기 상태 동기화
	UpdateHeadCollisions();

	// 자동 머리 열기 시스템 시작
	StartAutoHeadOpening();

	//공격 콜리전 비활성화
	DisableDashAttackCollision();
	DisableHandLAttackCollision();
	DisableHandRAttackCollision();
}

void ACFlowerEnemy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 타이머 정리
	if (GetWorldTimerManager().IsTimerActive(DeathTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(DeathTimerHandle);
	}
	
	if (GetWorldTimerManager().IsTimerActive(HeadCloseTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(HeadCloseTimerHandle);
	}
	if (GetWorldTimerManager().IsTimerActive(HeadAutoOpenTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(HeadAutoOpenTimerHandle);
	}
	
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ACFlowerEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsFalling)
	{
		if (FallCurve)
		{
			FallTimeline.TickTimeline(DeltaTime);
		}
		else
		{
			UpdateFallingAnimation(DeltaTime);
		}
	}
}

// Called to bind functionality to input
void ACFlowerEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

// IDamageable 인터페이스 구현
void ACFlowerEnemy::TakeDamage_Implementation(float DamageAmount)
{
	if (StatusComponent)
	{
		StatusComponent->TakeDamage(DamageAmount);
	}
}

bool ACFlowerEnemy::CanTakeDamage_Implementation() const
{
	if (StatusComponent)
	{
		return !StatusComponent->IsDead();
	}
	return false;
}

bool ACFlowerEnemy::IsDead_Implementation() const
{
	if (StatusComponent)
	{
		return StatusComponent->IsDead();
	}
	return false;
}

// UGameplayStatics::ApplyDamage를 위한 TakeDamage 오버라이드
float ACFlowerEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	// 이미 사망한 경우 데미지를 받지 않음
	if (StatusComponent && StatusComponent->IsDead())
	{
		return 0.0f;
	}

	// IDamageable 인터페이스의 TakeDamage_Implementation 호출
	TakeDamage_Implementation(DamageAmount);

	if (UWidgetComponent* WC = Cast<UWidgetComponent>(
		GetComponentByClass(UWidgetComponent::StaticClass())))
	{
		const bool bShown = WC->IsVisible(); // 월드 컴포넌트 가시성
		if (!bShown)
		{
			if (UCEnemyHealthBarComponent* HB = FindComponentByClass<UCEnemyHealthBarComponent>())
			{
				HB->ShowHealthBar();
			}
		}
	}

	// 약점 본 피격 즉시 그로기 100 누적
	bool bWeakSpotHit = false;
	if (const FPointDamageEvent* PointEvt = static_cast<const FPointDamageEvent*>(DamageEvent.GetTypeID() == FPointDamageEvent::ClassID ? &DamageEvent : nullptr))
	{
		const FName HitBone = PointEvt->HitInfo.BoneName;
		if (bIsHeadOpen && (HitBone == "head" || HitBone == "Head" || HitBone == "head_01" || HitBone == "Head_01" || HitBone == "skull" || HitBone == "Skull"))
		{
			bWeakSpotHit = true;

			// 디버그 출력
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, 
				FString::Printf(TEXT("Flower Enemy Weak Spot Hit")));
			}
		}
	}

	if (bWeakSpotHit)
	{
		GroggyGage += 100;
	}
	else
	{
		// 플라워는 3~4대: 평균 33씩 가정
		GroggyGage += 34;
	}

	if (GroggyGage >= 100)
	{
		bIsHitState = true;
		GroggyGage = 0; // 임계 도달 시 초기화(원치 않으면 제거)
		if (UCharacterMovementComponent* Move = GetCharacterMovement())
		{
			SavedMovementMode = Move->MovementMode;
			SavedCustomMovementMode = Move->CustomMovementMode;
			Move->DisableMovement();
		}
	}

	// 전투 비진입 상태에서 피격 시 타겟 및 전투 상태 설정
	if (AController* OwnerController = Cast<AController>(GetController()))
	{
		if (AAIController* AI = Cast<AAIController>(OwnerController))
		{
			if (UBlackboardComponent* BB = AI->GetBlackboardComponent())
			{
				const FName KeyIsInCombat = TEXT("IsInCombat");
				const FName KeyTargetPlayer = TEXT("TargetPlayer");
				const bool bInCombat = BB->GetValueAsBool(KeyIsInCombat);
				if (!bInCombat)
				{
					UObject* TargetObj = nullptr;
					if (EventInstigator)
					{
						APawn* InstigatorPawn = EventInstigator->GetPawn();
						TargetObj = InstigatorPawn ? static_cast<UObject*>(InstigatorPawn) : static_cast<UObject*>(EventInstigator);
					}
					if (TargetObj)
					{
                        BB->SetValueAsObject(KeyTargetPlayer, TargetObj);
                        BB->SetValueAsBool(KeyIsInCombat, true);
                        // 전투 돌입: 체력바 표시
//                         if (UCEnemyHealthBarComponent* HB = FindComponentByClass<UCEnemyHealthBarComponent>())
//                         {
//                             HB->ShowHealthBar();
//                         }
					}
				}
			}
		}
	}

	// 블랙보드에 IsRangedAttackGage 값 추가 (25~50 랜덤)
	ACFlowerEnemyAIController* FlowerAIController = Cast<ACFlowerEnemyAIController>(GetController());
	if (FlowerAIController)
	{
		UBlackboardComponent* BlackboardComp = FlowerAIController->GetBlackboardComponent();
		if (BlackboardComp)
		{
			// 현재 게이지 값 가져오기
			int32 CurrentGage = BlackboardComp->GetValueAsInt("IsRangedAttackGage");
			
			// 25~50 사이의 랜덤 값 생성
			int32 RandomValue = FMath::RandRange(25, 50);
			int32 NewGage = CurrentGage + RandomValue;
			
			// 블랙보드에 새로운 값 설정
			BlackboardComp->SetValueAsInt("IsRangedAttackGage", NewGage);
			
			// 디버그 출력
			if (GEngine)
			{
				FString DebugMessage = FString::Printf(TEXT("Flower Enemy Ranged Attack Gage: %d (+%d) = %d"), 
					CurrentGage, RandomValue, NewGage);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, DebugMessage);
			}
		}
	}

	// 디버그 출력
	if (GEngine)
	{
		float CurrentHealth = StatusComponent ? StatusComponent->GetCurrentHealth() : 0.0f;
		float HealthPercent = StatusComponent ? StatusComponent->GetHealthPercent() * 100.0f : 0.0f;
		
		FString DebugMessage = FString::Printf(TEXT("Flower Enemy took %.1f damage from %s! Health: %.1f (%.1f%%)"), 
			DamageAmount, DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"), CurrentHealth, HealthPercent);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, DebugMessage);
	}

	return DamageAmount;
}

void ACFlowerEnemy::EndHitState()
{
	bIsHitState = false;
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->SetMovementMode(static_cast<EMovementMode>(SavedMovementMode), SavedCustomMovementMode);
	}
}

// IGenericTeamAgentInterface 구현
FGenericTeamId ACFlowerEnemy::GetGenericTeamId() const
{
	return FGenericTeamId(1); // Team ID 1 (적 팀)
}

// 공격 함수들
void ACFlowerEnemy::PlayRangedAttack()
{
	// 원거리 공격 로직
	bIsRangedAttacking = true;
	bIsComboAttacking = false; // 다른 공격 상태 초기화
	bIsDashAttacking = false; // 다른 공격 상태 초기화
	
	// 다른 공격 콜리전들 비활성화
	DisableHandLAttackCollision();
	DisableHandRAttackCollision();
	DisableDashAttackCollision();
	
	// 원거리 공격 시 머리 열기
	SetHeadOpen(true);
	
	// 기존 머리 닫기 타이머가 있다면 클리어
	GetWorldTimerManager().ClearTimer(HeadCloseTimerHandle);
	
	// 3초 후 머리 자동 닫기
	GetWorldTimerManager().SetTimer(HeadCloseTimerHandle, this, &ACFlowerEnemy::CloseHeadAutomatically, 3.0f, false);
	
	// 디버그 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Flower Enemy Ranged Attack!"));
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Flower Enemy Head (Ranged) will close in 3.0 seconds"));
	}
}

void ACFlowerEnemy::PlayComboAttack()
{
	if (MeleeAttackComponent)
	{
		MeleeAttackComponent->ActivateMeleeAttack();
		bIsComboAttacking = true;
		bIsDashAttacking = false; // 다른 공격 상태 초기화
		bIsRangedAttacking = false; // 다른 공격 상태 초기화
		
		// 다른 공격 콜리전들 비활성화
		DisableHandLAttackCollision();
		DisableHandRAttackCollision();
		DisableDashAttackCollision();
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Flower Enemy Combo Attack!"));
		}
	}
}

void ACFlowerEnemy::PlayDashAttack()
{
	if (MeleeAttackComponent)
	{
		MeleeAttackComponent->ActivateMeleeAttack();
		bIsDashAttacking = true;
		bIsComboAttacking = false; // 다른 공격 상태 초기화
		bIsRangedAttacking = false; // 다른 공격 상태 초기화
		
		// 다른 공격 콜리전들 비활성화
		DisableHandLAttackCollision();
		DisableHandRAttackCollision();
		DisableDashAttackCollision();
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Flower Enemy Dash Attack!"));
		}

		// SkeletonEnemy와 동일한 대쉬 이동 시스템 사용
		StartDashMovementToPlayer();
	}
}

// 블랙보드 업데이트 함수들 (애니메이션 노티파이용)
void ACFlowerEnemy::NotifyComboAttackCompleted()
{
	// AI 컨트롤러를 통해 블랙보드 업데이트
	if (AController* MyController = GetController())
	{
		if (AAIController* AIController = Cast<AAIController>(MyController))
		{
			if (UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent())
			{
				// 콤보 공격 완료 신호 설정
				Blackboard->SetValueAsBool(TEXT("IsComboAttackComplete"), true);
				
				// 콤보 공격 상태 초기화
				bIsComboAttacking = false;
				
				// 디버그 출력
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Flower Enemy Combo Attack Completed - Blackboard Updated"));
				}
			}
		}
	}
}

void ACFlowerEnemy::NotifyDashAttackCompleted()
{
	// AI 컨트롤러를 통해 블랙보드 업데이트
	if (AController* MyController = GetController())
	{
		if (AAIController* AIController = Cast<AAIController>(MyController))
		{
			if (UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent())
			{
				// 대시 공격 완료 신호 설정
				Blackboard->SetValueAsBool(TEXT("IsDashAttackComplete"), true);
				
				// 대시 공격 상태 초기화
				bIsDashAttacking = false;
				
				// 디버그 출력
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Flower Enemy Dash Attack Completed - Blackboard Updated"));
				}
			}
		}
	}
}

void ACFlowerEnemy::NotifyRangedAttackCompleted()
{
	// AI 컨트롤러를 통해 블랙보드 업데이트
	if (AController* MyController = GetController())
	{
		if (AAIController* AIController = Cast<AAIController>(MyController))
		{
			if (UBlackboardComponent* Blackboard = AIController->GetBlackboardComponent())
			{
				// 원거리 공격 완료 신호 설정
				Blackboard->SetValueAsBool(TEXT("IsRangedAttackComplete"), true);
				
				// 원거리 공격 상태 초기화
				bIsRangedAttacking = false;
				
				// 디버그 출력
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, TEXT("Flower Enemy Ranged Attack Completed - Blackboard Updated"));
				}
			}
		}
	}
}

// 머리 상태 변경 함수들
void ACFlowerEnemy::SetHeadOpen(bool bOpen)
{
	bIsHeadOpen = bOpen;
	UpdateHeadCollisions();
	
	// 디버그 출력
	if (GEngine)
	{
		FString DebugMessage = bOpen ? TEXT("Flower Enemy Head Opened") : TEXT("Flower Enemy Head Closed");
		FColor DebugColor = bOpen ? FColor::Orange : FColor::Blue;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, DebugColor, DebugMessage);
	}
}

void ACFlowerEnemy::OpenHeadWithTimer()
{
	// 머리를 열기
	SetHeadOpen(true);
	
	// 기존 타이머가 있다면 클리어
	GetWorldTimerManager().ClearTimer(HeadCloseTimerHandle);
	
	// 4~6초 사이의 랜덤 시간 후 머리 자동 닫기
	float RandomTime = FMath::RandRange(4.0f, 6.0f);
	GetWorldTimerManager().SetTimer(HeadCloseTimerHandle, this, &ACFlowerEnemy::CloseHeadAutomatically, RandomTime, false);
	
	// 디버그 출력
	if (GEngine)
	{
		FString DebugMessage = FString::Printf(TEXT("Flower Enemy Head (Damage) will close in %.1f seconds"), RandomTime);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, DebugMessage);
	}
}

void ACFlowerEnemy::CloseHeadAutomatically()
{
	// 머리를 닫기
	SetHeadOpen(false);
	
	// 타이머 핸들 무효화
	HeadCloseTimerHandle.Invalidate();
	
	// 다음 자동 머리 열기 타이머 설정
	StartAutoHeadOpening();
	
	// 디버그 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("Flower Enemy Head automatically closed"));
	}
}

void ACFlowerEnemy::UpdateHeadCollisions()
{
//	// IsHeadOpen이 true이면 약점만 활성화, 평상시 콜리전 비활성화
//	if (HeadWeekPointCollision)
//	{
//		HeadWeekPointCollision->SetCollisionEnabled(bIsHeadOpen ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);
//	}
//	if (HeadTakeDamageCollision)
//	{
//		HeadTakeDamageCollision->SetCollisionEnabled(bIsHeadOpen ? ECollisionEnabled::NoCollision : ECollisionEnabled::QueryOnly);
//	}

	// 디버그 출력
//	if (GEngine)
//	{
//		const TCHAR* OpenClose = bIsHeadOpen ? TEXT("OPEN") : TEXT("CLOSE");
//		GEngine->AddOnScreenDebugMessage(-1, 1.5f, FColor::Purple, FString::Printf(TEXT("Head Collisions Updated: %s"), OpenClose));
//	}
}

void ACFlowerEnemy::StartAutoHeadOpening()
{
	// 기존 자동 머리 열기 타이머가 있다면 클리어
	GetWorldTimerManager().ClearTimer(HeadAutoOpenTimerHandle);
	
	// 12~15초 사이의 랜덤 시간 후 자동 머리 열기
	float RandomTime = FMath::RandRange(12.0f, 15.0f);
	GetWorldTimerManager().SetTimer(HeadAutoOpenTimerHandle, this, &ACFlowerEnemy::OpenHeadAutomatically, RandomTime, false);
	
	// 디버그 출력
	if (GEngine)
	{
		FString DebugMessage = FString::Printf(TEXT("Flower Enemy Head will auto-open in %.1f seconds"), RandomTime);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, DebugMessage);
	}
}

void ACFlowerEnemy::OpenHeadAutomatically()
{
	// 머리가 이미 열려있지 않을 때만 자동 열기
	if (!bIsHeadOpen)
	{
		// 머리를 열기
		SetHeadOpen(true);
		
		// 기존 타이머가 있다면 클리어
		GetWorldTimerManager().ClearTimer(HeadCloseTimerHandle);
		
		// 3~5초 사이의 랜덤 시간 후 머리 자동 닫기
		float RandomTime = FMath::RandRange(3.0f, 5.0f);
		GetWorldTimerManager().SetTimer(HeadCloseTimerHandle, this, &ACFlowerEnemy::CloseHeadAutomatically, RandomTime, false);
		
		// 디버그 출력
		if (GEngine)
		{
			FString DebugMessage = FString::Printf(TEXT("Flower Enemy Head (Auto) will close in %.1f seconds"), RandomTime);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, DebugMessage);
		}
	}
	else
	{
		// 머리가 이미 열려있으면 다음 자동 열기 타이머만 설정
		StartAutoHeadOpening();
	}
}

// 원거리 공격 오브젝트 소환 함수들
void ACFlowerEnemy::SpawnRangedProjectile(AActor* TargetPlayer)
{
	if (!TargetPlayer)
		return;
	
	// 애로우가 있으면 애로우 위치/회전에서 발사
	if (ProjectileSpawnArrow)
	{
		const FVector SpawnLocation = ProjectileSpawnArrow->GetComponentLocation();
		FVector ToTarget = TargetPlayer->GetActorLocation() - SpawnLocation;
		ToTarget.Normalize();
		const FRotator SpawnRotation = ToTarget.Rotation();
		SpawnRangedProjectileAtLocation(TargetPlayer, SpawnLocation, SpawnRotation);
		return;
	}

	// 기본 발사 위치 계산(폴백)
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 80.0f;
	FRotator SpawnRotation = GetActorRotation();
	
	SpawnRangedProjectileAtLocation(TargetPlayer, SpawnLocation, SpawnRotation);
}

// 소켓 기반 데미지 콜리전 생성 함수
void ACFlowerEnemy::CreateDamageCollisions()
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp)
		return;
		
	// 공격 콜리전들 생성
	// HandLAttackCollisionSocket 콜리전 생성
	HandLAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HandLAttackCollision"));
	HandLAttackCollision->SetupAttachment(MeshComp, TEXT("HandLAttackCollisionSocket"));
	HandLAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 기본적으로 비활성화
	HandLAttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	HandLAttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HandLAttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	HandLAttackCollision->SetRelativeLocation(FVector::ZeroVector);
	HandLAttackCollision->SetRelativeRotation(FRotator::ZeroRotator);
	HandLAttackCollision->SetBoxExtent(FVector(15.0f, 15.0f, 15.0f));

	// HandRAttackCollisionSocket 콜리전 생성
	HandRAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("HandRAttackCollision"));
	HandRAttackCollision->SetupAttachment(MeshComp, TEXT("HandRAttackCollisionSocket"));
	HandRAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 기본적으로 비활성화
	HandRAttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	HandRAttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	HandRAttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	HandRAttackCollision->SetRelativeLocation(FVector::ZeroVector);
	HandRAttackCollision->SetRelativeRotation(FRotator::ZeroRotator);
	HandRAttackCollision->SetBoxExtent(FVector(15.0f, 15.0f, 15.0f));

	// DashAttackCollision 생성 (메쉬의 자식으로)
	DashAttackCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("DashAttackCollision"));
	DashAttackCollision->SetupAttachment(MeshComp);
	DashAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 기본적으로 비활성화
	DashAttackCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	DashAttackCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DashAttackCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	DashAttackCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f)); // 메쉬 중심
	DashAttackCollision->SetRelativeRotation(FRotator::ZeroRotator);
	DashAttackCollision->SetBoxExtent(FVector(50.0f, 50.0f, 100.0f)); // 돌진 공격용 큰 콜리전
}

// 소켓 기반 데미지 콜리전 이벤트 바인딩 함수
void ACFlowerEnemy::BindDamageCollisionEvents()
{
	// 공격 콜리전들에 오버랩 이벤트 바인딩
	if (HandLAttackCollision)
	{
		HandLAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ACFlowerEnemy::OnAttackCollisionOverlap);
	}
	if (HandRAttackCollision)
	{
		HandRAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ACFlowerEnemy::OnAttackCollisionOverlap);
	}
	if (DashAttackCollision)
	{
		DashAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ACFlowerEnemy::OnAttackCollisionOverlap);
	}
}

// 소켓 기반 데미지 콜리전 비활성화 함수
void ACFlowerEnemy::DisableDamageCollisions()
{
	// 모든 데미지 콜리전을 비활성화
	TArray<UPrimitiveComponent*> DamageCollisions = {
		// 공격 콜리전들
		HandLAttackCollision,
		HandRAttackCollision,
		DashAttackCollision
	};

	for (UPrimitiveComponent* Collision : DamageCollisions)
	{
		if (Collision)
		{
			Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}

	// 디버그 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Flower Enemy all collisions disabled"));
	}
}

// 소켓 기반 데미지 콜리전 오버랩 이벤트 핸들러
void ACFlowerEnemy::OnDamageCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this)
		return;

	// 플레이어인지 확인
	if (!OtherActor->IsA<ACPlayerBullet>())
		return;

	// 이미 사망한 경우 데미지를 받지 않음
	if (StatusComponent && StatusComponent->IsDead())
		return;

	// 플레이어의 공격인지 확인 (예: 플레이어의 무기나 공격 콜리전)
	// 여기서는 간단히 Pawn으로 확인하지만, 실제로는 플레이어의 공격 콜리전을 확인해야 함
	ACPlayerBullet* PlayerBullet = Cast<ACPlayerBullet>(OtherActor);
	if (!PlayerBullet)
		return;

	// 기본 데미지 설정
	float DamageAmount = 10.0f; // 기본 데미지 (실제로는 플레이어의 공격력에 따라 결정되어야 함)
	
//	// HeadTakeDamageCollision인 경우 머리 열기
//	if (OverlappedComponent == HeadTakeDamageCollision)
//	{
//		// 머리를 열고 타이머 설정
//		OpenHeadWithTimer();
//	}
	
//	// HeadWeekPointCollision인 경우 2배 대미지 적용
//	if (OverlappedComponent == HeadWeekPointCollision)
//	{
//		DamageAmount *= 2.0f; // 헤드 약점에 2배 대미지
//	}
	
	// IDamageable 인터페이스의 TakeDamage_Implementation 호출
	TakeDamage_Implementation(DamageAmount);

	// 디버그 출력
	if (GEngine)
	{
		FString CollisionName = OverlappedComponent ? OverlappedComponent->GetName() : TEXT("Unknown");
		FString DebugMessage = FString::Printf(TEXT("Flower Enemy hit on %s by %s! Damage: %.1f"), 
			*CollisionName, *OtherActor->GetName(), DamageAmount);
		
	//	// 헤드 약점 히트 시 다른 색상으로 표시
	//	if (OverlappedComponent == HeadWeekPointCollision)
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMessage + TEXT(" (HEAD WEAK POINT!)"));
	//	}
	//	if
	//	{
	//		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, DebugMessage);
	//	}
	}
}

void ACFlowerEnemy::SpawnRangedProjectileAtLocation(AActor* TargetPlayer, FVector SpawnLocation, FRotator SpawnRotation)
{
	if (!TargetPlayer)
		return;
	
	// 월드 컨텍스트
	UWorld* World = GetWorld();
	if (!World)
		return;
	
	// 풀 매니저 찾기
	ACFlowerEnemyRangedATKManager* Pool = Cast<ACFlowerEnemyRangedATKManager>(UGameplayStatics::GetActorOfClass(World, ACFlowerEnemyRangedATKManager::StaticClass()));
	
	// 사용할 프로젝타일 클래스
	TSubclassOf<ACFlowerProjectile> ProjectileClassToUse = ACFlowerProjectile::StaticClass();
	
	if (Pool)
	{
		// 매니저에 클래스가 비어 있으면 설정
		if (!Pool->ProjectileClass && ProjectileClassToUse)
		{
			Pool->ProjectileClass = ProjectileClassToUse;
		}
		
		// 풀에서 하나 대여
		ACFlowerProjectile* PooledProjectile = Pool->AcquireProjectile(World);
		if (PooledProjectile)
		{
			PooledProjectile->SetActorLocation(SpawnLocation);
			PooledProjectile->SetActorRotation(SpawnRotation);
			PooledProjectile->SetOwner(this);
			PooledProjectile->SetInstigator(this);
			
			// 히트 이펙트 풀 매니저 사용 제거
			
			// 프로젝타일 활성화 및 타겟 설정
			FVector TargetLocation = TargetPlayer->GetActorLocation();
			PooledProjectile->ActivateProjectile(SpawnLocation, TargetLocation, TargetPlayer);
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, 
					FString::Printf(TEXT("Flower Enemy pooled projectile targeting %s"), *TargetPlayer->GetName()));
			}
			return;
		}
	}
	
	// 풀을 사용하지 못하면 기존 스폰 방식으로 폴백
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	
	ACFlowerProjectile* SpawnedProjectile = World->SpawnActor<ACFlowerProjectile>(
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);
	
	if (SpawnedProjectile)
	{
		// 프로젝타일 활성화 및 타겟 설정
		FVector TargetLocation = TargetPlayer->GetActorLocation();
		SpawnedProjectile->ActivateProjectile(SpawnLocation, TargetLocation, TargetPlayer);
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, 
				FString::Printf(TEXT("Flower Enemy spawned projectile targeting %s (fallback)"), *TargetPlayer->GetName()));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, 
				TEXT("Failed to spawn CFlowerProjectile!"));
		}
	}
}

TSubclassOf<AActor> ACFlowerEnemy::GetProjectileClass() const
{
	return ProjectileClass;
}

void ACFlowerEnemy::OnDeath()
{
	// 사망 시 처리 로직
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Flower Enemy Died! Will be removed in 3 seconds..."));
	}

	// 돌진 이동 정지
	bIsDashMoving = false;

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
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Flower Enemy removed from game!"));
			}
			// 블루프린트에서 구현된 흡수 애니메이션 실행
			StartAbsorbAnimation();
			SetActorEnableCollision(false);
			SetActorTickEnabled(false);
			if (USkeletalMeshComponent* MeshComp = GetMesh())
			{
				MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	});
	GetWorldTimerManager().SetTimer(DeathTimerHandle, DestroySelf, 3.0f, false);
	
	// 사망 애니메이션 재생, 사망 효과 등 추가 가능
	// 예: PlayDeathAnimation();

	// 낙하 연출 시작
	StartFallingAnimation();


	// 소켓 기반 데미지 콜리전 비활성화
	DisableDamageCollisions();

}

void ACFlowerEnemy::OnMeleeAttackHit(AActor* HitActor)
{
	// 근접 공격이 플레이어에게 히트했을 때의 처리
	if (HitActor && HitActor->IsA<APawn>())
	{
		if (GEngine)
		{
			float Damage = MeleeAttackComponent->GetMeleeDamage();
			FString DebugMessage = FString::Printf(TEXT("Flower Enemy Melee Attack Hit: %s with %.1f damage!"), *HitActor->GetName(), Damage);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, DebugMessage);
		}
		
		// 여기에 플레이어에게 데미지를 주는 로직 추가
		// 예: HitActor->TakeDamage(MeleeAttackComponent->GetMeleeDamage());
	}
}


void ACFlowerEnemy::StartFallingAnimation()
{
	if (!GetMesh())
		return;

	bIsFalling = true;

	// 시작 위치/높이 기록 (현재 메시 상대 위치 기준)
	OriginalMeshLocation = GetMesh()->GetRelativeLocation();
	FallStartZ = OriginalMeshLocation.Z;
	FallStartRotation = GetMesh()->GetRelativeRotation();

	FallStartTime = 0.0f;

	if (FallCurve)
	{
		FallTimeline.PlayFromStart();
	}
}

void ACFlowerEnemy::UpdateFallingAnimation(float DeltaTime)
{
	if (!GetMesh())
		return;

	if (FallDuration <= 0.0f)
	{
		FVector FinalLoc = OriginalMeshLocation;
		FinalLoc.Z = FallEndHeight;
		GetMesh()->SetRelativeLocation(FinalLoc);
		bIsFalling = false;
		return;
	}

	FallStartTime += DeltaTime;
	float Alpha = FMath::Clamp(FallStartTime / FallDuration, 0.0f, 1.0f);

	float NewZ = FMath::Lerp(FallStartZ, FallEndHeight, Alpha);
	FVector NewLoc = OriginalMeshLocation;
	NewLoc.Z = NewZ;
	GetMesh()->SetRelativeLocation(NewLoc);

	const FQuat StartQ = FallStartRotation.Quaternion();
	const FQuat EndQ = FallEndRotation.Quaternion();
	const FQuat Slerped = FQuat::Slerp(StartQ, EndQ, Alpha);
	GetMesh()->SetRelativeRotation(Slerped);

	if (Alpha >= 1.0f)
	{
		bIsFalling = false;
	}
}

void ACFlowerEnemy::OnFallTimelineUpdate(float Value)
{
	if (!GetMesh())
		return;

	float NewZ = FMath::Lerp(FallStartZ, FallEndHeight, Value);
	FVector NewLoc = OriginalMeshLocation;
	NewLoc.Z = NewZ;
	GetMesh()->SetRelativeLocation(NewLoc);

	const FQuat StartQ = FallStartRotation.Quaternion();
	const FQuat EndQ = FallEndRotation.Quaternion();
	const FQuat Slerped = FQuat::Slerp(StartQ, EndQ, Value);
	GetMesh()->SetRelativeRotation(Slerped);
}

void ACFlowerEnemy::OnFallTimelineFinished()
{
	if (!GetMesh())
		return;

	FVector FinalLoc = OriginalMeshLocation;
	FinalLoc.Z = FallEndHeight;
	GetMesh()->SetRelativeLocation(FinalLoc);
	GetMesh()->SetRelativeRotation(FallEndRotation);
	bIsFalling = false;
}


// 돌진 공격 이동 함수들
void ACFlowerEnemy::StartDashMovementToPlayer()
{
	// AI Controller 이동 중지
	if (AController* MyController = GetController())
	{
		if (AAIController* AIController = Cast<AAIController>(MyController))
		{
			AIController->StopMovement();
		}
	}

	// 플레이어 위치 가져오기
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		// 플레이어의 위치와 회전 정보 가져오기
		FVector PlayerLocation = PlayerPawn->GetActorLocation();
		FRotator PlayerRotation = PlayerPawn->GetActorRotation();
		
		// 플레이어가 바라보는 방향 계산 (Yaw 회전만 사용)
		FVector PlayerForward = FRotationMatrix(FRotator(0.0f, PlayerRotation.Yaw, 0.0f)).GetUnitAxis(EAxis::X);
		
		// 플레이어 정면 앞 70cm 지점 계산
		CachedPlayerLocation = PlayerLocation + (PlayerForward * 70.0f);
		
		// 시간 기반 이동 초기화
		DashStartLocation = GetActorLocation();
		DashElapsedTime = 0.0f;
		// DashTotalTime은 애니메이션 노티파이에서 설정됨
		
		bIsDashMoving = true;
	}
	else
	{
		// 플레이어를 찾을 수 없는 경우
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Player not found for dash movement!"));
		}
		bIsDashMoving = false;
	}
}

void ACFlowerEnemy::UpdateDashMovementToPlayer(float DeltaTime, float TotalTime)
{
	// 이동 중이 아니면 리턴
	if (!bIsDashMoving)
		return;

	// 사망한 경우 이동하지 않음
	if (StatusComponent && StatusComponent->IsDead())
	{
		bIsDashMoving = false;
		return;
	}

	// 총 이동 시간 설정 (첫 번째 호출 시)
	if (DashTotalTime <= 0.0f)
	{
		DashTotalTime = TotalTime;
	}

	// 경과 시간 업데이트
	DashElapsedTime += DeltaTime;

	// 시간 기반 이동 계산 (0.0 ~ 1.0)
	float Alpha = FMath::Clamp(DashElapsedTime / DashTotalTime, 0.0f, 1.0f);
	
	// EaseInOut 곡선 적용 (부드러운 가속/감속)
	float SmoothAlpha = FMath::InterpEaseInOut(0.0f, 1.0f, Alpha, 2.0f);
	
	// 시작 위치에서 목표 위치로 보간
	FVector NewLocation = FMath::Lerp(DashStartLocation, CachedPlayerLocation, SmoothAlpha);
	
	// 목표 방향으로 회전 (Yaw만 사용)
	FVector Direction = (CachedPlayerLocation - DashStartLocation).GetSafeNormal();
	FRotator TargetRotation = FRotationMatrix::MakeFromX(Direction).Rotator();
	TargetRotation.Pitch = 0.0f;
	TargetRotation.Roll = 0.0f;
	SetActorRotation(TargetRotation);
	
	// 위치 설정
	SetActorLocation(NewLocation);
	
	// 시간이 다 되면 이동 완료
	if (Alpha >= 1.0f)
	{
		// 정확히 목표 위치에 도달
		SetActorLocation(CachedPlayerLocation);
		bIsDashMoving = false;
	}
}

void ACFlowerEnemy::EndDashMovementToPlayer()
{
	bIsDashMoving = false;
	
	// 이동 정지
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
	}
}


// 공격 콜리전 활성화/비활성화 함수들
void ACFlowerEnemy::EnableHandLAttackCollision()
{
	if (HandLAttackCollision)
	{
		HandLAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		// HandL 공격 1회 타격 가드 초기화
		bHandLHasHit = false;
	}
}

void ACFlowerEnemy::DisableHandLAttackCollision()
{
	if (HandLAttackCollision)
	{
		HandLAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// HandL 공격 플래그 리셋 (다음 공격을 위해)
		bHandLHasHit = false;
	}
}

void ACFlowerEnemy::EnableHandRAttackCollision()
{
	if (HandRAttackCollision)
	{
		HandRAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		// HandR 공격 1회 타격 가드 초기화
		bHandRHasHit = false;
	}
}

void ACFlowerEnemy::DisableHandRAttackCollision()
{
	if (HandRAttackCollision)
	{
		HandRAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// HandR 공격 플래그 리셋 (다음 공격을 위해)
		bHandRHasHit = false;
	}
}

void ACFlowerEnemy::EnableDashAttackCollision()
{
    if (DashAttackCollision)
    {
        DashAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
        // 대쉬 공격 1회 타격 가드 초기화
        bDashHasHit = false;
    }
}

void ACFlowerEnemy::DisableDashAttackCollision()
{
	if (DashAttackCollision)
	{
		DashAttackCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 대쉬 공격 플래그 리셋 (다음 공격을 위해)
		bDashHasHit = false;
	}
}

// 공격 콜리전 오버랩 이벤트 핸들러
void ACFlowerEnemy::OnAttackCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 플레이어인지 확인
    if (!OtherActor || !OtherActor->IsA<ADDTPlayer>())
        return;

    ADDTPlayer* HitPawn = Cast<ADDTPlayer>(OtherActor);
    if (!HitPawn)
        return;

    // 플레이어인지 확인 (플레이어 컨트롤러가 있는지 체크)
    if (!HitPawn->GetController() || !HitPawn->GetController()->IsA<APlayerController>())
        return;

    // 공격별 중복 타격 방지
    if (OverlappedComponent == HandLAttackCollision)
    {
        if (bHandLHasHit)
            return;
        bHandLHasHit = true;
    }
    else if (OverlappedComponent == HandRAttackCollision)
    {
        if (bHandRHasHit)
            return;
        bHandRHasHit = true;
    }
    else if (OverlappedComponent == DashAttackCollision)
    {
        if (bDashHasHit)
            return;
        bDashHasHit = true;
    }

    // 공격 콜리전인지 확인
    if (OverlappedComponent != HandLAttackCollision && OverlappedComponent != HandRAttackCollision && OverlappedComponent != DashAttackCollision)
        return;

    // 데미지 적용 (돌진 공격은 더 높은 데미지)
    float DamageAmount = (OverlappedComponent == DashAttackCollision) ? 40.0f : 25.0f;
    AController* InstigatorController = GetInstigatorController();
    UGameplayStatics::ApplyDamage(HitPawn, DamageAmount, InstigatorController, this, nullptr);

    // 디버그 출력
    if (GEngine)
    {
        FString AttackType;
        if (OverlappedComponent == HandLAttackCollision)
        {
            AttackType = TEXT("Left Hand");
        }
        else if (OverlappedComponent == HandRAttackCollision)
        {
            AttackType = TEXT("Right Hand");
        }
        else if (OverlappedComponent == DashAttackCollision)
        {
            AttackType = TEXT("Dash");
        }
        
        /*FString DebugMessage = FString::Printf(TEXT("Flower Enemy %s Attack Hit Player! Damage: %.1f"), AttackType, DamageAmount);*/
        /*GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, DebugMessage);*/
    }
}




