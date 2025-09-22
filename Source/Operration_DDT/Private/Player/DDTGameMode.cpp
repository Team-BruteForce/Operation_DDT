// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DDTGameMode.h"
#include "Player/DDTPlayer.h"
#include "GameFramework/Character.h"
#include "Player/Components/CStateComponent.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Player/Widget/CPlayerUI.h"

ADDTGameMode::ADDTGameMode()
{
	CHelpers::GetClass(&UCPlayerUIWidget, AssetPaths::PlayerUI);
}

void ADDTGameMode::BeginPlay()
{
	Super::BeginPlay();
	playerUI = CreateWidget<UCPlayerUI>(GetWorld(),UCPlayerUIWidget);

	OwnerCharater = Cast<ADDTPlayer>(GetWorld()->GetFirstPlayerController()->GetOwner());

	//playerUI->AddToViewport();
	//playerUI->ShowCrosshair(false);
}

void ADDTGameMode::test()
{
	ACharacter* Character= Cast<ACharacter>(GetWorld()->GetFirstPlayerController()->GetOwner());
	CHelpers::GetComponent<UCStateComponent>(Character);
}
