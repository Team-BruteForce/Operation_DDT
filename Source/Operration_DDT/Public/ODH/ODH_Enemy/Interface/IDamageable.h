// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDamageable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDamageable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 에너미가 데미지를 받을 수 있다는 것을 나타내는 인터페이스
 */
class OPERRATION_DDT_API IDamageable
{
	GENERATED_BODY()

public:
	// 데미지를 받는 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	void TakeDamage(float DamageAmount);
	
	// 데미지를 받을 수 있는지 확인하는 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	bool CanTakeDamage() const;
	
	// 사망했는지 확인하는 함수
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Damage")
	bool IsDead() const;
};


