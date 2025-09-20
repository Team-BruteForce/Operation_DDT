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
	

	UPROPERTY()
	class ACBoss* Owner;

	UPROPERTY()
	FTimerHandle DelayHPTimer;

	UFUNCTION(BlueprintCallable)
	void UpdateBossHP(float CurrentHPValue, float MaxHPValue);

	UFUNCTION(BlueprintCallable)
	void SwitchBossCompleteUI();

};
