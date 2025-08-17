// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file Task_Action.cpp
 * @brief StateTree 액션 태스크 구현 파일
 * 
 * 이 파일은 StateTree에서 액션 상태를 관리하는 태스크를 구현합니다.
 * 액션 상태 진입 시 IsAction 플래그를 설정하여 다른 시스템에서
 * 현재 액션 상태임을 알 수 있도록 합니다.
 * 
 * 주요 기능:
 * - 액션 상태 진입 시 IsAction 플래그 설정
 * - StateTree와의 연동을 위한 기본 액션 태스크
 */

#include "Boss/Task/Task_Action.h"

#include "StateTreeExecutionContext.h"
#include "Boss/StateTree/CBossEnemyStateTreeEvaluator.h"
#include "Utilities/CLog.h"

EStateTreeRunStatus UTask_Action::EnterState(FStateTreeExecutionContext& Context,
                                             const FStateTreeTransitionResult& Transition)
{
	CLog::Log("UTask_Action");
	IsAction=true;
	return Super::EnterState(Context, Transition);
}
