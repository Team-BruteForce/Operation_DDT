// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CSkullRangedATKManager.generated.h"

UCLASS()
class OPERRATION_DDT_API ACSkullRangedATKManager : public AActor
{
	GENERATED_BODY()
	
public:
	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pooling")
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pooling")
	int32 InitialSize = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pooling")
	bool bCanExpand = true;

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Pooling")
	AActor* AcquireProjectile(UWorld* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Pooling")
	void ReleaseProjectile(AActor* Projectile);

	UFUNCTION(BlueprintCallable, Category = "Pooling")
	void WarmUp(UWorld* WorldContext);

private:
	TArray<TWeakObjectPtr<AActor>> Available;
	TSet<TWeakObjectPtr<AActor>> Active;

	AActor* SpawnOne(UWorld* WorldContext);
	void ActivateActor(AActor* Actor);
	void DeactivateActor(AActor* Actor);
};
