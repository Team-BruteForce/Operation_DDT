// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_TargetLocatonGap.generated.h"

/**
 * 
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
