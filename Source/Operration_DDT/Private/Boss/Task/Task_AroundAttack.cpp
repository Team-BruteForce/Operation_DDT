// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task/Task_AroundAttack.h"
#include "Global.h"
#include "Boss/Component/BossProjectileComponent.h"
#include "Boss/Component/FlyingComponent.h"

EStateTreeRunStatus UTask_AroundAttack::EnterState(FStateTreeExecutionContext& Context,
                                                   const FStateTreeTransitionResult& Transition)
{
	UFlyingComponent* FlyingComponent=CHelpers::GetComponent<UFlyingComponent>(Boss);
	UBossProjectileComponent* Projectile=CHelpers::GetComponent<UBossProjectileComponent>(Boss);
	Projectile->SpawnProjectileContinuously(0.5f);
	FlyingComponent->StartSplineSemicircleMovement();
	return EStateTreeRunStatus::Running;
}

void UTask_AroundAttack::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	UFlyingComponent* FlyingComponent = CHelpers::GetComponent<UFlyingComponent>(Boss);
	UBossProjectileComponent* Projectile = CHelpers::GetComponent<UBossProjectileComponent>(Boss);
	
	if (FlyingComponent)
	{
		FlyingComponent->ResetSplineSemicircleMovement();
	}
	 
	if (Projectile)
	{
		Projectile->CancelProjectileContinuousSpawning();
	}
}
