/**
 * @file STC_IsBossDistanceLessThan.cpp
 * @brief 보스 거리 조건 확인 구현 파일
 * 
 * @details
 * 보스가 너무 가까운 거리에 있는지 확인하는 조건의 구현 파일입니다.
 * StateTree에서 사용되는 조건 클래스로, Evaluator에서 설정된 CurrentRangeTag가
 * TooClose 태그와 일치하는지 확인하여 StateTree의 상태 전환 조건으로 사용됩니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#include "Boss/Condition/STC_IsBossDistanceLessThan.h"
#include "StateTreeExecutionContext.h"
#include "Boss/Component/CBossStatusComponent.h"
#include "Global.h"

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
	UCBossStatusComponent* StatusComp= CHelpers::GetComponent<UCBossStatusComponent>(Boss);
	StatusComp->IncreaseAP(50);
	return CurrentRangeTag.MatchesTag(RangeTags.TooClose);
}
