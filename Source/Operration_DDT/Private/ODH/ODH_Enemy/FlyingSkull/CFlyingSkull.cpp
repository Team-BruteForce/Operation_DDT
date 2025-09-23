// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/ODH_Enemy/FlyingSkull/CFlyingSkull.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h"
#include "ODH/Component/CEnemyProjectileComp.h"
#include "ODH/ODH_Enemy/FlyingSkull/CSkullRangedATKManager.h"
#include "ODH/ODH_AIController/CFlyingSkullAIController.h"
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
#include "ODH/ODH_Enemy/CCombatEncounterManager.h"
#include "../../AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyHealthBarComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyHealthBarComponent.h"
#include "../../UMG/Public/Components/WidgetComponent.h"
#include "Player/DDTGameMode.h"

// Sets default values
ACFlyingSkull::ACFlyingSkull()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// AI 컨트롤러 클래스 설정
	AIControllerClass = ACFlyingSkullAIController::StaticClass();

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

	// 애로우 컴포넌트 생성 (원거리 공격 발사 위치)
	ProjectileSpawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnArrow"));
	ProjectileSpawnArrow->SetupAttachment(RootComponent);
	ProjectileSpawnArrow->SetArrowColor(FLinearColor::Red);
	ProjectileSpawnArrow->SetArrowLength(50.0f);
	ProjectileSpawnArrow->SetArrowSize(2.0f);
	// 기본 위치는 전방 100cm (블루프린트에서 자유롭게 변경 가능)
	ProjectileSpawnArrow->SetRelativeLocation(FVector(100.0f, 0.0f, 0.0f));


}

// Called when the game starts or when spawned
void ACFlyingSkull::BeginPlay()
{
	Super::BeginPlay();
	
	// 타이머 핸들 초기화
	DeathTimerHandle.Invalidate();
	
	// Flying Skull 전용 스테이터스 설정
	if (StatusComponent)
	{
		// Flying Skull은 체력이 낮지만 공격력이 높음
		StatusComponent->SetMaxHealth(MaxHP);
		StatusComponent->SetCurrentHealth(MaxHP);
		StatusComponent->SetAttackPower(35.0f);
		StatusComponent->SetDefensePower(0.0f);
		
		// 사망 이벤트 바인딩
		StatusComponent->OnDeath.AddDynamic(this, &ACFlyingSkull::OnDeath);
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, 
				FString::Printf(TEXT("Flying Skull Spawned - Health: %.0f, Attack: %.0f"), 
				StatusComponent->GetHealthPercent() * 100, StatusComponent->GetAttackPower()));
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
	}
	
	// 근접 공격 컴포넌트의 히트 이벤트에 바인딩
	if (MeleeAttackComponent)
	{
		MeleeAttackComponent->OnMeleeAttackHit.AddDynamic(this, &ACFlyingSkull::OnMeleeAttackHit);
	}

	// 근접 공격 콜리전 오버랩 이벤트 바인딩
	if (MeleeAttackCollision)
	{
		MeleeAttackCollision->OnComponentBeginOverlap.AddDynamic(this, &ACFlyingSkull::OnMeleeAttackOverlap);
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
}

void ACFlyingSkull::EndPlay(const EEndPlayReason::Type EndPlayReason)
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
void ACFlyingSkull::Tick(float DeltaTime)
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

	// 메쉬 상대 이동 기반 근접 공격 연출 업데이트
	if (bIsMeleeVisualMoving)
	{
		UpdateMeleeVisualMove(DeltaTime);
	}
}

// Called to bind functionality to input
void ACFlyingSkull::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// IDamageable 인터페이스 구현
void ACFlyingSkull::TakeDamage_Implementation(float DamageAmount)
{
	if (StatusComponent)
	{
		StatusComponent->TakeDamage(DamageAmount);
	}
}

bool ACFlyingSkull::CanTakeDamage_Implementation() const
{
	if (StatusComponent)
	{
		return !StatusComponent->IsDead();
	}
	return false;
}

bool ACFlyingSkull::IsDead_Implementation() const
{
	if (StatusComponent)
	{
		return StatusComponent->IsDead();
	}
	return false;
}

