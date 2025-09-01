// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Player/CPlayerBullet.h"
#include "CFireComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCFireComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCFireComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	AActor* GetActorAttachedToSocket(const FName& SocketName);
	void SetMuzzleVector(const FName& SocketName);

public:
	void Fire();

private:
	class ADDTPlayer* OwnerCharacter;
	class ACAttachment* Rifle;

	
	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	TSubclassOf<ACPlayerBullet> PlayerBulletClass;

	FVector MuzzleVector;
	FName RifleSocketName;

		
};
