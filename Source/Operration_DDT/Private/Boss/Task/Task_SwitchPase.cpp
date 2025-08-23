// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task/Task_SwitchPase.h"

#include "AIController.h"
#include "Global.h"
#include "StateTreeExecutionContext.h"
#include "Boss/Component/CBossWeaponComponent.h"

EStateTreeRunStatus UTask_SwitchPase::EnterState(FStateTreeExecutionContext& Context,
                                                 const FStateTreeTransitionResult& Transition)
{
	if(bStarted) return EStateTreeRunStatus::Running;
	bStarted=true;
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task_SwitchPase - Boss actor not found!"));
		return EStateTreeRunStatus::Running;
	}
		// 컴포넌트가 아직 초기화되지 않았을 수 있음 - 안전하게 체크
	UCBossWeaponComponent* Weapon = CHelpers::GetComponent<UCBossWeaponComponent>(Owner);
	if (!Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task_SwitchPase - Weapon component not found!"));
		return EStateTreeRunStatus::Running;
	}
	Weapon->SetMode(OwningStateTag);
	CLog::Log(OwningStateTag.ToString());
	
	return EStateTreeRunStatus::Running;
}
