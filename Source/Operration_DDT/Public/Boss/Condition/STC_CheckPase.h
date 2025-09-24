// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_CheckPase.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API USTC_CheckPase : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conditions")
	class AActor* Owner;
};
