// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossProjectileActor.generated.h"

UCLASS()
class OPERRATION_DDT_API ABossProjectileActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossProjectileActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	class UNiagaraComponent* NiagaraProjectile;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	class UNiagaraComponent* NiagaraSpawnEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	class UNiagaraComponent* NiagaraDestroyEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	class UProjectileMovementComponent* ProjectileComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	class USphereComponent* Shape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	class USceneComponent* Root;

public:
	// 투사체 발사
	void FireProjectile(AActor* Target);
	
	// 새로운 기능: 스폰 후 n초 대기 후 원하는 지점으로 이동
	void FireProjectileToLocation(const FVector& TargetLocation, float WaitTime = 2.0f);
	
	// 이펙트 관련 함수들
	void PlaySpawnEffect();
	void PlayDestroyEffect();
	
	// 충돌 판정 함수
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

private:
	// 기본 변수들
	AActor* TargetActor;
	float ProjectileSpeed;
	float CurrentTime;
	FVector StartLocation;       // 시작 위치
	
	// 투사체 수명
	float LifeTime;
	
	// 관통 후 직진 관련 변수들
	bool bHasPassedTarget;       // 타겟을 통과했는지 여부
	FVector LastDirection;       // 마지막 이동 방향
	
	// 새로운 기능 관련 변수들
	bool bWaitingToMove;              // 대기 중인지 여부
	float WaitTimer;                  // 대기 타이머
	float WaitDuration;               // 대기 시간
	FVector DelayedTargetLocation;    // 지연 이동할 목표 위치
	bool bMovingToLocation;           // 지점으로 이동 중인지 여부
};
