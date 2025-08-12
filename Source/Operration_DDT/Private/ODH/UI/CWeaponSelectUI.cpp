// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/UI/CWeaponSelectUI.h"
#include "Components/Button.h"
#include "ODH/UI/CInterfaceUI.h"
#include "Components/Image.h"

bool UCWeaponSelectUI::Initialize()
{
	if (!Super::Initialize()) return false;

	if (WeaponA)
	{
		WeaponA->OnClicked.AddDynamic(this, &UCWeaponSelectUI::OnWeaponAButton);
	}

	if (WeaponB)
	{
		WeaponB->OnClicked.AddDynamic(this, &UCWeaponSelectUI::OnWeaponBButton);
	}

	if (WeaponC)
	{
		WeaponC->OnClicked.AddDynamic(this, &UCWeaponSelectUI::OnWeaponCButton);
	}

	

	return true;
}

void UCWeaponSelectUI::OnWeaponAButton()
{
	if (OwningPlayerInterfaceUI && WeaponAImage)
	{
		UTexture2D* Texture = Cast<UTexture2D>(WeaponAImage->GetBrush().GetResourceObject());
		if (Texture)
		{
			OwningPlayerInterfaceUI->SetWeaponImage(Texture, DisplayType);
		}
	}

	RemoveFromParent();
}

void UCWeaponSelectUI::OnWeaponBButton()
{
	if (OwningPlayerInterfaceUI && WeaponBImage)
	{
		UTexture2D* Texture = Cast<UTexture2D>(WeaponBImage->GetBrush().GetResourceObject());
		if (Texture)
		{
			OwningPlayerInterfaceUI->SetWeaponImage(Texture, DisplayType);
		}
	}

	RemoveFromParent();
}

void UCWeaponSelectUI::OnWeaponCButton()
{
	if (OwningPlayerInterfaceUI && WeaponCImage)
	{
		UTexture2D* Texture = Cast<UTexture2D>(WeaponCImage->GetBrush().GetResourceObject());
		if (Texture)
		{
			OwningPlayerInterfaceUI->SetWeaponImage(Texture, DisplayType);
		}
	}

	RemoveFromParent();
}

void UCWeaponSelectUI::SetOwiningUI(UCInterfaceUI* PlayerInterfaceUI)
{
	OwningPlayerInterfaceUI = PlayerInterfaceUI;
}