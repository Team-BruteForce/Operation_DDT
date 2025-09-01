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

	MoveAndSweep(DeltaTime);
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
			if (Other->IsA<APawn>())
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
	
	// 디버그 출력
	if (GEngine)
	{
		float Damage = GetProjectileDamage();
		FString DebugMessage = FString::Printf(TEXT("Projectile Hit: %s with %.1f damage!"), *HitActor->GetName(), Damage);
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, DebugMessage);
	}
}

