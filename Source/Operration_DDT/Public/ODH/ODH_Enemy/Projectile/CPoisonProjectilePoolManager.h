// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ODH/ODH_Enemy/Projectile/CPoisonProjectile.h"
#include "CPoisonProjectilePoolManager.generated.h"

UCLASS()
class OPERRATION_DDT_API ACPoisonProjectilePoolManager : public AActor
{
	GENERATED_BODY()
	
public:
	ACPoisonProjectilePoolManager();

public:
	// 풀링 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile Pooling")
	TSubclassOf<ACPoisonProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile Pooling")
	int32 InitialSize = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile Pooling")
	bool bCanExpand = true;

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Poison Projectile Pooling")
	ACPoisonProjectile* AcquireProjectile(UWorld* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Poison Projectile Pooling")
	void ReleaseProjectile(ACPoisonProjectile* Projectile);

	UFUNCTION(BlueprintCallable, Category = "Poison Projectile Pooling")
	void WarmUp(UWorld* WorldContext);

private:
	TArray<TWeakObjectPtr<ACPoisonProjectile>> Available;
	TSet<TWeakObjectPtr<ACPoisonProjectile>> Active;

	ACPoisonProjectile* SpawnOne(UWorld* WorldContext);
	void ActivateActor(ACPoisonProjectile* Actor);
	void DeactivateActor(ACPoisonProjectile* Actor);
};
