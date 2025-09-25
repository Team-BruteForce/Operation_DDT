// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Widget/DDTLoadingWidget.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Global.h"
#include "Components/CanvasPanel.h"
#include "Player/DDTGameMode.h"
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
	FLoadingStartEvent.BindDynamic(this,&UDDTLoadingWidget::StartLoading);
	BindToAnimationStarted(OrbLoopAnimation,FLoadingStartEvent);
}

void UDDTLoadingWidget::PlayLoadingAnimation()
{
	PlayAnimation(OrbLoopAnimation, 0.f, 1, EUMGSequencePlayMode::Forward, 1.22f);
}

void UDDTLoadingWidget::EndLoading()
{
	if (GetWorld()->GetAuthGameMode()){
		if (Cast<ADDTGameMode>(GetWorld()->GetAuthGameMode())->IsEnd){
			UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
		}
	}
	RemoveFromParent();
	APlayerController* C=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	FInputModeGameOnly InputMode;
	C->SetInputMode(InputMode);
	C->bShowMouseCursor = false;
	//PlayAnimation(OrbLoopAnimation, 0.f, 1, EUMGSequencePlayMode::Reverse, 1.22f);
	C->SetIgnoreMoveInput(false);
	C->SetIgnoreLookInput(false);

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
			RespawnComp->ResetIsRespawning();
		}
		
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem <UEnhancedInputLocalPlayerSubsystem>(C->GetLocalPlayer());
		if (subsys)
		{
			subsys->AddMappingContext(player->IMC_Player, 0);
		}
    
	}

	Reset();
	
	OnLoadingFadeoutEnd.Broadcast();
	
}

void UDDTLoadingWidget::Reset()
{
	if (CanvasPanel_32)
	{
		CanvasPanel_32->SetRenderOpacity(1.f);
	}
}

void UDDTLoadingWidget::StartLoading()
{
	APlayerController* C = Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	ADDTPlayer* player = Cast<ADDTPlayer>(C->GetPawn());
	if (player)
	{
		UCRespawnComponent* RespawnComp = CHelpers::GetComponent<UCRespawnComponent>(player);
		if (RespawnComp)
		{
			RespawnComp->RespawnPlayer();
		}
	}
}
