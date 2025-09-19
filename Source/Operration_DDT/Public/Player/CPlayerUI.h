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
	// Tick에서 보간 업데이트
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
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
	class UProgressBar* pb_HP_Background;

	UPROPERTY(blueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UProgressBar* pb_Stamina_Background;

	void SetHPBar_Background(float prevHP, float newHP);
	void SetStaminaBar_Background(float prevStamina, float newStamina);
	

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
	
	// HP Background delay timer
	FTimerHandle HPBgDelayHandle;
	// Stamina Background delay timer
	FTimerHandle StaminaBgDelayHandle;
	
	// ===================== HP Background Lerp State =====================
	// 마지막으로 전달받은 MaxHP (SetHPBar 호출 시 갱신)
	float LastMaxHp = 100.0f;
	// 보간 상태값들
	float HPBgPrev = 1.0f;
	float HPBgTarget = 1.0f;
	float HPBgCurrent = 1.0f;
	float HPBgLerpDuration = 1.f;
	float HPBgElapsed = 0.0f;
	bool bHPBgLerping = false;

	// ===================== Stamina Background Lerp State =====================
	float LastMaxStamina = 100.0f;
	float StaminaBgPrev = 1.0f;
	float StaminaBgTarget = 1.0f;
	float StaminaBgCurrent = 1.0f;
	float StaminaBgLerpDuration = 1.f;
	float StaminaBgElapsed = 0.0f;
	bool bStaminaBgLerping = false;

	// 지연 후 보간 시작
	void StartHPBackgroundLerp();
	void StartStaminaBackgroundLerp();

};
