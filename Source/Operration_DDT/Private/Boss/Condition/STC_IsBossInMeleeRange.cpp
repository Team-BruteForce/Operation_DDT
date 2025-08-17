// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Condition/STC_IsBossInMeleeRange.h"
#include "StateTreeExecutionContext.h"

bool USTC_IsBossInMeleeRange::TestCondition(FStateTreeExecutionContext& Context) const
{
	// Evaluator에서 설정된 CurrentRangeTag와 비교
	// Melee 범위 태그인지 확인
	return CurrentRangeTag.MatchesTag(RangeTags.Melee);
}
