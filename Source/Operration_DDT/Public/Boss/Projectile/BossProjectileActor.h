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
	
	// 오디오 컴포넌트 (발사 사운드용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* AudioComponent;

public:
	// 투사체 발사
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void FireProjectile(AActor* Target);
	
	// 새로운 기능: 스폰 후 n초 대기 후 원하는 지점으로 이동
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void FireProjectileToLocation(const FVector& TargetLocation, float WaitTime = 2.0f);
	
	// 이펙트 관련 함수들
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void PlaySpawnEffect();
	
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void PlayDestroyEffect();
	
	// 충돌 판정 함수
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

public:
	// 기본 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	AActor* TargetActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	float CurrentTime;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	FVector StartLocation;
	
	// 투사체 수명
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float LifeTime;
	
	// 관통 후 직진 관련 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	bool bHasPassedTarget;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	FVector LastDirection;
	
	// 추적 각도 제한 관련 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float MaxTrackingAngle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	FVector InitialDirection;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	FVector CurrentDirection;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	bool bCanTrack;
	
	// 새로운 기능 관련 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	bool bWaitingToMove;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	float WaitTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float WaitDuration;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	FVector DelayedTargetLocation;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	bool bMovingToLocation;
	
	// 오브젝트 풀 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pool")
	bool bUseObjectPool;
	
	// 타이머 핸들 (멤버 변수로 추가)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Object Pool")
	FTimerHandle DestroyTimerHandle;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ToolTip="투사체 발사 사운드"))
	class USoundBase* ProjectileSound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ToolTip="투사체 비행 사운드"))
	class USoundBase* FlightSound;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(ToolTip="투사체 착탄 사운드"))
	class USoundBase* ImpactSound;
};
