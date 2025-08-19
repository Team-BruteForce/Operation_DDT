// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossWeapon/CBossDoAction.h"
#include "Global.h"
#include "GameFramework/Character.h"

void UCBossDoAction::BeginPlay(class ACBossWeapon* InAttachment, class UCBossEquipment* InEquipment,
	class ACharacter* InOwner, TArray<FBossDoActionData>& InDoActionData, TArray<FBossHitData>& InHitData)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld ( );

	// State = CHelpers::GetComponent<UC> ( InOwner );
	Movement = CHelpers::GetComponent<UCBossMovementComponent> ( InOwner );

	BossDoActionDatas = InDoActionData;
	BossHitDatas = InHitData;
}

void UCBossDoAction::DoAction ( )
{
	// State->SetActionMode ();
}
void UCBossDoAction::Begin_DoAction ( )
{
	bBeginAction = true;
}

void UCBossDoAction::End_DoAction ( )
{
	// bBeginAction = false;

	// State->SetIdleMode ( );

}