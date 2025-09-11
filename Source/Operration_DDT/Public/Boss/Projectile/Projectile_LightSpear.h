// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile_LightSpear.generated.h"

UCLASS()
class OPERRATION_DDT_API AProjectile_LightSpear : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile_LightSpear();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	class UNiagaraComponent* NiagaraDestroyEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	class UNiagaraComponent* NiagaraProjectileEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	class UCapsuleComponent* Shape;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	class USceneComponent* Root;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	class UProjectileMovementComponent* ProjectileComp;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 발사 함수
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void FireProjectile();
	
	// 파괴 이펙트 재생
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void PlayDestroyEffect();
	
	// 충돌 판정 함수
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	// 기본 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ProjectileSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	float CurrentTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float LifeTime;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	FVector TargetDirection;
	
	// 타겟팅 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCBossTargetingComponent* TargetingComp;
	
	// 오브젝트 풀 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pool")
	bool bUseObjectPool;
	
	// 타이머 핸들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Object Pool")
	FTimerHandle LifeTimeTimerHandle;
};
