// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CEnemyStatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChanged, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMaxHealthChanged, float, NewMaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamageReceived, float, DamageAmount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OPERRATION_DDT_API UCEnemyStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCEnemyStatusComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 스테이터스 변수들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float AttackPower = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float DefensePower = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	bool bIsDead = false;

	// 이벤트 델리게이트들
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthChanged OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMaxHealthChanged OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDamageReceived OnDamageReceived;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDeath OnDeath;

public:
	// 체력 관련 함수들
	UFUNCTION(BlueprintCallable, Category = "Status")
	void TakeDamage(float DamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void Heal(float HealAmount);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetMaxHealth(float NewMaxHealth);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetCurrentHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Status")
	float GetHealthPercent() const;

	UFUNCTION(BlueprintCallable, Category = "Status")
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = "Status")
	bool IsDead() const { return bIsDead; }

	// 공격력 관련 함수들
	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetAttackPower(float NewAttackPower);

	UFUNCTION(BlueprintCallable, Category = "Status")
	float GetAttackPower() const { return AttackPower; }

	// 방어력 관련 함수들
	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetDefensePower(float NewDefensePower);

	UFUNCTION(BlueprintCallable, Category = "Status")
	float GetDefensePower() const { return DefensePower; }

	// 사망 처리
	UFUNCTION(BlueprintCallable, Category = "Status")
	void Die();

	// 상태 초기화 (EnemyRestart용)
	UFUNCTION(BlueprintCallable, Category = "Status")
	void ResetStatus();

private:
	// 체력이 0 이하로 떨어졌는지 확인
	void CheckDeath();
};


