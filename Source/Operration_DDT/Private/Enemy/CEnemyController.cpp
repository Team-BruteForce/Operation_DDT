// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CEnemyController.h"
#include "Global.h"
#include "Enemy/CEnemyStateTreeAIComponent.h"

ACEnemyController::ACEnemyController(const FObjectInitializer& ObjectInitializer)
{
	CHelpers::CreateActorComponent<UCEnemyStateTreeAIComponent>(this, &EnemyStateTreeAIComponent, "StateTreeAIComponent");
}

void ACEnemyController::BeginPlay()
{
	Super::BeginPlay();
}
