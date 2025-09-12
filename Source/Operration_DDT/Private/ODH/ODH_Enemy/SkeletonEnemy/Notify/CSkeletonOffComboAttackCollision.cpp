// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_Enemy/SkeletonEnemy/Notify/CSkeletonOffComboAttackCollision.h"
#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"

void UCSkeletonOffComboAttackCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp)
		return;

	// 소유자 액터를 SkeletonEnemy로 캐스팅
	AActor* Owner = MeshComp->GetOwner();
	if (ACSkeletonEnemy* SkeletonEnemy = Cast<ACSkeletonEnemy>(Owner))
	{
		// 콤보 콜리전 비활성화
		SkeletonEnemy->DisableComboCollisions();
	}
}

