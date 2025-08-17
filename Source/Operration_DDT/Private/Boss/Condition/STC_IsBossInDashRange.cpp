// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Condition/STC_IsBossInDashRange.h"

#include "Utilities/CLog.h"

bool USTC_IsBossInDashRange::TestCondition(FStateTreeExecutionContext& Context) const
{
	return CurrentRangeTag.MatchesTag(RangeTags.Dash);
}
