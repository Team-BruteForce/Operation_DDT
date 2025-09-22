// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_Enemy/Projectile/CFlowerProjectile.h"
#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemyRangedATKManager.h"
#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerHitEffectPoolManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DDTPlayer.h"
#include "ODH/ODH_Enemy/Interface/IDamageable.h"
#include "../../Engine/Classes/Engine/DamageEvents.h"

// Sets default values
ACFlowerProjectile::ACFlowerProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트 컴포넌트 생성
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// 콜리전 컴포넌트 생성
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetSphereRadius(20.0f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
	CollisionComponent->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);

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
	TargetPlayer = nullptr;
	PoolManager = nullptr;
}

// Called when the game starts or when spawned
void ACFlowerProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	// 충돌 이벤트 바인딩
	CollisionComponent->OnComponentHit.AddDynamic(this, &ACFlowerProjectile::OnProjectileHit);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACFlowerProjectile::OnProjectileBeginOverlap);
}

// Called every frame
void ACFlowerProjectile::Tick(float DeltaTime)
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

void ACFlowerProjectile::ActivateProjectile(FVector InStartLocation, FVector InTargetLocation, AActor* InTargetPlayer)
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

	// 위치 및 타겟 설정 (타겟 Z를 +100 보정)
	StartLocation = InStartLocation;
	TargetLocation = InTargetLocation;
	TargetLocation.Z += 100.0f;
	TargetPlayer = InTargetPlayer;

	// 프로젝타일을 시작 위치에 배치
	SetActorLocation(InStartLocation);
	PreviousLocation = InStartLocation;

	// 포물선 궤적 계산
	CalculateTrajectory();

	// 비행 시간 초기화
	FlightTime = 0.0f;
	MaxFlightTime_Internal = MaxFlightTime;

	// 현재 속도를 초기 속도로 설정
	CurrentVelocity = InitialVelocity;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, 
			FString::Printf(TEXT("FlowerProjectile Activated: Start(%s) Target(%s)"), 
			*InStartLocation.ToString(), *InTargetLocation.ToString()));
	}
}

void ACFlowerProjectile::DeactivateProjectile()
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
	TargetPlayer = nullptr;
	FlightTime = 0.0f;
	CurrentVelocity = FVector::ZeroVector;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("FlowerProjectile Deactivated"));
	}

	// 풀 매니저로 반환
	ReturnToPool();
}

void ACFlowerProjectile::ReturnToPool()
{
	if (PoolManager)
	{
		PoolManager->ReleaseProjectile(this);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("FlowerProjectile Returned to Pool"));
		}
	}
}

void ACFlowerProjectile::SetPoolManager(ACFlowerEnemyRangedATKManager* Manager)
{
	PoolManager = Manager;
}

void ACFlowerProjectile::CalculateTrajectory()
{
	// 목표 위치까지의 벡터 및 수평/수직 분해
	FVector ToTarget = TargetLocation - StartLocation;
	float HorizontalDistance = FVector(ToTarget.X, ToTarget.Y, 0.0f).Size();
	float VerticalDistance = ToTarget.Z;

	// 거리에 따른 발사각 조절 + 높이 제한
	float AngleDeg;
	if (HorizontalDistance < 500.0f) // 가까운 거리
	{
		AngleDeg = 30.0f; // 낮은 각도
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, 
				FString::Printf(TEXT("FlowerProjectile: 가까운 거리 (%.0fcm) - 각도: %.1f도"), HorizontalDistance, AngleDeg));
		}
	}
	else if (HorizontalDistance < 1000.0f) // 중간 거리
	{
		AngleDeg = 45.0f; // 기본 각도
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, 
				FString::Printf(TEXT("FlowerProjectile: 중간 거리 (%.0fcm) - 각도: %.1f도"), HorizontalDistance, AngleDeg));
		}
	}
	else // 먼 거리
	{
		// 먼 거리에서도 최대 높이 50cm로 제한
		float MaxHeight = 50.0f;
		float MaxAngle = FMath::RadiansToDegrees(FMath::Atan(MaxHeight / HorizontalDistance));
		AngleDeg = FMath::Clamp(MaxAngle, 30.0f, 45.0f); // 최소 30도, 최대 45도로 줄임
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, 
				FString::Printf(TEXT("FlowerProjectile: 먼 거리 (%.0fcm) - 계산된각도: %.1f도, 최종각도: %.1f도, 최대높이: %.0fcm"), 
					HorizontalDistance, MaxAngle, AngleDeg, MaxHeight));
		}
	}

	// 거리에 따른 속도 조절
	float DistanceMultiplier = FMath::Clamp(HorizontalDistance / 800.0f, 0.7f, 1.3f);
	float AdjustedSpeed = ProjectileSpeed * DistanceMultiplier;

	const float AngleRad = FMath::DegreesToRadians(AngleDeg);

	// 초기 속도 벡터 계산 (수평방향 + 상승 성분)
	FVector HorizontalDirection = FVector(ToTarget.X, ToTarget.Y, 0.0f).GetSafeNormal();
	InitialVelocity = HorizontalDirection * (AdjustedSpeed * FMath::Cos(AngleRad)) + FVector(0, 0, AdjustedSpeed * FMath::Sin(AngleRad));

	// 예상 도달 시간: 수평 성분 기준
	float HorizontalSpeed = AdjustedSpeed * FMath::Cos(AngleRad);
	float TimeToTarget = (HorizontalSpeed > KINDA_SMALL_NUMBER) ? (HorizontalDistance / HorizontalSpeed) : 0.25f;

	// 비행 시간 상한 설정: 예상시간 + 0.5초 여유, MaxFlightTime와 비교해 더 작은 값 사용
	MaxFlightTime_Internal = FMath::Min(TimeToTarget + 0.5f, MaxFlightTime);
}

