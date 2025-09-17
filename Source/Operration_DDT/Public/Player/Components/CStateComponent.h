// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"


UENUM()
enum class EStateType : uint8
{
	Idle = 0,
	Rolling,
	RifleAim,
	RifleAtt,
	SwordAtt,
	RevolverAtt,
	RifleReload,
	Equip,
	Healing,
	Hitted,
	Dead,
	Action,
	Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, InPrevType, EStateType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	FORCEINLINE bool IsIdleMode() { return Type == EStateType::Idle; }
	FORCEINLINE bool IsRollingMode() { return Type == EStateType::Rolling; }
	FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	FORCEINLINE bool IsReloadMode() { return Type == EStateType::RifleReload; }
	FORCEINLINE bool IsHealingMode() { return Type == EStateType::Healing; }
	
	FORCEINLINE bool IsRifleAimMode() { return bRifleAimMode; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsCanDodge() { return bCanDodge; }
	

	FORCEINLINE EStateType GetType() {return Type;}

	FString GetTypeString();

	void SetIdleMode();
	void SetRollingMode();
	void SetEquipMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetActionMode();
	void SetReloadMode();
	void SetHealingMode();
	
	FORCEINLINE void SetRifleAimMode(bool inBool) { bRifleAimMode = inBool; }
	FORCEINLINE void SetCanDodge(bool inBool) { bCanDodge = inBool; }
	FORCEINLINE void SetIsHammerPulled(bool inBool) {bIsHammerPulled = inBool; }
	FORCEINLINE bool GetIsHammerPulled() { return bIsHammerPulled; }
	
	FORCEINLINE bool GetIsDead() { return bIsDead; }
	
	// 부활 시 사망 상태를 리셋하는 함수
	UFUNCTION(BlueprintCallable, Category = "State")
	void ResetDeadState() { bIsDead = false; }

	FStateTypeChanged OnStateTypeChanged;


private:
	void ChangeType(EStateType InType);

	EStateType Type;

	bool bRifleAimMode = false;

	bool bCanDodge = false;
	
	bool bIsDead = false;

	bool bIsHammerPulled = true;
	
	// 장전 중단 추적 변수
	bool bReloadInterrupted = false;


		
};
