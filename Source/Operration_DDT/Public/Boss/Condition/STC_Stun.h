// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_Stun.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API USTC_Stun : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
protected:
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

	/**
 * @brief 보스 액터 (StateTree에서 설정)
 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rand Location Check")
	class APawn* Boss;
};
