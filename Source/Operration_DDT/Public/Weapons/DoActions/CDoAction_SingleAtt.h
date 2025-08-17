// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_SingleAtt.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCDoAction_SingleAtt : public UCDoAction
{
	GENERATED_BODY()

public:
	FORCEINLINE void SwitchComboAble(bool InBool) { bEnable = InBool; }

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction() override;

private:
	bool bEnable;;
	bool bExist;
	
};
