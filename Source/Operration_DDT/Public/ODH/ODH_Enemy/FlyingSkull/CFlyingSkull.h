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

	// 근접 공격 시 메쉬 상대 이동(연출) 설정 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|VisualMove")
	float MeleeVisualMaxDistance = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|VisualMove")
	float MeleeVisualOutTime = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|VisualMove")
	float MeleeVisualBackTime = 0.20f;

	// 0이면 Z를 고정하고, 양수면 Z 오프셋을 그 값으로 제한
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Melee|VisualMove")
	float MeleeVisualMaxZOffset = 0.0f;

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

	// 애로우 컴포넌트 (원거리 공격 발사 위치)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UArrowComponent* ProjectileSpawnArrow;

	// 근접 공격 콜리전 (Mesh의 자식)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* MeleeAttackCollision = nullptr;

	// 소켓 기반 데미지 TakeDamage 콜리전 멤버 제거됨

public:
	// 에디터에서 조정 가능한 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats|Health")
	float MaxHP = 320.0f;

private:
	// 이벤트 핸들러들
	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void OnMeleeAttackHit(AActor* HitActor);

	UFUNCTION()
	void OnMeleeAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 소켓 기반 데미지 콜리전 오버랩 이벤트 핸들러
	UFUNCTION()
	void OnDamageCollisionOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 프로젝타일 클래스 가져오기
	TSubclassOf<AActor> GetProjectileClass() const;

	// 소켓 기반 데미지 콜리전 이벤트 바인딩 함수
	void BindDamageCollisionEvents();

	// 소켓 기반 데미지 콜리전 비활성화 함수
	void DisableDamageCollisions();

	// 사망 타이머 핸들
	FTimerHandle DeathTimerHandle;

	// 공격 완료 플래그 해제를 위한 타이머 핸들
	FTimerHandle MeleeAttackTimerHandle;
	FTimerHandle RangedAttackTimerHandle;

	// 내부 상태 플래그
	bool bIsMeleeAttacking = false;
	bool bIsRangedAttacking = false;

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
	// 떨어지는 연출 관련 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Death Animation")
	bool bIsFalling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Animation")
	float FallDuration = 2.0f; // 떨어지는 시간 (초)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Animation")
	float FallEndHeight = -73.0f; // 떨어지기 끝나는 높이

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Animation")
	UCurveFloat* FallCurve = nullptr; // 떨어지는 곡선 (선택사항)

	// 회전 연출: 끝 회전값 (기본: Pitch=0, Yaw=370, Roll=285)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Death Animation")
	FRotator FallEndRotation = FRotator(0.0f, 370.0f, 285.0f);

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
