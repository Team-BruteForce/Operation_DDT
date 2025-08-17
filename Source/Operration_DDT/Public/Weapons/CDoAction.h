// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructures.h"
#include "CDoAction.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class OPERRATION_DDT_API UCDoAction : public UObject
{
	GENERATED_BODY()

public:
	UCDoAction();
	
	virtual void BeginPlay
	(
		class ACAttachment* InAttachment,
		class UCEquipment* InEquipment,
		class ACharacter* InOwner,
		const TArray<FDoActionData>& InDoActionDatas
		
	);
	virtual void DoAction();
	virtual void Begin_DoAction();
	virtual void End_DoAction();

protected:
	bool bBeginAction;

	class ACharacter* OwnerCharacter;
	class UWorld* World;

	class UCMovementComponent* Movement;
	class UCStateComponent* State;

	TArray<FDoActionData> DoActionDatas;


	
};
