// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerHealthChanged, float, NewHP, float, MaxHP);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetNowHp() { return NowHp; }
	FORCEINLINE float GetMaxHP() { return MaxHp; }

	FORCEINLINE int32 GetHealItemCount () { return HealItemCount; }
	FORCEINLINE bool GetIsHealing () { return bIsHealing; }

	FORCEINLINE void GainHealItem() { HealItemCount++; }

	void GetDamage(float value);
	void GetHeal(float value);
	
	// HP를 최대치로 복구하는 함수
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetFullHealth() { NowHp = MaxHp; StopHealing(); }

	// 점진적 회복 시작 함수 (키 입력 시 호출)
	UFUNCTION(BlueprintCallable, Category = "Health")
	void CalculateHealing();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void StartHealing();

	// 회복 중단 함수
	UFUNCTION(BlueprintCallable, Category = "Health")
	void StopHealing();

	// 회복 가능 여부 체크
	UFUNCTION(BlueprintCallable, Category = "Health")
	bool CanHeal() const;

	// 현재 체력 퍼센트 가져오기
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthPercentage() const;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHp = 100.f;

	UPROPERTY(EditAnywhere, Category = "Health")
	int32 HealItemCount = 3;

	// 회복 관련 설정
	UPROPERTY(EditAnywhere, Category = "Healing", meta = (AllowPrivateAccess = "true"))
	float HealDuration = 0.5f; // 회복 시간 (초)

	UPROPERTY(EditAnywhere, Category = "Healing", meta = (AllowPrivateAccess = "true"))
	float HealPercentage = 0.7f; // 최대 체력 대비 회복 비율

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnPlayerHealthChanged OnPlayerHealthChanged;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Health")
	float NowHp = MaxHp;

	class ADDTPlayer* OwnerCharater;
	class UCStateComponent* StateComp;

	// 회복 관련 변수들
	bool bIsHealing = false;
	float HealTargetAmount = 0.0f;  // 목표 회복량
	float HealStartAmount = 0.0f;   // 회복 시작 시점의 체력
	float HealElapsedTime = 0.0f;   // 경과 시간

	float ActualHealAmount = 0.0f;
	float HealAmount;

		
};
