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
#include "Player/CPlayerBullet.h"
#include "Player/DDTPlayer.h"
#include "ODH/ODH_Enemy/CCombatEncounterManager.h"
#include "ODH/ODH_Enemy/Component/CEnemyHealthBarComponent.h"
#include "../../UMG/Public/Components/WidgetComponent.h"

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
//	WeakPointCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("WeakPointCollision"));
//	WeakPointCollision->SetupAttachment(GetMesh(), TEXT("WeekPointSocket"));
//	WeakPointCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	WeakPointCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
//	WeakPointCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
//	WeakPointCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
//	WeakPointCollision->SetBoxExtent(FVector(16.9f, 14.27f, 8.0f));

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
		StatusComponent->SetMaxHealth(MaxHP);
		StatusComponent->SetCurrentHealth(MaxHP);
		StatusComponent->SetAttackPower(25.0f);
		StatusComponent->SetDefensePower(0.0f);
		
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
    // Encounter Manager 등록
    if (HasAuthority())
    {
        UWorld* World = GetWorld();
        if (World)
        {
            TArray<AActor*> Found;
            UGameplayStatics::GetAllActorsOfClass(World, ACCombatEncounterManager::StaticClass(), Found);
            if (Found.Num() > 0)
            {
                if (ACCombatEncounterManager* Mgr = Cast<ACCombatEncounterManager>(Found[0]))
                {
                    Mgr->RegisterEnemy(this);
                }
            }
        }
    }
	// 근접 공격 컴포넌트의 히트 이벤트에 바인딩
	if (MeleeAttackComponent)
	{
		MeleeAttackComponent->OnMeleeAttackHit.AddDynamic(this, &ACSkeletonEnemy::OnMeleeAttackHit);
	}

	// 소켓 콜리전 오버랩 이벤트 바인딩
//	if (WeakPointCollision)
//	{
//		WeakPointCollision->OnComponentBeginOverlap.AddDynamic(this, &ACSkeletonEnemy::OnWeakPointOverlap);
//	}
	if (MeleeAttackCollisionR)
	{
		MeleeAttackCollisionR->OnComponentBeginOverlap.AddDynamic(this, &ACSkeletonEnemy::OnMeleeAttackOverlap);
	}
	if (MeleeAttackCollisionL)
	{
		MeleeAttackCollisionL->OnComponentBeginOverlap.AddDynamic(this, &ACSkeletonEnemy::OnMeleeAttackOverlap);
	}
	if (ComboAttackLastCollision)
	{
		ComboAttackLastCollision->OnComponentBeginOverlap.AddDynamic(this, &ACSkeletonEnemy::OnMeleeAttackOverlap);
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
	
    // Encounter Manager 해제
    if (HasAuthority())
    {
        UWorld* World = GetWorld();
        if (World)
        {
            TArray<AActor*> Found;
            UGameplayStatics::GetAllActorsOfClass(World, ACCombatEncounterManager::StaticClass(), Found);
            if (Found.Num() > 0)
            {
                if (ACCombatEncounterManager* Mgr = Cast<ACCombatEncounterManager>(Found[0]))
                {
                    Mgr->UnregisterEnemy(this);
                }
            }
        }
    }

    Super::EndPlay(EndPlayReason);
}

