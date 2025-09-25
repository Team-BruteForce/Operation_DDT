// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_SideMoveGetLocation.generated.h"

/**
 * @brief 보스의 측면 이동 위치를 계산하는 StateTree Task
 * @details 보스가 플레이어의 측면으로 이동할 때 목표 위치를 계산합니다.
 * @author 이효원
 */
UCLASS()
class OPERRATION_DDT_API UTask_SideMoveGetLocation : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	UPROPERTY(EditAnywhere)
	APawn* Boss;
};
