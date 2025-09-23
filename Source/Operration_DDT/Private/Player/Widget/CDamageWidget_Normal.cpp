// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Widget/CDamageWidget_Normal.h"

#include <string>

#include "Components/TextBlock.h"

void UCDamageWidget_Normal::NativeConstruct()
{
	Super::NativeConstruct();
	ResetTextPosition();
	ShowDamageWidgetAnimation();
}

void UCDamageWidget_Normal::SetActive(bool inValue)
{
	bActive = inValue;
	if (bActive)
	{
		this->AddToViewport();
		ShowDamageWidgetAnimation();
	}
	else
	{
		if (this->IsInViewport())
		{
			this->RemoveFromParent();
			ResetTextPosition();
		}
	}
}

bool UCDamageWidget_Normal::IsActive() const
{
	return bActive;
}

void UCDamageWidget_Normal::SetDamageText(float value, bool isCritical, bool isGroggy)
{
	if (txt_Damage_Normal)
	{
		txt_Damage_Normal->SetColorAndOpacity(FSlateColor(FLinearColor::White));
		txt_Damage_Normal->SetText(FText::FromString(FString::FromInt(FMath::RoundToInt(value))));
		
		if (isCritical)
		{
			txt_Damage_Normal->SetColorAndOpacity(FSlateColor(FLinearColor(0.942708f, 0.032531f, 0.003298f, 1.f)));
			return;
		}
		if (isGroggy)
		{
			txt_Damage_Normal->SetColorAndOpacity(FSlateColor(FLinearColor(1.0f, 0.657953f, 0.111497f, 1.f)));
		}
		
	}
}

void UCDamageWidget_Normal::ShowDamageWidgetAnimation()
{
	if (Damage_Origin)
	{
		PlayAnimation(Damage_Origin, 0.f, 1, EUMGSequencePlayMode::Forward, 1.f);
	}
}

void UCDamageWidget_Normal::ResetTextPosition()
{
	if (txt_Damage_Normal)
	{
		txt_Damage_Normal->SetRenderOpacity(1.f);
		txt_Damage_Normal->SetRenderTranslation(FVector2D(0.f, 0.f));
		txt_Damage_Normal->SetRenderScale(FVector2D(1.f, 1.f));
		
	}
}

void UCDamageWidget_Normal::StartLifeTimer()
{
	// 수명 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(
		LifeTimerHandle,
		this,
		&UCDamageWidget_Normal::OnLifeTimeExpired,
		LifeTime,
		false  // 한 번만 실행
	);
}

void UCDamageWidget_Normal::StopLifeTimer()
{
	// 수명 타이머 중지
	GetWorld()->GetTimerManager().ClearTimer(LifeTimerHandle);
}

void UCDamageWidget_Normal::OnLifeTimeExpired()
{
	// 수명이 만료되면 풀로 돌아가기
	ReturnToPool();
}

void UCDamageWidget_Normal::ReturnToPool()
{
	if (bActive)
	{
		// 타이머 먼저 정지
		StopLifeTimer();
		
		// 상태 초기화
		SetActive(false);
	}
}

