// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/Item/CDropItemEffectPoolManager.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"

ACDropItemEffectPoolManager::ACDropItemEffectPoolManager()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// 기본값 설정
	PoolSize = 20;
	EffectDuration = 10.0f;
}

void ACDropItemEffectPoolManager::BeginPlay()
{
	Super::BeginPlay();
	
	// 풀 워밍업
	WarmUp();
}

void ACDropItemEffectPoolManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACDropItemEffectPoolManager::WarmUp()
{
	if (!DropItemEffectAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("CDropItemEffectPoolManager: DropItemEffectAsset is not set!"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("CDropItemEffectPoolManager: Starting warm up with PoolSize: %d"), PoolSize);

	// 풀 크기만큼 이펙트 생성
	for (int32 i = 0; i < PoolSize; ++i)
	{
		UNiagaraComponent* NewEffect = SpawnOne();
		if (NewEffect)
		{
			AvailableDropItemEffects.Add(NewEffect);
			UE_LOG(LogTemp, Log, TEXT("CDropItemEffectPoolManager: Created effect %d/%d"), i + 1, PoolSize);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("CDropItemEffectPoolManager: Failed to create effect %d/%d"), i + 1, PoolSize);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("CDropItemEffectPoolManager: Pool warmed up with %d effects"), AvailableDropItemEffects.Num());
}

UNiagaraComponent* ACDropItemEffectPoolManager::SpawnOne()
{
	if (!DropItemEffectAsset)
	{
		UE_LOG(LogTemp, Warning, TEXT("CDropItemEffectPoolManager: Cannot spawn effect - DropItemEffectAsset is null"));
		return nullptr;
	}

	UE_LOG(LogTemp, Log, TEXT("CDropItemEffectPoolManager: Creating NiagaraComponent with asset: %s"), *DropItemEffectAsset->GetName());

	// 나이아가라 컴포넌트 생성
	UNiagaraComponent* NewEffect = NewObject<UNiagaraComponent>(this, UNiagaraComponent::StaticClass(), 
		*FString::Printf(TEXT("DropItemEffect_%d"), AvailableDropItemEffects.Num()), RF_Transient);
	if (NewEffect)
	{
		UE_LOG(LogTemp, Log, TEXT("CDropItemEffectPoolManager: NiagaraComponent created successfully"));
		
		// 컴포넌트를 액터에 추가
		AddInstanceComponent(NewEffect);
		NewEffect->SetupAttachment(RootComponent);
		
		// SetAutoActivate는 RegisterComponent 전에 호출해야 함
		NewEffect->SetAutoActivate(false);
		NewEffect->RegisterComponent();
		
		NewEffect->SetAsset(DropItemEffectAsset);
		NewEffect->SetVisibility(false);
		
		UE_LOG(LogTemp, Log, TEXT("CDropItemEffectPoolManager: NiagaraComponent configured and registered"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CDropItemEffectPoolManager: Failed to create NiagaraComponent"));
	}

	return NewEffect;
}

UNiagaraComponent* ACDropItemEffectPoolManager::AcquireDropItemEffect()
{
	// 사용 가능한 이펙트가 있는지 확인
	if (AvailableDropItemEffects.Num() > 0)
	{
		UNiagaraComponent* Effect = AvailableDropItemEffects.Pop().Get();
		if (IsValid(Effect))
		{
			ActiveDropItemEffects.Add(Effect);
			return Effect;
		}
	}

	// 사용 가능한 이펙트가 없으면 새로 생성
	UE_LOG(LogTemp, Warning, TEXT("CDropItemEffectPoolManager: Pool exhausted, creating new effect"));
	UNiagaraComponent* NewEffect = SpawnOne();
	if (NewEffect)
	{
		ActiveDropItemEffects.Add(NewEffect);
	}

	return NewEffect;
}

void ACDropItemEffectPoolManager::ReleaseDropItemEffect(UNiagaraComponent* EffectComponent)
{
	if (!IsValid(EffectComponent))
	{
		return;
	}

	// 활성화된 이펙트에서 제거
	ActiveDropItemEffects.Remove(EffectComponent);

	// 타이머 정리
	if (DropItemEffectTimers.Contains(EffectComponent))
	{
		GetWorld()->GetTimerManager().ClearTimer(DropItemEffectTimers[EffectComponent]);
		DropItemEffectTimers.Remove(EffectComponent);
	}

	// 이펙트 비활성화
	DeactivateDropItemEffect(EffectComponent);

	// 사용 가능한 풀에 다시 추가
	AvailableDropItemEffects.Add(EffectComponent);
}

void ACDropItemEffectPoolManager::PlayDropItemEffectAtLocation(UNiagaraComponent* EffectComponent, FVector EffectLocation, FRotator EffectRotation)
{
	if (!IsValid(EffectComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("CDropItemEffectPoolManager: Invalid effect component"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("CDropItemEffectPoolManager: Playing effect at location: %s"), *EffectLocation.ToString());

	// 이펙트 활성화
	ActivateDropItemEffect(EffectComponent, EffectLocation, EffectRotation);

	// 자동 반환 타이머 설정
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, EffectComponent]()
	{
		ReleaseDropItemEffect(EffectComponent);
	}, EffectDuration, false);

	DropItemEffectTimers.Add(EffectComponent, TimerHandle);
}

void ACDropItemEffectPoolManager::ActivateDropItemEffect(UNiagaraComponent* EffectComponent, FVector EffectLocation, FRotator EffectRotation)
{
	if (!IsValid(EffectComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("CDropItemEffectPoolManager: Invalid effect component in ActivateDropItemEffect"));
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("CDropItemEffectPoolManager: Activating effect component"));

	// 위치와 회전 설정
	EffectComponent->SetWorldLocation(EffectLocation);
	EffectComponent->SetWorldRotation(EffectRotation);
	
	// 이펙트 활성화
	EffectComponent->SetVisibility(true);
	EffectComponent->Activate();
	
	UE_LOG(LogTemp, Log, TEXT("CDropItemEffectPoolManager: Effect component activated and visible"));
}

void ACDropItemEffectPoolManager::DeactivateDropItemEffect(UNiagaraComponent* EffectComponent)
{
	if (!IsValid(EffectComponent))
	{
		return;
	}

	// 이펙트 비활성화
	EffectComponent->Deactivate();
	EffectComponent->SetVisibility(false);
}
