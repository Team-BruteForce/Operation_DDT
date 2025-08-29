// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_Dead.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UTask_Dead : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tag")
	class AActor* Boss;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tag")
	FGameplayTag SelectedTag;
};