// Called every frame
void ACSkeletonEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 마지막 콤보 공격 부드러운 이동 업데이트
	if (bIsLastComboMoving)
	{
		// 현재 위치에서 목표 위치로의 방향 계산
		FVector CurrentLocation = GetActorLocation();
		FVector Direction = (LastComboTargetLocation - CurrentLocation).GetSafeNormal();
		
		// 이동 거리 계산
		float DistanceToTarget = FVector::Dist(CurrentLocation, LastComboTargetLocation);
		float MovementDistance = LastComboMovementSpeed * DeltaTime;
		
		// 목표 지점에 도달했거나 가까워졌는지 확인
		if (DistanceToTarget <= MovementDistance || DistanceToTarget < 5.0f)
		{
			// 목표 지점에 도달
			SetActorLocation(LastComboTargetLocation);
			bIsLastComboMoving = false;
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Last Combo Movement Completed"));
			}
		}
		else
		{
			// 목표 방향으로 부드럽게 이동
			FVector NewLocation = CurrentLocation + (Direction * MovementDistance);
			SetActorLocation(NewLocation);
		}
	}

	// 애니메이션에서 처리하므로 기타 코드 기반 이동 연출 업데이트 제거
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

	// 약점(헤드 등) 피격 시 즉시 그로기 100 누적
	bool bWeakSpotHit = false;
	if (const FPointDamageEvent* PointEvt = static_cast<const FPointDamageEvent*>(DamageEvent.GetTypeID() == FPointDamageEvent::ClassID ? &DamageEvent : nullptr))
	{
		const FName HitBone = PointEvt->HitInfo.BoneName;
		// 총알의 배율 로직과 일치하도록 단순 본명 판정
		if (HitBone == "head" || HitBone == "Head" || HitBone == "head_01" || HitBone == "Head_01" || HitBone == "skull" || HitBone == "Skull")
		{
			bWeakSpotHit = true;
		}
	}

	if (bWeakSpotHit)
	{
		GroggyGage += 100;
	}
	else
	{
		// 스켈레톤은 2~3대 맞으면 100 이상: 평균 50씩 가정
		GroggyGage += 50;
	}

	if (GroggyGage >= 100)
	{
		bIsHitState = true;
		GroggyGage = 0; // 임계 도달 시 초기화(원치 않으면 제거)
		// 이동 정지 처리
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
//                         // 전투 돌입: 체력바 표시
//                         if (UCEnemyHealthBarComponent* HB = FindComponentByClass<UCEnemyHealthBarComponent>())
//                         {
//                             HB->ShowHealthBar();
//                         }
					}
				}
			}
		}
	}

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

void ACSkeletonEnemy::EndHitState()
{
	bIsHitState = false;
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->SetMovementMode(static_cast<EMovementMode>(SavedMovementMode), SavedCustomMovementMode);
	}
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
		bIsComboAttacking = true;
		bIsDashAttacking = false; // 다른 공격 상태 초기화
		
		// 다른 공격 콜리전들 비활성화
		DisableComboCollisions();
		
		// 콤보 공격 쿨다운 설정 (빠른 연속 공격)
		AttackCooldown = 0.3f;
		
// 		// 기존 타이머 클리어 후 재설정
// 		GetWorldTimerManager().ClearTimer(MeleeAttackTimerHandle);
// 		FTimerDelegate ClearCombo;
// 		ClearCombo.BindLambda([this]()
// 		{
// 			bIsComboAttacking = false;
// 		});
// 		GetWorldTimerManager().SetTimer(MeleeAttackTimerHandle, ClearCombo, 0.6f, false);
		
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
		bIsDashAttacking = true;
		bIsComboAttacking = false; // 다른 공격 상태 초기화
		
		// 다른 공격 콜리전들 비활성화
		DisableComboCollisions();
		
		// 돌진 공격 쿨다운 설정 (강력한 단발 공격)
		AttackCooldown = 0.6f;
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Skeleton Enemy Dash Attack!"));
		}
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
	
	// 콤보 공격 시 전방 이동 제거 (애니메이션에서 처리)
	
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
		// ComboR 공격 플래그 리셋 (다음 공격을 위해)
		bComboRHasHit = false;
	}
	if (MeleeAttackCollisionL)
	{
		MeleeAttackCollisionL->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// ComboL 공격 플래그 리셋 (다음 공격을 위해)
		bComboLHasHit = false;
	}
	if (ComboAttackLastCollision)
	{
		ComboAttackLastCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// ComboLast 공격 플래그 리셋 (다음 공격을 위해)
		bComboLastHasHit = false;
	}
	
	// 쿨다운 초기화 (다음 공격을 위해)
	LastHitTime = 0.0f;
	
	// 디버그 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Combo Collisions Disabled - Cooldown Reset"));
	}
}

void ACSkeletonEnemy::EnableComboRCollision()
{
	if (MeleeAttackCollisionR)
	{
		MeleeAttackCollisionR->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		// ComboR 공격 1회 타격 가드 초기화
		bComboRHasHit = false;
	}
}

void ACSkeletonEnemy::EnableComboLCollision()
{
	if (MeleeAttackCollisionL)
	{
		MeleeAttackCollisionL->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		// ComboL 공격 1회 타격 가드 초기화
		bComboLHasHit = false;
	}
}

void ACSkeletonEnemy::DisableAllCollisions()
{
	// 약점 콜리전 비활성화
//	if (WeakPointCollision)
//	{
//		WeakPointCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
//	}
	
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
		// ComboLast 공격 1회 타격 가드 초기화
		bComboLastHasHit = false;
	}
	
	// 마지막 콤보 공격 시 전방 이동 제거 (애니메이션에서 처리)
	
	// 디버그 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Magenta, TEXT("Last Combo Collision Enabled"));
	}
}

