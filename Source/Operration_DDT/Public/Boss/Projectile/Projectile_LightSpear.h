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
	void FireProjectile();
	
	// 파괴 이펙트 재생
	void PlayDestroyEffect();
	
	// 충돌 판정 함수
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// 기본 변수들
	float ProjectileSpeed;
	float CurrentTime;
	float LifeTime;
	FVector TargetDirection;  // 타겟 방향 저장
	
	// 타겟팅 컴포넌트
	class UCBossTargetingComponent* TargetingComp;
};
