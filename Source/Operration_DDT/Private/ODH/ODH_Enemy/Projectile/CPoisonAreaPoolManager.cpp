// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_Enemy/Projectile/CPoisonAreaPoolManager.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

ACPoisonAreaPoolManager::ACPoisonAreaPoolManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ACPoisonAreaPoolManager::BeginPlay()
{
	Super::BeginPlay();

	// 필요 시 시작 시점에 풀을 워밍업
	WarmUp(GetWorld());
}

void ACPoisonAreaPoolManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Available.Empty();
	Active.Empty();

	Super::EndPlay(EndPlayReason);
}

void ACPoisonAreaPoolManager::WarmUp(UWorld* WorldContext)
{
	if (!WorldContext || !PoisonAreaClass)
		return;

	for (int32 i = Available.Num(); i < InitialSize; ++i)
	{
		ACPoisonArea* NewActor = SpawnOne(WorldContext);
		if (NewActor)
		{
			DeactivateActor(NewActor);
			Available.Add(NewActor);
		}
	}
}

ACPoisonArea* ACPoisonAreaPoolManager::AcquirePoisonArea(UWorld* WorldContext)
{
	if (!WorldContext || !PoisonAreaClass)
		return nullptr;

	ACPoisonArea* Result = nullptr;
	while (Available.Num() > 0 && Result == nullptr)
	{
		TWeakObjectPtr<ACPoisonArea> Candidate = Available.Pop(false);
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

void ACPoisonAreaPoolManager::ReleasePoisonArea(ACPoisonArea* PoisonArea)
{
	if (!IsValid(PoisonArea))
		return;

	DeactivateActor(PoisonArea);
	Active.Remove(PoisonArea);
	Available.Add(PoisonArea);
}

ACPoisonArea* ACPoisonAreaPoolManager::SpawnOne(UWorld* WorldContext)
{
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	ACPoisonArea* NewActor = WorldContext->SpawnActor<ACPoisonArea>(PoisonAreaClass, FTransform::Identity, Params);
	return NewActor;
}

void ACPoisonAreaPoolManager::ActivateActor(ACPoisonArea* Actor)
{
	if (!Actor) return;
	Actor->SetActorHiddenInGame(false);
	Actor->SetActorEnableCollision(true);
	Actor->SetActorTickEnabled(true);
}

void ACPoisonAreaPoolManager::DeactivateActor(ACPoisonArea* Actor)
{
	if (!Actor) return;
	Actor->SetActorHiddenInGame(true);
	Actor->SetActorEnableCollision(false);
	Actor->SetActorTickEnabled(false);
}
