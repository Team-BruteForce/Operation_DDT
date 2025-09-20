// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UBossStatusWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FWidgetAnimationDynamicEvent FCompleteUIFadeIn;
	FWidgetAnimationDynamicEvent FCompleteUIFadeOut;
public:
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (BindWidget))
	class UWidgetSwitcher* BossWidgetSwitcher;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (BindWidget))
	class UCanvasPanel* BossStatusCanvas;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (BindWidget))
	class UCanvasPanel* BossCompleteCanvas;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (BindWidget))
	class UProgressBar* DelayHP;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (BindWidget))
	class UProgressBar* CurrentHP;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (BindWidgetAnim),Transient)
	class UWidgetAnimation* FadeIn;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (BindWidgetAnim),Transient)
	class UWidgetAnimation* FadeOut;

	UPROPERTY()
	FTimerHandle Hander;
	
	UPROPERTY()
	class ACBoss* Owner;

	UPROPERTY()
	FTimerHandle DelayHPTimer;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void UpdateBossHP(float CurrentHPValue, float MaxHPValue);

	UFUNCTION()
	void SwitchBossCompleteUI();

	UFUNCTION()
	void FadeInHandler();

	UFUNCTION()
	void ShowCompleteUI();

	UFUNCTION()
	void FadeOutHandler();
	UFUNCTION()
	void EndWidget();

};
