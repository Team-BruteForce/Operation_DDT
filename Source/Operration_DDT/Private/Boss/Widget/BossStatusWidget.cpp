// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/Widget/BossStatusWidget.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetSwitcher.h"

void UBossStatusWidget::UpdateBossHP(float CurrentHPValue, float MaxHPValue)
{
	if (CurrentHP)
	{
		float HPPercent = MaxHPValue > 0 ? CurrentHPValue / MaxHPValue : 0.0f;
		CurrentHP->SetPercent(HPPercent);
	}
	
	// DelayHP는 천천히 감소하도록 타이머 설정
	if (DelayHP && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(DelayHPTimer);
		GetWorld()->GetTimerManager().SetTimer(DelayHPTimer, [this, CurrentHPValue, MaxHPValue]()
		{
			if (DelayHP)
			{
				float DelayHPPercent = MaxHPValue > 0 ? CurrentHPValue / MaxHPValue : 0.0f;
				DelayHP->SetPercent(DelayHPPercent);
			}
		}, 0.5f, false); // 0.5초 후에 DelayHP 업데이트
	}
}

void UBossStatusWidget::SwitchBossCompleteUI()
{
	BossWidgetSwitcher->SetActiveWidgetIndex(2);
}

