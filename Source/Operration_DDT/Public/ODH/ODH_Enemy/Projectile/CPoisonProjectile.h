// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Engine/Engine.h"
#include "CPoisonProjectile.generated.h"

UCLASS()
class OPERRATION_DDT_API ACPoisonProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPoisonProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 프로젝타일 활성화 함수
	UFUNCTION(BlueprintCallable, Category = "Poison Projectile")
	void ActivateProjectile(FVector InStartLocation, FVector InTargetLocation);

	// 프로젝타일 비활성화 함수
	UFUNCTION(BlueprintCallable, Category = "Poison Projectile")
	void DeactivateProjectile();

	// 풀 매니저로 반환하는 함수
	UFUNCTION(BlueprintCallable, Category = "Poison Projectile")
	void ReturnToPool();

	// 풀 매니저 설정 함수
	void SetPoolManager(class ACPoisonProjectilePoolManager* Manager);

	// 투사체 이동 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile|Movement")
	float LaunchHeight = 500.0f; // 위로 올라가는 높이

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile|Movement")
	float LaunchDuration = 1.0f; // 위로 올라가는 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile|Movement")
	float FallDuration = 1.5f; // 아래로 떨어지는 시간

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile|Movement")
	float MaxFlightTime = 5.0f; // 최대 비행 시간

	// PoisonArea 클래스 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile|Area")
	TSubclassOf<class ACPoisonArea> PoisonAreaClass;

	// 콜리전 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComponent;

	// 나이아가라 이펙트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* ProjectileEffect;

	// 나이아가라 이펙트 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile|Effect")
	UNiagaraSystem* ProjectileEffectAsset;

	// 프로젝타일 활성화 상태
	UPROPERTY(BlueprintReadOnly, Category = "Poison Projectile")
	bool bIsActive = false;

private:
	// 투사체 이동 계산
	void UpdateMovement(float DeltaTime);

	// 바닥 감지를 위한 라인 트레이스
	bool CheckGroundCollision();

	// PoisonArea 생성
	void SpawnPoisonArea(FVector Location);

	// 투사체 이동 관련 변수들
	FVector StartLocation;
	FVector TargetLocation;
	FVector PeakLocation; // 최고점 위치
	float FlightTime;
	float MaxFlightTime_Internal;
	
	// 이동 단계 (0: 위로 올라가기, 1: 아래로 떨어지기)
	int32 MovementPhase = 0;
	float PhaseTime = 0.0f;

	// 프로젝타일이 활성화되었는지 확인
	bool IsProjectileActive() const { return bIsActive; }

public:
	// 바닥 충돌 감지용 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile|Collision")
	float GroundCheckDistance = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile|Collision")
	TEnumAsByte<ECollisionChannel> GroundCollisionChannel = ECC_WorldStatic;

	// 디버그 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Projectile|Debug")
	bool bShowDebugTrajectory = false;

private:
	// 디버그 그리기 함수
	void DrawDebugTrajectory();

	// 풀 매니저 참조
	class ACPoisonProjectilePoolManager* PoolManager;
};
