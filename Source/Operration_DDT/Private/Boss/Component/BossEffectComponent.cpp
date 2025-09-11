// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/Component/BossEffectComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Boss/Effect/BossEffectManager.h"
#include "Boss/Effect/BossEffectExecute.h"

// Sets default values for this component's properties
UBossEffectComponent::UBossEffectComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	EffectDataTable = nullptr;
	MaxPoolSize = 20;
	bAutoExpandPool = true;
	EffectManager = nullptr;
	EffectExecute = nullptr;
	OwnerCharacter = nullptr;
}

// Called when the game starts
void UBossEffectComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	// 이펙트 매니저 초기화
	InitializeEffectManager();
}

// Called every frame
void UBossEffectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBossEffectComponent::PlayEffect(FGameplayTag EffectTag, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CheckNull(EffectManager);

	FVector location = GetEffectLocation(InLocation);
	FRotator rotation = GetEffectRotation(InRotation);

	EffectManager->PlayEffect(EffectTag, location, rotation, InScale);
}

void UBossEffectComponent::PlayEffects(TArray<FGameplayTag> EffectTags, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CheckNull(EffectManager);

	FVector location = GetEffectLocation(InLocation);
	FRotator rotation = GetEffectRotation(InRotation);

	// 여러 이펙트를 순차적으로 실행
	for (const FGameplayTag& EffectTag : EffectTags)
	{
		EffectManager->PlayEffect(EffectTag, location, rotation, InScale);
	}
}

void UBossEffectComponent::PlayEffectWithDelay(FGameplayTag EffectTag, float Delay, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CheckNull(EffectExecute);

	FVector location = GetEffectLocation(InLocation);
	FRotator rotation = GetEffectRotation(InRotation);

	EffectExecute->ExecuteEffectWithDelay(EffectTag, location, rotation, Delay, InScale);
}

void UBossEffectComponent::PlayEffectLoop(FGameplayTag EffectTag, float Interval, int32 LoopCount, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CheckNull(EffectExecute);

	FVector location = GetEffectLocation(InLocation);
	FRotator rotation = GetEffectRotation(InRotation);

	EffectExecute->ExecuteEffectLoop(EffectTag, location, rotation, Interval, LoopCount, InScale);
}

void UBossEffectComponent::PlayEffectAtSocket(FGameplayTag EffectTag, const FName& SocketName,const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CheckNull(EffectExecute);
	
	EffectExecute->ExecuteEffectAtSocket(EffectTag, SocketName, InLocation, InRotation, InScale);
}

void UBossEffectComponent::PlayEffectAttachedToSocket(FGameplayTag EffectTag, const FName& SocketName,const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CheckNull(EffectExecute);
	
	EffectExecute->ExecuteEffectAttachedToSocket(EffectTag, SocketName, InLocation, InRotation, InScale);
}

void UBossEffectComponent::PlayEffectAtSocketWithDelay(FGameplayTag EffectTag, const FName& SocketName, float Delay,const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CheckNull(EffectExecute);
	
	EffectExecute->ExecuteEffectAtSocketWithDelay(EffectTag, SocketName, Delay, InLocation, InRotation, InScale);
}

void UBossEffectComponent::PlayEffectAtSocketLoop(FGameplayTag EffectTag, const FName& SocketName, float Interval, int32 LoopCount,const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CheckNull(EffectExecute);
	
	EffectExecute->ExecuteEffectAtSocketLoop(EffectTag, SocketName, Interval, LoopCount, InLocation, InRotation, InScale);
}

void UBossEffectComponent::PlayEffectAttachedToSocketLoop(FGameplayTag EffectTag, const FName& SocketName, float Interval, int32 LoopCount, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CheckNull(EffectExecute);
	
	EffectExecute->ExecuteEffectAttachedToSocketLoop(EffectTag, SocketName, Interval, LoopCount, InLocation, InRotation, InScale);
}

void UBossEffectComponent::StopAllEffects()
{
	CheckNull(EffectManager);
	
	EffectManager->StopAllEffects();
}

void UBossEffectComponent::StopEffect(FGameplayTag EffectTag)
{
	CheckNull(EffectManager);
	
	EffectManager->StopEffect(EffectTag);
}

void UBossEffectComponent::PlayEffectAtBossLocation(FGameplayTag EffectTag)
{
	CheckNull(OwnerCharacter);
	
	PlayEffect(EffectTag, OwnerCharacter->GetActorLocation(), OwnerCharacter->GetActorRotation());
}

void UBossEffectComponent::SetPoolSize(int32 NewPoolSize)
{
	MaxPoolSize = FMath::Max(1, NewPoolSize);
	
	if (EffectManager)
	{
		EffectManager->SetMaxPoolSize(MaxPoolSize);
	}
}

void UBossEffectComponent::SetAutoExpandPool(bool bAutoExpand)
{
	bAutoExpandPool = bAutoExpand;
	
	if (EffectManager)
	{
		EffectManager->SetAutoExpandPool(bAutoExpandPool);
	}
}

int32 UBossEffectComponent::GetActiveEffectCount() const
{
	if (EffectManager)
	{
		return EffectManager->GetActiveEffectCount();
	}
	return 0;
}

int32 UBossEffectComponent::GetAvailableEffectCount() const
{
	if (EffectManager)
	{
		return EffectManager->GetAvailableEffectCount();
	}
	return 0;
}

void UBossEffectComponent::InitializeEffectManager()
{
	CheckNull(OwnerCharacter);

	// EffectManager 생성 및 초기화
	EffectManager = NewObject<UBossEffectManager>(this);
	EffectManager->BeginPlay(OwnerCharacter, EffectDataTable, MaxPoolSize, bAutoExpandPool);

	// EffectExecute 생성 및 초기화
	EffectExecute = NewObject<UBossEffectExecute>(this);
	EffectExecute->BeginPlay(OwnerCharacter, EffectManager->EffectDatas, EffectManager);
}

FVector UBossEffectComponent::GetEffectLocation(const FVector& InLocation)
{
	// 위치가 기본값이면 보스 위치 사용
	if (InLocation.IsZero())
	{
		return OwnerCharacter ? OwnerCharacter->GetActorLocation() : FVector::ZeroVector;
	}
	return InLocation;
}

FRotator UBossEffectComponent::GetEffectRotation(const FRotator& InRotation)
{
	// 회전이 기본값이면 보스 회전 사용
	if (InRotation.IsZero())
	{
		return OwnerCharacter ? OwnerCharacter->GetActorRotation() : FRotator::ZeroRotator;
	}
	return InRotation;
}

