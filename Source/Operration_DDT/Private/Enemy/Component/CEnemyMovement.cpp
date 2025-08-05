// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Component/CEnemyMovement.h"
#include "Enemy/AEnemy.h"
#include "Global.h"
#include "Components/SplineComponent.h"
#include "Enemy/CEnemyController.h"

// Sets default values for this component's properties
UCEnemyMovement::UCEnemyMovement()
{
	
}
void UCEnemyMovement::BeginPlay()
{
	Super::BeginPlay();

	USplineComponent* Spline= CHelpers::GetComponent<USplineComponent>(PatrolPath);
	TotalPatrolPoints=Spline->GetNumberOfSplinePoints();
	CurrentPatrolPoints=0;
}
void UCEnemyMovement::Patrol()
{
	CLog::Log("Patrol");
    FVector TargetLocation = PatrolPath->SplineComponent->GetLocationAtSplinePoint(CurrentPatrolPoints, ESplineCoordinateSpace::World);
	Owner->MoveToLocation(TargetLocation);
}
void UCEnemyMovement::IncreasePatrolPoint()
{
	CurrentPatrolPoints++;
	if (CurrentPatrolPoints>TotalPatrolPoints){
		CurrentPatrolPoints=0;
		Patrol();
	}
	else
		Patrol();
}