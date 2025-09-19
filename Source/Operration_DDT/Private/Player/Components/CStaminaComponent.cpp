// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CStaminaComponent.h"
#include "Global.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CMovementComponent.h"

// Sets default values for this component's properties
UCStaminaComponent::UCStaminaComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCharacter =Cast<ADDTPlayer>( GetOwner());
	state = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	NowStamina = MaxStamina;

	OnStaminaChanged.Broadcast(NowStamina,NowStamina,MaxStamina);
}


// Called every frame
void UCStaminaComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (movement->GetIsSprinting())
	{
		DrowningStamina(SprintStamina * DeltaTime);
	}
	else if (bIsRecovering)
	{
		if (movement->GetIsShiftPrssing()) return;
		if (NowStamina < MaxStamina)
		{
			float RecoverRate = RecoveryAmount * DeltaTime;
			float prevStamina = NowStamina;
			
			NowStamina = FMath::Clamp (NowStamina + RecoverRate, 0.f, MaxStamina);

			OnStaminaChanged.Broadcast(prevStamina,NowStamina,MaxStamina);
			
			if (NowStamina >= MaxStamina)
			{
				bIsRecovering = false;
				CLog::Log("Recovery End");
			}
		}
	}

}

void UCStaminaComponent::ConsumeStamina(float value)
{
	float prevStamina = NowStamina;
	NowStamina = FMath::Clamp(NowStamina - value, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast(prevStamina,NowStamina,MaxStamina);
	SetRecoverTimer();
}

void UCStaminaComponent::DrowningStamina(float value)
{
	float prevStamina = NowStamina;
	NowStamina = FMath::Clamp(NowStamina - value, 0.f, MaxStamina);
	OnStaminaChanged.Broadcast(prevStamina,NowStamina,MaxStamina);
}

void UCStaminaComponent::RecoverStamina()
{
	if (NowStamina >= MaxStamina)
	{
		bIsRecovering = false;
		return;
	}
	bIsRecovering = true;
	CLog::Log ("Stamina Recovery Start");
}

void UCStaminaComponent::SetRecoverTimer()
{
	if (!state->IsIdleMode()) return;
	if (movement->GetIsSprinting()) return;
	bIsRecovering = false;

	GetWorld()->GetTimerManager().ClearTimer(StaminaRecoveryTimerHandle);
	
	GetWorld()->GetTimerManager().SetTimer(
		StaminaRecoveryTimerHandle,
		this,
		&UCStaminaComponent::RecoverStamina,
		RollingRecoveryDelay,
		false
		);
	
}


