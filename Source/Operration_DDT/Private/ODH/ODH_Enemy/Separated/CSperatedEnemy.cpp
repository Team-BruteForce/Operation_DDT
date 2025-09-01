// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/ODH_Enemy/Separated/CSperatedEnemy.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h"
#include "ODH/Component/CEnemyProjectileComp.h"
#include "Engine/Engine.h"

// Sets default values
ACSperatedEnemy::ACSperatedEnemy()
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
void ACSperatedEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	// Separated Enemy 전용 스테이터스 설정
	if (StatusComponent)
	{
		// Separated Enemy는 체력이 매우 높고 공격력이 낮음 (탱커 역할)
		StatusComponent->SetMaxHealth(200.0f);
		StatusComponent->SetCurrentHealth(200.0f);
		StatusComponent->SetAttackPower(15.0f);
		StatusComponent->SetDefensePower(12.0f);
		
		// 사망 이벤트 바인딩
		StatusComponent->OnDeath.AddDynamic(this, &ACSperatedEnemy::OnDeath);
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, 
				FString::Printf(TEXT("Separated Enemy Spawned - Health: %.0f, Attack: %.0f"), 
				StatusComponent->GetHealthPercent() * 100, StatusComponent->GetAttackPower()));
		}
	}
	
	// 근접 공격 컴포넌트의 히트 이벤트에 바인딩
	if (MeleeAttackComponent)
	{
		MeleeAttackComponent->OnMeleeAttackHit.AddDynamic(this, &ACSperatedEnemy::OnMeleeAttackHit);
	}
	
	// 원거리 공격 컴포넌트의 히트 이벤트에 바인딩
	if (EnemyProjectileComponent)
	{
		EnemyProjectileComponent->OnProjectileHit.AddDynamic(this, &ACSperatedEnemy::OnProjectileHit);
	}
}

// Called every frame
void ACSperatedEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACSperatedEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// IDamageable 인터페이스 구현
void ACSperatedEnemy::TakeDamage_Implementation(float DamageAmount)
{
	if (StatusComponent)
	{
		StatusComponent->TakeDamage(DamageAmount);
	}
}

bool ACSperatedEnemy::CanTakeDamage_Implementation() const
{
	if (StatusComponent)
	{
		return !StatusComponent->IsDead();
	}
	return false;
}

bool ACSperatedEnemy::IsDead_Implementation() const
{
	if (StatusComponent)
	{
		return StatusComponent->IsDead();
	}
	return false;
}

// 공격 함수들
void ACSperatedEnemy::PlayMeleeAttack()
{
	if (MeleeAttackComponent)
	{
		MeleeAttackComponent->ActivateMeleeAttack();
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, TEXT("Separated Enemy Melee Attack!"));
		}
	}
}

void ACSperatedEnemy::PlayRangedAttack()
{
	if (EnemyProjectileComponent)
	{
		// 여기에 프로젝타일 발사 로직 추가
		// 예: 프로젝타일 액터 스폰 및 초기화
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, TEXT("Separated Enemy Ranged Attack!"));
		}
	}
}

// 원거리 공격 오브젝트 소환 함수들
void ACSperatedEnemy::SpawnRangedProjectile(AActor* TargetPlayer)
{
	if (!TargetPlayer)
		return;
	
	// 기본 발사 위치 계산
	FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
	FRotator SpawnRotation = GetActorRotation();
	
	SpawnRangedProjectileAtLocation(TargetPlayer, SpawnLocation, SpawnRotation);
}

void ACSperatedEnemy::SpawnRangedProjectileAtLocation(AActor* TargetPlayer, FVector SpawnLocation, FRotator SpawnRotation)
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
				TEXT("ProjectileClass not set in Separated Enemy!"));
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
					FString::Printf(TEXT("Separated Enemy spawned projectile targeting %s"), *TargetPlayer->GetName()));
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

TSubclassOf<AActor> ACSperatedEnemy::GetProjectileClass() const
{
	return ProjectileClass;
}

void ACSperatedEnemy::OnDeath()
{
	// 사망 시 처리 로직
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Separated Enemy Died!"));
	}
	
	// 여기에 사망 애니메이션 재생, 사망 효과 등 추가 가능
	// 예: Destroy(); // 즉시 제거
	// 또는 사망 애니메이션 후 제거하는 로직
}

void ACSperatedEnemy::OnMeleeAttackHit(AActor* HitActor)
{
	// 근접 공격이 플레이어에게 히트했을 때의 처리
	if (HitActor && HitActor->IsA<APawn>())
	{
		if (GEngine)
		{
			float Damage = MeleeAttackComponent->GetMeleeDamage();
			FString DebugMessage = FString::Printf(TEXT("Separated Enemy Melee Attack Hit: %s with %.1f damage!"), *HitActor->GetName(), Damage);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, DebugMessage);
		}
		
		// 여기에 플레이어에게 데미지를 주는 로직 추가
		// 예: HitActor->TakeDamage(MeleeAttackComponent->GetMeleeDamage());
	}
}

void ACSperatedEnemy::OnProjectileHit(AActor* HitActor)
{
	// 프로젝타일이 플레이어에게 히트했을 때의 처리
	if (HitActor && HitActor->IsA<APawn>())
	{
		if (GEngine)
		{
			float Damage = EnemyProjectileComponent->GetProjectileDamage();
			FString DebugMessage = FString::Printf(TEXT("Separated Enemy Projectile Hit: %s with %.1f damage!"), *HitActor->GetName(), Damage);
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, DebugMessage);
		}
		
		// 여기에 플레이어에게 데미지를 주는 로직 추가
		// 예: HitActor->TakeDamage(EnemyProjectileComponent->GetProjectileDamage());
	}
}

