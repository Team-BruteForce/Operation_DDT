// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPlayerUI.h"

#include "Components/Image.h"

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
