// Enemy Health Bar Widget

#include "ODH/UI/CEnemyHealthBarWidget.h"
#include "Components/ProgressBar.h"

void UCEnemyHealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCEnemyHealthBarWidget::SetHealthPercent(float InPercent)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(FMath::Clamp(InPercent, 0.0f, 1.0f));
	}
}


