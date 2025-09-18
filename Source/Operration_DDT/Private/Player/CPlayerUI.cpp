// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPlayerUI.h"
#include "Global.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UCPlayerUI::ShowCrosshair(bool bValue)
{
	CheckNull(img_Crosshair);
	if (bValue)
	{
		img_Crosshair->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		img_Crosshair->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UCPlayerUI::SetHPBar(float value, float maxHP)
{
	CheckNull(pb_HP);

	pb_HP->SetPercent(value / maxHP);
}

void UCPlayerUI::SetStaminaBar(float value, float maxStamina)
{
	CheckNull(pb_Stamina);
	pb_Stamina->SetPercent(value / maxStamina);
}

void UCPlayerUI::SetCurrentBullet(int32 value)
{
	CheckNull(txt_CurrentBullet);
	if (value > 9)
	{
		txt_CurrentBullet->SetText(FText::FromString(FString::FromInt(value)));
	}
	else
	{
		txt_CurrentBullet->SetText(FText::FromString("0" + FString::FromInt(value)));
	}
}

void UCPlayerUI::SetTotalBullet(int32 value)
{
	CheckNull(txt_TotalBullet);
	if (value > 9)
	{
		txt_TotalBullet->SetText(FText::FromString(FString::FromInt(value)));
	}
	else
	{
		txt_TotalBullet->SetText(FText::FromString("0" + FString::FromInt(value)));
	}
}

void UCPlayerUI::SetHealItem(int32 value)
{
	CheckNull(txt_HealItem);
	CheckNull(img_injection_empty);
	CheckNull(img_injection_full);

	if( value > 0 )
	{
		txt_HealItem->SetText(FText::FromString(FString::FromInt(value)));
		img_injection_full->SetVisibility(ESlateVisibility::Visible);
		img_injection_empty->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		txt_HealItem->SetVisibility(ESlateVisibility::Hidden);
		img_injection_full->SetVisibility(ESlateVisibility::Hidden);
		img_injection_empty->SetVisibility(ESlateVisibility::Visible);
	}
}
