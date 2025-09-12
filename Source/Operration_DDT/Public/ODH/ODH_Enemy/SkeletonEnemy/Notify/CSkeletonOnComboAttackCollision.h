// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CSkeletonOnComboAttackCollision.generated.h"

/**
 * SkeletonEnemy 콤보 공격 콜리전 활성화 노티파이
 */
UCLASS()
class OPERRATION_DDT_API UCSkeletonOnComboAttackCollision : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
