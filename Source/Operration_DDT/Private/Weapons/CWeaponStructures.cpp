// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Player/Components/CMovementComponent.h"

void FDoActionData::DoAction(class ACharacter* InOwner)
{
	UCMovementComponent* movement = CHelpers::GetComponent<UCMovementComponent>(InOwner);

	if (!!movement)
	{
		if (bFixedCamera)
			movement->EnableFixedCamera();

		if (bCanMove == false)
			movement->Stop();
	}
	if (!!Montage)
		InOwner->PlayAnimMontage(Montage, PlayRate);
}
