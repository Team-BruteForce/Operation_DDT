// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global.h"
#include "Player/DDTPlayer.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Player/Components/CStaminaComponent.h"

// Sets default values for this component's properties
UCMovementComponent::UCMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ADDTPlayer>(GetOwner());
	OwnerCharacter->InputBindingDelegate.AddUObject(this, &UCMovementComponent::SetupInputBinding);
	CharMove = CHelpers::GetComponent<UCharacterMovementComponent>(OwnerCharacter);
	OwnerState = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	OwnerStamina = CHelpers::GetComponent<UCStaminaComponent>(OwnerCharacter); 
	
}


// Called every frame
void UCMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//CLog::Log("Movement) Direction " + Direction.ToString());
	if (!Direction.IsNearlyZero())
	{
		FVector InputDirection = FTransform(OwnerCharacter->GetControlRotation()).TransformVector(Direction);

		InputDirection.Z = 0.f;
		InputDirection.Normalize ();

		OwnerCharacter->AddMovementInput (InputDirection);

		Direction = FVector::ZeroVector;

	}
	
	FRotator ControlRot = OwnerCharacter->GetControlRotation();
	ControlRot.Pitch = FMath::ClampAngle(ControlRot.Pitch, MinPitch, MaxPitch);
	OwnerCharacter->GetController()->SetControlRotation(ControlRot);

	if (OwnerStamina->GetNowStamina() <= 0.f && bIsSprinting)
	{
		OnRun();
	}

}

void UCMovementComponent::SetupInputBinding(class UEnhancedInputComponent* input)
{
	/*input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ThisClass::OnMove);
	input->BindAction(IA_TurnHor, ETriggerEvent::Triggered, this, &ThisClass::OnHorizontalLook);
	input->BindAction(IA_TurnVer, ETriggerEvent::Triggered, this, &ThisClass::OnVerticalLook);
	input->BindAction(IA_Sprint, ETriggerEvent::Started, this, &ThisClass::SprintStart);
	input->BindAction(IA_Sprint, ETriggerEvent::Completed, this, &ThisClass::SprintEnd);*/
}


void UCMovementComponent::OnSprint()
{
	if (OwnerStamina->GetNowStamina() <= 0.f) return;
	if (bOnMovePressing == false) return;
	
	SetSpeed(ESpeedType::Sprint);
	
	if (bIsSprinting == false)
	{
		bIsSprinting = true;
	}
}

void UCMovementComponent::OnRun()
{
	SetSpeed (ESpeedType::Run);
	if (bIsSprinting == true)
	{
		bIsSprinting = false;
	}	
	
}

void UCMovementComponent::OnWalk()
{
	SetSpeed (ESpeedType::Walk);
	if (bIsSprinting == true)
	{
		bIsSprinting = false;
	}
	if (bIsShiftPressing == true)
	{
		bIsShiftPressing = false;
	}
}

void UCMovementComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	//OwnerCharacter->GetCharacterMovement ()->bOrientRotationToMovement = false;
	CharMove->bOrientRotationToMovement = false;
}

void UCMovementComponent::DisableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	//OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	CharMove->bOrientRotationToMovement = true;
}

void UCMovementComponent::OnMove(const struct FInputActionValue& InAxis)
{
	FVector2D inputValue = InAxis.Get<FVector2D>();

	Direction.X = inputValue.X;
	Direction.Y = inputValue.Y;
	CachedDirection = Direction;
	CachedDirection.Z = 0.f;

	bOnMovePressing = true;

	//CLog::Log("Forward : " + FString::SanitizeFloat(GetForwardInput()) + " Right : " + FString::SanitizeFloat(GetRightInput()));
}

void UCMovementComponent::OnHorizontalLook(const struct FInputActionValue& InAxis)
{
	CheckTrue(bFixedCamera);

	float value = InAxis.Get<float>();
	
	OwnerCharacter->AddControllerYawInput (value);

}

void UCMovementComponent::OnVerticalLook(const struct FInputActionValue& InAxis)
{
	CheckTrue(bFixedCamera);

	float value = InAxis.Get<float>();
	
	OwnerCharacter->AddControllerPitchInput (value);
}

void UCMovementComponent::SprintStart()
{
	if (OwnerState->IsRifleAimMode()) return;
	OnSprint();
	bIsShiftPressing = true;
}

void UCMovementComponent::SprintEnd()
{
	if (OwnerState->IsRifleAimMode()) return;
	OnRun();
	bIsShiftPressing = false;
	
	if (OwnerStamina->GetNowStamina() <= 0.f)
	{
		OwnerStamina->SetRecoverTimer();
		return;
	}
	OwnerStamina->RecoverStamina();
}



void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	//OwnerCharacter->GetCharacterMovement ()->MaxWalkSpeed = Speed[(int32)InType];
	CharMove->MaxWalkSpeed = Speed[(int32)InType];
}

float UCMovementComponent::GetForwardInput() const
{
	if (!OwnerCharacter) return 0.0f;
	
	// 입력 방향을 캐릭터 기준으로 변환
	FVector LocalDirection = GetLocalInputDirection();
	return LocalDirection.X; // Forward 방향
}

float UCMovementComponent::GetRightInput() const
{
	if (!OwnerCharacter) return 0.0f;
	
	// 입력 방향을 캐릭터 기준으로 변환
	FVector LocalDirection = GetLocalInputDirection();
	return LocalDirection.Y; // Right 방향
}

FVector UCMovementComponent::GetLocalInputDirection() const
{
	if (!OwnerCharacter) return FVector::ZeroVector;
	
	// 입력 방향을 월드 좌표계로 변환
	FVector WorldDirection = Direction;
	WorldDirection.Z = 0.0f;
	WorldDirection.Normalize();
	
	// 컨트롤러 회전을 기준으로 로컬 방향 계산
	FVector LocalDirection = FRotationMatrix(OwnerCharacter->GetControlRotation()).InverseTransformVector(WorldDirection);
	
	return LocalDirection;
}



