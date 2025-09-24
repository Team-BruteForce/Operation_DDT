// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_ReloadLoopDone.h"
#include "Global.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CMagazineComponent.h"

void UCAnimNotify_ReloadLoopDone::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	OwnerCharacter = Cast<ADDTPlayer>(MeshComp->GetOwner());
	CheckNull(OwnerCharacter);
	Magazine = CHelpers::GetComponent<UCMagazineComponent>(OwnerCharacter);
	CheckNull(Magazine);

	
	if (Magazine->ReloadLoopCount < Magazine->MaxReloadLoopCount || Magazine->TotalRifleBullets <= 0)
	{
		OwnerCharacter->PlayAnimMontage(LoopMontage, 1.2f);
	}
	else
	{
		OwnerCharacter->PlayAnimMontage(EndMontage);
	}
}
