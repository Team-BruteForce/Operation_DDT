// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_SetCurrentAction.generated.h"

/**
 * @brief 보스의 현재 액션 상태를 변경하는 StateTree Task
 * @details 보스의 GameplayTag를 통해 현재 실행 중인 액션을 변경합니다.
 * @author 이효원
 */
UCLASS()
class OPERRATION_DDT_API UTask_SetCurrentAction : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ToolTip="보스 엑터"))
	class AActor* Boss;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ToolTip="변경할 테그 선택"))
	FGameplayTag SelectActionTag;
};
