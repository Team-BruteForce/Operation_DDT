// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_ReloadStartDone.h"
#include "Global.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CMagazineComponent.h"

void UCAnimNotify_ReloadStartDone::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                          const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	OwnerCharacter = Cast<ADDTPlayer>(MeshComp->GetOwner());
	CheckNull(OwnerCharacter);
	Magazine = CHelpers::GetComponent<UCMagazineComponent>(OwnerCharacter);
	CheckNull(Magazine);

	CLog::Log("Bullet ReloadStartDone");
	if (Magazine->ReloadLoopCount < Magazine->MaxReloadLoopCount)
	{
		CLog::Log("Loop Choosed");
		OwnerCharacter->PlayAnimMontage(LoopMontage, 1.2f);
	}
	else
	{
		CLog::Log("End Choosed");
		OwnerCharacter->PlayAnimMontage(EndMontage);
	}
	/*Magazine = CHelpers::GetComponent<UCMagazineComponent>(OwnerCharacter);
	CheckNull(Magazine);*/
		
}
