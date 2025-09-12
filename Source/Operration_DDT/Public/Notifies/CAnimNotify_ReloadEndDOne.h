// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_ReloadEndDOne.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCAnimNotify_ReloadEndDOne : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	class ADDTPlayer* OwnerCharacter;
	class UCStateComponent* StateComponent;
	class UCMagazineComponent* Magazine;
	
};
