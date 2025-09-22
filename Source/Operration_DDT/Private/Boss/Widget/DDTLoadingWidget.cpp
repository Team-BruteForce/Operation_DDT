// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Widget/DDTLoadingWidget.h"

void UDDTLoadingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FLoadingFadeOutEvent.BindDynamic(this,&UDDTLoadingWidget::EndLoading);
	BindToAnimationFinished(OrbLoopAnimation,FLoadingFadeOutEvent);
}

void UDDTLoadingWidget::PlayLoadingAnimation()
{
	PlayAnimation(OrbLoopAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.22f);
}

void UDDTLoadingWidget::EndLoading()
{
	RemoveFromParent();
	APlayerController* C=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	FInputModeGameOnly InputMode;
	C->SetInputMode(InputMode);
	C->bShowMouseCursor=false;
}
