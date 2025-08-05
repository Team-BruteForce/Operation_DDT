// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy/APatrolPath.h"
#include "Enemy/CEnemyController.h"
#include "Enemy/Interface/IAIController.h"
#include "CEnemyMovement.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCEnemyMovement : public UActorComponent, public IIAIController
{
	GENERATED_BODY()
public:	
	UCEnemyMovement();
// public:
// 	virtual AAPatrolPath* GetPatrolPath() override;
public:
    void SetOwnerController(ACEnemyController* InController) { Owner = InController; }
	
public:
	virtual void Patrol() override;
	
protected:
	virtual void BeginPlay() override;

public:
	void IncreasePatrolPoint();

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PatrolPath")
	class AAPatrolPath* PatrolPath;

private:
	class ACEnemyController* Owner;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 TotalPatrolPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 CurrentPatrolPoints;
	
};
