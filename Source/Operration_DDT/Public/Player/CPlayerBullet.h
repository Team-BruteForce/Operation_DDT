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

	// 본별 데미지 배율 반환 함수
	UFUNCTION(BlueprintCallable, Category = "Damage")
	float GetDamageMultiplierForBone(const FName& BoneName) const;

	// 타이머가 활성화되어 있는지 확인하는 함수
	UFUNCTION(BlueprintCallable, Category = "Timer")
	bool IsLifeTimerActive() const;

	class ADDTPlayer* OwnerCharacter;

	UFUNCTION()
	void OnBulletHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
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
	
	// 총알 상태 초기화 함수
	void ResetBulletState();

	// 오브젝트 풀 델리게이트
	UPROPERTY(BlueprintAssignable)
	FOnReturnToPool OnReturnToPool;

	FORCEINLINE bool GetIsInUse() { return bIsInUse; }
	FORCEINLINE void SetIsInUse(bool bValue) { bIsInUse = bValue; }

private:
	bool bIsActive = false;
	bool bIsInUse = false;  // 사용 중 플래그 추가
	FTimerHandle LifeTimerHandle;

	

};
