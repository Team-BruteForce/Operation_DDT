// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_ReloadEndDOne.h"
#include "Global.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CStateComponent.h"
#include "Player/Components/CMagazineComponent.h"


void UCAnimNotify_ReloadEndDOne::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	OwnerCharacter = Cast<ADDTPlayer>(MeshComp->GetOwner());
	CheckNull(OwnerCharacter);
	StateComponent = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	CheckNull(StateComponent);

	StateComponent->SetIsHammerPulled(true);

	Magazine = CHelpers::GetComponent<UCMagazineComponent>(OwnerCharacter);
	CheckNull(Magazine);

	Magazine->MaxReloadLoopCount = 0;
	Magazine->ReloadLoopCount = 0;

	//if (StateComponent->IsRifleAimMode())
	StateComponent->SetIdleMode();
	
}
