// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DDTGameMode.h"
#include "Player/DDTPlayer.h"
#include "GameFramework/Character.h"
#include "Player/Components/CStateComponent.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Boss/Widget/DDTLoadingWidget.h"
#include "Boss/Widget/DDTMainThemeWidget.h"
#include "Player/CPlayerUI.h"

ADDTGameMode::ADDTGameMode()
{
	CHelpers::GetClass(&UCPlayerUIWidget, AssetPaths::PlayerUI);
	CHelpers::GetClass(&MainUIClass, AssetPaths::UI_Main);
	CHelpers::GetClass(&LoadingUIClass, AssetPaths::UI_Loading);
}

void ADDTGameMode::BeginPlay()
{
	Super::BeginPlay();
	CheckNull(GetWorld());
	playerUI = CreateWidget<UCPlayerUI>(GetWorld(),UCPlayerUIWidget);
	MainUI = CreateWidget<UDDTMainThemeWidget>(GetWorld(),MainUIClass);
	LoadingUI = CreateWidget<UDDTLoadingWidget>(GetWorld(),LoadingUIClass);

	MainUI->AddToViewport();
	
	MainUI->FMainThemeFadeOutEvent.BindDynamic(this,&ADDTGameMode::LinkedMaintoLoading);
	MainUI->BindToAnimationFinished(MainUI->PressButtonClickAnimation,MainUI->FMainThemeFadeOutEvent);

	OwnerCharater = Cast<ADDTPlayer>(GetWorld()->GetFirstPlayerController()->GetOwner());

	APlayerController* C=Cast<APlayerController>(GetWorld()->GetFirstPlayerController());
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainUI->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	C->SetInputMode(InputMode);
	C->bShowMouseCursor=false;
	
	// 위젯에 포커스 설정
	MainUI->SetKeyboardFocus();

	//playerUI->AddToViewport();
	//playerUI->ShowCrosshair(false);
}

void ADDTGameMode::test()
{
	ACharacter* Character= Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetOwner());
	CHelpers::GetComponent<UCStateComponent>(Character);
}

void ADDTGameMode::LinkedMaintoLoading()
{
	MainUI->RemoveFromParent();
	LoadingUI->AddToViewport();
	LoadingUI->PlayLoadingAnimation();
}
