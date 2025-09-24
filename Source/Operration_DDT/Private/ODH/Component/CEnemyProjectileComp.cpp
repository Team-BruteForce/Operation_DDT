// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/Component/CEnemyProjectileComp.h"

#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DrawDebugHelpers.h"
#include "ODH/ODH_Enemy/FlyingSkull/CSkullRangedATKManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "UObject/UnrealType.h"
#include "Player/DDTPlayer.h"
#include "NiagaraFunctionLibrary.h"
#include "ODH/Component/CSoundCollectionComponent.h"

namespace
{
	static ACSkullRangedATKManager* FindSkullRangedATKManager(UWorld* World)
	{
		if (!World) return nullptr;
		AActor* Found = UGameplayStatics::GetActorOfClass(World, ACSkullRangedATKManager::StaticClass());
		return Cast<ACSkullRangedATKManager>(Found);
	}
}

UCEnemyProjectileComp::UCEnemyProjectileComp()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCEnemyProjectileComp::BeginPlay()
{
	Super::BeginPlay();

	TryInheritTargetFromSpawner();
	ComputeInitialDirection();
}

void UCEnemyProjectileComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	LifeTimer += DeltaTime;
	if (LifeTimer >= MaxLifeSeconds)
	{
		if (AActor* OwnerActor = GetOwner())
		{
			if (UWorld* World = GetWorld())
			{
				if (ACSkullRangedATKManager* Pool = FindSkullRangedATKManager(World))
				{
					Pool->ReleaseProjectile(OwnerActor);
					return;
				}
			}
			OwnerActor->Destroy();
		}
		return;
	}

	if (!bInitialized)
	{
		TryInheritTargetFromSpawner();
		ComputeInitialDirection();
	}

	// 블루프린트 콜리전을 사용하지 않는 경우에만 스윕 사용
	if (!bUseBlueprintCollision)
	{
		// 베지어 곡선 이동이 활성화된 경우
		if (IsBazierCurves)
		{
			MoveAlongBezierCurve(DeltaTime);
		}
		else
		{
			MoveAndSweep(DeltaTime);
		}
	}
	else
	{
		// 블루프린트 콜리전 사용 시
		if (IsBazierCurves)
		{
			MoveAlongBezierCurve(DeltaTime);
		}
		else
		{
			// 단순 이동
			AActor* OwnerActor = GetOwner();
			if (OwnerActor && !FlightDirection.IsNearlyZero())
			{
				FVector NewLocation = OwnerActor->GetActorLocation() + FlightDirection * ProjectileSpeed * DeltaTime;
				OwnerActor->SetActorLocation(NewLocation, false);
			}
		}
	}
}

void UCEnemyProjectileComp::InitializeTarget(AActor* InTargetPlayer)
{
	TargetPlayer = InTargetPlayer;
	LifeTimer = 0.0f;
	ComputeInitialDirection();
}

float UCEnemyProjectileComp::GetProjectileDamage() const
{
	// 소유자 액터에서 StatusComponent를 찾아서 공격력 반환
	AActor* Owner = GetOwner();
	if (Owner)
	{
		UCEnemyStatusComponent* StatusComponent = Owner->FindComponentByClass<UCEnemyStatusComponent>();
		if (StatusComponent)
		{
			return StatusComponent->GetAttackPower();
		}
	}
	
	// StatusComponent를 찾을 수 없는 경우 기본값 반환
	return 20.0f;
}

void UCEnemyProjectileComp::TryInheritTargetFromSpawner()
{
	if (TargetPlayer)
		return;

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
		return;

	APawn* InstigatorPawn = OwnerActor->GetInstigator();
	if (InstigatorPawn)
	{
		UClass* InstigatorClass = InstigatorPawn->GetClass();
		FProperty* FoundProp = InstigatorClass->FindPropertyByName(FName(TEXT("TargetPlayer")));
		if (FoundProp)
		{
			FObjectProperty* ObjProp = CastField<FObjectProperty>(FoundProp);
			if (ObjProp && ObjProp->PropertyClass->IsChildOf(AActor::StaticClass()))
			{
				AActor* const* TargetPtr = ObjProp->ContainerPtrToValuePtr<AActor*>(InstigatorPawn);
				if (TargetPtr && *TargetPtr)
				{
					TargetPlayer = *TargetPtr;
				}
			}
		}
	}

	if (!TargetPlayer)
	{
		UClass* OwnerClass = OwnerActor->GetClass();
		FProperty* FoundProp = OwnerClass->FindPropertyByName(FName(TEXT("TargetPlayer")));
		if (FoundProp)
		{
			FObjectProperty* ObjProp = CastField<FObjectProperty>(FoundProp);
			if (ObjProp && ObjProp->PropertyClass->IsChildOf(AActor::StaticClass()))
			{
				AActor* const* TargetPtr = ObjProp->ContainerPtrToValuePtr<AActor*>(OwnerActor);
				if (TargetPtr && *TargetPtr)
				{
					TargetPlayer = *TargetPtr;
				}
			}
		}
	}
}

