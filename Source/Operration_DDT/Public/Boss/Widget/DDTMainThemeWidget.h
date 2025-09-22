// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	UFUNCTION()
	void PlayClickEvent();

	UPROPERTY()
	bool IsClick=true;
	
};
