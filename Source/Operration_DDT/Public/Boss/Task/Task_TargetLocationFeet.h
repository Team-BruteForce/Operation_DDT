// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_TargetLocationFeet.generated.h"

/**
 * @brief 보스의 발 위치 기반 타겟 위치를 설정하는 StateTree Task
 * @details 보스의 발 위치를 기준으로 타겟 위치를 계산하여 모션 매칭에 사용합니다.
 * @author 이효원
 */
UCLASS()
class OPERRATION_DDT_API UTask_TargetLocationFeet : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

	UPROPERTY(EditAnywhere, meta=(ToolTip="보스 객체"))
	class APawn* Boss;
	UPROPERTY(VisibleAnywhere, meta=(ToolTip="타겟 위치 (Vector)"))
	FVector TargetLocation;
	UPROPERTY(EditAnywhere, meta=(ToolTip="모션 매칭에 사용할 타겟 이름"))
	FName TargetName = FName("Target");
};
