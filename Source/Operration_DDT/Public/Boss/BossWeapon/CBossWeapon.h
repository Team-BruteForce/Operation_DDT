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
	virtual void Tick(float DeltaTime) override;

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
	void OnSelectCollision(FName CollisionName);
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
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* HitEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UNiagaraSystem* HitEffect2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> BossSkillCollisionClass;
public:
	UPROPERTY()
	class ABossCollision* BossSkillCollision; 
	
	UFUNCTION(BlueprintCallable, Category = "Attach")
	void BossAttachToCollision(FName InCollisionName, FName InSocketName);
public:
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void StartCollisionAtSocket(FName InSocketName,bool IsMove);
	
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void EndCollisionToOwner(bool IsEndMove);

};
