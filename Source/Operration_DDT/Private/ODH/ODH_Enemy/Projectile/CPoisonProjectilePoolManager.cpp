// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_Enemy/Projectile/CPoisonProjectilePoolManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

ACPoisonProjectilePoolManager::ACPoisonProjectilePoolManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ACPoisonProjectilePoolManager::BeginPlay()
{
	Super::BeginPlay();

	// 필요 시 시작 시점에 풀을 워밍업
	WarmUp(GetWorld());
}

void ACPoisonProjectilePoolManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Available.Empty();
	Active.Empty();

	Super::EndPlay(EndPlayReason);
}

void ACPoisonProjectilePoolManager::WarmUp(UWorld* WorldContext)
{
	if (!WorldContext || !ProjectileClass)
		return;

	for (int32 i = Available.Num(); i < InitialSize; ++i)
	{
		ACPoisonProjectile* NewActor = SpawnOne(WorldContext);
		if (NewActor)
		{
			DeactivateActor(NewActor);
			Available.Add(NewActor);
		}
	}
}

ACPoisonProjectile* ACPoisonProjectilePoolManager::AcquireProjectile(UWorld* WorldContext)
{
	if (!WorldContext || !ProjectileClass)
		return nullptr;

	ACPoisonProjectile* Result = nullptr;
	while (Available.Num() > 0 && Result == nullptr)
	{
		TWeakObjectPtr<ACPoisonProjectile> Candidate = Available.Pop(false);
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
		// 풀 매니저 참조 설정
		Result->SetPoolManager(this);
	}
	return Result;
}

void ACPoisonProjectilePoolManager::ReleaseProjectile(ACPoisonProjectile* Projectile)
{
	if (!IsValid(Projectile))
		return;

	DeactivateActor(Projectile);
	Active.Remove(Projectile);
	Available.Add(Projectile);
}

ACPoisonProjectile* ACPoisonProjectilePoolManager::SpawnOne(UWorld* WorldContext)
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ACPoisonProjectile* NewActor = WorldContext->SpawnActor<ACPoisonProjectile>(ProjectileClass, FTransform::Identity, Params);
	return NewActor;
}

void ACPoisonProjectilePoolManager::ActivateActor(ACPoisonProjectile* Actor)
{
	if (!Actor) return;
	Actor->SetActorHiddenInGame(false);
	Actor->SetActorEnableCollision(true);
	Actor->SetActorTickEnabled(true);
}

void ACPoisonProjectilePoolManager::DeactivateActor(ACPoisonProjectile* Actor)
{
	if (!Actor) return;
	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
	Actor->SetActorTickEnabled(false);
}
