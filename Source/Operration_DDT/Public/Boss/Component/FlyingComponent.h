// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/ActorComponent.h"
#include "FlyingComponent.generated.h"

/**
 * @brief 비행 상태 머신
 */
UENUM(BlueprintType)
enum class EFlyingState : uint8
{
	Idle,      // 대기 상태
	Turn,      // 방향 회전 중
	Fly,       // 이동 중 (FlyStart + FlyMove 통합)
	FlyStop    // 정지 애니메이션 중
};

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
	
	/**
	 * @brief 이륙 가능한지 확인 (쿨타임 체크)
	 */
	UFUNCTION(BlueprintPure, Category = "Flying")
	bool CanTakeoff() const { return bCanTakeoff; }
	
	/**
	 * @brief 착륙 가능한지 확인 (쿨타임 체크)
	 */
	UFUNCTION(BlueprintPure, Category = "Flying")
	bool CanLanding() const { return bCanLanding; }
	
	/**
	 * @brief 이륙이 완료되었는지 확인 (몽타주와 동기화용)
	 */
	UFUNCTION(BlueprintPure, Category = "Flying")
	bool IsTakeoffCompleted() const { return !bIsTakingOff && bIsFlying; }
	
	/**
	 * @brief 이륙 완료까지 대기 후 호버링 시작 (몽타주와 동기화용)
	 * @param HoverLocation 호버링할 위치 (기본값: 현재 위치)
	 */
	UFUNCTION(BlueprintCallable, Category = "Hovering")
	void StartHoveringAfterTakeoff(FVector HoverLocation = FVector::ZeroVector);

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
	 * @brief 스플라인 반원 이동 시작 (StateTree용)
	 */
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void StartSplineSemicircleMovement();
	
	/**
	 * @brief 스플라인 반원 이동 상태 완전 초기화 (StateTree용)
	 */
	UFUNCTION(BlueprintCallable, Category = "Spline")
	void ResetSplineSemicircleMovement();
	
	/**
	 * @brief 랜덤 이동 중인지 확인 (StateTree용)
	 */
	UFUNCTION(BlueprintPure, Category = "Hovering")
	bool IsMovingToRandomPoint() const { return bIsMovingToRandomPoint; }
	
	/**
	 * @brief 거리 유지 이동 시작 (StateTree용)
	 * @param TargetDistance 목표 거리 (기본값: 1500)
	 */
	UFUNCTION(BlueprintCallable, Category = "Distance Maintenance")
	void StartDistanceMaintenance(float TargetDistance = 1500.0f);
	
	/**
	 * @brief 거리 유지 이동 중지 (StateTree용)
	 */
	UFUNCTION(BlueprintCallable, Category = "Distance Maintenance")
	void StopDistanceMaintenance();
	
	/**
	 * @brief 거리 유지 이동 중인지 확인 (StateTree용)
	 */
	UFUNCTION(BlueprintPure, Category = "Distance Maintenance")
	bool IsDistanceMaintaining() const { return bIsDistanceMaintaining; }
	
	/**
	 * @brief 가장 먼 스플라인으로 이동 시작 (StateTree용)
	 * @param TargetDistance 목표 거리 (기본값: 1500)
	 */
	UFUNCTION(BlueprintCallable, Category = "Spline Distance")
	void StartSplineDistanceMaintenance(float TargetDistance = 1500.0f);
	
	/**
	 * @brief 가장 먼 스플라인으로 이동 중지 (StateTree용)
	 */
	UFUNCTION(BlueprintCallable, Category = "Spline Distance")
	void StopSplineDistanceMaintenance();
	
	/**
	 * @brief 가장 먼 스플라인으로 이동 중인지 확인 (StateTree용)
	 */
	UFUNCTION(BlueprintPure, Category = "Spline Distance")
	bool IsSplineDistanceMaintaining() const { return bIsSplineDistanceMaintaining; }
	

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
	 * @brief 베지어 곡선 계산 (3점 기준)
	 * @param P0 시작점
	 * @param P1 제어점 (중간점)
	 * @param P2 끝점
	 * @param t 진행도 (0.0 ~ 1.0)
	 * @return 곡선 상의 위치
	 */
	UFUNCTION(BlueprintPure, Category = "Movement")
	FVector CalculateBezierPoint(const FVector& P0, const FVector& P1, const FVector& P2, float t) const;
	
	/**
	 * @brief 이징 함수 - 천천히 시작해서 빨라졌다가 천천히 끝남 (EaseInOut)
	 * @param t 진행도 (0.0 ~ 1.0)
	 * @return 속도 배수 (0.0 ~ 1.0+)
	 */
	UFUNCTION(BlueprintPure, Category = "Movement")
	float CalculateEaseInOutMultiplier(float t) const;
	
	/**
	 * @brief 호버링 중 환경 분석 디버그 (플레이어 기준 1000 반지름 원 체크)
	 * @details 플레이어를 중심으로 1000 반지름의 원을 그리고, 라인트레이스로 갈 수 있는 곳인지 체크
	 */
	UFUNCTION(BlueprintCallable, Category = "Hovering Debug")
	void DebugHoveringEnvironmentAnalysis();
	
	/**
	 * @brief 간단한 라인트레이스 테스트 (디버그용)
	 * @details 보스에서 플레이어로 직접 라인트레이스해서 충돌 체크
	 */
	UFUNCTION(BlueprintCallable, Category = "Hovering Debug")
	void DebugSimpleLineTraceTest();
	
	// ===== 상태 머신 관련 함수들 =====
	
	
	/**
	 * @brief 상태 전환
	 * @param NewState 새로운 상태
	 */
	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void ChangeState(EFlyingState NewState);
	
	/**
	 * @brief 상태 머신 업데이트
	 * @param DeltaTime 델타 타임
	 */
	UFUNCTION(BlueprintCallable, Category = "State Machine")
	void UpdateStateMachine(float DeltaTime);
	
	/**
	 * @brief Turn 상태 업데이트
	 * @param DeltaTime 델타 타임
	 */
	void UpdateTurnState(float DeltaTime);
	
	/**
	 * @brief Fly 상태 업데이트
	 * @param DeltaTime 델타 타임
	 */
	void UpdateFlyState(float DeltaTime);
	
	/**
	 * @brief FlyStop 상태 업데이트
	 * @param DeltaTime 델타 타임
	 */
	void UpdateFlyStopState(float DeltaTime);
	
	/**
	 * @brief 스플라인 Turn 상태 업데이트
	 * @param DeltaTime 델타 타임
	 */
	void UpdateSplineTurnState(float DeltaTime);
	
	/**
	 * @brief 스플라인 Fly 상태 업데이트
	 * @param DeltaTime 델타 타임
	 */
	void UpdateSplineFlyState(float DeltaTime);
	
	/**
	 * @brief 스플라인 FlyStop 상태 업데이트
	 * @param DeltaTime 델타 타임
	 */
	void UpdateSplineFlyStopState(float DeltaTime);
	
	
	
