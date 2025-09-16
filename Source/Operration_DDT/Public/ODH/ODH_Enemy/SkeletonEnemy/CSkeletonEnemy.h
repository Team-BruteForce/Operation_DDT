// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ODH/ODH_Enemy/Interface/IDamageable.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h"
#include "GenericTeamAgentInterface.h"
#include "CSkeletonEnemy.generated.h"

UCLASS()
class OPERRATION_DDT_API ACSkeletonEnemy : public ACharacter, public IDamageable, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACSkeletonEnemy();

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
	void PlayComboAttack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void PlayDashAttack();

	// 콜리전 제어 함수들 (애니메이션 노티파이용)
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void EnableComboCollisions();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void DisableComboCollisions();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void EnableComboRCollision();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void EnableComboLCollision();

	// 모든 콜리전 비활성화 (사망 시 사용)
	UFUNCTION(BlueprintCallable, Category = "Collision")
	void DisableAllCollisions();

	// 콤보 마지막 공격 콜리전 제어
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void EnableLastComboCollision();

	// 돌진 공격 콜리전 제어
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void EnableDashCollision();

	// 콤보 공격 시 전방 이동 함수 제거 (애니메이션에서 처리)

	// 돌진 공격 이동 함수들 (애니메이션 노티파이 스테이트용)
	UFUNCTION(BlueprintCallable, Category = "Dash Movement")
	void StartDashMovementToPlayer();

	UFUNCTION(BlueprintCallable, Category = "Dash Movement")
	void UpdateDashMovementToPlayer(float DeltaTime, float Speed);

	UFUNCTION(BlueprintCallable, Category = "Dash Movement")
	void EndDashMovementToPlayer();

	// 콤보 공격 이동 함수들 (애니메이션 노티파이 스테이트용)
	UFUNCTION(BlueprintCallable, Category = "Combo Movement")
	void StartComboAttackMovement();

	UFUNCTION(BlueprintCallable, Category = "Combo Movement")
	void UpdateComboAttackMovement(float DeltaTime, float Speed, float Distance);

	UFUNCTION(BlueprintCallable, Category = "Combo Movement")
	void EndComboAttackMovement();

	// 블랙보드 업데이트 함수들
	UFUNCTION(BlueprintCallable, Category = "AI")
	void NotifyComboAttackCompleted();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void NotifyDashAttackCompleted();

	// 근접 공격 이동 연출은 애니메이션에서 처리

	// 공격 상태 플래그
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool GetIsComboAttacking() const { return bIsComboAttacking; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool GetIsDashAttacking() const { return bIsDashAttacking; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCEnemyStatusComponent* StatusComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCEnemyMeleeAttackComponent* MeleeAttackComponent;

	// 소켓 부착 콜리전 (Mesh의 자식)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* WeakPointCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* MeleeAttackCollisionR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* MeleeAttackCollisionL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* ComboAttackLastCollision;

	// 바디 대미지 소켓별 콜리전 (L/R)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* UpperLTakeDamageCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* LowerLTakeDamageCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* HandLTakeDamageCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CalfLTakeDamageCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* FootLTakeDamageCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* UpperRTakeDamageCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* LowerRTakeDamageCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* HandRTakeDamageCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* CalfRTakeDamageCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* FootRTakeDamageCollision;

	// 중앙부 추가 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* PelvisTakeDamageCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* BustTakeDamageCollision;

private:
	// 이벤트 핸들러들
	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void OnMeleeAttackHit(AActor* HitActor);

	UFUNCTION()
	void OnMeleeAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWeakPointOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 사망 타이머 핸들
	FTimerHandle DeathTimerHandle;

	// 공격 완료 플래그 해제를 위한 타이머 핸들
	FTimerHandle MeleeAttackTimerHandle;

	// 내부 상태 플래그
	bool bIsComboAttacking = false;
	bool bIsDashAttacking = false;

	// 돌진 공격 이동 관련 변수들
	FVector CachedPlayerLocation = FVector::ZeroVector;
	bool bIsDashMoving = false;

	// 콤보 공격 이동 관련 변수들
	FVector ComboMovementDirection = FVector::ZeroVector;
	FVector ComboMovementStartLocation = FVector::ZeroVector;
	bool bIsComboMoving = false;
	float ComboMovementProgress = 0.0f;

public:
	// 공격 쿨다운 시스템
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackCooldown = 0.5f; // 공격 쿨다운 시간

	// 공격 이동 관련 변수들 제거됨 - 애니메이션에서 처리

private:
	float LastHitTime = 0.0f; // 마지막 히트 시간

	// 이동 연출 관련 상태/함수 제거됨

public:
	// 사망 연출은 애니메이션에서 처리
private:
};
