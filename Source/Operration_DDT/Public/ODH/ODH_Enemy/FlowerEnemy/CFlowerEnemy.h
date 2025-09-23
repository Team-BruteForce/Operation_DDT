// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ODH/ODH_Enemy/Interface/IDamageable.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h"
#include "ODH/Component/CSoundCollectionComponent.h"
#include "GenericTeamAgentInterface.h"
#include "Components/TimelineComponent.h"
#include "Components/ArrowComponent.h"

// 전방 선언: 콜리전 컴포넌트
class UBoxComponent;
class USphereComponent;

#include "CFlowerEnemy.generated.h"

UCLASS()
class OPERRATION_DDT_API ACFlowerEnemy : public ACharacter, public IDamageable, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	ACFlowerEnemy();

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
	void PlayRangedAttack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void PlayComboAttack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void PlayDashAttack();

	// 블랙보드 업데이트 함수들 (애니메이션 노티파이용)
	UFUNCTION(BlueprintCallable, Category = "AI")
	void NotifyComboAttackCompleted();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void NotifyDashAttackCompleted();

	UFUNCTION(BlueprintCallable, Category = "AI")
	void NotifyRangedAttackCompleted();

	// 머리 상태 변경 함수들
	UFUNCTION(BlueprintCallable, Category = "State")
	void SetHeadOpen(bool bOpen);

	UFUNCTION(BlueprintCallable, Category = "State")
	void OpenHeadWithTimer();

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
		// 그로기/피격 상태
		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Groggy")
		int32 GroggyGage = 0;

		UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Groggy")
		bool bIsHitState = false;

		uint8 SavedMovementMode = 0;
		uint8 SavedCustomMovementMode = 0;

		UFUNCTION(BlueprintCallable, Category = "Groggy")
		void EndHitState();
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool GetIsRangedAttacking() const { return bIsRangedAttacking; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool GetIsComboAttacking() const { return bIsComboAttacking; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool GetIsDashAttacking() const { return bIsDashAttacking; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool GetIsHeadOpen() const { return bIsHeadOpen; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCEnemyStatusComponent* StatusComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCEnemyMeleeAttackComponent* MeleeAttackComponent;

	// 애로우 컴포넌트 (원거리 공격 발사 위치)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* ProjectileSpawnArrow;

	// 소켓 기반 데미지 TakeDamage 콜리전 멤버 제거됨

	// 공격 콜리전들 (HandL/HandR Attack Sockets)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack Collisions")
	UBoxComponent* HandLAttackCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack Collisions")
	UBoxComponent* HandRAttackCollision = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack Collisions")
	UBoxComponent* DashAttackCollision = nullptr;

private:
	// 이벤트 핸들러들
	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void OnMeleeAttackHit(AActor* HitActor);


	// 프로젝타일 클래스 가져오기
	TSubclassOf<AActor> GetProjectileClass() const;

	// 소켓 기반 데미지 콜리전 생성 함수
	void CreateDamageCollisions();

	// 소켓 기반 데미지 콜리전 이벤트 바인딩 함수
	void BindDamageCollisionEvents();

	// 소켓 기반 데미지 콜리전 비활성화 함수
	void DisableDamageCollisions();

	// 소켓 기반 데미지 콜리전 오버랩 이벤트 핸들러
	UFUNCTION()
	void OnDamageCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 공격 콜리전 오버랩 이벤트 핸들러
	UFUNCTION()
	void OnAttackCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 머리 자동 닫기 함수 (타이머용)
	UFUNCTION()
	void CloseHeadAutomatically();

	// 머리 자동 열기 함수 (랜덤 타이머용)
	UFUNCTION()
	void OpenHeadAutomatically();

	// 헤드 상태에 따른 콜리전 토글
	void UpdateHeadCollisions();

	// 자동 머리 열기 시스템 시작
	void StartAutoHeadOpening();

public:
	// 돌진 공격 이동 함수들 (애니메이션 노티파이 스테이트용)
	UFUNCTION(BlueprintCallable, Category = "Dash Movement")
	void StartDashMovementToPlayer();

	UFUNCTION(BlueprintCallable, Category = "Dash Movement")
	void UpdateDashMovementToPlayer(float DeltaTime, float TotalTime);

	UFUNCTION(BlueprintCallable, Category = "Dash Movement")
	void EndDashMovementToPlayer();


	// 공격 콜리전 활성화/비활성화 함수들 (애니메이션 노티파이용)
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void EnableHandLAttackCollision();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void DisableHandLAttackCollision();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void EnableHandRAttackCollision();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void DisableHandRAttackCollision();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void EnableDashAttackCollision();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void DisableDashAttackCollision();

private:
	// 사망 타이머 핸들
	FTimerHandle DeathTimerHandle;


	// 머리 열림 상태 타이머 핸들
	FTimerHandle HeadCloseTimerHandle;
	FTimerHandle HeadAutoOpenTimerHandle;

	// 내부 상태 플래그
	bool bIsRangedAttacking = false;
	bool bIsComboAttacking = false;
	bool bIsDashAttacking = false;

	// 머리 상태 플래그
	bool bIsHeadOpen = false;

	// 돌진 공격 이동 관련 변수들
	FVector CachedPlayerLocation = FVector::ZeroVector;
	bool bIsDashMoving = false;
	
	// 시간 기반 이동을 위한 변수들
	FVector DashStartLocation = FVector::ZeroVector;
	float DashElapsedTime = 0.0f;
	float DashTotalTime = 0.0f;

	// 공격별 1회 타격 보장 가드
	bool bHandLHasHit = false;
	bool bHandRHasHit = false;
	bool bDashHasHit = false;


public:
	// 에디터에서 조정 가능한 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health")
	float MaxHP = 1130.0f;

public:
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

	// 사운드 상태 업데이트 함수
	UFUNCTION()
	void UpdateSoundState();

	// 공격 완료 콜백 함수
	UFUNCTION()
	void OnAttackCompleted();
};
