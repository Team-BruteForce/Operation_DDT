// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Notifies/AnimNotify_DropSwordMagic.h"
#include "Global.h"
#include "Boss/Component/BossProjectileComponent.h"
#include "Boss/Component/CBossTargetingComponent.h"
#include "GameFramework/Character.h"

FString UAnimNotify_DropSwordMagic::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_DropSwordMagic::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	UBossProjectileComponent* ProjectileComp = CHelpers::GetComponent<UBossProjectileComponent>(MeshComp->GetOwner());
	UCBossTargetingComponent* TargetComp = CHelpers::GetComponent<UCBossTargetingComponent>(MeshComp->GetOwner());
	CheckNull(ProjectileComp);
	CheckNull(TargetComp);
	ACharacter* Pawn=Cast<ACharacter>(TargetComp->FindPlayer());
	FVector TargetLocation = Pawn->GetMesh()->GetBoneLocation("root"); 
	ProjectileComp->SpawnHolySwordMagicRepeatedly(0.5f, 5);
}
