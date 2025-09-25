/**
 * @file STC_ProbabilityCheck.cpp
 * @brief 확률 체크 조건 구현 파일
 * 
 * @details
 * StateTree에서 확률 기반 조건을 체크하는 조건입니다.
 * 랜덤 값을 사용하여 확률에 따른 조건을 확인합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Condition/STC_ProbabilityCheck.h"
#include "Math/UnrealMathUtility.h"

bool USTC_ProbabilityCheck::TestCondition(FStateTreeExecutionContext& Context) const
{
	// 확률 계산 (0.0 ~ 100.0)
	float RandomValue = FMath::RandRange(0.0f, 100.0f);
	return RandomValue <= Probability;
}

