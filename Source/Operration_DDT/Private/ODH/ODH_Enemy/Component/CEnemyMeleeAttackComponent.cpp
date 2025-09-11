// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Player/DDTPlayer.h"

UCEnemyMeleeAttackComponent::UCEnemyMeleeAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	// 기본값 설정
	bIsAttackActive = false;
	AttackCooldown = 1.0f;
	LastAttackTime = 0.0f;
}

void UCEnemyMeleeAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCEnemyMeleeAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCEnemyMeleeAttackComponent::ActivateMeleeAttack()
{
	bIsAttackActive = true;
	
	// 디버그 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("Melee Attack Activated!"));
	}
}

void UCEnemyMeleeAttackComponent::DeactivateMeleeAttack()
{
	bIsAttackActive = false;
	
	// 디버그 출력
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Orange, TEXT("Melee Attack Deactivated!"));
	}
}

float UCEnemyMeleeAttackComponent::GetMeleeDamage() const
{
	// 소유자 액터에서 StatusComponent를 찾아서 공격력 반환
	AActor* Owner = GetOwner();
	if (Owner)
	{
		UCEnemyStatusComponent* StatusComponent = Owner->FindComponentByClass<UCEnemyStatusComponent>();
		if (StatusComponent)
		{
			return StatusComponent->GetAttackPower();
		}
	}
	
	// StatusComponent를 찾을 수 없는 경우 기본값 반환
	return 20.0f;
}

// OnMeleeAttackOverlap 함수는 CFlyingSkull에서 처리하므로 제거됨
