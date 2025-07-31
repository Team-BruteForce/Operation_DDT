// Fill out your copyright notice in the Description page of Project Settings.


#include "Eenmy/CEnemyTest.h"

// Sets default values
ACEnemyTest::ACEnemyTest()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACEnemyTest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACEnemyTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACEnemyTest::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

