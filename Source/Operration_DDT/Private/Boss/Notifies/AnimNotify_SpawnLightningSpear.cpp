// Fill out your copyright notice in the Description page of Project Settings.

#include "Global.h"
#include "Boss/Notifies/AnimNotify_SpawnLightningSpear.h"
#include "Boss/Component/BossProjectileComponent.h"

FString UAnimNotify_SpawnLightningSpear::GetNotifyName_Implementation() const
{
	return "SpawnLightningSpear";
}

void UAnimNotify_SpawnLightningSpear::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	APawn* Owner=Cast<APawn>(MeshComp->GetOwner());
	CheckNull(Owner);
	UBossProjectileComponent* ProjectileComp=CHelpers::GetComponent<UBossProjectileComponent>(Owner);
	CheckNull(ProjectileComp);
	ProjectileComp->ShotProjectile();
}