void ACFlowerProjectile::UpdateMovement(float DeltaTime)
{
	if (!bIsActive)
		return;

	FlightTime += DeltaTime;

	// 최대 비행 시간 초과 시 비활성화
	if (FlightTime >= MaxFlightTime_Internal)
	{
		DeactivateProjectile();
		return;
	}

	// 포물선 이동: 바닥 체크 및 중력 적용
	if (CheckGroundCollision())
	{
		DeactivateProjectile();
		return;
	}

	// 중력 적용
	CurrentVelocity.Z -= Gravity * DeltaTime;

	// 위치 업데이트
	FVector NewLocation = GetActorLocation() + CurrentVelocity * DeltaTime;
	PreviousLocation = GetActorLocation(); // 이전 위치 저장
	SetActorLocation(NewLocation);

	// 회전 업데이트 (이동 방향에 맞춰)
	if (!CurrentVelocity.IsNearlyZero())
	{
		FRotator NewRotation = CurrentVelocity.Rotation();
		SetActorRotation(NewRotation);
	}
}

bool ACFlowerProjectile::CheckGroundCollision()
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

void ACFlowerProjectile::OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!bIsActive)
		return;

	// 플레이어와 충돌 시 데미지 적용
	if (OtherActor && OtherActor != this)
	{
		// 플레이어인지 확인
		if (ADDTPlayer* Player = Cast<ADDTPlayer>(OtherActor))
		{
			// 데미지 적용
			FPointDamageEvent DamageEvent;
			DamageEvent.Damage = Damage;
			DamageEvent.HitInfo = Hit;
			DamageEvent.ShotDirection = CurrentVelocity.GetSafeNormal();
			DamageEvent.DamageTypeClass = nullptr;

            Player->TakeDamage(Damage, DamageEvent, nullptr, this);

            // 히트 이펙트: 즉시 스폰(풀링 제거)
            if (HitEffect)
            {
                const FVector EffectLocation = Player->GetActorLocation() + HitEffectOffset;
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, EffectLocation);
            }


			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, 
					FString::Printf(TEXT("FlowerProjectile Hit Player: %f Damage"), Damage));
			}
		}
	}

	// 충돌 시 프로젝타일 비활성화
	DeactivateProjectile();
}

void ACFlowerProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActive)
		return;

	// 플레이어와 오버랩 시 데미지 적용
	if (OtherActor && OtherActor != this)
	{
		if (ADDTPlayer* Player = Cast<ADDTPlayer>(OtherActor))
		{
			// 데미지 적용
			FPointDamageEvent DamageEvent;
			DamageEvent.Damage = Damage;
			DamageEvent.HitInfo = SweepResult;
			DamageEvent.ShotDirection = CurrentVelocity.GetSafeNormal();
			DamageEvent.DamageTypeClass = nullptr;

            Player->TakeDamage(Damage, DamageEvent, nullptr, this);

            // 히트 이펙트: 즉시 스폰(풀링 제거)
            if (HitEffect)
            {
                const FVector EffectLocation = Player->GetActorLocation() + HitEffectOffset;
                UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, EffectLocation);
            }


			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, 
					FString::Printf(TEXT("FlowerProjectile Overlap Player: %f Damage"), Damage));
			}

			// 플레이어와 충돌 시 프로젝타일 비활성화
			DeactivateProjectile();
		}
	}
}

void ACFlowerProjectile::DrawDebugTrajectory()
{
	if (!bIsActive)
		return;

	// 현재 위치에서 다음 프레임 위치까지의 선 그리기
	FVector CurrentLocation = GetActorLocation();
	FVector NextLocation = CurrentLocation + CurrentVelocity * GetWorld()->GetDeltaSeconds();

	DrawDebugLine(
		GetWorld(),
		CurrentLocation,
		NextLocation,
		FColor::Red,
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
}



FVector ACFlowerProjectile::CalculateHitLocation(AActor* HitActor)
{
	if (!HitActor)
		return GetActorLocation();

	// 이전 위치에서 현재 위치까지의 라인 트레이스로 정확한 충돌 지점 찾기
	FVector StartTrace = PreviousLocation;
	FVector EndTrace = GetActorLocation();
	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	QueryParams.AddIgnoredActor(GetOwner());
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartTrace,
		EndTrace,
		ECC_Pawn, // 플레이어와의 충돌 채널
		QueryParams
	);
	
	if (bHit && HitResult.GetActor() == HitActor)
	{
		// 정확한 충돌 지점 반환
		return HitResult.Location;
	}
	
	// 라인 트레이스가 실패하면 플레이어와 프로젝타일 위치의 중간점 사용
	return (GetActorLocation() + HitActor->GetActorLocation()) * 0.5f;
}
