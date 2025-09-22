// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBulletObjectPoolComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCBulletObjectPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCBulletObjectPoolComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	class ADDTPlayer* OwnerCharacter;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	int32 MaxMagazinePool = 10;

	UPROPERTY(EditAnywhere, Category = "Pool")
	TArray<class ACPlayerBullet*> MagazinePool;

	UPROPERTY(EditAnywhere, Category = "Pool")
	TArray<class ACPlayerBullet*> VFXPool;

	UPROPERTY(EditAnywhere, Category = "Pool")
	TArray<class ACNormalDamageUIActor*> DamageUIPool;

	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	int32 CurrentPoolIndex = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	TSubclassOf<ACPlayerBullet> PlayerBulletClass;

	UPROPERTY(EditDefaultsOnly, Category = "Gun")
	TSubclassOf<ACPlayerBullet> PlayerBulletVFXClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Pool")
	TSubclassOf<ACNormalDamageUIActor> DamageActorClass;

public:
	// 오브젝트 풀 관련 함수들
	ACPlayerBullet* CreateBulletForPool();
	ACPlayerBullet* CreateBulletVFXForPool();
	
	ACPlayerBullet* GetInactiveBullet();
	ACPlayerBullet* GetInactiveVFX();
};
