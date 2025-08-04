#pragma once

#include "Enemy/AEnemy.h"
#include "AIController.h"
#include "Global.h"
AAEnemy::AAEnemy()
{
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, AssetPaths::ENEMY_MESH);
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	
	TSubclassOf<UAnimInstance> Anim;
	CHelpers::GetClass<UAnimInstance>(&Anim, AssetPaths::ENEMY_ANIM);
	GetMesh()->SetAnimInstanceClass(Anim);

	TSubclassOf<AAIController> AIC;
	CHelpers::GetClass<AAIController>(&AIC, AssetPaths::ENEMY_AIC);
	AIControllerClass = AIC;

}

// Called when the game starts or when spawned
void AAEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called to bind functionality to input
void AAEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

