/**
 * @file STC_IsBossInMeleeRange.cpp
 * @brief 보스 근접 범위 조건 확인 구현 파일
 * 
 * @details
 * 보스가 근접 범위에 있는지 확인하는 조건의 구현 파일입니다.
 * StateTree에서 사용되는 조건 클래스로, Evaluator에서 설정된 CurrentRangeTag가
 * Melee 태그와 일치하는지 확인하여 StateTree의 상태 전환 조건으로 사용됩니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#include "Boss/Condition/STC_IsBossInMeleeRange.h"
#include "StateTreeExecutionContext.h"

/**
 * @brief 조건을 테스트하는 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @return true: 근접 범위, false: 다른 범위
 * 
 * Evaluator에서 설정된 CurrentRangeTag가 Melee 태그와 일치하는지 확인합니다.
 */
bool USTC_IsBossInMeleeRange::TestCondition(FStateTreeExecutionContext& Context) const
{
	// Evaluator에서 설정된 CurrentRangeTag와 비교
	// Melee 범위 태그인지 확인
	return CurrentRangeTag.MatchesTag(RangeTags.Melee);
}
