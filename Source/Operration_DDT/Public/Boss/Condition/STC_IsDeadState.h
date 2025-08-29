// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_IsDeadState.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API USTC_IsDeadState : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
	
	
};
