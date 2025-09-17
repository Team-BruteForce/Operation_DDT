// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "CFlowerHitEffectPoolManager.generated.h"

UCLASS()
class OPERRATION_DDT_API ACFlowerHitEffectPoolManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ACFlowerHitEffectPoolManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 히트 이펙트 획득
	UFUNCTION(BlueprintCallable, Category = "HitEffect")
	UNiagaraComponent* AcquireHitEffect();

	// 히트 이펙트 반환
	UFUNCTION(BlueprintCallable, Category = "HitEffect")
	void ReleaseHitEffect(UNiagaraComponent* HitEffect);

	// 히트 이펙트 재생
	UFUNCTION(BlueprintCallable, Category = "HitEffect")
	void PlayHitEffectAtLocation(FVector HitLocation);

	// 히트 이펙트 에셋 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitEffect|Settings")
	UNiagaraSystem* HitEffectAsset;

	// 풀 크기 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitEffect|Settings")
	int32 PoolSize = 10;

	// 히트 이펙트 재생 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitEffect|Settings")
	float HitEffectDuration = 3.0f;

private:
	// 히트 이펙트 풀 초기화
	void WarmUp();

	// 히트 이펙트 하나 생성
	UNiagaraComponent* SpawnOne();

	// 히트 이펙트 활성화
	void ActivateHitEffect(UNiagaraComponent* HitEffect, FVector HitLocation);

	// 히트 이펙트 비활성화
	void DeactivateHitEffect(UNiagaraComponent* HitEffect);

	// 사용 가능한 히트 이펙트들
	UPROPERTY()
	TArray<TWeakObjectPtr<UNiagaraComponent>> AvailableHitEffects;

	// 현재 활성화된 히트 이펙트들
	UPROPERTY()
	TSet<TWeakObjectPtr<UNiagaraComponent>> ActiveHitEffects;

	// 히트 이펙트 타이머들 (자동 반환용)
	TMap<TWeakObjectPtr<UNiagaraComponent>, FTimerHandle> HitEffectTimers;
};

