// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossDissolve.generated.h"

UCLASS()
class OPERRATION_DDT_API ABossDissolve : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossDissolve();


	/**
	 * @brief C++에서 블루프린트의 디졸브 기능을 호출하는 함수
	 * 블루프린트에서 이 이벤트를 구현하면 C++에서 호출 가능
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Dissolve")
	void TriggerDissolveEffect();

	UFUNCTION(BlueprintImplementableEvent, Category = "Dissolve")
	void TriggerDissolveEffectDead();


};
