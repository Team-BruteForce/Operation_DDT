// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DamageEvents.h"
#include "Engine/Engine.h"
#include "CPlayerBullet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnReturnToPool, class ACPlayerBullet*, Bullet);

UCLASS()
class OPERRATION_DDT_API ACPlayerBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACPlayerBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	class USceneComponent* Root;
	
	UPROPERTY(EditDefaultsOnly, Category = "Default")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Default")
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	class UProjectileMovementComponent* Movement;

	UPROPERTY(EditDefaultsOnly, Category = "VFX")
	class UNiagaraSystem* BulletTrailSystem;

	UPROPERTY(visibleAnywhere)
	class UNiagaraComponent* BulletTrailComp;
	

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	float LifeTime = 5.f;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	float Speed = 5000.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Default")
	float BulletDamage = 25.f;

	void SetVelocity(FVector value);
	
	// 랜덤 데미지 설정 함수
	UFUNCTION(BlueprintCallable, Category = "Damage")
	void SetRandomDamage();

	class ADDTPlayer* OwnerCharacter;

	UFUNCTION()
	void OnBulletOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	void SetActive(bool bValue);
	bool IsActive() const;

	// 오브젝트 풀 관련 함수들
	UFUNCTION()
	void StartLifeTimer();
	
	UFUNCTION()
	void StopLifeTimer();
	
	UFUNCTION()
	void OnLifeTimeExpired();
	
	void ReturnToPool();

	// 오브젝트 풀 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnReturnToPool OnReturnToPool;

private:
	bool bIsActive = false;
	FTimerHandle LifeTimerHandle;

};
