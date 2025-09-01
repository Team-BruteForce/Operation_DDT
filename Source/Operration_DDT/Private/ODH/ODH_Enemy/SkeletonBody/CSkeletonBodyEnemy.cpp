// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/ODH_Enemy/SkeletonBody/CSkeletonBodyEnemy.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h"
#include "ODH/Component/CEnemyProjectileComp.h"
#include "Engine/Engine.h"

// Sets default values
ACSkeletonBodyEnemy::ACSkeletonBodyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스테이터스 컴포넌트 생성
	StatusComponent = CreateDefaultSubobject<UCEnemyStatusComponent>(TEXT("StatusComponent"));
	
	// 근접 공격 컴포넌트 생성
	MeleeAttackComponent = CreateDefaultSubobject<UCEnemyMeleeAttackComponent>(TEXT("MeleeAttackComponent"));
	
	// 원거리 공격 컴포넌트 생성
	EnemyProjectileComponent = CreateDefaultSubobject<UCEnemyProjectileComp>(TEXT("EnemyProjectileComponent"));
}

// Called when the game starts or when spawned
void ACSkeletonBodyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// Skeleton Body 전용 스테이터스 설정
	if (StatusComponent)
	{
		// Skeleton Body는 체력이 높고 공격력이 중간
		StatusComponent->SetMaxHealth(150.0f);
		StatusComponent->SetCurrentHealth(150.0f);
		StatusComponent->SetAttackPower(25.0f);
		StatusComponent->SetDefensePower(8.0f);
		
		// 사망 이벤트 바인딩
		StatusComponent->OnDeath.AddDynamic(this, &ACSkeletonBodyEnemy::OnDeath);
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, 
				FString::Printf(TEXT("Skeleton Body Spawned - Health: %.0f, Attack: %.0f"), 
				StatusComponent->GetHealthPercent() * 100, StatusComponent->GetAttackPower()));
		}
	}
	
	// 근접 공격 컴포넌트의 히트 이벤트에 바인딩
	if (MeleeAttackComponent)
	{
		MeleeAttackComponent->OnMeleeAttackHit.AddDynamic(this, &ACSkeletonBodyEnemy::OnMeleeAttackHit);
	}
	
	// 원거리 공격 컴포넌트의 히트 이벤트에 바인딩
	if (EnemyProjectileComponent)
	{
		EnemyProjectileComponent->OnProjectileHit.AddDynamic(this, &ACSkeletonBodyEnemy::OnProjectileHit);
	}
}

// Called every frame
void ACSkeletonBodyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACSkeletonBodyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// IDamageable 인터페이스 구현
void ACSkeletonBodyEnemy::TakeDamage_Implementation(float DamageAmount)
{
	if (StatusComponent)
	{
		StatusComponent->TakeDamage(DamageAmount);
	}
}

bool ACSkeletonBodyEnemy::CanTakeDamage_Implementation() const
{
	if (StatusComponent)
	{
		return !StatusComponent->IsDead();
	}
	return false;
}

bool ACSkeletonBodyEnemy::IsDead_Implementation() const
{
	if (StatusComponent)
	{
		return StatusComponent->IsDead();
	}
	return false;
}

// 공격 함수들
void ACSkeletonBodyEnemy::PlayMeleeAttack()
{
	if (MeleeAttackComponent)
	{
		MeleeAttackComponent->ActivateMeleeAttack();
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Skeleton Body Melee Attack!"));
		}
	}
}

void ACSkeletonBodyEnemy::PlayRangedAttack()
{
	if (EnemyProjectileComponent)
	{
		// 여기에 프로젝타일 발사 로직 추가
		// 예: 프로젝타일 액터 스폰 및 초기화
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Skeleton Body Ranged Attack!"));
		}
	}
}

// 원거리 공격 오브젝트 소환 함수들
void ACSkeletonBodyEnemy::SpawnRangedProjectile(AActor* TargetPlayer)
{
	if (!TargetPlayer)
		return;
	
	// 기본 발사 위치 계산
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
	FRotator SpawnRotation = GetActorRotation();
	
	SpawnRangedProjectileAtLocation(TargetPlayer, SpawnLocation, SpawnRotation);
}

void ACSkeletonBodyEnemy::SpawnRangedProjectileAtLocation(AActor* TargetPlayer, FVector SpawnLocation, FRotator SpawnRotation)
{
	if (!TargetPlayer)
		return;
	
	// 프로젝타일 액터 스폰
	UWorld* World = GetWorld();
	if (!World)
		return;
	
	// 프로젝타일 클래스 가져오기
	TSubclassOf<AActor> ProjectileClassToUse = GetProjectileClass();
	if (!ProjectileClassToUse)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, 
				TEXT("ProjectileClass not set in Skeleton Body!"));
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
		// CEnemyProjectileComp 컴포넌트 찾기
		UCEnemyProjectileComp* ProjectileComponent = SpawnedProjectile->FindComponentByClass<UCEnemyProjectileComp>();
		if (ProjectileComponent)
		{
			// 타겟 설정
			ProjectileComponent->InitializeTarget(TargetPlayer);
			
			// 디버그 출력
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, 
					FString::Printf(TEXT("Skeleton Body spawned projectile targeting %s"), *TargetPlayer->GetName()));
			}
		}
		else
		{
			// 컴포넌트가 없으면 제거
			SpawnedProjectile->Destroy();
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, 
					TEXT("Failed to find ProjectileComponent on spawned projectile"));
			}
		}
	}
}

TSubclassOf<AActor> ACSkeletonBodyEnemy::GetProjectileClass() const
{
	return ProjectileClass;
}

void ACSkeletonBodyEnemy::OnDeath()
{
	// 사망 시 처리 로직
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Skeleton Body Enemy Died!"));
	}
	
	// 여기에 사망 애니메이션 재생, 사망 효과 등 추가 가능
	// 예: Destroy(); // 즉시 제거
	// 또는 사망 애니메이션 후 제거하는 로직
}

void ACSkeletonBodyEnemy::OnMeleeAttackHit(AActor* HitActor)
{
	// 근접 공격이 플레이어에게 히트했을 때의 처리
	if (HitActor && HitActor->IsA<APawn>())
	{
		if (GEngine)
		{
			float Damage = MeleeAttackComponent->GetMeleeDamage();
			FString DebugMessage = FString::Printf(TEXT("Skeleton Body Melee Attack Hit: %s with %.1f damage!"), *HitActor->GetName(), Damage);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, DebugMessage);
		}
		
		// 여기에 플레이어에게 데미지를 주는 로직 추가
		// 예: HitActor->TakeDamage(MeleeAttackComponent->GetMeleeDamage());
	}
}

void ACSkeletonBodyEnemy::OnProjectileHit(AActor* HitActor)
{
	// 프로젝타일이 플레이어에게 히트했을 때의 처리
	if (HitActor && HitActor->IsA<APawn>())
	{
		if (GEngine)
		{
			float Damage = EnemyProjectileComponent->GetProjectileDamage();
			FString DebugMessage = FString::Printf(TEXT("Skeleton Body Projectile Hit: %s with %.1f damage!"), *HitActor->GetName(), Damage);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, DebugMessage);
		}
		
		// 여기에 플레이어에게 데미지를 주는 로직 추가
		// 예: HitActor->TakeDamage(EnemyProjectileComponent->GetProjectileDamage());
	}
}

