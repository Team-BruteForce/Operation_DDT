// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_InsertBullet.h"
#include "Global.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CMagazineComponent.h"

void UCAnimNotify_InsertBullet::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                       const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	OwnerCharacter = Cast<ADDTPlayer>(MeshComp->GetOwner());
	CheckNull(OwnerCharacter);
	Magazine = CHelpers::GetComponent<UCMagazineComponent>(OwnerCharacter);
	CheckNull(Magazine);

	Magazine->ReloadRifleMagazine();
	Magazine->ReloadLoopCount += 1;
	CLog::Log("Bullet Inserted!");

}
