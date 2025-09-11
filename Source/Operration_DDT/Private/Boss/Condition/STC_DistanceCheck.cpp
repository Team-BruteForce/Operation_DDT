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

