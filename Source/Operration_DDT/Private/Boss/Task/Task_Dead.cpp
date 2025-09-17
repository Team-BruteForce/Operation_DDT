// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Task/Task_Dead.h"
#include "StateTreeExecutionContext.h"
#include "Boss/Component/CBossWeaponComponent.h"
#include "Global.h"
#include "Boss/CBoss.h"
#include "GameFramework/Character.h"

EStateTreeRunStatus UTask_Dead::EnterState(FStateTreeExecutionContext& Context,
                                           const FStateTreeTransitionResult& Transition)
{
	UCBossWeaponComponent* Weapon= CHelpers::GetComponent<UCBossWeaponComponent>(Boss);
	ACBoss* Owner=Cast<ACBoss>(Boss);
	Owner->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Weapon){
		Weapon->BossHitAction(SelectedTag);
		Context.Stop();
	}
	return EStateTreeRunStatus::Succeeded;
}
