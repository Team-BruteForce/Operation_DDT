// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_SwitchHandLtoR.h"
#include "Global.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CFireComponent.h"
#include "Player/Components/CWeaponComponent.h"

void UCAnimNotify_SwitchHandLtoR::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                         const FAnimNotifyEventReference& EventReference)
{

	Super::Notify(MeshComp, Animation, EventReference);
    
    CheckNull(MeshComp);
    CheckNull(MeshComp->GetOwner());
    OwnerCharacter = Cast<ADDTPlayer>(MeshComp->GetOwner());

    Firecomp = CHelpers::GetComponent<UCFireComponent>(MeshComp->GetOwner());
    CheckNull(Firecomp);
    
    if(MeshComp->DoesSocketExist(FName("Reload_Rifle")) && MeshComp->DoesSocketExist(FName("Hand_Rifle")))
    {
    	Weapon = Firecomp->GetActorAttachedToSocket(FName("Reload_Rifle"));
    	CheckNull(Weapon);
    	Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
    	Weapon->AttachToComponent (MeshComp, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName("Hand_Rifle"));
    }
	
	UCWeaponComponent* WeaponComp = CHelpers::GetComponent<UCWeaponComponent>(MeshComp->GetOwner());
	CheckNull(WeaponComp);
	if (!WeaponComp->IsRifleMode())
	{
		WeaponComp->SetRifleMode();
	}
	
}