void ACSkeletonEnemy::LastComboMovement(float ForwardDistance)
{
	// 사망한 경우 이동하지 않음
	if (StatusComponent && StatusComponent->IsDead())
	{
		return;
	}

	// 현재 바라보는 방향 계산 (Yaw 회전만 사용)
	FRotator CurrentRotation = GetActorRotation();
	FVector ForwardDirection = FRotationMatrix(FRotator(0.0f, CurrentRotation.Yaw, 0.0f)).GetUnitAxis(EAxis::X);
	
	// 시작 위치와 목표 위치 설정
	LastComboStartLocation = GetActorLocation();
	LastComboTargetLocation = LastComboStartLocation + (ForwardDirection * ForwardDistance);
	
	// 이동 상태 시작
	bIsLastComboMoving = true;
	
	// 디버그 출력
	if (GEngine)
	{
		FString DebugMessage = FString::Printf(TEXT("Last Combo Movement Started: Moving %.1f cm forward"), ForwardDistance);
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Magenta, DebugMessage);
	}
}

void ACSkeletonEnemy::EnableDashCollision()
{
	if (ComboAttackLastCollision)
	{
		ComboAttackLastCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		// Dash 공격 1회 타격 가드 초기화
		bDashHasHit = false;
	}
	
	// 돌진 공격은 전방 이동 효과 없음 (애니메이션에서 처리)
	
	// 디버그 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Dash Attack Collision Enabled"));
	}
}

void ACSkeletonEnemy::StartDashMovementToPlayer()
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
		
		// 디버그 출력
		if (GEngine)
		{
			FString DebugMessage = FString::Printf(TEXT("Skeleton Enemy Target Location (70cm in front of player): %s"), *CachedPlayerLocation.ToString());
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, DebugMessage);
		}
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

void ACSkeletonEnemy::UpdateDashMovementToPlayer(float DeltaTime, float TotalTime)
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
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Skeleton Enemy reached target location in time!"));
		}
	}
}

void ACSkeletonEnemy::EndDashMovementToPlayer()
{
	bIsDashMoving = false;
	
	// 이동 정지
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
	}
	
	// 디버그 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("Dash Movement Ended"));
	}
}

void ACSkeletonEnemy::StartComboAttackMovement()
{
	// 사망한 경우 이동하지 않음
	if (StatusComponent && StatusComponent->IsDead())
	{
		return;
	}

	// 현재 바라보는 방향 계산 (Yaw 회전만 사용)
	FRotator CurrentRotation = GetActorRotation();
	ComboMovementDirection = FRotationMatrix(FRotator(0.0f, CurrentRotation.Yaw, 0.0f)).GetUnitAxis(EAxis::X);
	
	// 시작 위치 저장
	ComboMovementStartLocation = GetActorLocation();
	
	// 이동 상태 설정
	bIsComboMoving = true;
	ComboMovementProgress = 0.0f;
	
	// 디버그 출력
	if (GEngine)
	{
		FString DebugMessage = FString::Printf(TEXT("Combo Movement Started - Direction: %s"), *ComboMovementDirection.ToString());
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, DebugMessage);
	}
}

void ACSkeletonEnemy::UpdateComboAttackMovement(float DeltaTime, float Speed, float Distance)
{
	// 이동 중이 아니면 리턴
	if (!bIsComboMoving)
		return;

	// 사망한 경우 이동하지 않음
	if (StatusComponent && StatusComponent->IsDead())
	{
		bIsComboMoving = false;
		return;
	}

	// 진행률 업데이트 (0.0 ~ 1.0)
	float ProgressIncrement = (Speed * DeltaTime) / Distance;
	ComboMovementProgress = FMath::Clamp(ComboMovementProgress + ProgressIncrement, 0.0f, 1.0f);
	
	// 목표 위치 계산
	FVector TargetLocation = ComboMovementStartLocation + (ComboMovementDirection * Distance);
	
	// Lerp를 사용한 부드러운 이동
	FVector NewLocation = FMath::Lerp(ComboMovementStartLocation, TargetLocation, ComboMovementProgress);
	SetActorLocation(NewLocation);
	
	// 이동 완료 체크
	if (ComboMovementProgress >= 1.0f)
	{
		bIsComboMoving = false;
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("Combo Movement Completed"));
		}
	}
}

