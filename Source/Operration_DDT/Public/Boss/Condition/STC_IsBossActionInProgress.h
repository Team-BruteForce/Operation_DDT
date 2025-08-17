// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_IsBossActionInProgress.generated.h"

/**
 * @brief 보스가 현재 어택 중인지 확인하는 조건 클래스
 * 
 * StateTree에서 사용되는 조건 클래스로, 보스가 현재
 * 어택 행동을 수행 중인지 확인합니다.
 */
UCLASS()
class OPERRATION_DDT_API USTC_IsBossActionInProgress : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
	
	/**
	 * @brief 조건을 테스트하는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @return true: 보스가 어택 중, false: 보스가 어택 중이 아님
	 * 
	 * 보스가 현재 어택 행동을 수행 중인지 확인합니다.
	 */
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	
	UPROPERTY(EditAnywhere, Category="Action State")
	FGameplayTag CurrentActionTag;
};
