// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ODH/ODH_Enemy/Interface/IDamageable.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h"
#include "ODH/Component/CEnemyProjectileComp.h"
#include "CSkeletonBodyEnemy.generated.h"

UCLASS()
class OPERRATION_DDT_API ACSkeletonBodyEnemy : public ACharacter, public IDamageable
{
	GENERATED_BODY()

public:
	ACSkeletonBodyEnemy();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IDamageable 인터페이스 구현
	virtual void TakeDamage_Implementation(float DamageAmount) override;
	virtual bool CanTakeDamage_Implementation() const override;
	virtual bool IsDead_Implementation() const override;

	// 공격 함수들
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void PlayMeleeAttack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void PlayRangedAttack();

	// 원거리 공격 오브젝트 소환 함수들
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SpawnRangedProjectile(AActor* TargetPlayer);

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void SpawnRangedProjectileAtLocation(AActor* TargetPlayer, FVector SpawnLocation, FRotator SpawnRotation);

	// 프로젝타일 클래스 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TSubclassOf<AActor> ProjectileClass;

protected:
	// 컴포넌트들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCEnemyStatusComponent* StatusComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCEnemyMeleeAttackComponent* MeleeAttackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCEnemyProjectileComp* EnemyProjectileComponent;

private:
	// 이벤트 핸들러들
	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void OnMeleeAttackHit(AActor* HitActor);

	UFUNCTION()
	void OnProjectileHit(AActor* HitActor);

	// 프로젝타일 클래스 가져오기
	TSubclassOf<AActor> GetProjectileClass() const;
};
