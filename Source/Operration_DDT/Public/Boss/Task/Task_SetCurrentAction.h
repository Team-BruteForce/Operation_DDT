// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_SetCurrentAction.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UTask_SetCurrentAction : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ToolTip="변경할 변수 넣기"))
	FGameplayTag CurrentActionTag;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ToolTip="변경할 테그 선택"))
	FGameplayTag SelectActionTag;
};
