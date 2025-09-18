// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPlayerUI.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCPlayerUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(blueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UImage* img_Crosshair;

	void ShowCrosshair(bool bValue);

	UPROPERTY(blueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UProgressBar* pb_HP;

	UPROPERTY(blueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UProgressBar* pb_Stamina;

	void SetHPBar(float value, float maxHP);
	void SetStaminaBar(float value, float maxStamina);

	UPROPERTY(blueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UTextBlock* txt_CurrentBullet;
	
	UPROPERTY(blueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UTextBlock* txt_TotalBullet;

	void SetCurrentBullet(int32 value);
	void SetTotalBullet(int32 value);

	UPROPERTY(blueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UTextBlock* txt_HealItem;

	void SetHealItem(int32 value);

	UPROPERTY(blueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UImage* img_injection_full;
	
	UPROPERTY(blueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UImage* img_injection_empty;
	
};
