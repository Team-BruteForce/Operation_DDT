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

	// ...
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
	
}

bool UCWeaponComponent::IsIdleMode()
{
	return CHelpers::GetComponent<UCStateComponent>(OwnerCharacter)->IsIdleMode();
}

class ACAttachment* UCWeaponComponent::GetAttachment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!DataAssets[(int8)Type], nullptr);

	return DataAssets[(int8)Type]->GetAttachment();
}

class UCEquipment* UCWeaponComponent::GetEquipment()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!DataAssets[(int8)Type], nullptr);

	return DataAssets[(int8)Type]->GetEquipment();
}

class UCDoAction* UCWeaponComponent::GetDoAction()
{
	CheckTrueResult(IsUnarmedMode(), nullptr);
	CheckFalseResult(!!DataAssets[(int32)Type], nullptr);

	return DataAssets[(int32)Type]->GetDoAction();
}

void UCWeaponComponent::SetUnarmedMode()
{
	GetEquipment()->UnEquip();
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
	if (GetDoAction())
		GetDoAction()->DoAction();
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
	if (OnWeaponTypeChanged.IsBound())
		OnWeaponTypeChanged.Broadcast(prevType, Type);
}
