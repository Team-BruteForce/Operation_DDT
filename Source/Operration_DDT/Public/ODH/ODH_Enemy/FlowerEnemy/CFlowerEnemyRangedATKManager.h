// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ODH/ODH_Enemy/Projectile/CFlowerProjectile.h"
#include "CFlowerEnemyRangedATKManager.generated.h"

UCLASS()
class OPERRATION_DDT_API ACFlowerEnemyRangedATKManager : public AActor
{
	GENERATED_BODY()
	
public:
	// 풀링 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pooling")
	TSubclassOf<ACFlowerProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pooling")
	int32 InitialSize = 15;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pooling")
	bool bCanExpand = true;

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Pooling")
	ACFlowerProjectile* AcquireProjectile(UWorld* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Pooling")
	void ReleaseProjectile(ACFlowerProjectile* Projectile);

	UFUNCTION(BlueprintCallable, Category = "Pooling")
	void WarmUp(UWorld* WorldContext);

private:
	TArray<TWeakObjectPtr<ACFlowerProjectile>> Available;
	TSet<TWeakObjectPtr<ACFlowerProjectile>> Active;

	ACFlowerProjectile* SpawnOne(UWorld* WorldContext);
	void ActivateActor(ACFlowerProjectile* Actor);
	void DeactivateActor(ACFlowerProjectile* Actor);
};
