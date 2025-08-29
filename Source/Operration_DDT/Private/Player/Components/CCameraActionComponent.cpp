// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CCameraActionComponent.h"

#include "GameFramework/SpringArmComponent.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CWeaponComponent.h"

// Sets default values for this component's properties
UCCameraActionComponent::UCCameraActionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCCameraActionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCharacter = Cast<ADDTPlayer>(GetOwner());
	OwnerSpringArm = OwnerCharacter->GetSpringArm();
	
}

void UCCameraActionComponent::SetAimPosition()
{
	OwnerSpringArm->SetRelativeLocation(FVector(locX, 0.f, locZ));
	OwnerSpringArm->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	OwnerSpringArm->TargetArmLength = ArmLen;
	OwnerSpringArm->bEnableCameraLag = false;
	
	OwnerCharacter->State->SetRifleAimMode(true);
	OwnerCharacter->WeaponComp->SetRifleMode();
}

void UCCameraActionComponent::SetIdlePosition()
{
	OwnerSpringArm->SetRelativeLocation(FVector(-60.f, 0.f, 180.f));
	OwnerSpringArm->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	OwnerSpringArm->TargetArmLength = 200.f;
	OwnerSpringArm->bEnableCameraLag = true;

	OwnerCharacter->State->SetIdleMode();
	OwnerCharacter->State->SetRifleAimMode(false);
	OwnerCharacter->WeaponComp->SetUnarmedMode();
}

