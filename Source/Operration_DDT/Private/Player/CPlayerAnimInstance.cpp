// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPlayerAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CMovementComponent.h"

void UCPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OwnerCharacter = Cast<ADDTPlayer>(TryGetPawnOwner());
	CheckNull(OwnerCharacter);

	Weapon = CHelpers::GetComponent<UCWeaponComponent>(OwnerCharacter);
	if (!!Weapon)
	{
		Weapon->OnWeaponTypeChanged.AddDynamic(this, &UCPlayerAnimInstance::OnWeaponTypeChanged);
	}
}

void UCPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	CheckNull(OwnerCharacter);
	
	Speed = OwnerCharacter->GetVelocity().Size2D();

	//CLog::Log("Speed: " + FString::SanitizeFloat(Speed));
	
	FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = OwnerCharacter->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);

	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation,delta,DeltaSeconds, 25.f);
	//Direction = PrevRotation.Yaw;
	Direction = FVector::DotProduct(OwnerCharacter->GetVelocity(),OwnerCharacter->GetActorRightVector());

	bAimMode = OwnerCharacter->State->IsRifleAimMode();
	//CLog::Log("bAimMode: " + bAimMode ? TEXT("true") : TEXT("false"));
	if (bAimMode)
	{
		Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25.f);
		Pitch = UKismetMathLibrary::ClampAngle(Pitch, -90.f, 90.f);
	}
	
	UCMovementComponent* Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	if (Movement)
	{
		ForwardInput = Movement->GetForwardInput() * 100.f;
		RightInput = Movement->GetRightInput() * 100.f;
	}
	bIsSprinting = Movement->GetIsSprinting();
	
}

void UCPlayerAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}
