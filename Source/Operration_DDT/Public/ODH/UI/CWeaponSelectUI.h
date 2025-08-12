// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ISetImage.h"
#include "CWeaponSelectUI.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCWeaponSelectUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize() override;

private:
	UPROPERTY()
	class UCInterfaceUI* OwningPlayerInterfaceUI;

	UPROPERTY(meta = (BindWidget))
	class UButton* WeaponA;

	UPROPERTY(meta = (BindWidget))
	class UButton* WeaponB;

	UPROPERTY(meta = (BindWidget))
	class UButton* WeaponC;

	UPROPERTY(meta = (BindWidget))
	class UImage* WeaponAImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* WeaponBImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* WeaponCImage;

	UPROPERTY()
	FName OwningButtonName;

	UFUNCTION()
	void OnWeaponAButton();

	UFUNCTION()
	void OnWeaponBButton();

	UFUNCTION()
	void OnWeaponCButton();

	UPROPERTY(EditAnywhere, Category = UI)
	UTexture2D* ButtonImageTexture;

public:
// 	UFUNCTION()
// 	void SetOwningButton(FName ButtonName);

	UFUNCTION()
	void SetOwiningUI(UCInterfaceUI* PlayerInterfaceUI);

	UPROPERTY()
	EImageDisplayType DisplayType;
};
