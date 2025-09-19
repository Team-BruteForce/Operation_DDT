// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponAsset.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"
#include "GameFramework/Character.h"
#include "Weapons/CDoAction.h"

UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass ();
	EquipmentClass = UCEquipment::StaticClass ();
	DoActionClass = UCDoAction::StaticClass();
}

void UCWeaponAsset::BeginPlay(class ACharacter* InOwner)
{
	if (AttachmentClass)
	{
		FActorSpawnParameters params;
		params.Owner = InOwner;

		Attachment = InOwner->GetWorld ()->SpawnActor <ACAttachment>(AttachmentClass, params);
		if (Attachment)
			Attachment->AddToRoot();
		
	}
	if (EquipmentClass)
	{
		Equipment = NewObject <UCEquipment>(this, EquipmentClass);
		Equipment->BeginPlay (InOwner, EquipmentData);
		if (Equipment)
		{
			Equipment->AddToRoot();
		}

		if (Attachment)
		{
			Equipment->OnEquipmentBeginEquip.AddDynamic(Attachment, &ACAttachment::OnBeginEquip);
			Equipment->OnEquipmentUnEquip.AddDynamic(Attachment, &ACAttachment::OnUnEquip);
			/*Equipment->OnEquipmentBeginEquip.AddDynamic(GetAttachment(), &ACAttachment::OnBeginEquip);
			Equipment->OnEquipmentUnEquip.AddDynamic(GetAttachment(), &ACAttachment::OnUnEquip);*/
		}
	}
	if (DoActionClass)
	{
		DoAction = NewObject <UCDoAction>(this, DoActionClass);
		DoAction->BeginPlay(Attachment, Equipment, InOwner, DoActionDatas);
		if (DoAction)
		{
			DoAction->AddToRoot();
		}
		//DoAction->BeginPlay(GetAttachment(),GetEquipment(), InOwner, DoActionDatas);
	}
}
