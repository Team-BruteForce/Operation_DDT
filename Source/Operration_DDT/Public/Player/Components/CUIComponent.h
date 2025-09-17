// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CUIComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCUIComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCUIComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> UCPlayerUIWidget;

	UPROPERTY()
	class UCPlayerUI* playerUI;

	void InitUIWidget();

	//델리게이트 핸들러
	UFUNCTION()
	void OnStaminaChanged(float nowStamina, float maxStamina);

	UFUNCTION()
	void OnHealthChanged(float nowHp, float maxHp);


private:
	class ADDTPlayer* OwnerCharater;
	class UCStaminaComponent* StaminaComp;
	class UCStatusComponent* StatusComp;


		
};
