// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CWeaponComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Player/Components/CStateComponent.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CWeaponAsset.h"

// Sets default values for this component's properties
UCWeaponComponent::UCWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	//CLog::Print("UCWeaponComponent Constructor - Initial Type: " + FString::FromInt((int32)Type));
	UE_LOG(LogTemp, Log, TEXT("UCWeaponComponent Constructor - Initial Type: %d"),(int32)Type );
}


// Called when the game starts
void UCWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	for (int8 i = 0; i < (int8)EWeaponType::Max; ++i)
	{
		if (DataAssets[i])
			DataAssets[i]->BeginPlay(OwnerCharacter);
	}
	
	// 기본적으로 Sword 모드로 설정
	/*CLog::Print("Setting up default Sword mode");
	CLog::Print("Initial Type: " + FString::FromInt((int32)Type));
	CLog::Print("Sword DataAsset exists: " + FString::FromInt(!!DataAssets[(int8)EWeaponType::Sword]));*/
	
	// 모든 DataAssets 상태 확인
	for (int8 i = 0; i < (int8)EWeaponType::Max; ++i)
	{
		//CLog::Print("DataAsset[" + FString::FromInt(i) + "]: " + FString::FromInt(!!DataAssets[i]));
	}
	
	if (!!DataAssets[(int8)EWeaponType::Sword])
	{
		//CLog::Print("Equipping Sword");
		DataAssets[(int8)EWeaponType::Sword]->GetEquipment()->Equip();
		ChangeType(EWeaponType::Sword);
		//CLog::Print("Sword mode set successfully");
	}
	else
	{
		CLog::Print("Sword DataAsset is null - but keeping Sword mode as default");
		// Sword DataAsset이 없어도 기본 상태는 Sword로 유지
		ChangeType(EWeaponType::Rifle);
	}
}

bool UCWeaponComponent::IsIdleMode()
{
	return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsIdleMode();
}

class ACAttachment* UCWeaponComponent::GetAttachment()
{
	//CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!DataAssets[(int8)Type], nullptr);

	return DataAssets[(int8)Type]->GetAttachment();
}

class UCEquipment* UCWeaponComponent::GetEquipment()
{
	CheckFalseResult(!!DataAssets[(int8)Type], nullptr);

	return DataAssets[(int8)Type]->GetEquipment();
}

class UCDoAction* UCWeaponComponent::GetDoAction()
{
	//CLog::Print("GetDoAction - Type: " + FString::FromInt((int32)Type));
	
	// Unarmed 모드일 때는 nullptr 반환
	if (IsUnarmedMode())
	{
		//CLog::Print("Unarmed mode - returning nullptr");
		return nullptr;
	}
	
	//CLog::Print("DataAssets check: " + FString::FromInt(!!DataAssets[(int32)Type]));
	
	// DataAsset이 없으면 nullptr 반환
	if (!DataAssets[(int32)Type])
	{
		//CLog::Print("DataAsset is null - returning nullptr");
		return nullptr;
	}
	UCDoAction* result = DataAssets[(int32)Type]->GetDoAction();
	//CLog::Print("GetDoAction result: " + FString::FromInt(!!result));
	
	return result;
}

void UCWeaponComponent::SetUnarmedMode()
{
	if (!IsUnarmedMode())
	{
		GetEquipment()->UnEquip();
	}
	ChangeType(EWeaponType::Max);
}

void UCWeaponComponent::SetRifleMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Rifle);
}

void UCWeaponComponent::SetSwordMode()
{
	CheckFalse(IsIdleMode());
	
	SetMode(EWeaponType::Sword);
}

void UCWeaponComponent::SetRevolverMode()
{
	CheckFalse(IsIdleMode());

	SetMode(EWeaponType::Revolver);
}

void UCWeaponComponent::DoAction()
{
	//CLog::Print("DoAction Called - Type: " + FString::FromInt((int32)Type));
	
	// Unarmed 모드일 때는 아무것도 하지 않음
	if (IsUnarmedMode())
	{
		//CLog::Print("Unarmed mode - no action available");
		return;
	}
	
	UCDoAction* doAction = GetDoAction();
	if (doAction)
	{
		//CLog::Print("DoAction Found - Executing");
		doAction->DoAction();
	}
	else
	{
		//CLog::Print("DoAction is nullptr - Check if Sword DataAsset is assigned in editor");
	}
}

void UCWeaponComponent::SetMode(EWeaponType InType)
{
	if (Type == InType)
	{
		SetUnarmedMode();
		return;
	}
	else if (IsUnarmedMode() == false)
	{
		GetEquipment()->UnEquip();
	}

	if (!!DataAssets[(int8)InType])
	{
		DataAssets[(int8)InType]->GetEquipment()->Equip();

		ChangeType(InType);
	}
}

void UCWeaponComponent::ChangeType(EWeaponType InType)
{
	EWeaponType prevType = Type;
	Type = InType;
	//CLog::Print("ChangeType: " + FString::FromInt((int32)prevType) + " -> " + FString::FromInt((int32)Type));
	
	if (OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(prevType, Type);
}

void UCWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (auto& data: DataAssets)
	{
		//data->Cleanup();
		if (IsValid(data))
		{
			data->Cleanup();
		}
	}
	Super::EndPlay(EndPlayReason);
}
