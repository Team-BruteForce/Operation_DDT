// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Interface/IAIController.h"
#include "CEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API ACEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	ACEnemyController(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;

	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	virtual void OnPossess(APawn* InPawn) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UCEnemyStateTreeAIComponent* EnemyStateTreeAIComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class UCEnemyMovement* MoveComp;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class AAEnemy* Enemy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USplineComponent* Spline;



};
	