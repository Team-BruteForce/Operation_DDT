// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemyRangedATKManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

void ACFlowerEnemyRangedATKManager::BeginPlay()
{
	Super::BeginPlay();

	// 필요 시 시작 시점에 풀을 워밍업
	WarmUp(GetWorld());
}

void ACFlowerEnemyRangedATKManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Available.Empty();
	Active.Empty();

	Super::EndPlay(EndPlayReason);
}

void ACFlowerEnemyRangedATKManager::WarmUp(UWorld* WorldContext)
{
	if (!WorldContext || !ProjectileClass)
		return;

	for (int32 i = Available.Num(); i < InitialSize; ++i)
	{
		ACFlowerProjectile* NewActor = SpawnOne(WorldContext);
		if (NewActor)
		{
			DeactivateActor(NewActor);
			Available.Add(NewActor);
		}
	}
}

ACFlowerProjectile* ACFlowerEnemyRangedATKManager::AcquireProjectile(UWorld* WorldContext)
{
	if (!WorldContext || !ProjectileClass)
		return nullptr;

	ACFlowerProjectile* Result = nullptr;
	while (Available.Num() > 0 && Result == nullptr)
	{
		TWeakObjectPtr<ACFlowerProjectile> Candidate = Available.Pop(false);
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

void ACFlowerEnemyRangedATKManager::ReleaseProjectile(ACFlowerProjectile* Projectile)
{
	if (!IsValid(Projectile))
		return;

	DeactivateActor(Projectile);
	Active.Remove(Projectile);
	Available.Add(Projectile);
}

ACFlowerProjectile* ACFlowerEnemyRangedATKManager::SpawnOne(UWorld* WorldContext)
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ACFlowerProjectile* NewActor = WorldContext->SpawnActor<ACFlowerProjectile>(ProjectileClass, FTransform::Identity, Params);
	return NewActor;
}

void ACFlowerEnemyRangedATKManager::ActivateActor(ACFlowerProjectile* Actor)
{
	if (!Actor) return;
	Actor->SetActorHiddenInGame(false);
	Actor->SetActorEnableCollision(true);
	Actor->SetActorTickEnabled(true);
}

void ACFlowerEnemyRangedATKManager::DeactivateActor(ACFlowerProjectile* Actor)
{
	if (!Actor) return;
	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
	Actor->SetActorTickEnabled(false);
}
