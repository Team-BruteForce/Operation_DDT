// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CDamageUIManageComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCDamageUIManageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCDamageUIManageComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class ADDTPlayer* OwnerCharacter;

	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	int32 MaxPoolSize = 10;

	UPROPERTY(EditAnywhere, Category = "Pool")
	TArray<class UCDamageWidget_Normal*> DamageUIPool;

	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	//TSubclassOf<class UCDamageWidget_Normal> DamageActorClass;
	TSubclassOf<class UUserWidget> DamageWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	int32 CurrentDamageUIPoolIndex = 0;

public:
	UCDamageWidget_Normal* GetInactiveUI();

	UCDamageWidget_Normal* CreateDamageUI();
	
		
};
