// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_SpawnLightningSpear.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotify_SpawnLightningSpear : public UAnimNotify
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation ( ) const override;

	void Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation ) override;
};
