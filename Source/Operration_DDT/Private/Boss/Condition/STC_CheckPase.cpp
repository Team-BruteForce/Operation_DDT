// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Condition/STC_CheckPase.h"
#include "Global.h"
#include "Boss/CBoss.h"

bool USTC_CheckPase::TestCondition(FStateTreeExecutionContext& Context) const
{
	ACBoss* Boss= Cast<ACBoss>(Owner);
	return Boss->IsPaseChange;
}
