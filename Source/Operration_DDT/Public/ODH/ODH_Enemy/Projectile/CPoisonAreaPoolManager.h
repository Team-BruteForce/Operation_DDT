// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ODH/ODH_Enemy/Projectile/CPoisonArea.h"
#include "CPoisonAreaPoolManager.generated.h"

UCLASS()
class OPERRATION_DDT_API ACPoisonAreaPoolManager : public AActor
{
	GENERATED_BODY()
	
public:
	ACPoisonAreaPoolManager();

public:
	// 풀링 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Area Pooling")
	TSubclassOf<ACPoisonArea> PoisonAreaClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Area Pooling")
	int32 InitialSize = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Area Pooling")
	bool bCanExpand = true;

public:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Poison Area Pooling")
	ACPoisonArea* AcquirePoisonArea(UWorld* WorldContext);

	UFUNCTION(BlueprintCallable, Category = "Poison Area Pooling")
	void ReleasePoisonArea(ACPoisonArea* PoisonArea);

	UFUNCTION(BlueprintCallable, Category = "Poison Area Pooling")
	void WarmUp(UWorld* WorldContext);

private:
	TArray<TWeakObjectPtr<ACPoisonArea>> Available;
	TSet<TWeakObjectPtr<ACPoisonArea>> Active;

	ACPoisonArea* SpawnOne(UWorld* WorldContext);
	void ActivateActor(ACPoisonArea* Actor);
	void DeactivateActor(ACPoisonArea* Actor);
};
