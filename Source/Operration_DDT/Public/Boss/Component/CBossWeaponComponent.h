// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CBossWeaponComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCBossWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category="DataAsset")
	TMap<FGameplayTag, class UCBossWeaponAsset*> DataAssets;

public:	
	UCBossWeaponComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	class ACBossWeapon* GetBossWeapon();
	class UCBossEquipment* GetBossEquipment();
	class UCBossDoAction* GetBossDoAction();

	void BossDoAction();
	
	class UCBossWeaponAsset* GetDataAssetByTag(const FGameplayTag& InTag);

private:
	class ACharacter* OwnerCharacter;
	class UCStateComponent* State;
};
