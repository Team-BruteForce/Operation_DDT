// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IAIController.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIAIController : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class OPERRATION_DDT_API IIAIController
{
	GENERATED_BODY()

public:
	virtual void Patrol()=0;
	
};
