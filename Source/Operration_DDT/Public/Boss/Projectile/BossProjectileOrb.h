// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossProjectileOrb.generated.h"

UCLASS()
class OPERRATION_DDT_API ABossProjectileOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossProjectileOrb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

public:
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile();
	
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void DestroyOrb();

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void OnOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<ABossProjectileActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time")
	float CurrentTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float MaxTime=5.0f;
	
	// 오브젝트 풀 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pool")
	bool bUseObjectPool;

};
