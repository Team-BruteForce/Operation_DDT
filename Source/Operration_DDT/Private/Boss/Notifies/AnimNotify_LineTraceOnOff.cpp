// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notifies/AnimNotify_LineTraceOnOff.h"
#include "Boss/BossWeapon/CBossWeapon.h"
#include "Boss/Component/CBossWeaponComponent.h"
#include "Global.h"

FString UAnimNotify_LineTraceOnOff::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_LineTraceOnOff::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	UCBossWeaponComponent* Weapon=CHelpers::GetComponent<UCBossWeaponComponent>(MeshComp->GetOwner());
	CheckNull(Weapon);
	// 노티파이 스테이트 Begin
	Weapon->GetBossWeapon()->StartCollisionAtSocket(FName("Slash"),IsMove);
}

void UAnimNotify_LineTraceOnOff::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UCBossWeaponComponent* Weapon=CHelpers::GetComponent<UCBossWeaponComponent>(MeshComp->GetOwner());
	CheckNull(Weapon);
	// 노티파이 스테이트 Begin
	Weapon->GetBossWeapon()->EndCollisionToOwner(IsEndMove);
}
