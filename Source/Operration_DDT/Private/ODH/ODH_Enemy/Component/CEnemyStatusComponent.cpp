// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"

UCEnemyStatusComponent::UCEnemyStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
	// 기본값 설정
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	AttackPower = 20.0f;
	DefensePower = 5.0f;
	bIsDead = false;
}

void UCEnemyStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// 초기 체력을 최대 체력으로 설정
	CurrentHealth = MaxHealth;
	
	// 초기 이벤트 발생
	OnHealthChanged.Broadcast(CurrentHealth);
	OnMaxHealthChanged.Broadcast(MaxHealth);
}

void UCEnemyStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCEnemyStatusComponent::TakeDamage(float DamageAmount)
{
	if (bIsDead)
		return;

	// 방어력을 고려한 실제 데미지 계산
	float ActualDamage = FMath::Max(0.0f, DamageAmount - DefensePower);
	
	// 체력 감소
	CurrentHealth = FMath::Max(0.0f, CurrentHealth - ActualDamage);
	
	// 데미지를 받았다는 이벤트 발생
	OnDamageReceived.Broadcast(ActualDamage);
	
	// 체력 변경 이벤트 발생
	OnHealthChanged.Broadcast(CurrentHealth);
	
	// 사망 여부 확인
	CheckDeath();
	

}

void UCEnemyStatusComponent::Heal(float HealAmount)
{
	if (bIsDead)
		return;

	// 체력 회복
	float OldHealth = CurrentHealth;
	CurrentHealth = FMath::Min(MaxHealth, CurrentHealth + HealAmount);
	
	// 체력이 실제로 회복되었을 때만 이벤트 발생
	if (CurrentHealth > OldHealth)
	{
		OnHealthChanged.Broadcast(CurrentHealth);
	}
}

void UCEnemyStatusComponent::SetMaxHealth(float NewMaxHealth)
{
	if (NewMaxHealth <= 0.0f)
		return;

	MaxHealth = NewMaxHealth;
	
	// 최대 체력이 줄어들었고 현재 체력이 새로운 최대 체력을 초과하는 경우 조정
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
	}
	
	// 이벤트 발생
	OnMaxHealthChanged.Broadcast(MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth);
}

void UCEnemyStatusComponent::SetCurrentHealth(float NewHealth)
{
	if (bIsDead)
		return;

	// 체력 범위 제한
	CurrentHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	
	// 이벤트 발생
	OnHealthChanged.Broadcast(CurrentHealth);
	
	// 사망 여부 확인
	CheckDeath();
}

float UCEnemyStatusComponent::GetHealthPercent() const
{
	if (MaxHealth <= 0.0f)
		return 0.0f;
		
	return CurrentHealth / MaxHealth;
}

void UCEnemyStatusComponent::SetAttackPower(float NewAttackPower)
{
	AttackPower = FMath::Max(0.0f, NewAttackPower);
}

void UCEnemyStatusComponent::SetDefensePower(float NewDefensePower)
{
	DefensePower = FMath::Max(0.0f, NewDefensePower);
}

void UCEnemyStatusComponent::Die()
{
	if (bIsDead)
		return;

	bIsDead = true;
	
	// 사망 이벤트 발생
	OnDeath.Broadcast();
	

	
	// 소유자 액터가 있다면 사망 처리
	AActor* Owner = GetOwner();
	if (Owner)
	{
		// 여기에 사망 애니메이션 재생, 사망 효과 등 추가 가능
		// 예: Owner->Destroy(); // 즉시 제거
		// 또는 사망 애니메이션 후 제거하는 로직
	}
}

void UCEnemyStatusComponent::CheckDeath()
{
	if (CurrentHealth <= 0.0f && !bIsDead)
	{
		Die();
	}
}

void UCEnemyStatusComponent::ResetStatus()
{
	// 사망 상태 리셋
	bIsDead = false;
	
	// 체력을 최대치로 복구
	CurrentHealth = MaxHealth;
	
	// 이벤트 발생
	OnHealthChanged.Broadcast(CurrentHealth);
	OnMaxHealthChanged.Broadcast(MaxHealth);
}


