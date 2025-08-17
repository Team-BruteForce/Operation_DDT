// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CDoAction.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Player/Components/CMovementComponent.h"
#include "Player/Components/CStateComponent.h"

UCDoAction::UCDoAction()
{
}

void UCDoAction::BeginPlay(class ACAttachment* InAttachment, class UCEquipment* InEquipment, class ACharacter* InOwner,
                           const TArray<FDoActionData>& InDoActionDatas)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();

	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);

	DoActionDatas = InDoActionDatas;
	
}

void UCDoAction::DoAction()
{
	State->SetActionMode();
}

void UCDoAction::Begin_DoAction()
{
	bBeginAction = true;
}

void UCDoAction::End_DoAction()
{
	bBeginAction = false;

	State->SetIdleMode();
	Movement->Move();
	Movement->DisableFixedCamera();
}
