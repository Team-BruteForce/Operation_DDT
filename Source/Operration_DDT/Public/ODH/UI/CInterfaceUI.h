// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ISetImage.h"
#include "CInterfaceUI.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCInterfaceUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* OpenWeaponUIButtonA;

	UPROPERTY(meta = (BindWidget))
	class UButton* OpenWeaponUIButtonB;

	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UImage* WeaponDisplayA;

	UPROPERTY(meta = (BindWidget))
	class UImage* WeaponDisplayB;

	UFUNCTION()
	void OnOpenWeaponUIButtonA();

	UFUNCTION()
	void OnOpenWeaponUIButtonB();

	UFUNCTION()
	void OnGameStartButton();

	void CheckDuplicateImage(UImage* Target, UImage* Other, UTexture2D* Texture);

	UPROPERTY(EditAnywhere, Category = UI)
	TSubclassOf<UUserWidget> WeaponSelectUI;

	UPROPERTY()
	class UCWeaponSelectUI* CurrentWeaponUI;
	
public:
	UFUNCTION(BlueprintCallable, Category = Image)
	void SetWeaponImage(UTexture2D* Texture, EImageDisplayType DisplayType);
};
