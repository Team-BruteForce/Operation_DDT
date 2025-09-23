// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Widget/CPlayerUI.h"
#include "Global.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"


void UCPlayerUI::NativeConstruct()
{
	Super::NativeConstruct();

	ResetDeathImg();
	OnDeathAnimFinishDelegate.BindDynamic(this, &UCPlayerUI::OnDeathAnimFinished);
	
}

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
    LastMaxHp = FMath::Max(1.0f, maxHP);
    pb_HP->SetPercent(value / LastMaxHp);
}

void UCPlayerUI::SetStaminaBar(float value, float maxStamina)
{
	CheckNull(pb_Stamina);
    LastMaxStamina = FMath::Max(1.0f, maxStamina);
    pb_Stamina->SetPercent(value / LastMaxStamina);
}

void UCPlayerUI::SetHPBar_Background(float prevHP, float newHP)
{
    CheckNull(pb_HP_Background);

    const float prevPct = FMath::Clamp(prevHP / FMath::Max(1.0f, LastMaxHp), 0.0f, 1.0f);
    const float newPct = FMath::Clamp(newHP / FMath::Max(1.0f, LastMaxHp), 0.0f, 1.0f);

    // 체력 증가 시에는 즉시 동기화 (원하면 보간으로 바꿀 수 있음)
    if (newPct >= prevPct)
    {
        pb_HP_Background->SetPercent(newPct);
        bHPBgLerping = false;
        HPBgCurrent = newPct;
        return;
    }

    // 감소일 때: 기존 대기/보간 취소 후 1초 지연 예약
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(HPBgDelayHandle);
    }
    bHPBgLerping = false;

    HPBgPrev = prevPct;
    HPBgTarget = newPct;
    HPBgCurrent = prevPct;
    HPBgElapsed = 0.0f;

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            HPBgDelayHandle,
            this,
            &UCPlayerUI::StartHPBackgroundLerp,
            1.0f,
            false
        );
    }
}

void UCPlayerUI::SetStaminaBar_Background(float prevStamina, float newStamina)
{
    CheckNull(pb_Stamina_Background);

    const float prevPct = FMath::Clamp(prevStamina / FMath::Max(1.0f, LastMaxStamina), 0.0f, 1.0f);
    const float newPct  = FMath::Clamp(newStamina / FMath::Max(1.0f, LastMaxStamina), 0.0f, 1.0f);

    // 증가(회복)일 때는 즉시 동기화
    if (newPct >= prevPct)
    {
        pb_Stamina_Background->SetPercent(newPct);
        bStaminaBgLerping = false;
        StaminaBgCurrent = newPct;
        return;
    }

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().ClearTimer(StaminaBgDelayHandle);
    }
    bStaminaBgLerping = false;

    StaminaBgPrev = prevPct;
    StaminaBgTarget = newPct;
    StaminaBgCurrent = prevPct;
    StaminaBgElapsed = 0.0f;

    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            StaminaBgDelayHandle,
            this,
            &UCPlayerUI::StartStaminaBackgroundLerp,
            1.0f,
            false
        );
    }
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


void UCPlayerUI::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    if (bHPBgLerping)
    {
        HPBgElapsed += InDeltaTime;
        const float t = FMath::Clamp(HPBgElapsed / HPBgLerpDuration, 0.0f, 1.0f);
        const float easedT = t; // 선형. 필요시 EaseOut: 1 - FMath::Pow(1 - t, 3)
        HPBgCurrent = FMath::Lerp(HPBgPrev, HPBgTarget, easedT);

        if (pb_HP_Background)
        {
            pb_HP_Background->SetPercent(HPBgCurrent);
        }

        if (t >= 1.0f)
        {
            bHPBgLerping = false;
            HPBgCurrent = HPBgTarget;
        }
    }

    if (bStaminaBgLerping)
    {
        StaminaBgElapsed += InDeltaTime;
        const float t = FMath::Clamp(StaminaBgElapsed / StaminaBgLerpDuration, 0.0f, 1.0f);
        const float easedT = t;
        StaminaBgCurrent = FMath::Lerp(StaminaBgPrev, StaminaBgTarget, easedT);

        if (pb_Stamina_Background)
        {
            pb_Stamina_Background->SetPercent(StaminaBgCurrent);
        }

        if (t >= 1.0f)
        {
            bStaminaBgLerping = false;
            StaminaBgCurrent = StaminaBgTarget;
        }
    }
}

void UCPlayerUI::StartHPBackgroundLerp()
{
    bHPBgLerping = true;
    HPBgElapsed = 0.0f;
}

void UCPlayerUI::StartStaminaBackgroundLerp()
{
    bStaminaBgLerping = true;
    StaminaBgElapsed = 0.0f;
}

void UCPlayerUI::CallDeathAnimation()
{
	img_GameOver->SetVisibility(ESlateVisibility::Visible);
	img_GameOver_Sub->SetVisibility(ESlateVisibility::Visible);
	img_GameOverBlack->SetVisibility(ESlateVisibility::Visible);

	BindToAnimationFinished(Death, OnDeathAnimFinishDelegate);

	PlayAnimation(Death); 
}

void UCPlayerUI::ResetDeathImg()
{
	img_GameOver->SetVisibility(ESlateVisibility::Hidden);
	img_GameOver_Sub->SetVisibility(ESlateVisibility::Hidden);
	img_GameOverBlack->SetVisibility(ESlateVisibility::Hidden);
}

void UCPlayerUI::OnDeathAnimFinished()
{
	StartDeathAnimTimer();
}

void UCPlayerUI::StartDeathAnimTimer()
{
	// 5초 후 델리게이트 브로드캐스트를 위한 타이머 설정
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			DeathAnimDelayHandle,
			this,
			&UCPlayerUI::DeathTimerEnd,
			5.0f,
			false
		);
	}
}

void UCPlayerUI::DeathTimerEnd()
{
	ResetDeathImg();
	OnAnimFinishedDelegate.Broadcast();
}
