// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDTLoadingWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLoadingFadeOutEnd);
UCLASS()
class OPERRATION_DDT_API UDDTLoadingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FWidgetAnimationDynamicEvent FLoadingFadeOutEvent;
	FWidgetAnimationDynamicEvent FLoadingStartEvent;
	FOnLoadingFadeOutEnd OnLoadingFadeoutEnd;
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* OrbLoopAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	class UCanvasPanel* CanvasPanel_32;
	
	void PlayLoadingAnimation();

	UFUNCTION()
	void EndLoading();

	void Reset();

	UFUNCTION()
	void StartLoading();
	
};
