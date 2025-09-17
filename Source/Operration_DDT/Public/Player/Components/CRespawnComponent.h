// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Engine.h"
#include "CRespawnComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerRespawned);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCRespawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCRespawnComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// DieDelegate 구독 함수
	UFUNCTION()
	void OnPlayerDied();

	// 실제 부활 처리 함수
	void RespawnPlayer();

	// 부활 위치 설정 함수
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void SetRespawnLocation(FVector NewLocation);

	// 부활 대기 시간 설정
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	void SetRespawnDelay(float NewDelay);

	// 부활 위치 가져오기
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	FVector GetRespawnLocation() const { return RespawnLocation; }

	// 부활 대기 시간 가져오기
	UFUNCTION(BlueprintCallable, Category = "Respawn")
	float GetRespawnDelay() const { return RespawnDelay; }

	// 부활 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Respawn")
	FOnPlayerRespawned OnPlayerRespawned;
	
	// 레벨에 배치된 BossManager BP 인스턴스를 드래그로 연결하거나, BeginPlay에서 자동 탐색합니다.
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Ref", meta = (AllowPrivateAccess = "true"))
	class ABossManager* BossManager;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RespawnPoint")
	FVector BossDoorLocation = FVector(-59584.154248,-50296.910881,4538.023350);
	
private:
	class ADDTPlayer* OwnerCharacter;
	class UCStateComponent* State;
	class UCapsuleComponent* Capsule;
	class UCMovementComponent* Movement;
	

	FVector RespawnLocation;
	
	
	// 부활 관련 변수들
	UPROPERTY(EditAnywhere, Category = "Respawn", meta = (AllowPrivateAccess = "true"))
	float RespawnDelay = 3.0f;

	FTimerHandle RespawnTimerHandle;
		
};
