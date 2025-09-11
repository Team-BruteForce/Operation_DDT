// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notifies/AnimNotify_EndFlying.h"
#include "Global.h"
#include "Boss/Component/FlyingComponent.h"

FString UAnimNotify_EndFlying::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_EndFlying::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	UFlyingComponent* FlyingComponent = CHelpers::GetComponent<UFlyingComponent>(MeshComp->GetOwner());
	CheckNull(FlyingComponent);
	FlyingComponent->StopFlying();
}
