// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "APatrolPath.generated.h"

UCLASS()
class OPERRATION_DDT_API AAPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	AAPatrolPath();

protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPath")
	class USplineComponent* SplineComponent;
};
