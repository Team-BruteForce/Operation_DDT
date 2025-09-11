// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStatusComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetNowHp() { return NowHp; }
	FORCEINLINE float GetMaxHP() { return MaxHp; }
	FORCEINLINE void GetDamage(float value) { NowHp = FMath::Clamp(NowHp - value, 0.0f, MaxHp); }
	FORCEINLINE void GetHeal(float value) { NowHp = FMath::Clamp(NowHp + value, 0.0f, MaxHp); }
	
	// HP를 최대치로 복구하는 함수
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetFullHealth() { NowHp = MaxHp; }

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHp = 100.f;
private:
	UPROPERTY(VisibleAnywhere, Category = "Health")
	float NowHp = MaxHp;

		
};
