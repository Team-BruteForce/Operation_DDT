// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_Enemy/Projectile/CPoisonProjectile.h"
#include "ODH/ODH_Enemy/Projectile/CPoisonArea.h"
#include "ODH/ODH_Enemy/Projectile/CPoisonProjectilePoolManager.h"
#include "ODH/ODH_Enemy/Projectile/CPoisonAreaPoolManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACPoisonProjectile::ACPoisonProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트 컴포넌트 생성
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// 콜리전 컴포넌트 생성
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetSphereRadius(25.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 바닥 충돌만 감지하므로 비활성화

	// 나이아가라 이펙트 컴포넌트 생성
	ProjectileEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileEffect"));
	ProjectileEffect->SetupAttachment(RootComponent);
	ProjectileEffect->SetAutoActivate(false); // 수동으로 활성화

	// 초기에는 비활성화 상태
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	// 초기값 설정
	bIsActive = false;
	FlightTime = 0.0f;
	MaxFlightTime_Internal = MaxFlightTime;
	MovementPhase = 0;
	PhaseTime = 0.0f;
	PoolManager = nullptr;
}

// Called when the game starts or when spawned
void ACPoisonProjectile::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACPoisonProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActive)
	{
		UpdateMovement(DeltaTime);
		
		// 디버그 그리기
		if (bShowDebugTrajectory)
		{
			DrawDebugTrajectory();
		}
	}
}

void ACPoisonProjectile::ActivateProjectile(FVector InStartLocation, FVector InTargetLocation)
{
	// 프로젝타일 활성화
	bIsActive = true;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	// 나이아가라 이펙트 활성화
	if (ProjectileEffect)
	{
		if (ProjectileEffectAsset)
		{
			ProjectileEffect->SetAsset(ProjectileEffectAsset);
		}
		ProjectileEffect->Activate();
	}

	// 위치 및 타겟 설정
	StartLocation = InStartLocation;
	TargetLocation = InTargetLocation;
	
	// 최고점 위치 계산 (시작 위치에서 위로 올라간 지점)
	PeakLocation = InStartLocation + FVector(0, 0, LaunchHeight);

	// 프로젝타일을 시작 위치에 배치
	SetActorLocation(InStartLocation);

	// 비행 시간 초기화
	FlightTime = 0.0f;
	MaxFlightTime_Internal = MaxFlightTime;
	MovementPhase = 0; // 위로 올라가기 단계
	PhaseTime = 0.0f;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, 
			FString::Printf(TEXT("PoisonProjectile Activated: Start(%s) Target(%s)"), 
			*InStartLocation.ToString(), *InTargetLocation.ToString()));
	}
}

void ACPoisonProjectile::DeactivateProjectile()
{
	// 프로젝타일 비활성화
	bIsActive = false;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	// 나이아가라 이펙트 비활성화
	if (ProjectileEffect)
	{
		ProjectileEffect->Deactivate();
	}

	// 변수 초기화
	FlightTime = 0.0f;
	MovementPhase = 0;
	PhaseTime = 0.0f;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("PoisonProjectile Deactivated"));
	}

	// 풀 매니저로 반환
	ReturnToPool();
}

void ACPoisonProjectile::ReturnToPool()
{
	if (PoolManager)
	{
		PoolManager->ReleaseProjectile(this);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("PoisonProjectile Returned to Pool"));
		}
	}
}

void ACPoisonProjectile::SetPoolManager(ACPoisonProjectilePoolManager* Manager)
{
	PoolManager = Manager;
}