void ACSkeletonEnemy::EndComboAttackMovement()
{
	bIsComboMoving = false;
	ComboMovementProgress = 0.0f;
	
	// 디버그 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("Combo Movement Ended"));
	}
}

void ACSkeletonEnemy::NotifyComboAttackCompleted()
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
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Combo Attack Completed - Blackboard Updated"));
				}
			}
		}
	}
}

void ACSkeletonEnemy::NotifyDashAttackCompleted()
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
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Dash Attack Completed - Blackboard Updated"));
				}
			}
		}
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

    // Encounter Manager 해제(사망 즉시)
    if (HasAuthority())
    {
        UWorld* World = GetWorld();
        if (World)
        {
            TArray<AActor*> Found;
            UGameplayStatics::GetAllActorsOfClass(World, ACCombatEncounterManager::StaticClass(), Found);
            if (Found.Num() > 0)
            {
                if (ACCombatEncounterManager* Mgr = Cast<ACCombatEncounterManager>(Found[0]))
                {
                    Mgr->UnregisterEnemy(this);
                }
            }
        }
    }

	// 이동/AI 즉시 정지
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->DisableMovement();
		MoveComp->SetMovementMode(MOVE_None);
	}
	
	// 모든 이동 상태 초기화
	bIsLastComboMoving = false;
	bIsDashMoving = false;
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
			// 비활성화 처리: 보이지 않음, 충돌 비활성화, 틱 중지
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);
			SetActorTickEnabled(false);
			if (USkeletalMeshComponent* MeshComp = GetMesh())
			{
				MeshComp->SetVisibility(false, true);
				MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
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

	// 공격별 중복 타격 방지
	if (OverlappedComponent == MeleeAttackCollisionR)
	{
		if (bComboRHasHit)
			return;
		bComboRHasHit = true;
	}
	else if (OverlappedComponent == MeleeAttackCollisionL)
	{
		if (bComboLHasHit)
			return;
		bComboLHasHit = true;
	}
	else if (OverlappedComponent == ComboAttackLastCollision)
	{
		if (bComboLastHasHit)
			return;
		bComboLastHasHit = true;
	}

	// 플레이어인지 확인
	if (OtherActor->IsA<ADDTPlayer>())
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
	// 기존 방식: UGameplayStatics::ApplyDamage 사용 -> ApplyPointDamage로 변경
	if (StatusComponent && !StatusComponent->IsDead())
	{
		float BaseDamage = StatusComponent->GetAttackPower();
		float DamageToApply = BaseDamage * 2.0f;

		// 가해자 컨트롤러는 상대(OtherActor) 기준으로 설정
		AController* InstigatorController = OtherActor ? OtherActor->GetInstigatorController() : nullptr;

		// 히트 방향과 히트 정보 계산
		const FVector HitFromDirection = (GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal();
		const FHitResult& HitInfo = SweepResult;

		UGameplayStatics::ApplyPointDamage(
			this,
			DamageToApply,
			HitFromDirection,
			HitInfo,
			InstigatorController,
			OtherActor,
			nullptr
		);
	}
}

void ACSkeletonEnemy::OnTakeDamageOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || OtherActor == this)
        return;

    // 플레이어 총알과 충돌했을 때만 처리
    if (ACPlayerBullet* Bullet = Cast<ACPlayerBullet>(OtherActor))
    {
        if (StatusComponent && !StatusComponent->IsDead())
        {
            const float Damage = FMath::Max(1.0f, Bullet->BulletDamage);

            // 가해자/컨트롤러 정보 수집
            AController* InstigatorController = Bullet->GetInstigatorController();

            // 히트 방향 계산 (총알 -> 적)
            const FVector HitFromDirection = (GetActorLocation() - Bullet->GetActorLocation()).GetSafeNormal();

            // 본 데미지 배율 적용 (FHitResult는 BoneName을 제공)
            float Multiplier = 1.0f;
            const FName HitBone = SweepResult.BoneName;
            Multiplier = Bullet->GetDamageMultiplierForBone(HitBone);

            // PointDamage 적용
            UGameplayStatics::ApplyPointDamage(
                this,
                Damage * Multiplier,
                HitFromDirection,
                SweepResult,
                InstigatorController,
                Bullet,
                nullptr
            );
        }
    }
}

// PerformForwardMovement 함수 제거됨 - 애니메이션에서 처리

// 이동/낙하 연출 관련 로직은 애니메이션으로 대체
