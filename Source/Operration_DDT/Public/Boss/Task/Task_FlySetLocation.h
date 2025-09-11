// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_FlySetLocation.generated.h"

/**
 * 보스 호버링 관련 StateTree Task
 */
UCLASS()
class OPERRATION_DDT_API UTask_FlySetLocation : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

protected:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	UPROPERTY(EditAnywhere)
	class APawn* Boss;
	bool bStarted=false;
	
};
