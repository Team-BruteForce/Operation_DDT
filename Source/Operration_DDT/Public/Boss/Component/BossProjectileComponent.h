// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Boss/Actor/GateOfBabylon.h"
#include "Boss/Projectile/BossProjectileActor.h"
#include "Boss/Projectile/BossProjectileOrb.h"
#include "Boss/Projectile/Projectile_LightSpear.h"
#include "Boss/Actor/HolySwordMagic.h"
#include "Components/ActorComponent.h"
#include "BossProjectileComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class OPERRATION_DDT_API UBossProjectileComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UBossProjectileComponent();
	
	// 투사체 발사 함수들
	UFUNCTION()
	void ShotProjectile();

	UFUNCTION()
	void SpawnOrb();
	
	UFUNCTION()
	void ShotProjectileToLocation(AActor* Target, float WaitTime = 2.0f);

	UFUNCTION()
	void DestroyOrb();
	
	// 오브 무한 반복 스폰 함수
	UFUNCTION(BlueprintCallable, Category = "Orb")
	void SpawnOrbContinuously(float IntervalTime);
	
	// 오브 무한 반복 스폰 취소 함수
	UFUNCTION(BlueprintCallable, Category = "Orb")
	void CancelOrbContinuousSpawning();
	
	// 투사체 무한 반복 스폰 함수
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectileContinuously(float IntervalTime);
	
	// 투사체 무한 반복 스폰 취소 함수
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void CancelProjectileContinuousSpawning();
	
	// HolySwordMagic 관련 함수들
	UFUNCTION(BlueprintCallable, Category = "Holy Sword Magic")
	void SpawnHolySwordMagicRepeatedly(float IntervalTime, int32 ExecuteCount);
	
	UFUNCTION(BlueprintCallable, Category = "Holy Sword Magic")
	void CancelHolySwordMagicSpawning();

	// 보스 투사체 시스템 완전 초기화 (매니저용)
	UFUNCTION(BlueprintCallable, Category = "Boss Reset")
	void ResetProjectileSystem();
	
	// 프로퍼티들
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABossProjectileActor> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AProjectile_LightSpear> ProjectileSpearClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABossProjectileOrb> ProjectileOrbClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Holy Sword Magic")
	TSubclassOf<AHolySwordMagic> HolySwordMagicClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Holy Sword Magic")
	TSubclassOf<AGateOfBabylon> MagicCircle;

	UPROPERTY(EditAnywhere)
	bool ExitOrb = false;

private:
	// 내부적으로 사용하는 HolySwordMagic 스폰 함수들
	void SpawnHolySwordMagicAtLocation(const FVector& TargetLocation);
	void SpawnHolySwordMagicAtCurrentPlayerLocation();
	
	// 내부적으로 사용하는 오브 스폰 함수
	void SpawnSingleOrb();
	
	// 내부적으로 사용하는 투사체 스폰 함수
	void SpawnSingleProjectile();
	
	// 오브 관련 변수들
	FTimerHandle OrbTimerHandle;
	int32 OrbSpawnCount = 0;
	
	// 오브 무한 반복 스폰 관련 변수들
	FTimerHandle OrbContinuousTimerHandle;
	bool bOrbContinuousSpawning = false;
	
	// 투사체 무한 반복 스폰 관련 변수들
	FTimerHandle ProjectileContinuousTimerHandle;
	bool bProjectileContinuousSpawning = false;
	
	// HolySwordMagic 반복 스폰 관련 변수들
	FTimerHandle HolySwordMagicTimerHandle;
	int32 HolySwordMagicExecuteCount = 0;
	int32 HolySwordMagicCurrentCount = 0;
	bool bHolySwordMagicSpawning = false;
	
	// 마법진 스폰 관련 변수들
	FTimerHandle MagicCircleSpawnTimerHandle;
	
	// 오브젝트 풀 관련 변수들
	TArray<AGateOfBabylon*> GatePool;
	TArray<ABossProjectileOrb*> OrbPool;
	TArray<AProjectile_LightSpear*> ProjectilePool;
	TArray<AHolySwordMagic*> HolySwordMagicPool;
	TArray<ABossProjectileActor*> BossProjectilePool;
	bool bObjectPoolInitialized = false;
	
	// 오브젝트 풀 초기화 함수
	void InitializeObjectPool();
	
	// 오브젝트 풀에서 오브젝트 가져오기
	AGateOfBabylon* GetGateFromPool();
	ABossProjectileOrb* GetOrbFromPool();
	AProjectile_LightSpear* GetProjectileFromPool();
	AHolySwordMagic* GetHolySwordMagicFromPool();
public:
	ABossProjectileActor* GetBossProjectileFromPool();

public:
	// 오브젝트를 풀로 반환하기
	void ReturnGateToPool(AGateOfBabylon* Gate);
	void ReturnOrbToPool(ABossProjectileOrb* Orb);
	void ReturnProjectileToPool(AProjectile_LightSpear* Projectile);
	void ReturnHolySwordMagicToPool(AHolySwordMagic* HolySwordMagic);
	void ReturnBossProjectileToPool(ABossProjectileActor* BossProjectile);
public:
	// 사각형 범위 관련 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rectangle Range")
	bool bShowRectangleRange = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rectangle Range")
	float RectangleWidth = 3300.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rectangle Range")
	float RectangleHeight = 1200.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rectangle Range")
	FColor RectangleColor = FColor::Red;
	
	// 사각형 범위 관련 함수들
	UFUNCTION(BlueprintCallable, Category = "Rectangle Range")
	void SetRectangleRange(bool bShow, float Width = 500.0f, float Height = 300.0f, FColor Color = FColor::Red);
	
	UFUNCTION(BlueprintCallable, Category = "Rectangle Range")
	void ToggleRectangleRange();
	
	UFUNCTION(BlueprintCallable, Category = "Rectangle Range")
	void TestRectangleRange();
	
	UFUNCTION(BlueprintCallable, Category = "Magic Circle")
	void SpawnMagicCirclesAtCirclePositions();
	
protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	void DrawRectangleRangeDebug();
};
