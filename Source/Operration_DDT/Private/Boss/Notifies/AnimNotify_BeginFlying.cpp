// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notifies/AnimNotify_BeginFlying.h"
#include "Global.h"
#include "Boss/CBoss.h"
#include "Boss/Component/CBossMovementComponent.h"
#include "Boss/Component/FlyingComponent.h"

FString UAnimNotify_BeginFlying::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_BeginFlying::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	ACBoss* Owner = Cast<ACBoss>(MeshComp->GetOwner());
	CheckNull(Owner);
	UFlyingComponent* FlyingComponent = CHelpers::GetComponent<UFlyingComponent>(Owner);
	CheckNull(FlyingComponent);
	FlyingComponent->StartTakeoff(Height,speed);
}
