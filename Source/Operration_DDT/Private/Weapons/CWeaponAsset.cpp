// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CWeaponAsset.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"
#include "GameFramework/Character.h"
#include "Weapons/CDoAction.h"
#include "Global.h"

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
		{
			if (!Attachment->IsRooted())
				Attachment->AddToRoot();
		}
		
	}
	if (EquipmentClass)
	{
		Equipment = NewObject <UCEquipment>(this, EquipmentClass);
		Equipment->BeginPlay (InOwner, EquipmentData);
		if (Equipment)
		{
			if (!Equipment->IsRooted())
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
			if (!DoAction->IsRooted())
				DoAction->AddToRoot();
		}
		//DoAction->BeginPlay(GetAttachment(),GetEquipment(), InOwner, DoActionDatas);
	}
}

void UCWeaponAsset::Cleanup()
{
	/*try
	{
		// DoAction 정리
		if (DoAction)
		{
			//DoAction->ConditionalBeginDestroy();
			DoAction->RemoveFromRoot();
			DoAction = nullptr;
		}

		// Equipment 정리
		if (Equipment)
		{
			//Equipment->ConditionalBeginDestroy();
			Equipment->RemoveFromRoot();
			Equipment = nullptr;
		}

		// Attachment 정리
		if (Attachment)
		{
			//Attachment->ConditionalBeginDestroy();
			Attachment->RemoveFromRoot();
			Attachment = nullptr;
		}
	}
	catch (...)
	{
		// 모든 예외를 무시하고 안전하게 종료
		// 메모리 손상이나 Access Violation 발생 시 크래시 방지
		UE_LOG(LogTemp, Error, TEXT("CleanUp Error"));
	}*/
	// DoAction 정리
	if (DoAction)
	{
		//DoAction->ConditionalBeginDestroy();
		DoAction->RemoveFromRoot();
		DoAction = nullptr;
	}

	// Equipment 정리
	if (Equipment)
	{
		//Equipment->ConditionalBeginDestroy();
		Equipment->RemoveFromRoot();
		Equipment = nullptr;
	}

	// Attachment 정리
	if (Attachment)
	{
		//Attachment->ConditionalBeginDestroy();
		Attachment->RemoveFromRoot();
		Attachment = nullptr;
	}
}
