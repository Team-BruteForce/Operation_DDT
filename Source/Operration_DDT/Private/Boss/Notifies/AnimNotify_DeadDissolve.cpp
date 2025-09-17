// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notifies/AnimNotify_DeadDissolve.h"
#include "Boss/CBoss.h"
#include "Global.h"

FString UAnimNotify_DeadDissolve::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_DeadDissolve::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	ACBoss* Boss=Cast<ACBoss>(MeshComp->GetOwner());
	CheckNull(Boss);
	Boss->BossDissolve->TriggerDissolveEffectDead();
}
