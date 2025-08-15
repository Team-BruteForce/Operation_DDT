// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CBossEndAttack.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCBossEndAttack : public UAnimNotify
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation ( ) const override;

	void Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation ) override;

};
