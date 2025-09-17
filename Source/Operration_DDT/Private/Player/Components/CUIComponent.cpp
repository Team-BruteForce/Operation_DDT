// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CUIComponent.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Player/CPlayerUI.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CStaminaComponent.h"
#include "Player/Components/CStatusComponent.h"

// Sets default values for this component's properties
UCUIComponent::UCUIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCUIComponent::BeginPlay()
{
	Super::BeginPlay();
	
	InitUIWidget();
	// ...
	OwnerCharater = Cast<ADDTPlayer>(GetOwner());
	StaminaComp = CHelpers::GetComponent<UCStaminaComponent>(OwnerCharater);
	StatusComp = CHelpers::GetComponent<UCStatusComponent>(OwnerCharater);

	if (OwnerCharater && playerUI)
	{
		if (StaminaComp)
		{
			StaminaComp->OnStaminaChanged.AddDynamic(this, &UCUIComponent::OnStaminaChanged);
			OnStaminaChanged(StaminaComp->GetNowStamina(), StaminaComp->GetMaxStamina());
		}
		if (StatusComp)
		{
			StatusComp->OnPlayerHealthChanged.AddDynamic(this, &UCUIComponent::OnHealthChanged);
			OnHealthChanged(StatusComp->GetNowHp(), StatusComp->GetMaxHP());
		}
	}
	
}


// Called every frame
void UCUIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCUIComponent::InitUIWidget()
{
	if (UCPlayerUIWidget)
	{
		playerUI = Cast<UCPlayerUI>(CreateWidget(GetWorld(),UCPlayerUIWidget));
		playerUI->AddToViewport();
		playerUI->ShowCrosshair(false);
	}
}

void UCUIComponent::OnStaminaChanged(float nowStamina, float maxStamina)
{
	if (playerUI)
	{
		playerUI->SetStaminaBar(nowStamina, maxStamina);
		CLog::Log(FString::Printf(TEXT("스테미나 UI 업데이트: %.1f/%.1f"), nowStamina, maxStamina));
	}
}

void UCUIComponent::OnHealthChanged(float nowHp, float maxHp)
{
	if (playerUI)
	{

		playerUI->SetHPBar(nowHp, maxHp);
		CLog::Log(FString::Printf(TEXT("체력 UI 업데이트: %.1f/%.1f"), nowHp, maxHp));
		
	}
}

