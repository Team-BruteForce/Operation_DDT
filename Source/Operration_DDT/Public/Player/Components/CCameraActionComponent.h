// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CCameraActionComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCCameraActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCCameraActionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	class ADDTPlayer* OwnerCharacter;

	class USpringArmComponent* OwnerSpringArm;
	class UCMovementComponent* Movement;
	class UCStateComponent* State;
	class UCUIComponent* UIComp;

public:
	void SetAimPosition();
	void SetIdlePosition();

	UPROPERTY(EditAnywhere)
	float locX = -80.f;

	UPROPERTY(EditAnywhere)
	float locZ = 180.f;
	
	UPROPERTY(EditAnywhere)
	float ArmLen = 75.f;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	class USoundCue* AimSound;

	UPROPERTY(EditDefaultsOnly, Category = "CameraShake")
	TSubclassOf<class UCameraShakeBase> HitCameraShake;

	void ShakeCameraByHit();

		
};