void ACPoisonProjectile::UpdateMovement(float DeltaTime)
{
	if (!bIsActive)
		return;

	FlightTime += DeltaTime;
	PhaseTime += DeltaTime;

	// 최대 비행 시간 초과 시 비활성화
	if (FlightTime >= MaxFlightTime_Internal)
	{
		DeactivateProjectile();
		return;
	}

	// 바닥 충돌 체크 (떨어지는 단계에서만)
	if (MovementPhase == 1 && CheckGroundCollision())
	{
		// PoisonArea 생성 후 비활성화
		SpawnPoisonArea(GetActorLocation());
		DeactivateProjectile();
		return;
	}

	FVector NewLocation;
	float Alpha;

	switch (MovementPhase)
	{
	case 0: // 위로 올라가기
		Alpha = FMath::Clamp(PhaseTime / LaunchDuration, 0.0f, 1.0f);
		NewLocation = FMath::Lerp(StartLocation, PeakLocation, Alpha);
		
		if (Alpha >= 1.0f)
		{
			// 다음 단계로 전환 (떨어지기)
			MovementPhase = 1;
			PhaseTime = 0.0f;
		}
		break;

	case 1: // 아래로 떨어지기
		Alpha = FMath::Clamp(PhaseTime / FallDuration, 0.0f, 1.0f);
		NewLocation = FMath::Lerp(PeakLocation, TargetLocation, Alpha);
		break;

	default:
		NewLocation = GetActorLocation();
		break;
	}

	SetActorLocation(NewLocation);

	// 회전 업데이트 (떨어지는 방향으로)
	if (MovementPhase == 1)
	{
		FVector FallDirection = (TargetLocation - PeakLocation).GetSafeNormal();
		if (!FallDirection.IsNearlyZero())
		{
			FRotator NewRotation = FallDirection.Rotation();
			SetActorRotation(NewRotation);
		}
	}
}

bool ACPoisonProjectile::CheckGroundCollision()
{
	FVector CurrentLocation = GetActorLocation();
	FVector StartTrace = CurrentLocation;
	FVector EndTrace = CurrentLocation + FVector(0, 0, -GroundCheckDistance);

	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartTrace,
		EndTrace,
		GroundCollisionChannel,
		QueryParams
	);

	return bHit;
}

void ACPoisonProjectile::SpawnPoisonArea(FVector Location)
{
	UWorld* World = GetWorld();
	if (!World)
		return;

	// PoisonArea 풀 매니저 찾기
	ACPoisonAreaPoolManager* PoisonAreaPool = Cast<ACPoisonAreaPoolManager>(UGameplayStatics::GetActorOfClass(World, ACPoisonAreaPoolManager::StaticClass()));
	
	if (PoisonAreaPool)
	{
		// 풀에서 PoisonArea 대여
		ACPoisonArea* PooledPoisonArea = PoisonAreaPool->AcquirePoisonArea(World);
		if (PooledPoisonArea)
		{
			PooledPoisonArea->SetActorLocation(Location);
			PooledPoisonArea->SetOwner(GetOwner());
			PooledPoisonArea->SetInstigator(GetInstigator());
			
			// PoisonArea 활성화
			PooledPoisonArea->ActivatePoisonArea(10.0f); // 10초간 지속

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, 
					FString::Printf(TEXT("PoisonArea spawned from pool at %s"), *Location.ToString()));
			}
			return;
		}
	}

	// 풀을 사용하지 못하면 기존 스폰 방식으로 폴백
	if (!PoisonAreaClass)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, 
				TEXT("PoisonAreaClass not set in PoisonProjectile!"));
		}
		return;
	}

	// PoisonArea 스폰
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = GetInstigator();

	ACPoisonArea* SpawnedPoisonArea = World->SpawnActor<ACPoisonArea>(
		PoisonAreaClass,
		Location,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (SpawnedPoisonArea)
	{
		// PoisonArea 활성화
		SpawnedPoisonArea->ActivatePoisonArea(10.0f); // 10초간 지속

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, 
				FString::Printf(TEXT("PoisonArea spawned (fallback) at %s"), *Location.ToString()));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, 
				TEXT("Failed to spawn PoisonArea!"));
		}
	}
}

void ACPoisonProjectile::DrawDebugTrajectory()
{
	if (!bIsActive)
		return;

	// 현재 위치에서 다음 프레임 위치까지의 선 그리기
	FVector CurrentLocation = GetActorLocation();

	// 시작점에서 최고점까지
	DrawDebugLine(
		GetWorld(),
		StartLocation,
		PeakLocation,
		FColor::Magenta,
		false,
		0.1f,
		0,
		2.0f
	);

	// 최고점에서 목표점까지
	DrawDebugLine(
		GetWorld(),
		PeakLocation,
		TargetLocation,
		FColor::Magenta,
		false,
		0.1f,
		0,
		2.0f
	);

	// 목표 지점 표시
	DrawDebugSphere(
		GetWorld(),
		TargetLocation,
		50.0f,
		8,
		FColor::Green,
		false,
		0.1f,
		0,
		2.0f
	);

	// 최고점 표시
	DrawDebugSphere(
		GetWorld(),
		PeakLocation,
		30.0f,
		8,
		FColor::Yellow,
		false,
		0.1f,
		0,
		2.0f
	);
}
