// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "TimerManager.h"
#include "Boss/Actor/GateOfBabyonProjectile.h"
#include "GateOfBabylon.generated.h"

UCLASS()
class OPERRATION_DDT_API AGateOfBabylon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGateOfBabylon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	// 나이아가라 컴포넌트들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* MagicCircleComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* ProjectileNiagaraComponent;
	
	// 나이아가라 시스템들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* MagicCircleSystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects")
	UNiagaraSystem* ProjectileSystem;
	
	// 타이머 핸들들
	FTimerHandle ProjectileDelayTimerHandle;
	FTimerHandle ProjectileSpawnTimerHandle;
	FTimerHandle AutoDeactivateTimerHandle;
	
	// 투사체 지연 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing")
	float ProjectileDelayTime = 1.0f;
	
	// 프로젝타일 스폰 지연 시간 (두 번째 나이아가라 후)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing")
	float ProjectileSpawnDelayTime = 2.0f;
	
	// 프로젝타일 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<AGateOfBabyonProjectile> ProjectileClass;

	// 오브젝트 풀 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pool")
	bool bUseObjectPool = true;

	// 자체 프로젝타일 오브젝트 풀
	TArray<AGateOfBabyonProjectile*> ProjectilePool;
	bool bProjectilePoolInitialized = false;

	// 활성화/비활성화 함수들
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void ActivateGate();

	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void DeactivateGate();

private:
	// 프로젝타일 오브젝트 풀 초기화
	void InitializeProjectilePool();
	
	// 프로젝타일 풀에서 가져오기
	AGateOfBabyonProjectile* GetProjectileFromPool();
	
	// 프로젝타일 스폰 함수
	void SpawnProjectile();
	
	// 플레이어를 바라보도록 회전 업데이트
	void UpdateLookAtPlayer();

};
