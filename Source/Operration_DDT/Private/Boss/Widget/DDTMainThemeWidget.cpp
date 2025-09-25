/**
 * @file DDTMainThemeWidget.cpp
 * @brief DDT 메인 테마 위젯 구현 파일
 * 
 * @details
 * 게임의 메인 테마를 표시하는 위젯입니다.
 * 오디오 컴포넌트를 통해 배경음악을 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Widget/DDTMainThemeWidget.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

void UDDTMainThemeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(PressButtonLoopAnimation, 0.f, 0, EUMGSequencePlayMode::Forward, 1.0f);
	StartBGM();
}

FReply UDDTMainThemeWidget::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	PlayClickEvent();
	return Super::NativeOnKeyDown(InGeometry, InKeyEvent);
}

void UDDTMainThemeWidget::PlayClickEvent()
{
	if (!IsClick)return;
	IsClick=false;
	StopAnimation(PressButtonLoopAnimation);
	PlayAnimation(PressButtonClickAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.0f);
	
	if (ClickSound)
		UGameplayStatics::PlaySound2D(this, ClickSound);
}

void UDDTMainThemeWidget::StartBGM()
{
	if (!BGMSound) return;
	
	BGMComponent = UGameplayStatics::SpawnSound2D(this, BGMSound);
	if (BGMComponent)
	{
		CurrentBGMVolume = 0.5f;
		TargetBGMVolume = 1.0f;
		BGMComponent->SetVolumeMultiplier(CurrentBGMVolume);
		
		GetWorld()->GetTimerManager().SetTimer(FadeInTimerHandle, this, &UDDTMainThemeWidget::UpdateBGMVolume, 0.1f, true);
	}
}

void UDDTMainThemeWidget::UpdateBGMVolume()
{
	if (!BGMComponent) return;
	
	CurrentBGMVolume = FMath::FInterpTo(CurrentBGMVolume, TargetBGMVolume, GetWorld()->GetDeltaSeconds(), 2.0f);
	BGMComponent->SetVolumeMultiplier(CurrentBGMVolume);
	
	if (FMath::IsNearlyEqual(CurrentBGMVolume, TargetBGMVolume, 0.01f))
	{
		GetWorld()->GetTimerManager().ClearTimer(FadeInTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(FadeOutTimerHandle);
		
		if (TargetBGMVolume <= 0.0f)
		{
			BGMComponent->Stop();
		}
	}
}

void UDDTMainThemeWidget::FadeOutBGM()
{
	if (!BGMComponent) return;
	
	TargetBGMVolume = 0.0f;
	GetWorld()->GetTimerManager().SetTimer(FadeOutTimerHandle, this, &UDDTMainThemeWidget::UpdateBGMVolume, 0.1f, true);
}