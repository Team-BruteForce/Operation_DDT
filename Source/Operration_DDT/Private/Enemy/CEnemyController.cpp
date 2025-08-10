// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CEnemyController.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Enemy/Component/CEnemyMovement.h"
#include "Enemy/AEnemy.h"
#include "Enemy/CEnemyStateTreeAIComponent.h"
#include "Navigation/PathFollowingComponent.h"

ACEnemyController::ACEnemyController(const FObjectInitializer& ObjectInitializer)
{
	CHelpers::CreateActorComponent<UCEnemyStateTreeAIComponent>(this, &EnemyStateTreeAIComponent, "StateTreeAIComponent");
}

/**
 * @brief 폰에 빙의하고 필요한 컴포넌트들을 연결하는 초기화 작업을 수행합니다.
 */
void ACEnemyController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	Enemy = Cast<AAEnemy>(InPawn);
	CheckNull(Enemy);
	
	MoveComp = CHelpers::GetComponent<UCEnemyMovement>(Enemy);
	CheckNull(MoveComp);
	MoveComp->SetOwnerController(this);

	TotalPatrolPoints = Enemy->GetPatrolPath()->SplineComponent->GetNumberOfSplinePoints();
	CurrentPatrolPoints = 0;
}

void ACEnemyController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.IsSuccess())
	{
		MoveComp->IncreasePatrolPoint();
	}
	else
	{
		
	}
}

void ACEnemyController::BeginPlay()
{
	Super::BeginPlay();
}