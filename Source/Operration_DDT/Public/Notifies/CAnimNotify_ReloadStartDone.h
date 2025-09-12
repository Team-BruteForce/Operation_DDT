// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_ReloadStartDone.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCAnimNotify_ReloadStartDone : public UAnimNotify
{
	GENERATED_BODY()

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	class UAnimMontage* LoopMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	class UAnimMontage* EndMontage;

private:
	class ADDTPlayer* OwnerCharacter;
	class UCMagazineComponent* Magazine;
	
};
