// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMagazineComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentBulletChanged, int32, NewCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTotalBulletChanged, int32, NewCount);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCMagazineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCMagazineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool CheckCanReload();

	int32 RifleMagazines = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magazine")
	int32 TotalRifleBullets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magazine")
	int32 CurrentRifleBullets;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* Reload_Start;

	int32 RevolverMagazines = 6;
	int32 CurrentRevolverBullets = 0;
	
	FORCEINLINE void FireBullet() { --CurrentRifleBullets; OnCurrentBulletChanged.Broadcast(CurrentRifleBullets); }
	
	// 장전 시퀀스 관련 변수들
	int32 ReloadLoopCount = 0;           // 현재 루프 반복 횟수
	int32 MaxReloadLoopCount = 0;        // 최대 루프 반복 횟수
	class ADDTPlayer* OwnerCharacter;
	
	bool bIsReloading = false;
	FORCEINLINE bool GetIsReloading() { return bIsReloading; }
	FORCEINLINE void SetIsReloading(bool bValue) { bIsReloading = bValue; }

	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnCurrentBulletChanged OnCurrentBulletChanged;
	
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnTotalBulletChanged OnTotalBulletChanged;
	
public:
	void ReloadRifleMagazine();
	void LootRifleBullets(int32 InValue);

	void Reloading();

	void ResetMagazines();
};
