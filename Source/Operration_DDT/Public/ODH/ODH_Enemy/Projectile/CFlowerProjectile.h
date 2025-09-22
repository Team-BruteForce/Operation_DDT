// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Engine/Engine.h"
#include "CFlowerProjectile.generated.h"

UCLASS()
class OPERRATION_DDT_API ACFlowerProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ACFlowerProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 프로젝타일 활성화 함수
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void ActivateProjectile(FVector InStartLocation, FVector InTargetLocation, AActor* InTargetPlayer);

	// 프로젝타일 비활성화 함수
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void DeactivateProjectile();

	// 풀 매니저로 반환하는 함수
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void ReturnToPool();

	// 풀 매니저 설정 함수
	void SetPoolManager(class ACFlowerEnemyRangedATKManager* Manager);

	// 포물선 이동 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Movement")
	float ProjectileSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Movement")
	float Gravity = 980.0f; // 중력 가속도

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Movement")
	float MaxFlightTime = 5.0f; // 최대 비행 시간 (초)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Damage")
	float Damage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Damage")
	float DamageRadius = 100.0f;

	// 콜리전 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComponent;

	// 나이아가라 이펙트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* ProjectileEffect;

	// 나이아가라 이펙트 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Effect")
	UNiagaraSystem* ProjectileEffectAsset;

	// 프로젝타일 활성화 상태
	UPROPERTY(BlueprintReadOnly, Category = "Projectile")
	bool bIsActive = false;

private:
	// 포물선 이동 계산
	void CalculateTrajectory();
	void UpdateMovement(float DeltaTime);

	// 충돌 이벤트 핸들러
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 바닥 감지를 위한 라인 트레이스
	bool CheckGroundCollision();

	// 포물선 이동 관련 변수들
	FVector StartLocation;
	FVector TargetLocation;
	FVector InitialVelocity;
	FVector CurrentVelocity;
	float FlightTime;
	float MaxFlightTime_Internal;

	// 타겟 플레이어 (충돌 감지용)
	AActor* TargetPlayer;

	// 프로젝타일이 활성화되었는지 확인
	bool IsProjectileActive() const { return bIsActive; }

public:
	// 바닥 충돌 감지용 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Collision")
	float GroundCheckDistance = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Collision")
	TEnumAsByte<ECollisionChannel> GroundCollisionChannel = ECC_WorldStatic;

	// 디버그 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile|Debug")
	bool bShowDebugTrajectory = false;

private:
	// 디버그 그리기 함수
	void DrawDebugTrajectory();

	// 풀 매니저 참조 (비활성화 시 설정됨)
	class ACFlowerEnemyRangedATKManager* PoolManager;
};
