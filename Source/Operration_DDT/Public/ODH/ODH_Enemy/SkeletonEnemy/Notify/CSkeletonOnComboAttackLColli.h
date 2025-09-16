// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CSkeletonOnComboAttackLColli.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCSkeletonOnComboAttackLColli : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UCSkeletonOnComboAttackLColli();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
