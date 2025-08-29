// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Condition/STC_IsDeadState.h"
#include "Global.h"
#include "StateTreeExecutionContext.h"
#include "Boss/Component/CBossStatusComponent.h"
#include "GameFramework/Character.h"

bool USTC_IsDeadState::TestCondition(FStateTreeExecutionContext& Context) const
{
	ACharacter* Owner=Cast<ACharacter>(Context.GetOwner());
	if (Owner)
		CLog::Log("USTC_IsDeadState : "+Owner->GetName());
	// UCBossStatusComponent* Status= CHelpers::GetComponent<UCBossStatusComponent>(Owner);
	// CLog::Log("USTC_IsDeadState");
	// return Status->BossCurrentStats.CurrentHP<=0;
	return false;
}

