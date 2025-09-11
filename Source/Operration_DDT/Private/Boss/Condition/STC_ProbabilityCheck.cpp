#include "Boss/Condition/STC_ProbabilityCheck.h"
#include "Math/UnrealMathUtility.h"

bool USTC_ProbabilityCheck::TestCondition(FStateTreeExecutionContext& Context) const
{
	// 확률 계산 (0.0 ~ 100.0)
	float RandomValue = FMath::RandRange(0.0f, 100.0f);
	return RandomValue <= Probability;
}

