// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Widget/DDTLoadingWidget.h"
#include "Global.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CRespawnComponent.h"
#include "Player/Components/CUIComponent.h"
#include "Player/Widget/CPlayerUI.h"
#include "Slate/SGameLayerManager.h"
#include "Sound/SoundCue.h"

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
	PlayAnimation(OrbLoopAnimation, 0.f, 1, EUMGSequencePlayMode::Reverse, 1.22f);
	

	ADDTPlayer* player = Cast<ADDTPlayer>(C->GetPawn());
	if (player)
	{
		UCUIComponent* UIComp = CHelpers::GetComponent<UCUIComponent>(player);
		if (UIComp)
		{
			UIComp->InitUIWidget();
			UGameplayStatics::PlaySound2D(player->GetWorld(), UIComp->RespawnSound);
		}
		UCRespawnComponent* RespawnComp = CHelpers::GetComponent<UCRespawnComponent>(player);
		if (RespawnComp)
		{
			RespawnComp->RespawnPlayer();
		}
	}
}
