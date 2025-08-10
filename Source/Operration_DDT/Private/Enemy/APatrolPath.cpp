// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/APatrolPath.h"
#include "Global.h"
#include "Components/SplineComponent.h"

AAPatrolPath::AAPatrolPath()
{
	CHelpers::CreateActorComponent<USplineComponent>(this,&SplineComponent, "SplineComponent");

}

void AAPatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
}