// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPlayerUI.h"

#include "Components/Image.h"
#include "Components/ProgressBar.h"

void UCPlayerUI::ShowCrosshair(bool bValue)
{
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
	if (pb_HP)
	{
		pb_HP->SetPercent(value / maxHP);
	}
}

void UCPlayerUI::SetStaminaBar(float value, float maxStamina)
{
	if (pb_Stamina)
	{
		pb_Stamina->SetPercent(value / maxStamina);
	}
}
