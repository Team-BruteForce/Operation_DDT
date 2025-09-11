#include "Boss/Condition/STC_RandLocationCheck.h"
#include "Boss/Component/FlyingComponent.h"
#include "Global.h"

bool USTC_RandLocationCheck::TestCondition(FStateTreeExecutionContext& Context) const
{
	if (!Boss)
	{
		return false;
	}

	UFlyingComponent* FlyingComp = CHelpers::GetComponent<UFlyingComponent>(Boss);
	if (!FlyingComp)
	{
		return false;
	}

	// 랜덤 이동 완료 체크 (이동 중이 아니면 완료)
	return !FlyingComp->IsMovingToRandomPoint();
}

