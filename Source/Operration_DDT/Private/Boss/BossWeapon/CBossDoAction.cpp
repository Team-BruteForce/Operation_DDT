// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossWeapon/CBossDoAction.h"
#include "Global.h"
#include "Boss/Component/BossStateComponent.h"
#include "GameFramework/Character.h"


void UCBossDoAction::BeginPlay(class ACBossWeapon* InAttachment, class UCBossEquipment* InEquipment,
	class ACharacter* InOwner, TMap<FGameplayTag, FBossDoActionData>& InDoActionData,
	TMap<FGameplayTag, FBossHitData>& InHitData)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld ( );

	// State = CHelpers::GetComponent<UC> ( InOwner );
	Movement = CHelpers::GetComponent<UCBossMovementComponent> ( InOwner );

	BossDoActionDatas = InDoActionData;
	BossHitDatas = InHitData;
}

void UCBossDoAction::DoAction (FGameplayTag Tag)
{
	BossDoActionDatas[Tag].DoAction(OwnerCharacter);
}
void UCBossDoAction::Begin_DoAction ( )
{
	// bBeginAction = true;
}

void UCBossDoAction::End_DoAction ( )
{
	// bBeginAction = false;

	// State->SetIdleMode ( );

}