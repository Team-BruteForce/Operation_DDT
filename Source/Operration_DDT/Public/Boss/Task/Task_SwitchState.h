// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_SwitchState.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UTask_SwitchState : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	
public:
	UPROPERTY(EditAnywhere,Category="Owner")
	class APawn* Owner;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State", meta=(AllowBinding))
	FGameplayTag OwningStateTag;
	
};
