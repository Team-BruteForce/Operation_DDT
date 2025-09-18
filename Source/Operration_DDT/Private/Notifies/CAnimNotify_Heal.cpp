// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_Heal.h"
#include "Global.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CStatusComponent.h"

void UCAnimNotify_Heal::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                               const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	OwnerCharacter = Cast<ADDTPlayer>(MeshComp->GetOwner());
	CheckNull(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	CheckNull(Status);

	Status->StartHealing();
	
}
