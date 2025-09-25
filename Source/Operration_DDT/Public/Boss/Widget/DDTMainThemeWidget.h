/**
 * @file DDTMainThemeWidget.h
 * @brief DDT 메인 테마 위젯 헤더 파일
 * 
 * @details
 * 게임의 메인 테마를 표시하는 위젯입니다.
 * 오디오 컴포넌트를 통해 배경음악을 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "DDTMainThemeWidget.generated.h"

/**
 * @brief DDT 메인 테마 위젯
 * 
 * 게임의 메인 테마를 표시하는 위젯입니다.
 * 오디오 컴포넌트를 통해 배경음악을 관리합니다.
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
