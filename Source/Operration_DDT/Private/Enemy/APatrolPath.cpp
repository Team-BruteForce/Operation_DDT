// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/APatrolPath.h"
#include "Global.h"
#include "Components/SplineComponent.h"

// Sets default values
AAPatrolPath::AAPatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateActorComponent<USplineComponent>(this,&SplineComponent, "SplineComponent");

}

// Called when the game starts or when spawned
void AAPatrolPath::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAPatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

