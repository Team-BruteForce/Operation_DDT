// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "DDTPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UEnhancedInputComponent*);

UCLASS(Blueprintable)
class OPERRATION_DDT_API ADDTPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADDTPlayer();

	FInputBindingDelegate InputBindingDelegate;

private:
	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;
 
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	class UStaticMeshComponent* SwordHolster;
public:
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return this->SpringArm; }

#pragma region InputActions
private:
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* IMC_Player;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Rifle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Sword;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Revolver;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Attack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_AimRifle;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Roll;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Reload;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* IA_Heal;
	
#pragma endregion 
 
public:
	UPROPERTY(VisibleAnywhere)
		class UCMontageComponent* Montages;
 
	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* Movement;
 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UCStateComponent* State;
	
	UPROPERTY(visibleAnywhere)
		class UCWeaponComponent* WeaponComp;
	
	UPROPERTY(VisibleAnywhere)
	class UCCameraActionComponent* CameraActionComp;

	UPROPERTY(VisibleAnywhere)
	class UCFireComponent* FireComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UCStatusComponent* Status;

	UPROPERTY(visibleAnywhere)
	class UCRespawnComponent* RespawnComp;

	UPROPERTY(visibleAnywhere, BlueprintReadOnly)
	class UCMagazineComponent* MagazineComp;

  
protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/*UFUNCTION()
	void OnPlayerOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);*/
	float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	
  
private:
	UFUNCTION()
	void OnStateTypeChanged(EStateType InPrevType, EStateType InNewType);

	void OnAvoid();
	FVector GetCurrentInputDirection();
	
	void Roll();
	void Hitted();
	void Dead();
	void Reload();
	void Heal();

public:
	void End_Rolling();
	void End_Hitted();
	void End_Healing();
	void End_Reload();

};
