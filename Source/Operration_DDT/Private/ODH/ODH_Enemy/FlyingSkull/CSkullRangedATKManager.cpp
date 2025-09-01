// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/ODH_Enemy/FlyingSkull/CSkullRangedATKManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

void ACSkullRangedATKManager::BeginPlay()
{
	Super::BeginPlay();

	// 필요 시 시작 시점에 풀을 워밍업
	WarmUp(GetWorld());
}

void ACSkullRangedATKManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Available.Empty();
	Active.Empty();

	Super::EndPlay(EndPlayReason);
}

void ACSkullRangedATKManager::WarmUp(UWorld* WorldContext)
{
	if (!WorldContext || !ProjectileClass)
		return;

	for (int32 i = Available.Num(); i < InitialSize; ++i)
	{
		AActor* NewActor = SpawnOne(WorldContext);
		if (NewActor)
		{
			DeactivateActor(NewActor);
			Available.Add(NewActor);
		}
	}
}

AActor* ACSkullRangedATKManager::AcquireProjectile(UWorld* WorldContext)
{
	if (!WorldContext || !ProjectileClass)
		return nullptr;

	AActor* Result = nullptr;
	while (Available.Num() > 0 && Result == nullptr)
	{
		TWeakObjectPtr<AActor> Candidate = Available.Pop(false);
		if (Candidate.IsValid())
		{
			Result = Candidate.Get();
		}
	}

	if (!Result)
	{
		if (bCanExpand)
		{
			Result = SpawnOne(WorldContext);
		}
		else
		{
			return nullptr;
		}
	}

	if (Result)
	{
		Active.Add(Result);
		ActivateActor(Result);
	}
	return Result;
}

void ACSkullRangedATKManager::ReleaseProjectile(AActor* Projectile)
{
	if (!IsValid(Projectile))
		return;

	DeactivateActor(Projectile);
	Active.Remove(Projectile);
	Available.Add(Projectile);
}

AActor* ACSkullRangedATKManager::SpawnOne(UWorld* WorldContext)
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* NewActor = WorldContext->SpawnActor<AActor>(ProjectileClass, FTransform::Identity, Params);
	return NewActor;
}

void ACSkullRangedATKManager::ActivateActor(AActor* Actor)
{
	if (!Actor) return;
	Actor->SetActorHiddenInGame(false);
	Actor->SetActorEnableCollision(true);
	Actor->SetActorTickEnabled(true);
}

void ACSkullRangedATKManager::DeactivateActor(AActor* Actor)
{
	if (!Actor) return;
	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
	Actor->SetActorTickEnabled(false);
}