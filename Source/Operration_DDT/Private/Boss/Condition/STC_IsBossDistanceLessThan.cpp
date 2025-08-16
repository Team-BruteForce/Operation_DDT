// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file STC_IsBossDistanceLessThan.cpp
 * @brief 보스가 너무 가까운 거리에 있는지 확인하는 조건 구현 파일
 * 
 * 이 파일은 StateTree에서 사용되는 조건 클래스를 구현합니다.
 * Evaluator에서 설정된 CurrentRangeTag가 TooClose 태그와 일치하는지 확인하여
 * StateTree의 상태 전환 조건으로 사용됩니다.
 * 
 * 주요 기능:
 * - 보스와 플레이어 간 거리 비교
 * - 태그 기반 조건 판단
 * - StateTree 상태 전환 조건 제공
 */

#include "Boss/Condition/STC_IsBossDistanceLessThan.h"
#include "StateTreeExecutionContext.h"

/**
 * @brief 조건을 테스트하는 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @return true: 너무 가까운 거리, false: 적당한 거리
 * 
 * Evaluator에서 설정된 CurrentRangeTag가 TooClose 태그와 일치하는지 확인합니다.
 */
bool USTC_IsBossDistanceLessThan::TestCondition(FStateTreeExecutionContext& Context) const
{
	// Evaluator에서 설정된 CurrentRangeTag와 비교
	// TooClose 범위 태그인지 확인
	return CurrentRangeTag.MatchesTag(RangeTags.TooClose);
}
