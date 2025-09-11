// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notifies/AnimNotify_OrbSpawn.h"
#include "Global.h"
#include "Boss/Component/BossProjectileComponent.h"

FString UAnimNotify_OrbSpawn::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_OrbSpawn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	APawn* Owner=Cast<APawn>(MeshComp->GetOwner());
	CheckNull(Owner);
	UBossProjectileComponent* ProjectileComp=CHelpers::GetComponent<UBossProjectileComponent>(Owner);
	CheckNull(ProjectileComp);
	ProjectileComp->SpawnOrb();
}
