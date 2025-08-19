// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBossWeapon.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FBossAttachmentBeginCollision );
DECLARE_DYNAMIC_MULTICAST_DELEGATE ( FBossAttachmentEndCollision );

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams ( FBossAttachmentBeginOverlap , class ACharacter* , InAttacker , AActor* , InAttackCuaser , class ACharacter* , InOther );
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams ( FBossAttachmentEndOverlap , class ACharacter* , InAttacker , class ACharacter* , InOther );
UCLASS()
class OPERRATION_DDT_API ACBossWeapon : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly,VisibleAnywhere)
	class USceneComponent* Root;

public:
	ACBossWeapon();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintNativeEvent)
	void OnBossBeginEquip();
	virtual void OnBossBeginEquip_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
	void OnBossUnequip();
	virtual void OnBossUnequip_Implementation() {}


protected:
	UFUNCTION(BlueprintCallable,Category="Attach")
	void BossAttachTo(FName InSocketName);


public:
	void OnBossCollisions ( );
	void OffBossCollisions ( );
private:
	UFUNCTION()
	void OnBossComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBossComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
public:
	FBossAttachmentBeginCollision OnBossAttachmentBeginCollision;
	FBossAttachmentEndCollision OnBossAttachmentEndCollision;

	FBossAttachmentBeginOverlap OnBossAttachmentBeginOverlap;
	FBossAttachmentEndOverlap OnBossAttachmentEndOverlap;
protected:
	UPROPERTY ( BlueprintReadOnly , Category = "Game" )
	TArray<class UShapeComponent*>Collisions;

	UPROPERTY ( BlueprintReadOnly , Category = "Game" )
	class ACharacter* OwnerCharacter;
	
	// UPROPERTY ( BlueprintReadOnly , Category = "Game" )
	// class UCStatusComponent* Status;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* ColorMesh;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UNiagaraSystem* HitEffect;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UNiagaraSystem* HitEffect2;

	UFUNCTION(BlueprintCallable, Category = "Attach")
	void BossAttachToCollision(FName InCollisionName);

// private:
// 	bool bCollisionTraceEnabled = false;
//
// 	FVector PrevStartLocation;
// 	FVector PrevEndLocation;
// 	FVector CurrentStartLocation;
// 	FVector CurrentEndLocation;
//
//
// 	TArray<FHitResult> HitResults;
// 	void PerformTriangleTrace(
// 		const FVector& A,
// 		const FVector& B,
// 		const FVector& C,
// 		TArray<FHitResult>& OutHits);
// 	FVector BezierCurve(const FVector& P0, const FVector& P1, const FVector& P2, float T);
// 	bool bInitialized = false;
//
// 	class AActor* EnemyActor;
//
// 	FTimerHandle ScaleTimerHandle;
// 	float ScaleTime;
// 	UPROPERTY() // UPROPERTY 매크로를 붙여 GC가 관리하도록 합니다.
// 	TSet<AActor*> HitActorsThisSwing; 
//
// 	// 베지어 곡선 접선 계산 함수 (필요 시 선언)
// 	FVector GetBezierCurveTangent(const FVector& P0, const FVector& P1, const FVector& P2, float T);
//
// 	void ResetTimeDilation();
//
// 	void OnHitEvent();
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FRotator HorizontalRotation;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FRotator VerticalRotation;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<USoundCue*> HitSoundCues;
};
