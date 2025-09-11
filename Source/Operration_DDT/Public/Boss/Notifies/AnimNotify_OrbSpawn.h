// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_OrbSpawn.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotify_OrbSpawn : public UAnimNotify
{
	GENERATED_BODY()
	FString GetNotifyName_Implementation ( ) const override;

	void Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation ) override;
};
