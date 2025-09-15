// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ActorComponent.h"
#include "FlyingComponent.generated.h"

/**
 * @brief 보스 비행 컴포넌트
 * 
 * 보스 캐릭터의 비행 기능을 담당합니다.
 * 이륙/착륙, 고도 유지, 스플라인 기반 궤도 이동을 제공합니다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OPERRATION_DDT_API UFlyingComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UFlyingComponent(); 

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ===== 비행 제어 함수들 =====
	
	/**
	 * @brief 이륙 시작
	 * @param TargetHeight 목표 고도
	 * @param Speed 이륙 속도
	 */
	UFUNCTION(BlueprintCallable, Category = "Flying")
	void StartTakeoff(float TargetHeight = 300.0f, float Speed = 300.0f);
	
	/**
	 * @brief 착륙 시작
	 * @param LandingLocation 착륙할 위치 (기본값: 현재 위치 아래)
	 */
	UFUNCTION(BlueprintCallable, Category = "Flying")
	void StartLanding(FVector LandingLocation = FVector::ZeroVector);
	
	/**
	 * @brief 비행 중지
	 */
	UFUNCTION(BlueprintCallable, Category = "Flying")
	void StopFlying();

	// ===== 상태 확인 함수들 =====
	
	/**
	 * @brief 현재 비행 중인지 확인
	 */
	UFUNCTION(BlueprintPure, Category = "Flying")
	bool IsFlying() const { return bIsFlying; }
	
	/**
	 * @brief 현재 이륙 중인지 확인
	 */
	UFUNCTION(BlueprintPure, Category = "Flying")
	bool IsTakingOff() const { return bIsTakingOff; }
	
	/**
	 * @brief 현재 착륙 중인지 확인
	 */
	UFUNCTION(BlueprintPure, Category = "Flying")
	bool IsLanding() const { return bIsLanding; }

	// ===== 스플라인 관련 함수들 =====
	
	/**
	 * @brief 스플라인 액터 스폰
	 * @param SpawnLocation 스폰할 위치 (기본값: 0,0,0)
	 * @return 스폰된 스플라인 액터
	 */
	UFUNCTION(BlueprintCallable, Category = "Spline")
	class AFlySpline* SpawnSplineActor(FVector SpawnLocation = FVector::ZeroVector);
	
	/**
	 * @brief 스폰된 스플라인 액터 가져오기
	 * @return 스폰된 스플라인 액터
	 */
	UFUNCTION(BlueprintPure, Category = "Spline")
	class AFlySpline* GetSpawnedSplineActor() const { return SpawnedSplineActor; }
	
	/**
	 * @brief 가장 높은 스플라인 찾기
	 * @return 가장 높은 스플라인 컴포넌트
	 */
	UFUNCTION(BlueprintCallable, Category = "Spline")
	class USplineComponent* FindHighestSpline() const;
	
	/**
	 * @brief 스플라인 궤도 이동 시작
	 * @param TargetSpline 궤도 이동할 스플라인
	 */
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void StartSplineOrbit();
	
	/**
	 * @brief 스플라인 궤도 이동 중지
	 */
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void StopSplineOrbit();

	// ===== 호버링 관련 함수들 =====
	
	/**
	 * @brief 호버링 모드 시작
	 * @param HoverLocation 호버링할 위치 (기본값: 현재 위치)
	 */
	UFUNCTION(BlueprintCallable, Category = "Hovering")
	void StartHovering(FVector HoverLocation = FVector::ZeroVector);
	
	/**
	 * @brief 호버링 모드 중지
	 */
	UFUNCTION(BlueprintCallable, Category = "Hovering")
	void StopHovering();
	
	/**
	 * @brief 현재 호버링 중인지 확인
	 */
	UFUNCTION(BlueprintPure, Category = "Hovering")
	bool IsHovering() const { return bIsHovering; }
	
	/**
	 * @brief 랜덤 이동 시작 (StateTree용)
	 */
	UFUNCTION(BlueprintCallable, Category = "Hovering")
	void StartRandomMovementFromStateTree();
	
	
	/**
	 * @brief 좌우로 한번 이동 (StateTree용)
	 */
	UFUNCTION(BlueprintCallable, Category = "Hovering")
	void MoveSideways();
	
	/**
	 * @brief 랜덤 이동 완료 이벤트 발생 (StateTree용)
	 */
	UFUNCTION(BlueprintCallable, Category = "Hovering")
	void OnRandomMovementCompleted();
	
	/**
	 * @brief 사이드 이동 완료 이벤트 발생 (StateTree용)
	 */
	UFUNCTION(BlueprintCallable, Category = "Hovering")
	void OnSideMovementCompleted();
	
	/**
	 * @brief 스플라인 반원 이동 시작 (StateTree용)
	 */
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void StartSplineSemicircleMovement();
	
	/**
	 * @brief 스플라인 반원 이동 완료 이벤트 발생 (StateTree용)
	 */
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void OnSplineSemicircleMovementCompleted();
	
	/**
	 * @brief 스플라인 반원 이동 타겟 업데이트 (내부용)
	 */
	void UpdateSplineSemicircleTarget();
	
	/**
	 * @brief 스플라인 반원 이동 상태 완전 초기화 (StateTree용)
	 */
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void ResetSplineSemicircleMovement();
	
	/**
	 * @brief 자동 호버링 로직 끄기/켜기 (StateTree 제어용)
	 */
	UFUNCTION(BlueprintCallable, Category = "Hovering")
	void SetAutoHoveringLogic(bool bEnabled);

	/**
	 * @brief 비행 시스템 완전 초기화 (매니저용)
	 * 
	 * @details
	 * 모든 비행 관련 상태를 초기화하고 착륙 상태로 리셋합니다.
	 * 보스 매니저에서 보스 리셋 시 사용됩니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "Boss Reset")
	void ResetFlyingSystem();
	
	/**
	 * @brief 거리 체크 결과 반환 (StateTree용)
	 */
	UFUNCTION(BlueprintPure, Category = "Hovering")
	bool CheckDistanceConditions() const;
	
	/**
	 * @brief 주기적 이동 시간 체크 결과 반환 (StateTree용)
	 */
	UFUNCTION(BlueprintPure, Category = "Hovering")
	bool CheckPeriodicMovementTime() const;
	
	/**
	 * @brief 랜덤 이동 중인지 확인 (StateTree용)
	 */
	UFUNCTION(BlueprintPure, Category = "Hovering")
	bool IsMovingToRandomPoint() const { return bIsMovingToRandomPoint; }
	
