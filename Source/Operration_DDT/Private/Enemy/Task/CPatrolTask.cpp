// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/CPatrolTask.h"
#include "Global.h"
#include "StateTreeExecutionContext.h"
#include "Enemy/AEnemy.h"
#include "Enemy/CEnemyController.h"
#include "Enemy/Component/CEnemyMovement.h"

/**
 * @brief 태스크에 진입하면 적 캐릭터의 이동 컴포넌트를 통해 순찰을 시작시킵니다.
 * @details 이 함수는 상태가 시작될 때 한 번만 호출됩니다.
 */
EStateTreeRunStatus UCPatrolTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	CLog::Log("Patrol Task EnterState");
	
	CLog::Log(Enemy->GetName());
	UCEnemyMovement* Movement = CHelpers::GetComponent<UCEnemyMovement>(Cast<AAEnemy>(Enemy));
	Movement->Patrol();
	
	return EStateTreeRunStatus::Running;
}

/**
 * @brief 이 태스크는 Tick에서 별도의 로직을 수행하지 않고, 상태를 Running으로 유지합니다.
 * @details 상태 전환은 다른 조건(예: 이동 완료 이벤트)에 의해 처리됩니다.
 */
EStateTreeRunStatus UCPatrolTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	return EStateTreeRunStatus::Running;
}