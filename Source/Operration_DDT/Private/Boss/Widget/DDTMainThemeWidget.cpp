// Fill out your copyright notice in the Description page of Project Settings.


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