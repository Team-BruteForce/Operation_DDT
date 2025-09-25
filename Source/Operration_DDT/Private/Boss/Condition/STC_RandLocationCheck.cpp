/**
 * @file STC_RandLocationCheck.cpp
 * @brief 랜덤 위치 체크 조건 구현 파일
 * 
 * @details
 * StateTree에서 보스의 랜덤 위치를 체크하는 조건입니다.
 * 비행 컴포넌트를 통해 위치 상태를 확인합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

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

