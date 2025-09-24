// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CUIComponent.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Player/Widget/CPlayerUI.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CMagazineComponent.h"
#include "Player/Components/CRespawnComponent.h"
#include "Player/Components/CStaminaComponent.h"
#include "Player/Components/CStatusComponent.h"
#include "Player/Widget/CPauseWidget.h"

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

	playerUI = Cast<UCPlayerUI>(CreateWidget(GetWorld(),UCPlayerUIWidget));
	pauseWidget = Cast<UCPauseWidget>(CreateWidget(GetWorld(), UCPauseWidgetUI));
	//InitUIWidget();
	// ...
	OwnerCharater = Cast<ADDTPlayer>(GetOwner());
	StaminaComp = CHelpers::GetComponent<UCStaminaComponent>(OwnerCharater);
	StatusComp = CHelpers::GetComponent<UCStatusComponent>(OwnerCharater);
	MagazineComp = CHelpers::GetComponent<UCMagazineComponent>(OwnerCharater);
	RespawnComp = CHelpers::GetComponent<UCRespawnComponent>(OwnerCharater);

	if (OwnerCharater && playerUI)
	{
		if (StaminaComp)
		{
			StaminaComp->OnStaminaChanged.AddDynamic(this, &UCUIComponent::OnStaminaChanged);
			OnStaminaChanged(StaminaComp->GetNowStamina(),StaminaComp->GetNowStamina(), StaminaComp->GetMaxStamina());
		}
		if (StatusComp)
		{
			StatusComp->OnPlayerHealthChanged.AddDynamic(this, &UCUIComponent::OnHealthChanged);
			OnHealthChanged(StatusComp->GetNowHp(),StatusComp->GetNowHp(), StatusComp->GetMaxHP());

			StatusComp->OnHealItemChanged.AddDynamic(this, &UCUIComponent::OnHealItemChanged);
			playerUI->SetHealItem(StatusComp->GetHealItemCount());
		}
		if (MagazineComp)
		{
			MagazineComp->OnCurrentBulletChanged.AddDynamic(this, &UCUIComponent::OnCurrentBulletChanged);
			OnCurrentBulletChanged(MagazineComp->CurrentRifleBullets);
			MagazineComp->OnTotalBulletChanged.AddDynamic(this, &UCUIComponent::OnTotalBulletChanged);
			OnTotalBulletChanged(MagazineComp->TotalRifleBullets);
		}
		if (RespawnComp)
		{
			RespawnComp->OnPlayerDeath.AddDynamic(this, &UCUIComponent::OnPlayerDeathCallAnimation);
		}
		
		//playerUI->OnAnimFinishedDelegate.AddDynamic(this,&UCUIComponent::OnDeathAnimationEnd );
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
		playerUI->AddToViewport();
		playerUI->ShowCrosshair(false);
	}
}

void UCUIComponent::OnStaminaChanged(float prevStamina, float nowStamina, float maxStamina)
{
	if (playerUI)
	{
		playerUI->SetStaminaBar(nowStamina, maxStamina);
		playerUI->SetStaminaBar_Background(prevStamina, nowStamina);
		//CLog::Log(FString::Printf(TEXT("스테미나 UI 업데이트: %.1f/%.1f"), nowStamina, maxStamina));
	}
}

void UCUIComponent::OnHealthChanged(float prevHp, float nowHp, float maxHp)
{
	if (playerUI)
	{
		playerUI->SetHPBar(nowHp, maxHp);
		playerUI->SetHPBar_Background(prevHp,nowHp);
		//CLog::Log(FString::Printf(TEXT("체력 UI 업데이트: %.1f/%.1f"), nowHp, maxHp));
		
	}
}

void UCUIComponent::OnHealItemChanged(int32 NewCount)
{
	if (playerUI)
	{
		playerUI->SetHealItem(NewCount);
	}
}

void UCUIComponent::OnCurrentBulletChanged(int32 value)
{
	if (playerUI)
	{
		playerUI->SetCurrentBullet(value);
	}
}

void UCUIComponent::OnTotalBulletChanged(int32 value)
{
	if (playerUI)
	{
		playerUI->SetTotalBullet(value);
	}
}

void UCUIComponent::OnPlayerDeathCallAnimation()
{
	if (playerUI)
	{
		playerUI->CallDeathAnimation();
	}
}

void UCUIComponent::OnDeathAnimationEnd()
{
	if (playerUI && playerUI->IsInViewport())
	{
		playerUI->RemoveFromParent();
	}
}


