// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @brief 보스 이동 관리 컴포넌트 클래스
 * 
 * 보스 캐릭터의 이동 로직을 관리하는 컴포넌트입니다.
 * 플레이어 추적, 회전, 이동 패턴 등을 처리합니다.
 */

#pragma once

#include "CoreMinimal.h"
#include "Boss/Data/BossTagStructure.h"
#include "Components/ActorComponent.h"
#include "CBossMovementComponent.generated.h"

/**
 * @file CBossMovementComponent.h
 * @brief 보스 이동 관리 컴포넌트 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 보스 캐릭터의 이동 로직을 관리하는 컴포넌트를 정의합니다.
 * 플레이어 추적, 부드러운 회전, 이동 패턴 등을 처리합니다.
 * 
 * @section architecture 아키텍처
 * - UCBossMovementComponent: 보스 이동 관리 메인 컴포넌트
 * - 플레이어 추적 시스템
 * - 백스탭 위치 계산 시스템
 * - 거리 유지 시스템
 * 
 * @section features 주요 기능
 * - 플레이어 방향으로 부드러운 회전
 * - 백스탭 위치 자동 계산
 * - Nav Mesh 기반 안전 위치 검색
 * - 거리 기반 이동 제어
 * - 디버그 시각화
 * 
 * @section movement_system 이동 시스템
 * - RotateTowardsPlayer: 플레이어 방향 회전
 * - FindBackstepPosition: 백스탭 위치 계산
 * - ExecuteSmartMovement: 거리 유지 이동
 * - CalculatePlayerDistance: 거리 계산
 * 
 * @author 이효원
 * @date 2024-12-19
 * @version 1.0
 */
/**
 * @brief 보스 이동 관리 컴포넌트 클래스
 * 
 * @details
 * 보스 캐릭터의 이동 로직을 관리하는 컴포넌트입니다.
 * 플레이어 추적, 부드러운 회전, 이동 패턴 등을 처리합니다.
 * 
 * @section design_patterns 설계 패턴
 * - 컴포넌트 패턴: 언리얼 엔진 컴포넌트 시스템 활용
 * - 전략 패턴: 다양한 이동 전략 지원
 * - 옵저버 패턴: 디버그 정보 시각화
 * 
 * @section responsibilities 책임
 * - 플레이어 방향 회전 관리
 * - 백스탭 위치 계산 및 검증
 * - Nav Mesh 기반 안전 위치 검색
 * - 거리 기반 이동 제어
 * - 디버그 정보 제공
 * 
 * @section integration 연동 시스템
 * - AI 컨트롤러: 이동 명령 전달
 * - 타겟팅 컴포넌트: 플레이어 위치 정보
 * - 비행 컴포넌트: 공중 이동 지원
 * - StateTree: 이동 상태 관리
 * 
 * @section usage 사용법
 * 1. 보스 블루프린트에 컴포넌트 추가
 * 2. 이동 관련 함수 호출
 * 3. 디버그 정보 확인
 * 4. StateTree와 연동
 */

	//=== Public Functions ===
	
	// Lifecycle
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Player Tracking
	UFUNCTION(BlueprintCallable, Category="Player Tracking")
	void RotateTowardsPlayer(float DeltaTime, float RotationSpeed = 5.0f);
	
	UFUNCTION(BlueprintCallable, Category="Target Look")
	void LookAtTarget(const FVector& DirectionToTarget);
	
	class APawn* FindPlayer();

	// Movement Functions
	UFUNCTION(BlueprintCallable, Category="Backstep Movement")
	FVector FindBackstepPosition();
	
	UFUNCTION(BlueprintCallable, Category="Movement")
	void ExecuteSmartMovement(float DeltaTime, float MinDistance, float MaxDistance);

	// Distance Calculation
	UFUNCTION(BlueprintCallable, Category="Distance Calculation")
	void CalculatePlayerDistance(FVector& OutTargetLocation, FVector& OutOwnerLocation, 
		FVector& OutDirectionToTarget, float& OutCurrentDistance);

	// State Management
	FGameplayTag GetPlayerMovementStateTag();
	void SetMovementStateWalk();
	void SetMovementStateFly();
	
	UFUNCTION(BlueprintCallable, Category = "Boss Reset")
	void ResetMovementSystem();

	// Debug Functions
	FVector GetDebugTargetLocation() const;
	FVector GetDebugOwnerLocation() const;
	FVector GetDebugClosestPosition() const;
	float GetDebugCurrentDistance() const;

	//=== Public Variables ===
	UPROPERTY()
	FVector SafePosition;
	
	UPROPERTY(EditAnywhere)
	bool IsFlying = false;

protected:
	//=== Protected Functions ===
	FVector FindSafePositionOnNavMesh(const FVector& Direction, float Distance);
	bool IsPositionFarFromPlayer(const FVector& Position, float MinDistanceFromPlayer);

	//=== Protected Variables ===
	UPROPERTY(EditAnywhere)
	FBossTargetState TargetStateTag;

private:
	//=== Private Variables ===
	UPROPERTY()
	class ACharacter* Owner;
	
	UPROPERTY()
	class AAIController* AIC;

	UPROPERTY()
	class UCBossTargetingComponent* TargetingComp;

	UPROPERTY()
	class UFlyingComponent* FlyingComponent;

	// Debug data
	FVector DebugTargetLocation;
	FVector DebugOwnerLocation;
	FVector DebugClosestPosition;
	float DebugCurrentDistance;
};
// ===== 백스탭 상수들 =====
namespace BackstepConstants
{
	const int32 MAX_ANGLE_STEPS = 10;
	const float ANGLE_STEPS[MAX_ANGLE_STEPS] = { 0.0f, 15.0f, 30.0f, 45.0f, 60.0f, 90.0f, 120.0f, 150.0f, 180.0f, 210.0f };
	const FColor DEBUG_COLORS[MAX_ANGLE_STEPS] = { 
		FColor::Green, FColor::Yellow, FColor::Orange, FColor::Red, FColor::Purple, 
		FColor::Blue, FColor::Cyan, FColor::Magenta, FColor::White, FColor::Black 
	};
}

