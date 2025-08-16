// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file STC_IsBossActionInProgress.cpp
 * @brief 보스가 현재 어택 중인지 확인하는 조건 구현 파일
 * 
 * 이 파일은 StateTree에서 사용되는 조건 클래스를 구현합니다.
 * 보스가 현재 어택 행동을 수행 중인지 확인하여
 * StateTree의 상태 전환 조건으로 사용됩니다.
 * 
 * 주요 기능:
 * - 보스 어택 상태 확인
 * - 어택 태그 기반 조건 판단
 * - StateTree 상태 전환 조건 제공
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
	// 방법 2: 어택 태그가 유효한지 체크 (어택 중인 경우)
		CLog::Log(CurrentActionTag.ToString());
	
	if (CurrentActionTag.IsValid())
	{
		// 어택 관련 태그인지 확인 (예: "BOSS.Action.Attack" 등)
		CLog::Log(CurrentActionTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("BOSS.State.Attacking"))));
		return !CurrentActionTag.MatchesTag(FGameplayTag::RequestGameplayTag(TEXT("BOSS.State.Attacking")));
	}
	
	return true;
}
