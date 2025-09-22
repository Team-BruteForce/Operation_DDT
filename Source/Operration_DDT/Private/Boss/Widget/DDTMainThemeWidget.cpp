// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Widget/DDTMainThemeWidget.h"
void UDDTMainThemeWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(PressButtonLoopAnimation, 0.f, 0, EUMGSequencePlayMode::Forward, 1.0f);
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
}