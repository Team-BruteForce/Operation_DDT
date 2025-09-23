// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CDamageWidget_Normal.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDamageUIFinished);
UCLASS()
class OPERRATION_DDT_API UCDamageWidget_Normal : public UUserWidget
{
	GENERATED_BODY()

private:
	bool bActive = false;

public:

	void NativeConstruct() override;

	void SetActive(bool inValue);
	bool IsActive() const;
	
	FTimerHandle LifeTimerHandle;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	float LifeTime = 1.f;
	
	UPROPERTY(blueprintReadWrite, Category = "UI", meta = (BindWidget))
	class UTextBlock* txt_Damage_Normal;

	void SetDamageText(float value, bool isCritical = false, bool isGroggy = false);

	UPROPERTY(Transient, blueprintreadwrite, Category = "UI", meta = (BindWidgetAnim))
	class UWidgetAnimation* Damage_Origin;
	
	FWidgetAnimationDynamicEvent OnDamageUIFinished;

	void ShowDamageWidgetAnimation();

	void ResetTextPosition();
	void StartLifeTimer();
	void StopLifeTimer();
	void OnLifeTimeExpired();
	void ReturnToPool();
};
