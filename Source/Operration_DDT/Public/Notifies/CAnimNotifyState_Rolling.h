// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotifyState_Rolling.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCAnimNotifyState_Rolling : public UAnimNotifyState
{
	GENERATED_BODY()
	
	public:
    	FString GetNotifyAction() const;
    
    	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
    
    	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	class UCStateComponent* State;
	class UCCameraActionComponent* CameraAction;
	
};
