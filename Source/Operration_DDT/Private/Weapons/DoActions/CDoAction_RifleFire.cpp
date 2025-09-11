// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DoActions/CDoAction_RifleFire.h"
#include "Global.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CFireComponent.h"
#include "Player/Components/CMagazineComponent.h"
#include "Player/Components/CStateComponent.h"

void UCDoAction_RifleFire::DoAction()
{
	CheckTrue(DoActionDatas.Num() < 1);

	if (bEnable) //Enable == true -> Combo구간
	{
		bEnable = false;
		bExist = true;

		return;
	}
	
	CheckFalse(State->IsIdleMode() && State->IsRifleAimMode());
	
	Super::DoAction();
	//첫 타격 이후에 Combo의 Enable이 호출되어야 한다.
	//첫 타격 시 IsIdleMode를 체크 후 부모의 DoAction 호출, State->ActionMode()로 전환
	
	
	FireComp = CHelpers::GetComponent<UCFireComponent>(OwnerCharacter);
	StateComp = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	MagazineComponent = CHelpers::GetComponent<UCMagazineComponent>(OwnerCharacter);
	
	if (FireComp)
	{
		if (StateComp->GetIsHammerPulled() == false)
		{
			DoActionDatas[1].DoAction(OwnerCharacter);
		}
			
		if (StateComp->GetIsHammerPulled() == true && MagazineComponent->CurrentRifleBullets > 0)
		{
			DoActionDatas[0].DoAction(OwnerCharacter);
			FireComp->Fire();
			StateComp->SetIsHammerPulled(false);
			DoActionDatas[1].DoAction(OwnerCharacter);
		}
		
	}
}

void UCDoAction_RifleFire::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);
	bExist = false;
	DoActionDatas[0].DoAction(OwnerCharacter);

	CLog::Log("Fire Next");
}

void UCDoAction_RifleFire::End_DoAction()
{
	Super::End_DoAction();
}