private:

	// ===== 비행 상태 변수들 =====
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flying State")
	bool bIsFlying = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flying State")
	EFlyingState CurrentFlyingState = EFlyingState::Idle;
	
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
	float HoveringSpeed = 50.0f;  // 호버링 위치 이동 속도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Settings")
	float LookDownAngle = -30.0f;  // 플레이어를 내려다보는 각도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Settings")
	float PeriodicMovementMinTime = 5.0f;  // 주기적 이동 최소 시간 (초)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Settings")
	float PeriodicMovementMaxTime = 10.0f;  // 주기적 이동 최대 시간 (초)
	

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
	FVector RandomTargetLocation = FVector::ZeroVector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hovering")
	float PeriodicMovementTimer = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hovering")
	float NextPeriodicMovementTime = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hovering")
	bool bWaitingForTakeoffCompletion = false;  // 이륙 완료 대기 중
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hovering")
	FVector PendingHoverLocation = FVector::ZeroVector;  // 대기 중인 호버링 위치
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Distance Maintenance")
	bool bIsDistanceMaintaining = false;  // 거리 유지 중
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Distance Maintenance")
	float TargetDistanceFromPlayer = 1500.0f;  // 플레이어로부터 목표 거리
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Distance Maintenance")
	FVector DistanceMaintenanceTarget = FVector::ZeroVector;  // 거리 유지 목표 위치
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Distance Maintenance")
	float FlyStopEventTimer = 0.0f;  // FlyStop 상태에서 이벤트 전송 타이머
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Distance Maintenance")
	bool bFlyStopEventSent = false;  // FlyStop 이벤트 전송 여부
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Distance Maintenance")
	bool bFlyStopTimerStarted = false;  // FlyStop 타이머 시작 여부
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Distance")
	bool bIsSplineDistanceMaintaining = false;  // 스플라인 거리 유지 중
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spline Distance")
	FVector SplineDistanceMaintenanceTarget = FVector::ZeroVector;  // 스플라인 거리 유지 목표 위치
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Natural Movement")
	float AccelerationRate = 500.0f;  // 가속률
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Natural Movement")
	float DecelerationRate = 500.0f;  // 감속률
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Natural Movement")
	float CurrentSpeed = 0.0f;  // 현재 속도
	
	
	
	

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

	// ===== 자연스러운 이동 시스템 변수들 =====
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Natural Movement")
	FVector CurrentVelocity = FVector::ZeroVector;  // 현재 속도 벡터
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Natural Movement")
	FVector TargetVelocity = FVector::ZeroVector;  // 목표 속도 벡터
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Natural Movement")
	FVector CurrentDirection = FVector::ForwardVector;  // 현재 이동 방향
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Natural Movement")
	FVector TargetDirection = FVector::ForwardVector;  // 목표 이동 방향
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Natural Movement")
	float RotationSpeed = 180.0f;  // 회전 속도 (도/초)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Natural Movement")
	float InertiaFactor = 0.8f;  // 관성 계수 (0.0~1.0)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Natural Movement")
	float MinSpeed = 50.0f;  // 최소 이동 속도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Natural Movement")
	float MaxSpeed = 1000.0f;  // 최대 이동 속도
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Natural Movement")
	bool bUseCurvedPath = true;  // 곡선 경로 사용 여부
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Natural Movement")
	float CurveIntensity = 0.3f;  // 곡선 강도 (0.0~1.0)
	


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
	 * @brief 자연스러운 고도 변화 업데이트
	 */
	void UpdateAltitudeVariation(float DeltaTime);
	
	/**
	 * @brief 목표 위치로 부드럽게 이동 (기존 방식)
	 * @param TargetLocation 목표 위치
	 * @param Speed 이동 속도
	 * @param ReachDistance 도달 거리 (기본값: 50.0f)
	 * @return 도달했으면 true, 아니면 false
	 */
	bool MoveToLocation(const FVector& TargetLocation, float Speed, float ReachDistance = 50.0f);
	
	/**
	 * @brief 자연스러운 이동 시스템으로 목표 위치로 이동
	 * @param TargetLocation 목표 위치
	 * @param Speed 목표 속도
	 * @param ReachDistance 도달 거리 (기본값: 50.0f)
	 * @return 도달했으면 true, 아니면 false
	 */
	bool MoveToLocationNatural(const FVector& TargetLocation, float Speed, float ReachDistance = 50.0f);
	
	/**
	 * @brief 자연스러운 이동 시스템 업데이트
	 * @param DeltaTime 델타 타임
	 */
	void UpdateNaturalMovement(float DeltaTime);
	
	/**
	 * @brief 거리 기반 속도 계산
	 * @param Distance 목표까지의 거리
	 * @param MaxSpeed 최대 속도
	 * @return 계산된 속도
	 */
	float CalculateDistanceBasedSpeed(float Distance, float MaxSpeed) const;
	
	/**
	 * @brief 곡선 경로 계산
	 * @param Start 시작점
	 * @param End 끝점
	 * @param CurrentDirection 현재 방향
	 * @return 곡선 경로의 제어점
	 */
	
	/**
	 * @brief 플레이어 정면 60도 내 위치 찾기
	 * @param TargetDistance 목표 거리
	 * @return 찾은 위치 (없으면 FVector::ZeroVector)
	 */
	FVector FindPositionInPlayerView(float TargetDistance) const;
	
	/**
	 * @brief 플레이어 주변 360도 랜덤 위치 찾기
	 * @param TargetDistance 목표 거리
	 * @return 랜덤 위치
	 */
	FVector FindRandomPositionAroundPlayer(float TargetDistance) const;
	
	/**
	 * @brief 거리 유지 이동 업데이트
	 * @param DeltaTime 델타 타임
	 */
	void UpdateDistanceMaintenance(float DeltaTime);
	
	/**
	 * @brief 스플라인 거리 유지 이동 업데이트
	 * @param DeltaTime 델타 타임
	 */
	void UpdateSplineDistanceMaintenance(float DeltaTime);
	
	/**
	 * @brief 가장 먼 스플라인 위치 찾기
	 * @param TargetDistance 목표 거리
	 * @return 가장 먼 스플라인 위치
	 */
	FVector FindFarthestSplinePosition(float TargetDistance) const;
	
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
	float TakeoffCooldownTime = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight Cooldown")
	float LandingCooldownTime = 8.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight Cooldown")
	float TakeoffCooldownTimer = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Flight Cooldown")
	float LandingCooldownTimer = 0.0f;
	
	// ===== 호버링 환경 분석 디버그 설정 =====
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Debug")
	bool bEnableHoveringDebug = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Debug")
	float EnvironmentAnalysisRadius = 1500.0f;  // 분석할 원의 반지름
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Debug")
	int32 EnvironmentAnalysisPoints = 16;  // 원주상의 체크 포인트 수
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hovering Debug")
	float LineTraceDistance = 2000.0f;  // 라인트레이스 거리
	
};
