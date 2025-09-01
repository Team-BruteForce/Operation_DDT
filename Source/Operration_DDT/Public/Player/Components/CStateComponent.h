// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"


UENUM()
enum class EStateType : uint8
{
	Idle = 0,
	Sliding,
	RifleAim,
	RifleAtt,
	SwordAtt,
	RevolverAtt,
	Equip,
	BackStep,
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
	FORCEINLINE bool IsSlidingMode() { return Type == EStateType::Sliding; }
	FORCEINLINE bool IsEquipMode() { return Type == EStateType::Equip; }
	FORCEINLINE bool IsHittedMode() { return Type == EStateType::Hitted; }
	FORCEINLINE bool IsDeadMode() { return Type == EStateType::Dead; }
	FORCEINLINE bool IsActionMode() { return Type == EStateType::Action; }
	FORCEINLINE bool IsRifleAimMode() { return bRifleAimMode; }
	

	FORCEINLINE EStateType GetType() {return Type;}

	FString GetTypeString();

	void SetIdleMode();
	void SetSlidingMode();
	void SetEquipMode();
	void SetHittedMode();
	void SetDeadMode();
	void SetActionMode();
	FORCEINLINE void SetRifleAimMode(bool inBool) { bRifleAimMode = inBool; }

	FStateTypeChanged OnStateTypeChanged;


private:
	void ChangeType(EStateType InType);

	EStateType Type;

	bool bRifleAimMode = false;


		
};
