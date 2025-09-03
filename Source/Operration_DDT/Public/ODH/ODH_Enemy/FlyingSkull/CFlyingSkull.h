// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ODH/ODH_Enemy/Interface/IDamageable.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h"
#include "GenericTeamAgentInterface.h"
#include "CFlyingSkull.generated.h"

UCLASS()
class OPERRATION_DDT_API ACFlyingSkull : public ACharacter, public IDamageable, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACFlyingSkull();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IDamageable 인터페이스 구현
	virtual void TakeDamage_Implementation(float DamageAmount) override;
	virtual bool CanTakeDamage_Implementation() const override;
	virtual bool IsDead_Implementation() const override;

	// UGameplayStatics::ApplyDamage를 위한 TakeDamage 오버라이드
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// IGenericTeamAgentInterface 구현
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Team")
	virtual FGenericTeamId GetGenericTeamId() const override;

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

	// 공격/사망 상태 플래그
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool GetIsMeleeAttacking() const { return bIsMeleeAttacking; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool GetIsRangedAttacking() const { return bIsRangedAttacking; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCEnemyStatusComponent* StatusComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCEnemyMeleeAttackComponent* MeleeAttackComponent;

private:
	// 이벤트 핸들러들
	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void OnMeleeAttackHit(AActor* HitActor);

	// 프로젝타일 클래스 가져오기
	TSubclassOf<AActor> GetProjectileClass() const;

	// 사망 타이머 핸들
	FTimerHandle DeathTimerHandle;

	// 공격 완료 플래그 해제를 위한 타이머 핸들
	FTimerHandle MeleeAttackTimerHandle;
	FTimerHandle RangedAttackTimerHandle;

	// 내부 상태 플래그
	bool bIsMeleeAttacking = false;
	bool bIsRangedAttacking = false;
};
