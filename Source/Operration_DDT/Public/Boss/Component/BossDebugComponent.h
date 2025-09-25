// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file BossDebugComponent.h
 * @brief 보스 디버그 컴포넌트 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 보스 캐릭터의 디버그 시각화를 관리하는 컴포넌트를 정의합니다.
 * 거리, 궤도, 백스탭 위치 등의 디버그 정보를 시각화합니다.
 * 
 * @section architecture 아키텍처
 * - UBossDebugComponent: 보스 디버그 시각화 메인 컴포넌트
 * - 거리 디버그 시스템
 * - 궤도 디버그 시스템
 * - 백스탭 디버그 시스템
 * 
 * @section features 주요 기능
 * - 거리 정보 시각화
 * - 궤도 경로 표시
 * - 백스탭 위치 디버그
 * - 아크 디버그 정보
 * - 실시간 디버그 업데이트
 * 
 * @section debug_system 디버그 시스템
 * - DrawDistanceDebug: 거리 정보 그리기
 * - DrawOrbitDebug: 궤도 정보 그리기
 * - DrawBackstepDebug: 백스탭 정보 그리기
 * - ExecuteDebugVisualization: 전체 디버그 실행
 * 
 * @author 이효원
 * @date 2024-12-19
 * @version 1.0
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossDebugComponent.generated.h"


/**
 * @brief 보스 디버그 컴포넌트 클래스
 * 
 * @details
 * 보스 캐릭터의 디버그 시각화를 관리하는 컴포넌트입니다.
 * 거리, 궤도, 백스탭 위치 등의 디버그 정보를 시각화합니다.
 * 
 * @section design_patterns 설계 패턴
 * - 컴포넌트 패턴: 언리얼 엔진 컴포넌트 시스템 활용
 * - 옵저버 패턴: 디버그 정보 실시간 업데이트
 * - 전략 패턴: 다양한 디버그 시각화 방식 지원
 * 
 * @section responsibilities 책임
 * - 거리 정보 시각화
 * - 궤도 경로 표시
 * - 백스탭 위치 디버그
 * - 아크 정보 표시
 * - 실시간 디버그 업데이트
 * 
 * @section integration 연동 시스템
 * - 이동 컴포넌트: 이동 관련 디버그 정보
 * - 타겟팅 컴포넌트: 타겟 관련 디버그 정보
 * - 디버그 드로잉 시스템: 시각화 렌더링
 * 
 * @section usage 사용법
 * 1. 보스 블루프린트에 컴포넌트 추가
 * 2. 이동 컴포넌트 참조 설정
 * 3. 디버그 함수 호출
 * 4. 시각화 정보 확인
 */

	//=== Public Functions ===
	
	// Lifecycle
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Debug Visualization

	UFUNCTION(BlueprintCallable, Category="Distance Debug")
	void DrawDistanceDebug(const FVector& TargetLocation, const FVector& OwnerLocation, 
		float MinDistance, float MaxDistance, float CurrentDistance, const FVector& ClosestPosition);

	UFUNCTION(BlueprintCallable, Category="Orbit Debug")
	void DrawOrbitDebug(const FVector& TargetLocation, const FVector& OwnerLocation,
		const FVector& ArcStart, const FVector& ArcEnd, float ArcRadius,
		const FVector& BossForward, const FVector& BossBackward, const FVector& ClosestPosition);

	UFUNCTION(BlueprintCallable, Category="Backstep Debug")
	void DrawBackstepDebug(const FVector& BackstepPosition, const FVector& PlayerLocation, int32 StepIndex);

	UFUNCTION(BlueprintCallable, Category="Backstep Debug")
	void DrawBackstepFailureDebug(const FVector& BossLocation);
	
	UFUNCTION(BlueprintCallable, Category="Orbit Debug")
	void DrawOrbitDistanceDebug(const FVector& TargetLocation, const FVector& OwnerLocation, 
		float MinDistance, float MaxDistance, const FVector& ClosestPosition);
	
	UFUNCTION(BlueprintCallable, Category="Arc Debug")
	void DrawArcDebug(const FVector& TargetLocation, const FVector& OwnerLocation,
		const FVector& ArcStart, const FVector& ArcEnd, float ArcRadius,
		const FVector& BossForward, const FVector& BossBackward, const FVector& ClosestPosition);
	
	UFUNCTION(BlueprintCallable, Category="Debug Visualization")
	void ExecuteDebugVisualization();
	
	// Setup Functions
	UFUNCTION(BlueprintCallable, Category="Debug Setup")
	void SetMovementComponent(class UCBossMovementComponent* InMovementComponent);
	
private:
	//=== Private Variables ===
	UPROPERTY()
	class UCBossMovementComponent* MovementComponent;
};
