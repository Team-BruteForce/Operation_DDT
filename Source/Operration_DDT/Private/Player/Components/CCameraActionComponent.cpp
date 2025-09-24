// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CCameraActionComponent.h"

#include "Global.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/Widget/CPlayerUI.h"
#include "Player/DDTGameMode.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CMovementComponent.h"
#include "Player/Components/CUIComponent.h"
#include "Player/Components/CWeaponComponent.h"
#include "Sound/SoundCue.h"

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
	Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	UIComp = CHelpers::GetComponent<UCUIComponent>(OwnerCharacter);
	
}

void UCCameraActionComponent::SetAimPosition()
{
	CheckTrue(State->IsRollingMode());
	CheckTrue(State->IsHittedMode());
	CheckTrue(State->IsDeadMode());
	
	OwnerCharacter->WeaponComp->SetRifleMode();
	OwnerSpringArm->SetRelativeLocation(FVector(locX, 0.f, locZ));
	OwnerSpringArm->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	OwnerSpringArm->TargetArmLength = ArmLen;
	OwnerSpringArm->bEnableCameraLag = false;
	UGameplayStatics::PlaySound2D(OwnerCharacter->GetWorld(), AimSound);
	OwnerCharacter->State->SetRifleAimMode(true);
	//CLog::Log("SetAimPosition: " + OwnerCharacter->State->IsRifleAimMode() ? TEXT("true") : TEXT("false"));
	Movement->OnWalk();

	/*CheckNull(GetWorld()->GetAuthGameMode<ADDTGameMode>());
	GetWorld()->GetAuthGameMode<ADDTGameMode>()->playerUI->ShowCrosshair(true);
	*/
	UIComp->playerUI->ShowCrosshair(true);
	
}

void UCCameraActionComponent::SetIdlePosition()
{
	OwnerSpringArm->SetRelativeLocation(FVector(-60.f, 0.f, 180.f));
	OwnerSpringArm->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	OwnerSpringArm->TargetArmLength = 200.f;
	OwnerSpringArm->bEnableCameraLag = true;

	OwnerCharacter->State->SetIdleMode();
	OwnerCharacter->State->SetRifleAimMode(false);
	//CLog::Log("SetIdlePosition: " + OwnerCharacter->State->IsRifleAimMode() ? TEXT("true") : TEXT("false"));

	OwnerCharacter->WeaponComp->SetUnarmedMode();
	Movement->OnRun();

	/*CheckNull(GetWorld()->GetAuthGameMode<ADDTGameMode>());
	GetWorld()->GetAuthGameMode<ADDTGameMode>()->playerUI->ShowCrosshair(false);*/

	UIComp->playerUI->ShowCrosshair(false);
}

void UCCameraActionComponent::ShakeCameraByHit()
{
	CLog::Log("CameraActionComponent::ShakeCameraByHit");
	APlayerController* pc = Cast<APlayerController>(OwnerCharacter->GetController());
	pc->PlayerCameraManager->StartCameraShake(HitCameraShake);
}

