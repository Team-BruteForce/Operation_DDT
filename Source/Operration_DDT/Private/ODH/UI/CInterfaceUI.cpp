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

	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UCInterfaceUI::OnGameStartButton);
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

void UCInterfaceUI::OnGameStartButton()
{
	bool HasTextureA = WeaponDisplayA && WeaponDisplayA->GetBrush().GetResourceObject();
	bool HasTextureB = WeaponDisplayB && WeaponDisplayB->GetBrush().GetResourceObject();

	if (HasTextureA && HasTextureB)
	{
		//플레이어를 참조하지 않고 어떤 무기를 선택했는지 전달이 가능할까? 잘 모르겠으니 나중에 물어보든가 팀원하고 얘기하기

		FString WeaponNameA = WeaponDisplayA->GetBrush().GetResourceObject()->GetName();
		FString WeaponNameB = WeaponDisplayB->GetBrush().GetResourceObject()->GetName();

		UE_LOG(LogTemp, Warning, TEXT("Player Weapon A : %s"), *WeaponNameA);
		UE_LOG(LogTemp, Warning, TEXT("Player Weapon B : %s"), *WeaponNameB);
	}

	else
	{
		FString MissingText = TEXT("");
		if(!HasTextureA) MissingText += TEXT("WeaponDisplayA");
		if (!HasTextureB) MissingText += TEXT("WeaponDisplayB");
		UE_LOG(LogTemp, Warning, TEXT("Cannot start game: Missing textures in %s"), *MissingText);
	}
}

void UCInterfaceUI::SetWeaponImage(UTexture2D* Texture, EImageDisplayType DisplayType)
{
	if(!Texture) return;

	//이미지가 중복되는지 확인하기 위한 변수 설정
	UImage* TargetImage = nullptr;
	UImage* OtherImage = nullptr;

	switch (DisplayType)
	{
	case EImageDisplayType::Display1:
		if (WeaponDisplayA)
		{
			//이미지가 중복되는지 확인하기 위한 변수 설정
			TargetImage =  WeaponDisplayA;
			OtherImage = WeaponDisplayB;
			/*WeaponDisplayA->SetBrushFromTexture(Texture);*/
		}
		break;
	case EImageDisplayType::Display2:
		if (WeaponDisplayB)
		{
			//이미지가 중복되는지 확인하기 위한 변수 설정
			TargetImage = WeaponDisplayB;
			OtherImage = WeaponDisplayA;
			/*WeaponDisplayB->SetBrushFromTexture(Texture);*/
		}
		break;
	default:
		break;
	}

	CheckDuplicateImage(TargetImage, OtherImage, Texture);
}

void UCInterfaceUI::CheckDuplicateImage(UImage* Target, UImage* Other, UTexture2D* Texture)
{
	if (Target)
	{
		//먼저 타겟 이미지에 이미지를 설정하게 함
		Target->SetBrushFromTexture(Texture);

		//그리고 다른 이미지와 이미지를 비교한 다음 같은 이미지일 경우
		if (Other && Other->GetBrush().GetResourceObject())
		{
			if (Other->GetBrush().GetResourceObject()->GetName() == Texture->GetName())
			{
				//다른 이미지에 설정된 것을 없애버림
				Other->SetBrushFromTexture(nullptr);
			}
		}
	}
}
