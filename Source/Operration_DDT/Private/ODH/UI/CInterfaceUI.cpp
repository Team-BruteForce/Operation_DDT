// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/UI/CInterfaceUI.h"
#include "Components/Button.h"
#include "ODH/UI/CWeaponSelectUI.h"
#include "Engine/Texture2D.h"
#include "Components/Image.h"

void UCInterfaceUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (OpenWeaponUIButtonA)
	{
		OpenWeaponUIButtonA->OnClicked.AddDynamic(this, &UCInterfaceUI::OnOpenWeaponUIButtonA);
	}

	if (OpenWeaponUIButtonB)
	{
		OpenWeaponUIButtonB->OnClicked.AddDynamic(this, &UCInterfaceUI::OnOpenWeaponUIButtonB);
	}
}

void UCInterfaceUI::OnOpenWeaponUIButtonA()
{
	if (WeaponSelectUI)
	{
		if (CurrentWeaponUI)
		{
			CurrentWeaponUI->RemoveFromParent();
			CurrentWeaponUI = nullptr;
		}

		UWorld* World = UObject::GetWorld();
		if (World)
		{
			UCWeaponSelectUI* OpenWeaponUI = CreateWidget<UCWeaponSelectUI>(World, WeaponSelectUI);
			if (OpenWeaponUI)
			{
				OpenWeaponUI->SetOwiningUI(this);
				OpenWeaponUI->DisplayType = EImageDisplayType::Display1;
// 				OpenWeaponUI->SetOwningButton(FName("ButtonA"));
				OpenWeaponUI->AddToViewport();
				CurrentWeaponUI = OpenWeaponUI;
			}
		}
	}
}

void UCInterfaceUI::OnOpenWeaponUIButtonB()
{
	if (WeaponSelectUI)
	{
		if (CurrentWeaponUI)
		{
			CurrentWeaponUI->RemoveFromParent();
			CurrentWeaponUI = nullptr;
		}

		UWorld* World = UObject::GetWorld();
		if (World)
		{
			UCWeaponSelectUI* OpenWeaponUI = CreateWidget<UCWeaponSelectUI>(World, WeaponSelectUI);
			if (OpenWeaponUI)
			{
				OpenWeaponUI->SetOwiningUI(this);
				OpenWeaponUI->DisplayType = EImageDisplayType::Display2;
// 				OpenWeaponUI->SetOwningButton(FName("ButtonB"));
				OpenWeaponUI->AddToViewport();
				CurrentWeaponUI = OpenWeaponUI;
			}
		}
	}
}

void UCInterfaceUI::SetWeaponImage(UTexture2D* Texture, EImageDisplayType DisplayType)
{
	if(!Texture) return;

	switch (DisplayType)
	{
	case EImageDisplayType::Display1:
		if (WeaponDisplayA)
		{
			WeaponDisplayA->SetBrushFromTexture(Texture);
		}
		break;
	case EImageDisplayType::Display2:
		if (WeaponDisplayB)
		{
			WeaponDisplayB->SetBrushFromTexture(Texture);
		}
		break;
	default:
		break;
	}
}

