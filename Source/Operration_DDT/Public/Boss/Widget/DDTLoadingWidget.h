/**
 * @file DDTLoadingWidget.h
 * @brief DDT 로딩 위젯 헤더 파일
 * 
 * @details
 * 게임의 로딩 화면을 표시하는 위젯입니다.
 * 플레이어 입력과 UI 컴포넌트를 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DDTLoadingWidget.generated.h"

/**
 * @brief DDT 로딩 위젯
 * 
 * 게임의 로딩 화면을 표시하는 위젯입니다.
 * 플레이어 입력과 UI 컴포넌트를 관리합니다.
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