void UCEnemyProjectileComp::ComputeInitialDirection()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
		return;

	FVector From = OwnerActor->GetActorLocation();
	FVector To = From + OwnerActor->GetActorForwardVector();

	if (TargetPlayer)
	{
		To = TargetPlayer->GetActorLocation();
	}

	FlightDirection = (To - From).GetSafeNormal();
	bInitialized = FlightDirection.IsNearlyZero() == false;

	// 베지어 곡선 이동이 활성화된 경우 곡선 초기화
	if (IsBazierCurves)
	{
		InitializeBezierCurve();
	}
}

void UCEnemyProjectileComp::MoveAndSweep(float DeltaTime)
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor || FlightDirection.IsNearlyZero())
		return;

	FVector Start = OwnerActor->GetActorLocation();
	FVector End = Start + FlightDirection * ProjectileSpeed * DeltaTime;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(EnemyProjectileMove), false, OwnerActor);
	Params.bReturnPhysicalMaterial = false;

	FHitResult Hit;
	const float Radius = 8.0f;
	bool bHit = GetWorld()->SweepSingleByChannel(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(Radius),
		Params
	);

	if (bHit)
	{
		OnHitAndMaybeDestroy(Hit);
	}
	else
	{
		OwnerActor->SetActorLocation(End, false);
	}
}

void UCEnemyProjectileComp::OnHitAndMaybeDestroy(const FHitResult& Hit)
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
		return;

	AActor* Other = Hit.GetActor();
	bool bShouldFinish = false;

	if (Other == nullptr)
	{
		bShouldFinish = true;
	}
	else
	{
		ECollisionChannel Channel = Hit.Component.IsValid() ? Hit.Component->GetCollisionObjectType() : ECC_WorldStatic;
		if (Channel == ECC_WorldStatic || Channel == ECC_WorldDynamic || Channel == ECC_Pawn || Other->ActorHasTag(FName(TEXT("Player"))))
		{
			bShouldFinish = true;
			
			// 플레이어에게 데미지 주기
			if (Other->IsA<ADDTPlayer>())
			{
				DealProjectileDamage(Other);
			}
		}
	}

	if (!bShouldFinish)
		return;

	if (UWorld* World = GetWorld())
	{
		if (ACSkullRangedATKManager* Pool = FindSkullRangedATKManager(World))
		{
			Pool->ReleaseProjectile(OwnerActor);
			return;
		}
	}

	OwnerActor->Destroy();
}

void UCEnemyProjectileComp::DealProjectileDamage(AActor* HitActor)
{
	if (!HitActor)
		return;

	// 프로젝타일 히트 이벤트 발생
	OnProjectileHit.Broadcast(HitActor);

	// 실제 데미지 적용
	float Damage = GetProjectileDamage();
	AActor* OwnerActor = GetOwner();
	AController* InstigatorController = OwnerActor ? OwnerActor->GetInstigatorController() : nullptr;
	UGameplayStatics::ApplyDamage(HitActor, Damage, InstigatorController, OwnerActor, nullptr);

	// 히트 이펙트 스폰 (나이아가라)
	if (HitEffect)
	{
		FVector EffectLocation = HitActor->GetActorLocation() + EffectSpawnOffset;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HitEffect, EffectLocation);
	}
	
	// 히트 사운드 재생
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitSound, HitActor->GetActorLocation());
	}


}

void UCEnemyProjectileComp::RegisterCollisionComponent(UPrimitiveComponent* CollisionComponent)
{
	if (!CollisionComponent)
		return;

	// 기존 등록된 컴포넌트가 있다면 델리게이트 해제
	if (RegisteredCollisionComponent)
	{
		RegisteredCollisionComponent->OnComponentBeginOverlap.RemoveAll(this);
		RegisteredCollisionComponent->OnComponentHit.RemoveAll(this);
	}

	// 새 컴포넌트 등록
	RegisteredCollisionComponent = CollisionComponent;

	// 델리게이트 바인딩
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &UCEnemyProjectileComp::OnBlueprintCollisionOverlap);
	CollisionComponent->OnComponentHit.AddDynamic(this, &UCEnemyProjectileComp::OnBlueprintCollisionHit);

	// 블루프린트 콜리전 사용 모드로 전환
	bUseBlueprintCollision = true;


}

void UCEnemyProjectileComp::OnBlueprintCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
		return;

	// 자기 자신은 제외
	if (OtherActor == GetOwner())
		return;

	// 플레이어인지 확인
	if (OtherActor->IsA<ADDTPlayer>())
	{
		// 데미지 처리
		DealProjectileDamage(OtherActor);

		// 투사체 파괴
		if (AActor* OwnerActor = GetOwner())
		{
            if (UCSoundCollectionComponent* SoundComp = OwnerActor->FindComponentByClass<UCSoundCollectionComponent>())
            {
                SoundComp->PlayRangedAttackHitSound();
            }
			
			if (UWorld* World = GetWorld())
			{
				if (ACSkullRangedATKManager* Pool = FindSkullRangedATKManager(World))
				{
					Pool->ReleaseProjectile(OwnerActor);
					return;
				}
			}
			OwnerActor->Destroy();
		}
	}
}

