// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/ODH_Enemy/FlyingSkull/CFlyingSkull.h"

// Sets default values
ACFlyingSkull::ACFlyingSkull()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACFlyingSkull::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACFlyingSkull::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACFlyingSkull::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

