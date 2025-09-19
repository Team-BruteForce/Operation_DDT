// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CWeaponStructures.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "CWeaponAsset.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UCWeaponAsset : public UDataAsset
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACAttachment> AttachmentClass;

	UPROPERTY(EditAnywhere)
	FEquipmentData EquipmentData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoActionDatas;

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }
	FORCEINLINE class UCDoAction* GetDoAction() { return DoAction; }
	/*FORCEINLINE ACAttachment* GetAttachment(){ return Attachment.Get(); }
	FORCEINLINE UCEquipment* GetEquipment(){ return Equipment.Get(); }
	FORCEINLINE UCDoAction* GetDoAction(){ return DoAction.Get(); }*/
	
	

	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner);

private:

	UPROPERTY()
	class ACAttachment* Attachment;

	UPROPERTY()
	class UCEquipment* Equipment;

	UPROPERTY()
	class UCDoAction* DoAction;

	/*UPROPERTY()
	TObjectPtr<class ACAttachment> Attachment;
	
	UPROPERTY()
	TObjectPtr<class UCEquipment> Equipment;
	
	UPROPERTY()
	TObjectPtr<class UCDoAction> DoAction;*/

	
};
