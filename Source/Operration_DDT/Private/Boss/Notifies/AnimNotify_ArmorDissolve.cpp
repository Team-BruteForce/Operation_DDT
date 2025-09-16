// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notifies/AnimNotify_ArmorDissolve.h"

#include "Boss/CBoss.h"
#include "Global.h"

FString UAnimNotify_ArmorDissolve::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_ArmorDissolve::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	ACBoss* Boss=Cast<ACBoss>(MeshComp->GetOwner());
	CheckNull(Boss);
	Boss->StartDualLayerDissolve();
}
