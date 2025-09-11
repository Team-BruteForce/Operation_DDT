// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Condition/STC_Stun.h"

#include "Global.h"
#include "Boss/Component/CBossStatusComponent.h"

bool USTC_Stun::TestCondition(FStateTreeExecutionContext& Context) const
{
	UCBossStatusComponent* Status=CHelpers::GetComponent<UCBossStatusComponent>(Boss);
	return Status->BossCurrentStats.CurrentGroggyGauge>=Status->BossCurrentStats.MaxGroggyGauge;
}
