// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStaminaChanged, float, NewStamina, float, MaxStamina);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCStaminaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetNowStamina() { return NowStamina; }
	FORCEINLINE float GetMaxStamina() { return MaxStamina; }

	void ConsumeStamina(float value);
	void DrowningStamina(float value);

	void RecoverStamina();

	void SetRecoverTimer();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina")
	float RollingStamina = 30.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stamina")
	float SprintStamina = 20.f;

	UPROPERTY(BlueprintAssignable)
	FOnStaminaChanged OnStaminaChanged;


private:
	class ADDTPlayer* OwnerCharacter;
	class UCStateComponent* state;
	class UCMovementComponent* movement;
	
	UPROPERTY(EditDefaultsOnly)
	float MaxStamina = 100.f;
	
	float NowStamina = 0.f;
	
	UPROPERTY(EditDefaultsOnly)
	float RollingRecoveryDelay = 2.f;

	UPROPERTY(EditDefaultsOnly)
	float RecoveryAmount = 25.f;
	
	
	//bool bIsConsumingStamina = false;

	FTimerHandle StaminaRecoveryTimerHandle;
	bool bIsRecovering = false;
	bool bCanRecover = false;
};
