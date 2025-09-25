/**
 * @file BossStatusWidget.h
 * @brief 보스 상태 위젯 헤더 파일
 * 
 * @details
 * 보스의 상태를 표시하는 UI 위젯입니다.
 * 보스의 체력과 페이즈 상태를 시각화합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossStatusWidget.generated.h"

/**
 * @brief 보스 상태 위젯
 * 
 * 보스의 상태를 표시하는 UI 위젯입니다.
 * 보스의 체력과 페이즈 상태를 시각화합니다.
 */
UCLASS()
class OPERRATION_DDT_API UBossStatusWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	FWidgetAnimationDynamicEvent FCompleteUIFadeIn;
	FWidgetAnimationDynamicEvent FCompleteUIFadeOut;
	FWidgetAnimationDynamicEvent FBlackFadeIn;
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

	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta = (BindWidgetAnim),Transient)
	class UWidgetAnimation* BlackBoard;

	UPROPERTY()
	FTimerHandle Hander;
	
	UPROPERTY()
	class ACBoss* Owner;

	UPROPERTY()
	FTimerHandle DelayHPTimer;

	UPROPERTY()
	float TargetDelayHPPercent;

	UPROPERTY()
	float CurrentDelayHPPercent;

	UPROPERTY()
	FTimerHandle SmoothHPTimer;

	UPROPERTY()
	class USoundBase* VictorySound;

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

	UFUNCTION()
	void RestartReady();

	UFUNCTION()
	void SmoothUpdateDelayHP();

};
