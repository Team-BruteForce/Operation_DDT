// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss/Projectile/BossProjectileActor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BossProjectileComponent.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UBossProjectileComponent : public UProjectileMovementComponent
{
	GENERATED_BODY()
	UBossProjectileComponent();
public:
	void ShotProjectile();
	
	// 새로운 기능: 타겟 액터를 받아서 발사 직전에 타겟의 위치로 이동
	void ShotProjectileToLocation(AActor* Target, float WaitTime = 2.0f);
	
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<ABossProjectileActor> ProjectileClass;

};
