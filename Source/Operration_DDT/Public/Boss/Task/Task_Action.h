// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_Action.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UTask_Action : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tag")
	bool IsAction;
};
