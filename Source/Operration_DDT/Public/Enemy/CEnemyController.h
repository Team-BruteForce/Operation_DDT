// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UCEnemyStateTreeAIComponent* EnemyStateTreeAIComponent;

};
	