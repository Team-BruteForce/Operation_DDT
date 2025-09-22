// Enemy Health Bar Widget

#include "ODH/UI/CEnemyHealthBarWidget.h"
#include "Components/ProgressBar.h"

void UCEnemyHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCEnemyHealthBarWidget::SetHealthPercent(float InPercent, float InFrontDecreaseSpeed, float InGhostDecreaseSpeed)
{
    TargetPercent = FMath::Clamp(InPercent, 0.0f, 1.0f);
    FrontDecreaseSpeed = InFrontDecreaseSpeed;
    GhostDecreaseSpeed = InGhostDecreaseSpeed;
	// 증가(회복)는 즉시 반영, 감소는 Tick에서 보간
	if (!bAnimateOnlyOnDecrease || TargetPercent >= DisplayPercent)
	{
		DisplayPercent = TargetPercent;
        if (HealthProgressBar)
		{
			HealthProgressBar->SetPercent(DisplayPercent);
		}
        // 감소인 경우, 고스트는 이전 값을 유지하고 Tick에서 내려감
        if (bAnimateOnlyOnDecrease && HealthGhostBar)
        {
            GhostPercent = FMath::Max(GhostPercent, DisplayPercent);
            HealthGhostBar->SetPercent(GhostPercent);
        }
    }
}

void UCEnemyHealthBarWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

    // FrontBar: 감소 시에만 보간(옵션)
    if (bAnimateOnlyOnDecrease && TargetPercent < DisplayPercent)
	{
        const float Step = FrontDecreaseSpeed * InDeltaTime;
		DisplayPercent = FMath::Max(TargetPercent, DisplayPercent - Step);
		if (HealthProgressBar)
		{
			HealthProgressBar->SetPercent(DisplayPercent);
		}
	}

    // GhostBar: 항상 TargetPercent까지 느리게 감소시키되, TargetPercent 이상이면 즉시 동기화
    if (HealthGhostBar)
    {
        if (GhostPercent > TargetPercent)
        {
            const float GhostStep = GhostDecreaseSpeed * InDeltaTime;
            GhostPercent = FMath::Max(TargetPercent, GhostPercent - GhostStep);
            HealthGhostBar->SetPercent(GhostPercent);
        }
        else if (GhostPercent < TargetPercent)
        {
            // 회복 시 고스트는 즉시 따라감(격겜 연출 일반 규칙)
            GhostPercent = TargetPercent;
            HealthGhostBar->SetPercent(GhostPercent);
        }
    }
}


