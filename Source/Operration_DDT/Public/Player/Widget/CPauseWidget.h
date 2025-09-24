// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CPauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCPauseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	class ADDTPlayer* Player;
	class APlayerController* PC;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UImage* img_Normal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UImage* img_Yes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UImage* img_No;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UButton* btn_Yes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, meta=(BindWidget))
	class UButton* btn_No;

	UFUNCTION()
	void OnHoverYesBtn();

	UFUNCTION()
	void OnHoverNoBtn();

	UFUNCTION()
	void UnhoverBtn();

	UFUNCTION()
	void OnClickYesBtn();

	UFUNCTION()
	void OnClickNoBtn();
	
};
