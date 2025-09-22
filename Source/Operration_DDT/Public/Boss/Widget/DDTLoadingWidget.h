// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDTLoadingWidget.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UDDTLoadingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FWidgetAnimationDynamicEvent FLoadingFadeOutEvent;
	
public:
	virtual void NativeConstruct() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,meta=(BindWidgetAnim),Transient)
	class UWidgetAnimation* OrbLoopAnimation;

	void PlayLoadingAnimation();

	UFUNCTION()
	void EndLoading();
};
