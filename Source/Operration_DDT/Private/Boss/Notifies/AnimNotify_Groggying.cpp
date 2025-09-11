// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notifies/AnimNotify_Groggying.h"
#include "Global.h"
#include "Boss/Component/CBossStatusComponent.h"

FString UAnimNotify_Groggying::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_Groggying::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	UCBossStatusComponent* Status = CHelpers::GetComponent<UCBossStatusComponent>(MeshComp->GetOwner());
	CheckNull(Status);
	Status->OnGroggy();
}

void UAnimNotify_Groggying::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UCBossStatusComponent* Status = CHelpers::GetComponent<UCBossStatusComponent>(MeshComp->GetOwner());
	CheckNull(Status);
	Status->OffGroggy();
}