// UGameplayStatics::ApplyDamage를 위한 TakeDamage 오버라이드
float ACFlyingSkull::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
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
	bool bCritical = false;
	// 약점 본 피격 시 즉시 피격 상태 진입 (그로기 누적은 하지 않음)
	if (const FPointDamageEvent* PointEvt = static_cast<const FPointDamageEvent*>(DamageEvent.GetTypeID() == FPointDamageEvent::ClassID ? &DamageEvent : nullptr))
	{
		const FName HitBone = PointEvt->HitInfo.BoneName;
		if (HitBone == "head" || HitBone == "Head" || HitBone == "head_01" || HitBone == "Head_01" || HitBone == "skull" || HitBone == "Skull")
		{
			bCritical = true;
			GroggyGage += 100; // 명세상 누적은 불필요하지만, 임계 체크를 위해 더해도 무방
			bIsHitState = true;
			GroggyGage = 0; // 즉시 초기화(선호에 따라 유지 가능)
			if (UCharacterMovementComponent* Move = GetCharacterMovement())
			{
				SavedMovementMode = Move->MovementMode;
				SavedCustomMovementMode = Move->CustomMovementMode;
				Move->DisableMovement();
			}
		}
	}
	if (ADDTGameMode* GM = GetWorld()->GetAuthGameMode<ADDTGameMode>())
	{
		GM->BroadCastDamage(DamageAmount, bCritical, false);
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

	// 디버그 출력
	if (GEngine)
	{
		float CurrentHealth = StatusComponent ? StatusComponent->GetCurrentHealth() : 0.0f;
		float HealthPercent = StatusComponent ? StatusComponent->GetHealthPercent() * 100.0f : 0.0f;
		
		FString DebugMessage = FString::Printf(TEXT("Flying Skull took %.1f damage from %s! Health: %.1f (%.1f%%)"), 
			DamageAmount, DamageCauser ? *DamageCauser->GetName() : TEXT("Unknown"), CurrentHealth, HealthPercent);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, DebugMessage);
	}

	return DamageAmount;
}

void ACFlyingSkull::EndHitState()
{
	bIsHitState = false;
	if (UCharacterMovementComponent* Move = GetCharacterMovement())
	{
		Move->SetMovementMode(static_cast<EMovementMode>(SavedMovementMode), SavedCustomMovementMode);
	}
}

// IGenericTeamAgentInterface 구현
FGenericTeamId ACFlyingSkull::GetGenericTeamId() const
{
	return FGenericTeamId(1); // Team ID 1 (적 팀)
}

// 공격 함수들
void ACFlyingSkull::PlayMeleeAttack()
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
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Flying Skull Melee Attack!"));
		}

		// 근접 공격 콜리전 활성화
		if (MeleeAttackCollision)
		{
			MeleeAttackCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			
			// 디버그 시각화
			DrawDebugBox(GetWorld(), MeleeAttackCollision->GetComponentLocation(), 
				MeleeAttackCollision->GetScaledBoxExtent(), 
				MeleeAttackCollision->GetComponentRotation().Quaternion(), 
				FColor::Red, false, 0.5f);
		}

		// 시각 연출 이동 시작(서버 권한에서만)
		if (HasAuthority())
		{
			APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
			StartMeleeVisualMove(PlayerPawn);
		}
	}
}

void ACFlyingSkull::PlayRangedAttack()
{
	// 원거리 공격 로직
	bIsRangedAttacking = true;
	
	GetWorldTimerManager().ClearTimer(RangedAttackTimerHandle);
	FTimerDelegate ClearRanged;
	ClearRanged.BindLambda([this]()
	{
		bIsRangedAttacking = false;
	});
	GetWorldTimerManager().SetTimer(RangedAttackTimerHandle, ClearRanged, 0.6f, false);
	
	// 디버그 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Flying Skull Ranged Attack!"));
	}
}

// 원거리 공격 오브젝트 소환 함수들
void ACFlyingSkull::SpawnRangedProjectile(AActor* TargetPlayer)
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
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
	FRotator SpawnRotation = GetActorRotation();
	
	SpawnRangedProjectileAtLocation(TargetPlayer, SpawnLocation, SpawnRotation);
}

