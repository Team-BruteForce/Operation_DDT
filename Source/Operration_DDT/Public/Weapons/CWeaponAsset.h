// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CWeaponStructures.h"
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
	TSubclassOf<class ACAttachment> ACAttachmentClass;

	UPROPERTY(EditAnywhere)
	FEquipmentData EquipmentData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEquipment> EquipmentClass;

public:
	FORCEINLINE class ACAttachment* GetAttachment() { return Attachment; }
	FORCEINLINE class UCEquipment* GetEquipment() { return Equipment; }

	UCWeaponAsset();

	void BeginPlay(class ACharacter* InOwner);

private:
	// UPROPERTY ?? ??? ?????? ?????? ??????? ?????? ???? ??? ?????.
	// UWeaponAsset ?? UObject?¥ê??? ????? Actor?? ??????? ????X -> ?????? ?????? ???????
	UPROPERTY()
	class ACAttachment* Attachment;

	UPROPERTY()
	class UCEquipment* Equipment;

	
};