private:

	// ===== 비행 상태 변수들 =====
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flying State")
	bool bIsFlying = false;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flying State")
	bool bIsTakingOff = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flying State")
	bool bIsLanding = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flying State")
	bool bIsOrbiting = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flying State")
	bool bIsHovering = false;

	// ===== 비행 설정값들 =====
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flying Settings")
	float LandingSpeed = 200.0f;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flying Settings")
	float FlyingSpeed = 500.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flying Settings")
	float TakeoffSpeed = 300.0f;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flying Settings")
	float DefaultFlyingHeight = 300.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flying Settings")
	float AltitudeVariationRange = 50.0f;  // 고도 변화 범위
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flying Settings")
	float AltitudeChangeSpeed = 1.0f;  // 고도 변화 속도
	
	// ===== 호버링 설정값들 =====
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Settings")
	float HoveringHeight = 400.0f;  // 호버링 고도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Settings")
	float HoveringSpeed = 50.0f;  // 호버링 위치 이동 속도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Settings")
	float LookDownAngle = -30.0f;  // 플레이어를 내려다보는 각도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Settings")
	float RotationSpeed = 5.0f;  // 회전 속도 (도/초)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Settings")
	float HoveringFloatRange = 30.0f;  // 호버링 위아래 움직임 범위
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Settings")
	float HoveringFloatSpeed = 1.0f;  // 호버링 위아래 움직임 속도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Settings")
	float MinPlayerDistance = 1500.0f;  // 플레이어와의 최소 거리
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Settings")
	float RandomMovementSpeed = 1200.0f;  // 랜덤 이동 속도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Settings")
	float PeriodicMovementMinTime = 5.0f;  // 주기적 이동 최소 시간 (초)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Settings")
	float PeriodicMovementMaxTime = 10.0f;  // 주기적 이동 최대 시간 (초)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Settings")
	float MaxPlayerDistance = 3000.0f;
	
	/**
	 * @brief 좌우 이동 거리
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Side Movement Settings")
	float SideMovementDistance = 500.0f;

	// ===== 궤도 이동 변수들 =====
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	FVector OrbitCenter = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	float OrbitRadius = 200.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	float OrbitSpeed = 1.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Orbit")
	float CurrentOrbitAngle = 0.0f;

	// ===== 스플라인 액터 관련 =====
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline")
	class AFlySpline* SpawnedSplineActor = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
	TSubclassOf<class AFlySpline> SplineActorClass = nullptr;

	// ===== 스플라인 기반 궤도 이동 =====
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Orbit")
	bool bIsOrbitingWithSpline = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Orbit")
	int32 CurrentSplineIndex = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Orbit")
	float SplineProgress = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Orbit")
	float SplineOrbitSpeed = 1.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Orbit")
	FVector CurrentRandomOffset = FVector::ZeroVector;
	
	// ===== 호버링 관련 변수들 =====
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hovering")
	FVector HoveringLocation = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hovering")
	float HoveringProgress = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hovering")
	float HoveringFloatTimer = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hovering")
	bool bIsMovingToRandomPoint = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hovering")
	bool bIsMovingSideways = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hovering")
	FVector RandomTargetLocation = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hovering")
	FVector SideTargetLocation = FVector::ZeroVector;
	
	// 스플라인 반원 이동 관련 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Semicircle")
	bool bIsMovingSplineSemicircle = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Semicircle")
	int32 CurrentSplineSemicircleIndex = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Semicircle")
	int32 TargetSplineSemicircleIndex = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Semicircle")
	FVector CurrentSplineSemicircleTarget = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Semicircle")
	int32 SplineMoveCount = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Semicircle")
	int32 StartSplineSemicircleIndex = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Semicircle")
	float MovedAngle = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bJustStartedMovement = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bWaitingForRandomCompletion = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bWaitingForSideCompletion = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bWaitingForSplineCompletion = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CompletionDelayTimer = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CompletionDelay = 0.5f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hovering")
	float PeriodicMovementTimer = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hovering")
	float NextPeriodicMovementTime = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hovering")
	bool bAutoHoveringLogicEnabled = false;  // 자동 호버링 로직 비활성화 (수동 제어만)
	

	// ===== 고도 관련 변수들 =====
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Altitude")
	float TargetAltitude = 300.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Altitude")
	float CurrentAltitude = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Altitude")
	float AltitudeVariationTimer = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Altitude")
	float AltitudeVariationOffset = 0.0f;

	// ===== 내부 변수들 =====
	
	ACharacter* OwnerCharacter = nullptr;
	UCharacterMovementComponent* CharacterMovement = nullptr;
	FVector StartLocation = FVector::ZeroVector;
	FVector LandingLocation = FVector::ZeroVector;
	float TakeoffProgress = 0.0f;
	float LandingProgress = 0.0f;
	


	// ===== 내부 함수들 =====
	
	/**
	 * @brief 이륙 로직 업데이트
	 */
	void UpdateTakeoff(float DeltaTime);
	
	/**
	 * @brief 착륙 로직 업데이트
	 */
	void UpdateLanding(float DeltaTime);
	
	/**
	 * @brief 궤도 이동 로직 업데이트
	 */
	void UpdateOrbit(float DeltaTime);
	
	/**
	 * @brief 스플라인 궤도 이동 로직 업데이트
	 */
	void UpdateSplineOrbit(float DeltaTime);
	
	/**
	 * @brief 호버링 로직 업데이트
	 */
	void UpdateHovering(float DeltaTime);
	
	/**
	 * @brief 플레이어와의 거리 체크
	 */
	bool IsPlayerTooClose(float MinDistance = 300.0f) const;
	
	/**
	 * @brief 가장 높은 스플라인의 랜덤 포인트 선택
	 */
	FVector GetRandomSplinePoint() const;
	
	/**
	 * @brief 랜덤 이동 시작
	 */
	void StartRandomMovement();
	
	/**
	 * @brief 최대 거리 체크
	 */
	bool IsPlayerTooFar() const;
	
	/**
	 * @brief 자연스러운 고도 변화 업데이트
	 */
	void UpdateAltitudeVariation(float DeltaTime);
	
	/**
	 * @brief 목표 위치로 부드럽게 이동
	 * @param TargetLocation 목표 위치
	 * @param Speed 이동 속도
	 * @param ReachDistance 도달 거리 (기본값: 50.0f)
	 * @return 도달했으면 true, 아니면 false
	 */
	bool MoveToLocation(const FVector& TargetLocation, float Speed, float ReachDistance = 50.0f);
	
	/**
	 * @brief 현재 고도 계산
	 */
	float GetCurrentAltitude() const;
	
	/**
	 * @brief 목표 고도 설정
	 */
	void SetTargetAltitude(float NewAltitude);
	
	/**
	 * @brief 쿨타임 업데이트
	 */
	void UpdateCooldowns(float DeltaTime);
public:
	// ===== 쿨타임 관련 =====
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight Cooldown")
	bool bCanTakeoff = true;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight Cooldown")
	bool bCanLanding = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Cooldown")
	float TakeoffCooldownTime = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Cooldown")
	float LandingCooldownTime = 10.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight Cooldown")
	float TakeoffCooldownTimer = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight Cooldown")
	float LandingCooldownTimer = 0.0f;
};
