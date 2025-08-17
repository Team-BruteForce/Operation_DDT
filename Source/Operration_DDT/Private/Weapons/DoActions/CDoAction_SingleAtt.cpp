// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/DoActions/CDoAction_SingleAtt.h"
#include "Global.h"
#include "Player/Components/CStateComponent.h"

void UCDoAction_SingleAtt::DoAction()
{
	CheckTrue(DoActionDatas.Num() < 1);

	if (bEnable) //Enable == true -> Combo구간
	{
		bEnable = false;
		bExist = true;

		return;
	}

	CheckFalse(State->IsIdleMode());
	
	Super::DoAction();
	//첫 타격 이후에 Combo의 Enable이 호출되어야 한다.
	//첫 타격 시 IsIdleMode를 체크 후 부모의 DoAction 호출, State->ActionMode()로 전환
	DoActionDatas[0].DoAction(OwnerCharacter);
}

void UCDoAction_SingleAtt::Begin_DoAction()
{
	Super::Begin_DoAction();
	CheckFalse(bExist);
	bExist = false;
	DoActionDatas[0].DoAction(OwnerCharacter);
}

void UCDoAction_SingleAtt::End_DoAction()
{
	Super::End_DoAction();
}
