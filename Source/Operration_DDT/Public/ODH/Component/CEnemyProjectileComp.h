// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NiagaraSystem.h"
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

	// 히트 이펙트 설정 (나이아가라)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* HitEffect = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	USoundBase* HitSound = nullptr;
	
	// 이펙트 스폰 위치 오프셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	FVector EffectSpawnOffset = FVector::ZeroVector;

	// 데미지 계산 (StatusComponent의 공격력 사용)
	UFUNCTION(BlueprintCallable, Category = "Damage")
	float GetProjectileDamage() const;

	// 블루프린트 콜리전 등록 함수
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void RegisterCollisionComponent(UPrimitiveComponent* CollisionComponent);

	// 블루프린트 콜리전 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	bool bUseBlueprintCollision = false;

	// 베지어 곡선 이동 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	bool IsBazierCurves = false;

	// 베지어 곡선 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float BezierCurveHeight = 200.0f; // 곡선의 높이

	// 오프셋은 랜덤하게 300 또는 -300으로 설정됨 (더 이상 수동 설정 불가)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	float BezierCurveOffset = 0.0f; // 랜덤하게 설정된 오프셋 (300 또는 -300)

private:
	FVector FlightDirection = FVector::ZeroVector;
	float LifeTimer = 0.0f;
	bool bInitialized = false;

	// 베지어 곡선 관련 변수들
	FVector BezierStartPoint = FVector::ZeroVector;
	FVector BezierControlPoint = FVector::ZeroVector;
	FVector BezierEndPoint = FVector::ZeroVector;
	float BezierProgress = 0.0f;

private:
	void TryInheritTargetFromSpawner();
	void ComputeInitialDirection();
	void MoveAndSweep(float DeltaTime);
	void OnHitAndMaybeDestroy(const FHitResult& Hit);
	void DealProjectileDamage(AActor* HitActor);

	// 베지어 곡선 이동 관련 함수들
	void InitializeBezierCurve();
	void MoveAlongBezierCurve(float DeltaTime);
	FVector CalculateBezierPoint(float t);

	// 블루프린트 콜리전 이벤트 핸들러
	UFUNCTION()
	void OnBlueprintCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBlueprintCollisionHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// 등록된 콜리전 컴포넌트
	UPROPERTY()
	UPrimitiveComponent* RegisteredCollisionComponent = nullptr;
};
