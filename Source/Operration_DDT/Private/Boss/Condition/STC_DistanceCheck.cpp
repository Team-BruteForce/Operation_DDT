/**
 * @file STC_DistanceCheck.cpp
 * @brief 거리 체크 조건 구현 파일
 * 
 * @details
 * StateTree에서 보스와 플레이어 간의 거리를 체크하는 조건입니다.
 * 최소/최대 거리 범위를 확인합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Condition/STC_DistanceCheck.h"


bool USTC_DistanceCheck::TestCondition(FStateTreeExecutionContext& Context) const
{
	if (Current_Dist<MinDistance)
		return true;
	else if (Current_Dist>MaxDistance)
		return true;
	else
		return false;
}