void ACFlyingSkull::SpawnRangedProjectileAtLocation(AActor* TargetPlayer, FVector SpawnLocation, FRotator SpawnRotation)
{
	if (!TargetPlayer)
		return;
	
	// 월드 컨텍스트
	UWorld* World = GetWorld();
	if (!World)
		return;
	
	// 풀 매니저 찾기
	ACSkullRangedATKManager* Pool = Cast<ACSkullRangedATKManager>(UGameplayStatics::GetActorOfClass(World, ACSkullRangedATKManager::StaticClass()));
	
	// 사용할 프로젝타일 클래스
	TSubclassOf<AActor> ProjectileClassToUse = GetProjectileClass();
	
	if (Pool)
	{
		// 매니저에 클래스가 비어 있으면 설정
		if (!Pool->ProjectileClass && ProjectileClassToUse)
		{
			Pool->ProjectileClass = ProjectileClassToUse;
		}
		
		// 풀에서 하나 대여
		AActor* PooledProjectile = Pool->AcquireProjectile(World);
		if (PooledProjectile)
		{
			PooledProjectile->SetActorLocation(SpawnLocation);
			PooledProjectile->SetActorRotation(SpawnRotation);
			PooledProjectile->SetOwner(this);
			PooledProjectile->SetInstigator(this);
			
			if (UCEnemyProjectileComp* ProjectileComponent = PooledProjectile->FindComponentByClass<UCEnemyProjectileComp>())
			{
				ProjectileComponent->InitializeTarget(TargetPlayer);
			}
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, 
					FString::Printf(TEXT("Flying Skull pooled projectile targeting %s"), *TargetPlayer->GetName()));
			}
			return;
		}
	}
	
	// 풀을 사용하지 못하면 기존 스폰 방식으로 폴백
	if (!ProjectileClassToUse)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, 
				TEXT("ProjectileClass not set in Flying Skull!"));
		}
		return;
	}
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = this;
	
	AActor* SpawnedProjectile = World->SpawnActor<AActor>(
		ProjectileClassToUse,
		SpawnLocation,
		SpawnRotation,
		SpawnParams
	);
	
	if (SpawnedProjectile)
	{
		if (UCEnemyProjectileComp* ProjectileComponent = SpawnedProjectile->FindComponentByClass<UCEnemyProjectileComp>())
		{
			ProjectileComponent->InitializeTarget(TargetPlayer);
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, 
					FString::Printf(TEXT("Flying Skull spawned projectile targeting %s (fallback)"), *TargetPlayer->GetName()));
			}
		}
		else
		{
			SpawnedProjectile->Destroy();
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, 
					TEXT("Failed to find ProjectileComponent on spawned projectile (fallback)"));
			}
		}
	}
}

TSubclassOf<AActor> ACFlyingSkull::GetProjectileClass() const
{
	return ProjectileClass;
}

void ACFlyingSkull::OnDeath()
{
    // 사망 즉시 HP Bar 비표시
    if (UCEnemyHealthBarComponent* HB = FindComponentByClass<UCEnemyHealthBarComponent>())
    {
        HB->HideHealthBar();
    }
	// 사망 시 처리 로직
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Flying Skull Died! Will be removed in 3 seconds..."));
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
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("Flying Skull removed from game!"));
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
}



void ACFlyingSkull::OnMeleeAttackHit(AActor* HitActor)
{
	// 근접 공격이 플레이어에게 히트했을 때의 처리
	if (HitActor && HitActor->IsA<APawn>())
	{
		if (GEngine)
		{
			float Damage = MeleeAttackComponent->GetMeleeDamage();
			FString DebugMessage = FString::Printf(TEXT("Flying Skull Melee Attack Hit: %s with %.1f damage!"), *HitActor->GetName(), Damage);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, DebugMessage);
		}
		
		// 여기에 플레이어에게 데미지를 주는 로직 추가
		// 예: HitActor->TakeDamage(MeleeAttackComponent->GetMeleeDamage());
	}
}

void ACFlyingSkull::OnMeleeAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!MeleeAttackComponent || !MeleeAttackComponent->IsAttackActive() || !OtherActor)
		return;

	// 자기 자신은 제외
	if (OtherActor == this)
		return;

	// 쿨다운 체크 (MeleeAttackComponent의 AttackCooldown 사용)
	float CurrentTime = GetWorld()->GetTimeSeconds();
	static float LastAttackTime = 0.0f;
	if (CurrentTime - LastAttackTime < MeleeAttackComponent->AttackCooldown)
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
		LastAttackTime = CurrentTime;
		
		// 디버그 출력
		if (GEngine)
		{
			FString DebugMessage = FString::Printf(TEXT("Flying Skull Melee Attack Hit: %s with %.1f damage!"), *OtherActor->GetName(), Damage);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMessage);
		}
	}
}

void ACFlyingSkull::StartFallingAnimation()
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

void ACFlyingSkull::UpdateFallingAnimation(float DeltaTime)
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

void ACFlyingSkull::OnFallTimelineUpdate(float Value)
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

void ACFlyingSkull::OnFallTimelineFinished()
{
	if (!GetMesh())
		return;

	FVector FinalLoc = OriginalMeshLocation;
	FinalLoc.Z = FallEndHeight;
	GetMesh()->SetRelativeLocation(FinalLoc);
	GetMesh()->SetRelativeRotation(FallEndRotation);
	bIsFalling = false;
}

