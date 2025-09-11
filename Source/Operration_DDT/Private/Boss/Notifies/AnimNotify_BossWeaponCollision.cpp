// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notifies/AnimNotify_BossWeaponCollision.h"
#include "GLobal.h"
#include "Boss/BossWeapon/CBossWeapon.h"
#include "Boss/Component/CBossWeaponComponent.h"

FString UAnimNotify_BossWeaponCollision::GetNotifyName_Implementation() const
{
	return "BossCollision";
}

void UAnimNotify_BossWeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCBossWeaponComponent* weapon = CHelpers::GetComponent<UCBossWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetBossWeapon());

	weapon->GetBossWeapon()->OnBossCollisions();
}

void UAnimNotify_BossWeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCBossWeaponComponent* weapon = CHelpers::GetComponent<UCBossWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetBossWeapon());

	weapon->GetBossWeapon()->OffBossCollisions();

}