// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_TargetLocatonGap.generated.h"

/**
 * @brief 보스와 타겟 간의 거리 유지를 위한 위치 계산 StateTree Task
 * @details 보스가 타겟과 일정한 거리를 유지하면서 위치를 조정할 때 사용됩니다.
 * @author 이효원
 */
UCLASS()
class OPERRATION_DDT_API UTask_TargetLocatonGap : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

	UPROPERTY(EditAnywhere, meta=(ToolTip="보스 객체"))
	class APawn* Boss;
	UPROPERTY(EditAnywhere, meta=(ToolTip="타겟과 유지할 거리"))
	float KeepDistance;
	UPROPERTY(EditAnywhere, meta=(ToolTip="모션 매칭에 사용할 타겟 이름"))
	FName TargetName = FName("Target");
};
