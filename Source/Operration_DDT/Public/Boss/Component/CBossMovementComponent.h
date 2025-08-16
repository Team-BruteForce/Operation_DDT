// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBossMovementComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCBossMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCBossMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief 플레이어 방향으로 부드럽게 회전하는 함수
	 * 
	 * @param DeltaTime 델타 타임
	 * @param RotationSpeed 회전 속도 (높을수록 빠름)
	 */
	UFUNCTION(BlueprintCallable, Category="Player Tracking")
	void RotateTowardsPlayer(float DeltaTime, float RotationSpeed = 5.0f);

private:
	/**
	 * @brief 플레이어를 찾는 함수
	 * 
	 * @return 찾은 플레이어 액터, 없으면 nullptr
	 */
	class APawn* FindPlayer();

	UPROPERTY()
	class APawn* Owner;
	
	UPROPERTY()
	class AAIController* AIC;
};
