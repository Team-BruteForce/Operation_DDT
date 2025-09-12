// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotify_LineTraceOnOff.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotify_LineTraceOnOff : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation ( ) const override;
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsMove=true;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsEndMove=true;
};
