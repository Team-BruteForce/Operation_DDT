// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_SetVectorTargetLocation.generated.h"

/**
 * @brief 보스의 벡터 타겟 위치를 설정하는 StateTree Task
 * @details 보스가 특정 벡터 위치를 목표로 하여 이동할 때 사용됩니다.
 * @author 이효원
 */
UCLASS()
class OPERRATION_DDT_API UTask_SetVectorTargetLocation : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

	UPROPERTY(EditAnywhere, meta=(ToolTip="보스 객체"))
	class APawn* Boss;
	UPROPERTY(EditAnywhere, meta=(ToolTip="타겟 위치 (Vector)"))
	FVector TargetLocation;
	UPROPERTY(EditAnywhere, meta=(ToolTip="모션 매칭에 사용할 타겟 이름"))
	FName TargetName = FName("Target");
};
