// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "CDropItemEffectPoolManager.generated.h"

UCLASS()
class OPERRATION_DDT_API ACDropItemEffectPoolManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDropItemEffectPoolManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 드랍 아이템 이펙트 획득
	UFUNCTION(BlueprintCallable, Category = "DropItemEffect")
	UNiagaraComponent* AcquireDropItemEffect();

	// 드랍 아이템 이펙트 반환
	UFUNCTION(BlueprintCallable, Category = "DropItemEffect")
	void ReleaseDropItemEffect(UNiagaraComponent* EffectComponent);

	// 드랍 아이템 이펙트 재생
	UFUNCTION(BlueprintCallable, Category = "DropItemEffect")
	void PlayDropItemEffectAtLocation(UNiagaraComponent* EffectComponent, FVector EffectLocation, FRotator EffectRotation);

	// 드랍 아이템 이펙트 에셋 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropItemEffect|Settings")
	UNiagaraSystem* DropItemEffectAsset;

	// 풀 크기 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropItemEffect|Settings")
	int32 PoolSize = 20;

	// 이펙트 재생 시간 (자동 반환용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropItemEffect|Settings")
	float EffectDuration = 10.0f;

private:
	// 드랍 아이템 이펙트 풀 초기화
	void WarmUp();

	// 드랍 아이템 이펙트 하나 생성
	UNiagaraComponent* SpawnOne();

	// 드랍 아이템 이펙트 활성화
	void ActivateDropItemEffect(UNiagaraComponent* EffectComponent, FVector EffectLocation, FRotator EffectRotation);

	// 드랍 아이템 이펙트 비활성화
	void DeactivateDropItemEffect(UNiagaraComponent* EffectComponent);

	// 사용 가능한 드랍 아이템 이펙트들
	UPROPERTY()
	TArray<TWeakObjectPtr<UNiagaraComponent>> AvailableDropItemEffects;

	// 현재 활성화된 드랍 아이템 이펙트들
	UPROPERTY()
	TSet<TWeakObjectPtr<UNiagaraComponent>> ActiveDropItemEffects;

	// 드랍 아이템 이펙트 타이머들 (자동 반환용)
	TMap<TWeakObjectPtr<UNiagaraComponent>, FTimerHandle> DropItemEffectTimers;
};