// ===== 메쉬 상대 이동 근접 연출 =====
void ACFlyingSkull::StartMeleeVisualMove(AActor* TargetActor)
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp)
		return;

	// 시작 상대 위치 저장
	MeshStartRelativeLocation = MeshComp->GetRelativeLocation();

	// 타겟 위치 계산(수평 위주)
	FVector TargetWorld = GetActorLocation();
	if (IsValid(TargetActor))
	{
		TargetWorld = TargetActor->GetActorLocation();
	}

	// 플레이어 위치로 직선 이동 (몸통 박치기)
	FVector TargetWorldPos = TargetWorld;
	
	// 최대 거리 제한 (너무 멀리 가는 것 방지)
	FVector ToTarget = TargetWorldPos - GetActorLocation();
	float Distance = ToTarget.Length();
	if (Distance > MeleeVisualMaxDistance)
	{
		FVector Direction = ToTarget / Distance;
		TargetWorldPos = GetActorLocation() + Direction * MeleeVisualMaxDistance;
	}
	
	// 월드 위치를 상대 위치로 변환
	FVector LocalTargetPos = GetActorTransform().InverseTransformPosition(TargetWorldPos);

	MeshTargetRelativeLocation = LocalTargetPos;

	MeleeVisualElapsed = 0.0f;
	bMeleeVisualGoingOut = true;
	bIsMeleeVisualMoving = true;
}

void ACFlyingSkull::UpdateMeleeVisualMove(float DeltaTime)
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp)
	{
		bIsMeleeVisualMoving = false;
		return;
	}

	const float Duration = bMeleeVisualGoingOut ? MeleeVisualOutTime : MeleeVisualBackTime;
	if (Duration <= 0.0f)
	{
		// 즉시 스냅
		if (bMeleeVisualGoingOut)
		{
			MeshComp->SetRelativeLocation(MeshTargetRelativeLocation);
			bMeleeVisualGoingOut = false;
			MeleeVisualElapsed = 0.0f;
		}
		else
		{
			MeshComp->SetRelativeLocation(MeshStartRelativeLocation);
			bIsMeleeVisualMoving = false;
		}
		return;
	}

	MeleeVisualElapsed += DeltaTime;
	float Alpha = FMath::Clamp(MeleeVisualElapsed / Duration, 0.0f, 1.0f);
	// EaseInOut 가중치
	float Smooth = FMath::InterpEaseInOut(0.0f, 1.0f, Alpha, 2.0f);

	// 직선 이동 계산
	FVector NewRel;
	
	if (bMeleeVisualGoingOut)
	{
		// 전진: 시작 → 목표 (직선)
		NewRel = FMath::Lerp(MeshStartRelativeLocation, MeshTargetRelativeLocation, Smooth);
	}
	else
	{
		// 복귀: 목표 → 시작 (직선)
		NewRel = FMath::Lerp(MeshTargetRelativeLocation, MeshStartRelativeLocation, Smooth);
	}
	
	MeshComp->SetRelativeLocation(NewRel);

	if (Alpha >= 1.0f)
	{
		if (bMeleeVisualGoingOut)
		{
			// 왕복의 복귀 단계로 전환
			bMeleeVisualGoingOut = false;
			MeleeVisualElapsed = 0.0f;
		}
		else
		{
		// 종료
		bIsMeleeVisualMoving = false;
		// 잔오차 제거
		MeshComp->SetRelativeLocation(MeshStartRelativeLocation);
		}
	}
}

void ACFlyingSkull::EndMeleeVisualMove(bool bSnapToStart)
{
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp)
		return;

	bIsMeleeVisualMoving = false;
	bMeleeVisualGoingOut = false;
	MeleeVisualElapsed = 0.0f;

	if (bSnapToStart)
	{
		MeshComp->SetRelativeLocation(MeshStartRelativeLocation);
	}
}



// 소켓 기반 데미지 콜리전 비활성화 함수
void ACFlyingSkull::DisableDamageCollisions()
{
	// 모든 데미지 콜리전을 비활성화
	TArray<UPrimitiveComponent*> DamageCollisions = {
		MeleeAttackCollision  // 근접 공격 콜리전도 포함
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
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Flying Skull all collisions disabled"));
	}
}

// 소켓 기반 데미지 콜리전 오버랩 이벤트 핸들러
void ACFlyingSkull::OnDamageCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

	// 데미지 적용
	float DamageAmount = 10.0f; // 기본 데미지 (실제로는 플레이어의 공격력에 따라 결정되어야 함)
	
	// IDamageable 인터페이스의 TakeDamage_Implementation 호출
	TakeDamage_Implementation(DamageAmount);

	// 디버그 출력
	if (GEngine)
	{
		FString CollisionName = OverlappedComponent ? OverlappedComponent->GetName() : TEXT("Unknown");
		FString DebugMessage = FString::Printf(TEXT("Flying Skull hit on %s by %s! Damage: %.1f"), 
			*CollisionName, *OtherActor->GetName(), DamageAmount);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, DebugMessage);
	}
}

