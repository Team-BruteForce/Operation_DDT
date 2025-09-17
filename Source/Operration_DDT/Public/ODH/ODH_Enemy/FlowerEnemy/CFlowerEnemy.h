// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ODH/ODH_Enemy/Interface/IDamageable.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h"
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

	// 근접 공격 시 메쉬 상대 이동(연출) 설정 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|VisualMove")
	float MeleeVisualMaxDistance = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|VisualMove")
	float MeleeVisualOutTime = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|VisualMove")
	float MeleeVisualBackTime = 0.25f;

	// 0이면 Z를 고정하고, 양수면 Z 오프셋을 그 값으로 제한
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|VisualMove")
	float MeleeVisualMaxZOffset = 0.0f;

	// 공격/사망 상태 플래그
public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool GetIsRangedAttacking() const { return bIsRangedAttacking; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool GetIsComboAttacking() const { return bIsComboAttacking; }

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool GetIsDashAttacking() const { return bIsDashAttacking; }

	// 대쉬 공격 전환 트리거(거리 임계 도달 표시)
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool GetDashAttackTrigger() const { return bDashAttackTrigger; }

	// 대쉬 러닝 상태(달리기 애니메이션용)
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "State")
	bool GetIsDashRunning() const { return bIsDashRunning; }

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
	void UpdateDashMovementToPlayer(float DeltaTime, float Speed);

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

	// 대쉬 러닝 상태 및 이동 속도 제어
	bool bIsDashRunning = false;
	float PrevMaxWalkSpeed = -1.0f;

public:
	// 대쉬 러닝 설정값 (블루프린트에서 조정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash|Run")
	float DashRunSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dash|Run")
	float DashStopDistance = 70.0f;

	// 대쉬 공격 전환 신호 (거리 임계 도달 시 true)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dash|Run")
	bool bDashAttackTrigger = false;

	// 대쉬 공격 1회 타격 보장 가드
	bool bDashHasHitOnce = false;

private:
	// ===== 메쉬 상대 이동 근접 연출 상태 =====
	bool bIsMeleeVisualMoving = false;
	bool bMeleeVisualGoingOut = false;
	float MeleeVisualElapsed = 0.0f;
	FVector MeshStartRelativeLocation = FVector::ZeroVector;
	FVector MeshTargetRelativeLocation = FVector::ZeroVector;

	void StartMeleeVisualMove(AActor* TargetActor);
	void UpdateMeleeVisualMove(float DeltaTime);
	void EndMeleeVisualMove(bool bSnapToStart);

public:
	// 에디터에서 조정 가능한 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health")
	float MaxHP = 1130.0f;

public:
	// 떨어지는 연출 관련 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Death Animation")
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Animation")
	float FallDuration = 2.0f; // 떨어지는 시간 (초)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Animation")
	float FallEndHeight = -50.0f; // 떨어지기 끝나는 높이

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Animation")
	UCurveFloat* FallCurve = nullptr; // 떨어지는 곡선 (선택사항)

	// 회전 연출: 끝 회전값 (기본: Pitch=0, Yaw=180, Roll=0)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Animation")
	FRotator FallEndRotation = FRotator(0.0f, 180.0f, 0.0f);

private:
	// 떨어지는 연출 함수
	UFUNCTION()
	void StartFallingAnimation();

	UFUNCTION()
	void UpdateFallingAnimation(float DeltaTime);

	UFUNCTION()
	void OnFallTimelineUpdate(float Value);

	UFUNCTION()
	void OnFallTimelineFinished();

	// 떨어지는 연출용 변수들
	float FallStartTime = 0.0f;
	FVector OriginalMeshLocation;
	FRotator FallStartRotation;
	float FallStartZ = 0.0f;
	FTimeline FallTimeline;
};
