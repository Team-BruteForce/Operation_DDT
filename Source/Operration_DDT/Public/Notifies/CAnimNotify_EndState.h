// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Player/Components/CStateComponent.h"
#include "CAnimNotify_EndState.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCAnimNotify_EndState : public UAnimNotify
{
	GENERATED_BODY()

	private:
		UPROPERTY(EditAnywhere, Category = "Type")
		EStateType StateType;
	
	public:
		FString GetNotifyName_Implementation() const override;

		void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
