// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_RotateTowardsPlayer.generated.h"

/**
 * @brief 보스가 플레이어 방향으로 회전하는 태스크
 * 
 * StateTree에서 사용되는 태스크로, 보스가 플레이어를 향해
 * 부드럽게 회전하는 기능을 제공합니다.
 */
UCLASS()
class OPERRATION_DDT_API UTask_RotateTowardsPlayer : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
	/**
	 * @brief 매 프레임 호출되는 Tick 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @param DeltaTime 델타 타임
	 * @return EStateTreeRunStatus::Running 상태가 계속 실행됨을 나타냄
	 */
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

	UPROPERTY(EditAnywhere,Category="Owner")
	class APawn* Owner;
	
};
