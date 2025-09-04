// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss/Projectile/BossProjectileActor.h"
#include "Boss/Projectile/BossProjectileOrb.h"
#include "Boss/Projectile/Projectile_LightSpear.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BossProjectileComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class OPERRATION_DDT_API UBossProjectileComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()
	UBossProjectileComponent();
public:
	UFUNCTION()
	void ShotProjectile();

	UFUNCTION()
	void SpawnOrb();
	
	// 새로운 기능: 타겟 액터를 받아서 발사 직전에 타겟의 위치로 이동
	UFUNCTION()
	void ShotProjectileToLocation(AActor* Target, float WaitTime = 2.0f);

	UFUNCTION()
	void DestroyOrb();
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<ABossProjectileActor> ProjectileClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<AProjectile_LightSpear> ProjectileSpearClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<ABossProjectileOrb> ProjectileOrbClass;

private:
	FTimerHandle OrbTimerHandle;
	int32 OrbSpawnCount = 0;
public:
	UPROPERTY(EditAnywhere)
	bool ExitOrb= false;
};
