// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponAsset.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"
#include "GameFramework/Character.h"

UCWeaponAsset::UCWeaponAsset()
{
	ACAttachmentClass = ACAttachment::StaticClass ();
	EquipmentClass = UCEquipment::StaticClass ();
}

void UCWeaponAsset::BeginPlay(class ACharacter* InOwner)
{
	if (ACAttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		Attachment = InOwner->GetWorld ()->SpawnActor <ACAttachment>(ACAttachmentClass, params);
	}
	if (EquipmentClass)
	{
		Equipment = NewObject <UCEquipment>(this, EquipmentClass);
		Equipment->BeginPlay (InOwner, EquipmentData);
	}
}
