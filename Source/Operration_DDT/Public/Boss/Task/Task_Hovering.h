// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_Hovering.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UTask_Hovering : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	/**
* @brief 상태 진입 시 호출되는 함수
*/
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	
	// virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
	/**
 * @brief 상태 종료 시 호출되는 함수
 */
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	UPROPERTY(EditAnywhere)
	class APawn* Boss;
};
