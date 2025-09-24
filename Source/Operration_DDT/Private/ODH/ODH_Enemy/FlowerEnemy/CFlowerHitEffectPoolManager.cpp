// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerHitEffectPoolManager.h"
#include "Engine/Engine.h"
#include "TimerManager.h"

ACFlowerHitEffectPoolManager::ACFlowerHitEffectPoolManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACFlowerHitEffectPoolManager::BeginPlay()
{
	Super::BeginPlay();
	
	// 히트 이펙트 풀 초기화
	WarmUp();
}

void ACFlowerHitEffectPoolManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACFlowerHitEffectPoolManager::WarmUp()
{
	if (!HitEffectAsset)
	{
		return;
	}

	// 풀 크기만큼 히트 이펙트 생성
	for (int32 i = 0; i < PoolSize; ++i)
	{
		UNiagaraComponent* HitEffect = SpawnOne();
		if (HitEffect)
		{
			AvailableHitEffects.Add(HitEffect);
		}
	}


}

UNiagaraComponent* ACFlowerHitEffectPoolManager::SpawnOne()
{
	if (!HitEffectAsset)
		return nullptr;

	// 히트 이펙트 컴포넌트 생성
	UNiagaraComponent* HitEffect = NewObject<UNiagaraComponent>(this);
	HitEffect->SetAsset(HitEffectAsset);
	HitEffect->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	HitEffect->SetAutoDestroy(false);
	HitEffect->SetHiddenInGame(true);
	HitEffect->Deactivate();

	return HitEffect;
}

UNiagaraComponent* ACFlowerHitEffectPoolManager::AcquireHitEffect()
{
	// 사용 가능한 히트 이펙트가 있는지 확인
	if (AvailableHitEffects.Num() > 0)
	{
		UNiagaraComponent* HitEffect = AvailableHitEffects.Pop().Get();
		if (IsValid(HitEffect))
		{
			ActiveHitEffects.Add(HitEffect);
			return HitEffect;
		}
	}

	// 사용 가능한 것이 없으면 새로 생성
	UNiagaraComponent* HitEffect = SpawnOne();
	if (HitEffect)
	{
		ActiveHitEffects.Add(HitEffect);
	}

	return HitEffect;
}

void ACFlowerHitEffectPoolManager::ReleaseHitEffect(UNiagaraComponent* HitEffect)
{
	if (!IsValid(HitEffect))
		return;

	// 활성화된 목록에서 제거
	ActiveHitEffects.Remove(HitEffect);

	// 히트 이펙트 비활성화
	DeactivateHitEffect(HitEffect);

	// 사용 가능한 목록에 추가
	AvailableHitEffects.Add(HitEffect);

	// 타이머 정리
	if (HitEffectTimers.Contains(HitEffect))
	{
		GetWorld()->GetTimerManager().ClearTimer(HitEffectTimers[HitEffect]);
		HitEffectTimers.Remove(HitEffect);
	}
}

void ACFlowerHitEffectPoolManager::PlayHitEffectAtLocation(FVector HitLocation)
{


	if (!HitEffectAsset)
	{
		return;
	}

	UNiagaraComponent* HitEffect = AcquireHitEffect();
	if (HitEffect)
	{


		ActivateHitEffect(HitEffect, HitLocation);

		// 자동 반환 타이머 설정
		FTimerHandle AutoReturnTimer;
		GetWorld()->GetTimerManager().SetTimer(AutoReturnTimer, [this, HitEffect]()
		{
			ReleaseHitEffect(HitEffect);
		}, HitEffectDuration, false);

		HitEffectTimers.Add(HitEffect, AutoReturnTimer);


	}
	else
	{
	}
}

void ACFlowerHitEffectPoolManager::ActivateHitEffect(UNiagaraComponent* HitEffect, FVector HitLocation)
{
	if (!IsValid(HitEffect))
	{
		return;
	}



	// 위치 설정
	HitEffect->SetWorldLocation(HitLocation);
	HitEffect->SetWorldRotation(FRotator::ZeroRotator);
	HitEffect->SetWorldScale3D(FVector::OneVector);

	// 활성화
	HitEffect->SetHiddenInGame(false);
	HitEffect->Activate();


}

void ACFlowerHitEffectPoolManager::DeactivateHitEffect(UNiagaraComponent* HitEffect)
{
	if (!IsValid(HitEffect))
		return;

	// 비활성화
	HitEffect->Deactivate();
	HitEffect->SetHiddenInGame(true);
}
