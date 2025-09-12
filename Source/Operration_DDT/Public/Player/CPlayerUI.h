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
	
};
