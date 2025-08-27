// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_MotionWarpTargetSetting.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UTask_MotionWarpTargetSetting : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	UPROPERTY(EditAnywhere, meta=(ToolTip="보스 객체"))
	class APawn* Boss;
	UPROPERTY(EditAnywhere, meta=(ToolTip="타겟 위치 (Vector)"))
	FVector TargetLocation;
	UPROPERTY(EditAnywhere, meta=(ToolTip="모션 매칭에 사용할 타겟 이름"))
	FName TargetName = FName("Target");
};
