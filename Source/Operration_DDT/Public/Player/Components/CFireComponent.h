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
	AActor* GetActorAttachedToSocket(const FName& SocketName);
protected:
	
	void SetMuzzleVector(const FName& SocketName);

public:
	void Fire();
	

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UNiagaraSystem* MuzzleFireSystem;

	UPROPERTY(visibleAnywhere)
	class UNiagaraComponent* MuzzleFireComp;

private:
	class ADDTPlayer* OwnerCharacter;
	class ACAttachment* Rifle;
	class UCMagazineComponent* MagazineComponent;

	
	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	TSubclassOf<ACPlayerBullet> PlayerBulletClass;

	FVector MuzzleVector;
	FRotator MuzzleRotator;
	FVector MuzzleForwardDirection;
	FName RifleSocketName;

private:
	
	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	int32 MaxMagazinePool = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	TArray<class ACPlayerBullet*> MagazinePool;

	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	int32 CurrentPoolIndex = 0;

	// 라인트레이스 최대 거리
	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float MaxTraceDistance = 10000.0f;

	// 오브젝트 풀 관련 함수들
	ACPlayerBullet* CreateBulletForPool();
	ACPlayerBullet* GetInactiveBullet();
	
	UFUNCTION()
	void ReturnBulletToPool(class ACPlayerBullet* bullet);

	void FindInactiveBullet();

		
};
