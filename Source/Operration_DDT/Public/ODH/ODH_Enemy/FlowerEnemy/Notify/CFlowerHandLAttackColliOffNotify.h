// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CFlowerHandLAttackColliOffNotify.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCFlowerHandLAttackColliOffNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UCFlowerHandLAttackColliOffNotify();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
