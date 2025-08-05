// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/CPatrolTask.h"
#include "Global.h"
#include "StateTreeExecutionContext.h"
#include "Enemy/AEnemy.h"
#include "Enemy/CEnemyController.h"
#include "Enemy/Component/CEnemyMovement.h"

EStateTreeRunStatus UCPatrolTask::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	CLog::Log("Patrol Task EnterState");
	
	CLog::Log(Enemy->GetName());
	UCEnemyMovement* Movement= CHelpers::GetComponent<UCEnemyMovement>(Cast<AAEnemy>(Enemy));
	Movement->Patrol();
	
	
	
	FinishTask();
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus UCPatrolTask::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	return EStateTreeRunStatus::Running;
}

