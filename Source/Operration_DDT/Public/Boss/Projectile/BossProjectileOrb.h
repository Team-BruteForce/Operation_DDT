// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossProjectileOrb.generated.h"

UCLASS()
class OPERRATION_DDT_API ABossProjectileOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossProjectileOrb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	void SpawnProjectile();

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<ABossProjectileActor> ProjectileClass;

	UPROPERTY()
	float CurrentTime = 0.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Time")
	float MaxTime=5.0f;
	

};
