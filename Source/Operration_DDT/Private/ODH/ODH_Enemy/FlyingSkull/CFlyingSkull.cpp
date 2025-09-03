// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/ODH_Enemy/FlyingSkull/CFlyingSkull.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h"
#include "ODH/Component/CEnemyProjectileComp.h"
#include "ODH/ODH_Enemy/FlyingSkull/CSkullRangedATKManager.h"
#include "ODH/ODH_AIController/CFlyingSkullAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

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
		StatusComponent->SetMaxHealth(80.0f);
		StatusComponent->SetCurrentHealth(80.0f);
		StatusComponent->SetAttackPower(35.0f);
		StatusComponent->SetDefensePower(3.0f);
		
		// 사망 이벤트 바인딩
		StatusComponent->OnDeath.AddDynamic(this, &ACFlyingSkull::OnDeath);
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, 
				FString::Printf(TEXT("Flying Skull Spawned - Health: %.0f, Attack: %.0f"), 
				StatusComponent->GetHealthPercent() * 100, StatusComponent->GetAttackPower()));
		}
	}
	
	// 근접 공격 컴포넌트의 히트 이벤트에 바인딩
	if (MeleeAttackComponent)
	{
		MeleeAttackComponent->OnMeleeAttackHit.AddDynamic(this, &ACFlyingSkull::OnMeleeAttackHit);
	}
}

void ACFlyingSkull::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 타이머 정리
	if (GetWorldTimerManager().IsTimerActive(DeathTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(DeathTimerHandle);
	}
	
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ACFlyingSkull::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		});
		GetWorldTimerManager().SetTimer(MeleeAttackTimerHandle, ClearMelee, 0.6f, false);
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Flying Skull Melee Attack!"));
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
	
	// 기본 발사 위치 계산
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
	// 사망 시 처리 로직
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Flying Skull Died! Will be removed in 3 seconds..."));
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
			Destroy();
		}
	});
	GetWorldTimerManager().SetTimer(DeathTimerHandle, DestroySelf, 3.0f, false);
	
	// 사망 애니메이션 재생, 사망 효과 등 추가 가능
	// 예: PlayDeathAnimation();
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

