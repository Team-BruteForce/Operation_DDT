// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notifies/AnimNotify_PaseChangeDissolve.h"

FString UAnimNotify_PaseChangeDissolve::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_PaseChangeDissolve::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
}
