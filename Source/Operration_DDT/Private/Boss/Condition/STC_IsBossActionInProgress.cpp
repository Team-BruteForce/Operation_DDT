/**
 * @file STC_IsBossActionInProgress.cpp
 * @brief 보스 액션 진행 상태 조건 확인 구현 파일
 * 
 * @details
 * 보스가 현재 어택 중인지 확인하는 조건의 구현 파일입니다.
 * StateTree에서 사용되는 조건 클래스로, 보스가 현재 어택 행동을 수행 중인지 확인하여
 * StateTree의 상태 전환 조건으로 사용됩니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#include "Boss/Condition/STC_IsBossActionInProgress.h"
#include "StateTreeExecutionContext.h"
#include "Utilities/CLog.h"

/**
 * @brief 조건을 테스트하는 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @return true: 보스가 어택 중, false: 보스가 어택 중이 아님
 * 
 * 보스가 현재 어택 행동을 수행 중인지 확인합니다.
 */
bool USTC_IsBossActionInProgress::TestCondition(FStateTreeExecutionContext& Context) const
{
	
	if (CurrentActionTag.IsValid())
	{
		// 어택 관련 태그인지 확인 (예: "BOSS.Action.Attack" 등)
		return !CurrentActionTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("BOSS.State.Attacking")));
	}
	
	return true;
}
