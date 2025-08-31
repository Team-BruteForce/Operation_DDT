// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_ResetAP.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UTask_ResetAP : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	
	/**
	 * @brief 보스 Pawn 참조
	 */
	UPROPERTY(EditAnywhere)
	class APawn* Boss;
};
