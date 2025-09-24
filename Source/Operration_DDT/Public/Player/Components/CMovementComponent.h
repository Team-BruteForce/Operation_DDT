// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMovementComponent.generated.h"

UENUM()
enum class ESpeedType : uint8
{
	Walk = 0,
	Run,
	Sprint,
	Max
};
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float HorizontalLook = 45.f;

	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
	float VerticalLook = 45.f;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float Speed[(int32)ESpeedType::Max] = { 200.f, 400.f, 600.f };

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupInputBinding(class UEnhancedInputComponent* input);


#pragma region Getter/Setter
	FORCEINLINE bool CanMove() { return bCanMove; }
	FORCEINLINE void Move() { bCanMove = true; }
	FORCEINLINE void Stop() { bCanMove = false; }

	FORCEINLINE float GetWalkSpeed() { return Speed[(int32)ESpeedType::Walk]; }
	FORCEINLINE float GetRunSpeed() { return Speed[(int32)ESpeedType::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int32)ESpeedType::Sprint]; }

	FORCEINLINE	bool GetFixedCamera() { return bFixedCamera; }
	FORCEINLINE void EnableFixedCamera() { bFixedCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixedCamera = false; }
	
	FORCEINLINE FVector GetCurrentDirection() { return Direction; }
	
	FORCEINLINE bool GetIsSprinting() { return bIsSprinting; }
	FORCEINLINE void SetIsSprinting(bool inBool) { bIsSprinting = inBool; }

	FORCEINLINE bool GetIsShiftPressing() { return bIsShiftPressing; }
	
	// BlendSpace용 방향 계산
	FORCEINLINE float GetForwardInput() const;
	FORCEINLINE float GetRightInput() const;
	FVector GetLocalInputDirection() const;

#pragma endregion Getter/Setter
	
	void OnSprint();
	void OnRun();
	void OnWalk();

	void EnableControlRotation();
	void DisableControlRotation();

	FORCEINLINE FVector GetCachedDirection() { return CachedDirection; }

#pragma region InputFunction
	
	void OnMove(const struct FInputActionValue& InAxis);
	void OnHorizontalLook(const struct FInputActionValue& InAxis);
	void OnVerticalLook(const struct FInputActionValue& InAxis);

#pragma endregion InputFunction

public:
	void SprintStart();
	void SprintEnd();

	FORCEINLINE void SetFalseOnMovePressing(){ bOnMovePressing = false; }
	FORCEINLINE bool GetOnMovePressing() { return bOnMovePressing; }
	void ResetDirection();

private:
	void SetSpeed(ESpeedType InType);

private:
	UPROPERTY()
	class ADDTPlayer* OwnerCharacter;
	UPROPERTY()
	class UCStateComponent* OwnerState;
	UPROPERTY()
	class UCStaminaComponent* OwnerStamina;
	UPROPERTY()
	class UCharacterMovementComponent* CharMove;

	bool bCanMove = true;	
	bool bFixedCamera;
	
	bool bIsSprinting = false;
	bool bIsShiftPressing = false;

	FVector Direction = FVector::ZeroVector;
	FVector CachedDirection;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinPitch = -65.f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxPitch = 130.f;

	bool bOnMovePressing = false;

		
};
