// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/Widget/BossStatusWidget.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetSwitcher.h"

void UBossStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FCompleteUIFadeIn.BindDynamic(this,&UBossStatusWidget::ShowCompleteUI);
	BindToAnimationFinished(FadeIn,FCompleteUIFadeIn);
	FCompleteUIFadeOut.BindDynamic(this,&UBossStatusWidget::EndWidget);
	BindToAnimationFinished(FadeOut,FCompleteUIFadeOut);
}

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
	FadeInHandler();
}

void UBossStatusWidget::FadeInHandler()
{
	PlayAnimation(FadeIn);
}

void UBossStatusWidget::ShowCompleteUI()
{
	auto Timer=[this](){FadeOutHandler();};
	GetWorld()->GetTimerManager().SetTimer(Hander,Timer,2.0f,false);
}

void UBossStatusWidget::FadeOutHandler()
{
	PlayAnimation(FadeOut);
}

void UBossStatusWidget::EndWidget()
{
	RemoveFromParent();
}

