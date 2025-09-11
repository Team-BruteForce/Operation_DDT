// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_SideMoveGetLocation.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UTask_SideMoveGetLocation : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	UPROPERTY(EditAnywhere)
	APawn* Boss;
};
