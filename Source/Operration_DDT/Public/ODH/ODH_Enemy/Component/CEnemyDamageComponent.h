// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ODH/ODH_Enemy/Interface/IDamageable.h"
#include "CEnemyDamageComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OPERRATION_DDT_API UCEnemyDamageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCEnemyDamageComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 데미지 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	float DamageAmount = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	bool bCanDamage = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
	TArray<TEnumAsByte<ECollisionChannel>> DamageChannels;

	// 데미지를 주는 함수
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void DealDamage(AActor* TargetActor);

	// 범위 내의 모든 데미지 가능한 액터들에게 데미지를 주는 함수
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void DealDamageInRadius(FVector Origin, float Radius);

	// 데미지 설정 함수들
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void SetDamageAmount(float NewDamageAmount);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	void SetCanDamage(bool bNewCanDamage);

	UFUNCTION(BlueprintCallable, Category = "Damage")
	float GetDamageAmount() const { return DamageAmount; }

	UFUNCTION(BlueprintCallable, Category = "Damage")
	bool GetCanDamage() const { return bCanDamage; }

private:
	// 데미지 처리 로직
	void ProcessDamage(AActor* TargetActor);
};