void UCEnemyProjectileComp::OnBlueprintCollisionHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor)
		return;

	// 자기 자신은 제외
	if (OtherActor == GetOwner())
		return;

	// 플레이어인지 확인
	if (OtherActor->IsA<ADDTPlayer>())
	{
		// 데미지 처리
		DealProjectileDamage(OtherActor);
	}

	// 벽이나 다른 오브젝트에 맞았을 때도 파괴
	if (AActor* OwnerActor = GetOwner())
	{
		if (UWorld* World = GetWorld())
		{
			if (ACSkullRangedATKManager* Pool = FindSkullRangedATKManager(World))
			{
				Pool->ReleaseProjectile(OwnerActor);
				return;
			}
		}
		OwnerActor->Destroy();
	}
}

void UCEnemyProjectileComp::InitializeBezierCurve()
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor || !TargetPlayer)
		return;

	// 시작점: 현재 위치
	BezierStartPoint = OwnerActor->GetActorLocation();
	
	// 끝점: 타겟 위치
	BezierEndPoint = TargetPlayer->GetActorLocation();
	
	// 랜덤 오프셋 설정 (300 또는 -300)
	BezierCurveOffset = (FMath::RandRange(0, 1) == 0) ? 300.0f : -300.0f;
	
	// 중간 제어점: 시작점과 끝점 사이의 중점에서 높이와 오프셋 적용
	FVector MidPoint = (BezierStartPoint + BezierEndPoint) * 0.5f;
	
	// 오프셋 방향 계산 (시작점에서 끝점으로의 방향에 수직)
	FVector DirectionToTarget = (BezierEndPoint - BezierStartPoint).GetSafeNormal();
	FVector RightVector = FVector::CrossProduct(DirectionToTarget, FVector::UpVector).GetSafeNormal();
	
	// 제어점 위치 계산
	BezierControlPoint = MidPoint + FVector::UpVector * BezierCurveHeight + RightVector * BezierCurveOffset;
	
	// 진행률 초기화
	BezierProgress = 0.0f;
	
// 	// 디버그 출력
// 	if (GEngine)
// 	{
// 		FString DirectionText = (BezierCurveOffset > 0) ? TEXT("Right") : TEXT("Left");
// 		FString DebugMessage = FString::Format(TEXT("Bezier Curve Initialized - {0} curve (Offset: {1}) - Start: {2}, Control: {3}, End: {4}"), 
// 			DirectionText, BezierCurveOffset, BezierStartPoint.ToString(), BezierControlPoint.ToString(), BezierEndPoint.ToString());
// 		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Cyan, DebugMessage);
// 	}
}

void UCEnemyProjectileComp::MoveAlongBezierCurve(float DeltaTime)
{
	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
		return;

	// 진행률 업데이트 (속도에 따라)
	float Distance = FVector::Dist(BezierStartPoint, BezierEndPoint);
	float SpeedFactor = (ProjectileSpeed * DeltaTime) / FMath::Max(Distance, 1.0f);
	BezierProgress += SpeedFactor;
	
	// 진행률을 0~1 사이로 클램프
	BezierProgress = FMath::Clamp(BezierProgress, 0.0f, 1.0f);
	
	// 베지어 곡선상의 현재 위치 계산
	FVector NewLocation = CalculateBezierPoint(BezierProgress);
	OwnerActor->SetActorLocation(NewLocation, false);
	
	// 방향 업데이트 (이동 방향으로 회전)
	if (BezierProgress < 1.0f)
	{
		FVector NextLocation = CalculateBezierPoint(FMath::Min(BezierProgress + 0.01f, 1.0f));
		FVector MoveDirection = (NextLocation - NewLocation).GetSafeNormal();
		if (!MoveDirection.IsNearlyZero())
		{
			OwnerActor->SetActorRotation(MoveDirection.Rotation());
		}
	}
	
	// 곡선 완료 시 타겟 도달로 처리
	if (BezierProgress >= 1.0f)
	{
		// 타겟 도달 시 파괴 처리
		if (UWorld* World = GetWorld())
		{
			if (ACSkullRangedATKManager* Pool = FindSkullRangedATKManager(World))
			{
				Pool->ReleaseProjectile(OwnerActor);
				return;
			}
		}
		OwnerActor->Destroy();
	}
}

FVector UCEnemyProjectileComp::CalculateBezierPoint(float t)
{
	// 2차 베지어 곡선 공식: B(t) = (1-t)²P₀ + 2(1-t)tP₁ + t²P₂
	float u = 1.0f - t;
	float tt = t * t;
	float uu = u * u;
	float uut = 2.0f * u * t;
	
	return (uu * BezierStartPoint) + (uut * BezierControlPoint) + (tt * BezierEndPoint);
}

