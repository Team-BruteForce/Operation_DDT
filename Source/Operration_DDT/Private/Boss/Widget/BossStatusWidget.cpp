/**
 * @file BossStatusWidget.cpp
 * @brief 보스 상태 위젯 구현 파일
 * 
 * @details
 * 보스의 상태를 표시하는 UI 위젯입니다.
 * 보스의 체력과 페이즈 상태를 시각화합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Widget/BossStatusWidget.h"

#include "Boss/CBoss.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"

void UBossStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	FCompleteUIFadeIn.BindDynamic(this,&UBossStatusWidget::ShowCompleteUI);
	BindToAnimationFinished(FadeIn,FCompleteUIFadeIn);
	FCompleteUIFadeOut.BindDynamic(this,&UBossStatusWidget::EndWidget);
	BindToAnimationFinished(FadeOut,FCompleteUIFadeOut);
	FBlackFadeIn.BindDynamic(Owner,&ACBoss::RestartUI);
	BindToAnimationFinished(BlackBoard,FBlackFadeIn);
	
}

void UBossStatusWidget::UpdateBossHP(float CurrentHPValue, float MaxHPValue)
{
	if (CurrentHP)
	{
		float HPPercent = MaxHPValue > 0 ? CurrentHPValue / MaxHPValue : 0.0f;
		CurrentHP->SetPercent(HPPercent);
	}
	
	// DelayHP는 부드럽게 감소하도록 설정
	if (DelayHP && GetWorld())
	{
		// 기존 타이머들 정리
		GetWorld()->GetTimerManager().ClearTimer(DelayHPTimer);
		GetWorld()->GetTimerManager().ClearTimer(SmoothHPTimer);
		
		// 목표 HP 퍼센트 설정
		TargetDelayHPPercent = MaxHPValue > 0 ? CurrentHPValue / MaxHPValue : 0.0f;
		
		// 현재 DelayHP 퍼센트를 현재 값으로 초기화 (처음 호출시)
		if (CurrentDelayHPPercent == 0.0f)
		{
			CurrentDelayHPPercent = TargetDelayHPPercent;
		}
		
		// 0.3초 후에 부드러운 업데이트 시작
		GetWorld()->GetTimerManager().SetTimer(DelayHPTimer, [this]()
		{
			// 부드러운 업데이트 시작 (0.02초마다 호출)
			GetWorld()->GetTimerManager().SetTimer(SmoothHPTimer, this, &UBossStatusWidget::SmoothUpdateDelayHP, 0.02f, true);
		}, 0.3f, false);
	}
}

void UBossStatusWidget::SwitchBossCompleteUI()
{
	BossWidgetSwitcher->SetActiveWidgetIndex(2);
	FadeInHandler();
	
	if (VictorySound && GetWorld())
	{
		UGameplayStatics::PlaySound2D(GetWorld(), VictorySound);
	}
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
	auto Timer=[this](){PlayAnimation(BlackBoard);};
	GetWorld()->GetTimerManager().SetTimer(Hander,Timer,6.0f,false);
}

void UBossStatusWidget::RestartReady()
{
	RemoveFromParent();
}

void UBossStatusWidget::SmoothUpdateDelayHP()
{
	if (!DelayHP) return;
	
	// 현재 값과 목표 값의 차이 계산
	float Difference = TargetDelayHPPercent - CurrentDelayHPPercent;
	
	// 차이가 매우 작으면 목표값으로 설정하고 타이머 정지
	if (FMath::Abs(Difference) < 0.001f)
	{
		CurrentDelayHPPercent = TargetDelayHPPercent;
		DelayHP->SetPercent(CurrentDelayHPPercent);
		GetWorld()->GetTimerManager().ClearTimer(SmoothHPTimer);
		return;
	}
	
	// 부드럽게 목표값으로 이동 (Lerp 사용)
	CurrentDelayHPPercent = FMath::FInterpTo(CurrentDelayHPPercent, TargetDelayHPPercent, 0.02f, 3.0f);
	DelayHP->SetPercent(CurrentDelayHPPercent);
}

