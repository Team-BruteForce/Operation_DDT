// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CStateComponent.h"

// Sets default values for this component's properties
UCStateComponent::UCStateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();

	// 기본 상태를 Idle로 설정
	SetIdleMode();
	
}


// Called every frame
void UCStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FString UCStateComponent::GetTypeString()
{
	UEnum* Enum = StaticEnum<EStateType>();
	FString result = "State : ";
	result += Enum->GetNameStringByValue((int64)Type);
	return result;
}

void UCStateComponent::SetIdleMode()
{
	ChangeType (EStateType::Idle);
}

void UCStateComponent::SetRollingMode()
{
	ChangeType(EStateType::Rolling);
}

void UCStateComponent::SetEquipMode()
{
	ChangeType(EStateType::Equip);
}

void UCStateComponent::SetHittedMode()
{
	ChangeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChangeType(EStateType::Dead);
}

void UCStateComponent::SetActionMode()
{
	ChangeType(EStateType::Action);
}

void UCStateComponent::ChangeType(EStateType InType)
{
	EStateType prevType = Type;
	Type = InType;

	if (OnStateTypeChanged.IsBound())
		OnStateTypeChanged.Broadcast (prevType, Type);

}

