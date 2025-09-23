// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notifies/AnimNotifyState_PaseChange.h"
#include "Global.h"
#include "Boss/Component/CBossStatusComponent.h"
FString UAnimNotifyState_PaseChange::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotifyState_PaseChange::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float TotalDuration)
{
	CheckNull(MeshComp->GetOwner());
	UCBossStatusComponent* Status = CHelpers::GetComponent<UCBossStatusComponent>(MeshComp->GetOwner());
	CheckNull(Status);
	Status->OnPaseChange();
}

void UAnimNotifyState_PaseChange::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	CheckNull(MeshComp->GetOwner());
	UCBossStatusComponent* Status = CHelpers::GetComponent<UCBossStatusComponent>(MeshComp->GetOwner());
	CheckNull(Status);
	Status->OffPaseChange();
}
