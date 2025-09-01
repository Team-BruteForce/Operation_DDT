// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CEnemyProjectileComp.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnProjectileHit, AActor*, HitActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCEnemyProjectileComp : public UActorComponent
{
	GENERATED_BODY()

public:
	UCEnemyProjectileComp();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void InitializeTarget(AActor* InTargetPlayer);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileSpeed = 1200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float MaxLifeSeconds = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	AActor* TargetPlayer = nullptr;

	// 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnProjectileHit OnProjectileHit;

	// 데미지 계산 (StatusComponent의 공격력 사용)
	UFUNCTION(BlueprintCallable, Category = "Damage")
	float GetProjectileDamage() const;

private:
	FVector FlightDirection = FVector::ZeroVector;
	float LifeTimer = 0.0f;
	bool bInitialized = false;

private:
	void TryInheritTargetFromSpawner();
	void ComputeInitialDirection();
	void MoveAndSweep(float DeltaTime);
	void OnHitAndMaybeDestroy(const FHitResult& Hit);
	void DealProjectileDamage(AActor* HitActor);
};
