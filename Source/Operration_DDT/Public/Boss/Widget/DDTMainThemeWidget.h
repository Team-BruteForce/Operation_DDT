// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "DDTMainThemeWidget.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UDDTMainThemeWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FWidgetAnimationDynamicEvent FMainThemeFadeOutEvent;
public:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent) override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (BindWidgetAnim),Transient)
	class UWidgetAnimation* PressButtonLoopAnimation;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (BindWidgetAnim),Transient)
	class UWidgetAnimation* PressButtonClickAnimation;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta = (BindWidget))
	class UImage* FadeIn;

	UFUNCTION()
	void PlayClickEvent();

	UPROPERTY()
	bool IsClick=true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAudioComponent* BGMComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundBase* BGMSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class USoundBase* ClickSound;

	UPROPERTY()
	FTimerHandle FadeInTimerHandle;

	UPROPERTY()
	FTimerHandle FadeOutTimerHandle;

	UPROPERTY()
	float CurrentBGMVolume = 0.0f;

	UPROPERTY()
	float TargetBGMVolume = 1.0f;

	UFUNCTION()
	void UpdateBGMVolume();

	UFUNCTION()
	void StartBGM();

	UFUNCTION()
	void FadeOutBGM();
	
};
