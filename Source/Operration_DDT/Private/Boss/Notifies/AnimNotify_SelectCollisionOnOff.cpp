// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notifies/AnimNotify_SelectCollisionOnOff.h"
#include "GLobal.h"
#include "Boss/BossWeapon/CBossWeapon.h"
#include "Boss/Component/CBossWeaponComponent.h"
FString UAnimNotify_SelectCollisionOnOff::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_SelectCollisionOnOff::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCBossWeaponComponent* weapon = CHelpers::GetComponent<UCBossWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetBossWeapon());
	weapon->GetBossWeapon()->OnSelectCollision(CollisionName);
}

void UAnimNotify_SelectCollisionOnOff::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCBossWeaponComponent* weapon = CHelpers::GetComponent<UCBossWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetBossWeapon());

	weapon->GetBossWeapon()->OffBossCollisions();
}
