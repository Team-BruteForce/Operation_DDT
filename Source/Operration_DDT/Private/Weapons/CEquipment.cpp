// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CEquipment.h"
#include "GameFramework/Character.h"
#include "Global.h"
#include "Player/Components/CStateComponent.h"
#include "Player/Components/CMovementComponent.h"

void UCEquipment::BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData)
{
	OwnerCharacter = InOwner;
	Data = InData;

	Movement = CHelpers::GetComponent<UCMovementComponent> (OwnerCharacter);
	State = CHelpers::GetComponent<UCStateComponent> (OwnerCharacter);
}

void UCEquipment::Equip_Implementation()
{
	State->SetEquipMode();

	if (Data.bCanMove == false)
		Movement->Stop();

	if (Data.Montage)
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRate);
	else
	{
		Begin_Equip();
		End_Equip();
	}
}

void UCEquipment::Begin_Equip_Implementation()
{
	bBeginEquip = true;
}

void UCEquipment::End_Equip_Implementation()
{	
	bBeginEquip = false;
	bEquipped = true;

	Movement->Move ();
	State->SetIdleMode ();

}

void UCEquipment::UnEquip_Implementation()
{
	bEquipped = false;
	Movement->EnableControlRotation();
}
