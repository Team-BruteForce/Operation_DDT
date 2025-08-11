// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global.h"
#include "Player/DDTPlayer.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

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
	
}


// Called every frame
void UCMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	Direction = FTransform(OwnerCharacter->GetControlRotation ()).TransformVector (Direction);
	OwnerCharacter->AddMovementInput (Direction);

	Direction = FVector::ZeroVector;

	// 카메라 각도 클램프
	FRotator ControlRot = OwnerCharacter->GetControlRotation();
	ControlRot.Pitch = FMath::ClampAngle(ControlRot.Pitch, MinPitch, MaxPitch);
	OwnerCharacter->GetController()->SetControlRotation(ControlRot);

}

void UCMovementComponent::SetupInputBinding(class UEnhancedInputComponent* input)
{
	input->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ThisClass::OnMove);
	input->BindAction(IA_TurnHor, ETriggerEvent::Triggered, this, &ThisClass::OnHorizontalLook);
	input->BindAction(IA_TurnVer, ETriggerEvent::Triggered, this, &ThisClass::OnVerticalLook);
}


void UCMovementComponent::OnSprint()
{
	SetSpeed(ESpeedType::Sprint);
}

void UCMovementComponent::OnRun()
{
	SetSpeed (ESpeedType::Run);
}

void UCMovementComponent::OnWalk()
{
	SetSpeed (ESpeedType::Walk);
}

void UCMovementComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->GetCharacterMovement ()->bOrientRotationToMovement = false;
}

void UCMovementComponent::DisableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UCMovementComponent::OnMove(const struct FInputActionValue& InAxis)
{
	FVector2D inputValue = InAxis.Get<FVector2D>();

	Direction.X = inputValue.X;
	Direction.Y = inputValue.Y;
}

void UCMovementComponent::OnHorizontalLook(const struct FInputActionValue& InAxis)
{
	CheckTrue(bFixedCamera);

	float value = InAxis.Get<float>();

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Blue, FString::Printf(TEXT("Hor : %.2f"), value));
	
	OwnerCharacter->AddControllerYawInput (value);

}

void UCMovementComponent::OnVerticalLook(const struct FInputActionValue& InAxis)
{
	CheckTrue(bFixedCamera);

	float value = InAxis.Get<float>();

	GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("Ver : %.2f"), value));

	OwnerCharacter->AddControllerPitchInput (value);
}

void UCMovementComponent::SetSpeed(ESpeedType InType)
{
	OwnerCharacter->GetCharacterMovement ()->MaxWalkSpeed = Speed[(int32)InType];
}



