// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ODH/ODH_Enemy/Interface/IDamageable.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h"
#include "ODH/Component/CSoundCollectionComponent.h"
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

	// 마지막 콤보 공격 전진 움직임
	UFUNCTION(BlueprintCallable, Category = "Combo Movement")
	void LastComboMovement(float ForwardDistance = 100.0f);

	// 돌진 공격 콜리전 제어
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void EnableDashCollision();

	// 콤보 공격 시 전방 이동 함수 제거 (애니메이션에서 처리)

	// 돌진 공격 이동 함수들 (애니메이션 노티파이 스테이트용)
	UFUNCTION(BlueprintCallable, Category = "Dash Movement")
	void StartDashMovementToPlayer();

	UFUNCTION(BlueprintCallable, Category = "Dash Movement")
	void UpdateDashMovementToPlayer(float DeltaTime, float TotalTime);

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
		// 그로기/피격 상태
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Groggy")
		int32 GroggyGage = 0;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Groggy")
		bool bIsHitState = false;

		// 이동 복구를 위한 저장된 이동 모드
		uint8 SavedMovementMode = 0;
		uint8 SavedCustomMovementMode = 0;

		UFUNCTION(BlueprintCallable, Category = "Groggy")
		void EndHitState();
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
//	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
//	UBoxComponent* WeakPointCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* MeleeAttackCollisionR;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* MeleeAttackCollisionL;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* ComboAttackLastCollision;

	// 바디 데미지용 TakeDamage 콜리전 멤버 제거됨
public:
	// 에디터에서 조정 가능한 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health")
	float MaxHP = 810.0f;

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

	// 바디 피격(타격 가능) 콜리전 오버랩 이벤트 (TakeDamage 라벨용)
	UFUNCTION()
	void OnTakeDamageOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 사망 타이머 핸들
	FTimerHandle DeathTimerHandle;

	// 공격 완료 플래그 해제를 위한 타이머 핸들
	FTimerHandle MeleeAttackTimerHandle;

	// 내부 상태 플래그
	bool bIsComboAttacking = false;
	bool bIsDashAttacking = false;

	// 공격별 1회 타격 보장 가드
	bool bComboRHasHit = false;
	bool bComboLHasHit = false;
	bool bComboLastHasHit = false;
	bool bDashHasHit = false;

	// 돌진 공격 이동 관련 변수들
	FVector CachedPlayerLocation = FVector::ZeroVector;
	bool bIsDashMoving = false;
	
	// 시간 기반 이동을 위한 변수들
	FVector DashStartLocation = FVector::ZeroVector;
	float DashElapsedTime = 0.0f;
	float DashTotalTime = 0.0f;

	// 콤보 공격 이동 관련 변수들
	FVector ComboMovementDirection = FVector::ZeroVector;
	FVector ComboMovementStartLocation = FVector::ZeroVector;
	bool bIsComboMoving = false;
	float ComboMovementProgress = 0.0f;

	// 마지막 콤보 공격 이동 관련 변수들
	FVector LastComboStartLocation = FVector::ZeroVector;
	FVector LastComboTargetLocation = FVector::ZeroVector;
	bool bIsLastComboMoving = false;
	float LastComboMovementSpeed = 800.0f; // 이동 속도 (cm/s)

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
	
	// 블루프린트에서 구현될 흡수 애니메이션 함수
	UFUNCTION(BlueprintImplementableEvent, Category = "Animation")
	void StartAbsorbAnimation();

	// 사운드 관련 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	UCSoundCollectionComponent* SoundCollectionComponent;

	// 사운드 상태 관리 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	bool bIsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	bool bIsDead = false;

	// 이전 상태 저장 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	bool bPreviousIsInCombat = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	bool bPreviousIsAttacking = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	bool bPreviousIsDead = false;

	// 사운드 상태가 이미 설정되었는지 체크하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound")
	bool bSoundStateInitialized = false;

private:
	// 사운드 상태 업데이트 함수
	UFUNCTION()
	void UpdateSoundState();

	// 공격 완료 콜백 함수
	UFUNCTION()
	void OnAttackCompleted();
};
