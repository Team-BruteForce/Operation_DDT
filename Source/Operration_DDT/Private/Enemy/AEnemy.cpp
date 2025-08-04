#pragma once

#include "Enemy/AEnemy.h"
#include "Global.h"
AAEnemy::AAEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	// USkeletalMesh* mesh;
	// CHelpers::GetAsset<USkeletalMesh>(&mesh, AssetPaths::ENEMY_MESH);
	//
	// TSubclassOf<UAnimInstance> Anim;
	// CHelpers::GetClass<UAnimInstance>(&Anim, AssetPaths::ENEMY_ANIM);
	// GetMesh()->SetAnimInstanceClass(Anim);
}

// Called when the game starts or when spawned
void AAEnemy::BeginPlay()
{
	Super::BeginPlay();
	// CHelpers::GetAsset<USkeletalMesh>(&mesh, AssetPaths::CHARACTER_MESH);
	
}

// Called every frame
void AAEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

