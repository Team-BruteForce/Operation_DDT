// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_Enemy/Component/CEnemyDamageComponent.h"
#include "ODH/ODH_Enemy/Interface/IDamageable.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/EngineTypes.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

UCEnemyDamageComponent::UCEnemyDamageComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	// 기본값 설정
	DamageAmount = 25.0f;
	bCanDamage = true;
	
	// 기본 데미지 채널 설정 (Pawn 채널)
	DamageChannels.Add(ECollisionChannel::ECC_Pawn);
}

void UCEnemyDamageComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCEnemyDamageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCEnemyDamageComponent::DealDamage(AActor* TargetActor)
{
	if (!bCanDamage || !TargetActor)
		return;

	// 데미지 처리
	ProcessDamage(TargetActor);
}

void UCEnemyDamageComponent::DealDamageInRadius(FVector Origin, float Radius)
{
	if (!bCanDamage)
		return;

	// 범위 내의 모든 액터들을 찾기
	TArray<AActor*> ActorsInRadius;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	
	// Pawn 타입의 오브젝트들을 찾기
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	
	// 범위 내의 액터들 찾기
	UKismetSystemLibrary::SphereOverlapActors(
		GetWorld(),
		Origin,
		Radius,
		ObjectTypes,
		nullptr,
		TArray<AActor*>(),
		ActorsInRadius
	);

	// 각 액터에게 데미지 주기
	for (AActor* Actor : ActorsInRadius)
	{
		if (Actor && Actor != GetOwner())
		{
			ProcessDamage(Actor);
		}
	}

	// 디버그 시각화 (개발 중에만 사용)
	if (GEngine)
	{
		DrawDebugSphere(GetWorld(), Origin, Radius, 12, FColor::Red, false, 2.0f);
	}
}

void UCEnemyDamageComponent::SetDamageAmount(float NewDamageAmount)
{
	DamageAmount = FMath::Max(0.0f, NewDamageAmount);
}

void UCEnemyDamageComponent::SetCanDamage(bool bNewCanDamage)
{
	bCanDamage = bNewCanDamage;
}

void UCEnemyDamageComponent::ProcessDamage(AActor* TargetActor)
{
	if (!TargetActor)
		return;

	// IDamageable 인터페이스를 구현한 액터인지 확인
	if (TargetActor->GetClass()->ImplementsInterface(UDamageable::StaticClass()))
	{
		// 데미지를 받을 수 있는지 확인
		bool bCanTakeDamage = IDamageable::Execute_CanTakeDamage(TargetActor);
		if (bCanTakeDamage)
		{
			// 데미지 주기
			IDamageable::Execute_TakeDamage(TargetActor, DamageAmount);
			
			// 디버그 출력
			if (GEngine)
			{
				FString DebugMessage = FString::Printf(TEXT("Dealt %.1f damage to %s"), DamageAmount, *TargetActor->GetName());
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, DebugMessage);
			}
		}
	}
}
