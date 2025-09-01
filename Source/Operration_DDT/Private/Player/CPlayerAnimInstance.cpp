// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPlayerAnimInstance.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Player/DDTPlayer.h"

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
	
	FRotator rotator = OwnerCharacter->GetVelocity().ToOrientationRotator();
	FRotator rotator2 = OwnerCharacter->GetControlRotation();
	FRotator delta = UKismetMathLibrary::NormalizedDeltaRotator(rotator, rotator2);

	PrevRotation = UKismetMathLibrary::RInterpTo(PrevRotation,delta,DeltaSeconds, 25.f);
	//Direction = PrevRotation.Yaw;
	Direction = FVector::DotProduct(OwnerCharacter->GetVelocity(),OwnerCharacter->GetActorRightVector());

	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25.f);

	bAimMode = OwnerCharacter->State->IsRifleAimMode();
	
}

void UCPlayerAnimInstance::OnWeaponTypeChanged(EWeaponType InPrevType, EWeaponType InNewType)
{
	WeaponType = InNewType;
}
