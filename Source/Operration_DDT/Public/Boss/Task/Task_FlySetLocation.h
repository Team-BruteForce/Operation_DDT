// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_FlySetLocation.generated.h"

/**
 * @brief 보스의 호버링 위치 설정을 담당하는 StateTree Task
 * @details 보스가 공중에서 호버링할 때 목표 위치를 설정하고 관리합니다.
 * @author 이효원
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
