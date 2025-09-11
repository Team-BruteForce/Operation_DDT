// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/PrimitiveComponent.h"
#include "GateOfBabyonProjectile.generated.h"

UCLASS()
class OPERRATION_DDT_API AGateOfBabyonProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGateOfBabyonProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* Root;
	// 나이아가라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UNiagaraComponent* ProjectileNiagaraComponent;

	// 충돌 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UCapsuleComponent* CollisionComponent;

	// 프로젝타일 이동 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	// 나이아가라 시스템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	class UNiagaraSystem* ProjectileSystem;

	// 플레이어 주변 반지름 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
	float RandomRadius = 200.0f;

	// 이동 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile Settings")
	float MoveSpeed = 1000.0f;

	// 충돌 시 나이아가라 시스템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* ImpactSystem;

	// 오브젝트 풀 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pool")
	bool bUseObjectPool = true;

	// 활성화/비활성화 함수들
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void ActivateProjectile();

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void DeactivateProjectile();

private:
	// 목표 위치
	FVector TargetLocation;
	
	// 충돌 중인지 여부
	bool bIsCollided = false;
	
	// 타이머 핸들
	FTimerHandle DeactivateTimerHandle;
	
	// 충돌 콜백 함수
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	// 플레이어 주변 랜덤 위치로 이동하는 함수
	void MoveToRandomLocationAroundPlayer(AActor* Player);

};
