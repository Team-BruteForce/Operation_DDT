// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/ODH_Enemy/SkeletonBody/CSkeletonBodyEnemy.h"

// Sets default values
ACSkeletonBodyEnemy::ACSkeletonBodyEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACSkeletonBodyEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACSkeletonBodyEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACSkeletonBodyEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

