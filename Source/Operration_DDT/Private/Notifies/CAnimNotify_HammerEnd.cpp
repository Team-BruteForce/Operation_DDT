// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_HammerEnd.h"
#include "Global.h"
#include "Player/Components/CStateComponent.h"
#include "Player/Components/CFireComponent.h"

void UCAnimNotify_HammerEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                    const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	StateComp = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());	
	CheckNull(StateComp);
	
	StateComp->SetIsHammerPulled(true);
}
