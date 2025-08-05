// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "CPatrolTask.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCPatrolTask : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="AI")
	class AActor* Enemy;
	// UPROPERTY(EditAnywhere, Category = "Patrol")
	// float PatrolSpeed = 300.0f;
	
	// UPROPERTY(EditAnywhere, Category = "Patrol")
	// float WaitTime = 2.0f;
};
