// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file Task_KeepingDistance.h
 * @brief 보스 거리 유지 태스크 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 보스가 플레이어와 적절한 거리를 유지하면서 이동하는 태스크를 정의합니다.
 * 플레이어 방향에 따라 호의 시작점 또는 끝점으로 이동하는 고급 AI 행동을 구현합니다.
 * 
 * @section architecture 아키텍처
 * - UTask_KeepingDistance: 거리 유지 태스크 메인 클래스
 * - StateTree 기반 태스크 시스템
 * - 플레이어 방향 감지 및 호 기반 이동
 * 
 * @section features 주요 기능
 * - 플레이어와의 거리 계산 및 유지
 * - 플레이어 방향에 따른 호 기반 이동
 * - 실시간 거리 모니터링
 * - 디버그 시각화 시스템
 * 
 * @section movement_system 이동 시스템
 * - 호 기반 이동: 플레이어를 중심으로 한 호 형태 이동
 * - 방향 감지: 플레이어가 보스의 어느 쪽에 있는지 감지
 * - 거리 조절: 최소/최대 거리 범위 내에서 이동
 * 
 * @author [개발자명]
 * @date [작성일]
 * @version 1.0
 */

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "Engine/World.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Task_KeepingDistance.generated.h"

/**
 * @brief 보스 거리 유지 태스크 클래스
 * 
 * @details
 * 보스가 플레이어와 적절한 거리를 유지하면서 이동하는 태스크입니다.
 * 플레이어 방향에 따라 호의 시작점 또는 끝점으로 이동하는 고급 AI 행동을 구현합니다.
 * 
 * @section design_patterns 설계 패턴
 * - StateTree Task 패턴: StateTree 시스템과 연동
 * - 전략 패턴: 다양한 이동 전략 지원
 * - 옵저버 패턴: 실시간 거리 모니터링
 * 
 * @section movement_logic 이동 로직
 * 1. 플레이어와의 거리 계산
 * 2. 플레이어 방향 감지 (왼쪽/오른쪽)
 * 3. 호의 시작점 또는 끝점 결정
 * 4. 목표 위치로 이동
 * 5. 실시간 거리 조절
 * 
 * @section debug_features 디버그 기능
 * - 거리 구체 시각화
 * - 호 경로 시각화
 * - 목표 위치 표시
 * - 실시간 거리 정보 출력
 * 
 * @section usage 사용법
 * 1. StateTree에서 이 태스크를 추가
 * 2. 최소/최대 거리 설정
 * 3. 플레이어 타겟 설정
 * 4. 런타임에서 자동 실행
 */
UCLASS()
class OPERRATION_DDT_API UTask_KeepingDistance : public UStateTreeTaskBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief 생성자
	 * 
	 * @details
	 * 태스크의 기본 설정을 초기화합니다.
	 * 기본 거리 값과 디버그 설정을 적용합니다.
	 */
	UTask_KeepingDistance();

	/**
	 * @brief 태스크 진입 시 호출되는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @param Transition 상태 전환 정보
	 * @return EStateTreeRunStatus::Running 또는 Failed
	 * 
	 * @details
	 * 태스크가 시작될 때 초기화 작업을 수행합니다.
	 * 타겟과 소유자 확인, 초기 궤도 각도 설정을 담당합니다.
	 * 
	 * @section initialization 초기화 과정
	 * 1. 타겟과 소유자 유효성 검사
	 * 2. 보스 캐릭터 캐스팅
	 * 3. 초기 궤도 각도 설정 (랜덤)
	 * 4. 기본 변수 초기화
	 */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	/**
	 * @brief 태스크 실행 중 매 프레임 호출되는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @param DeltaTime 프레임 간 시간 간격
	 * @return EStateTreeRunStatus::Running 또는 Failed
	 * 
	 * @details
	 * 매 프레임마다 거리 계산, 이동 로직, 디버그 시각화를 수행합니다.
	 * 플레이어 방향에 따른 호 기반 이동을 구현합니다.
	 * 
	 * @section execution_flow 실행 흐름
	 * 1. 거리 계산 (PlayerDistanceCalculate)
	 * 2. 거리 범위 확인 및 이동 로직 결정
	 * 3. 호 기반 이동 또는 거리 조절 이동
	 * 4. 타겟 바라보기 (LookAtTarget)
	 * 5. 디버그 시각화 (DrawOrbitDebug, DrawArcDebug)
	 */
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

	/**
	 * @brief 태스크 종료 시 호출되는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @param Transition 상태 전환 정보
	 * 
	 * @details
	 * 태스크가 종료될 때 정리 작업을 수행합니다.
	 * 이동 중지 및 로그 출력을 담당합니다.
	 * 
	 * @section cleanup 정리 과정
	 * 1. 이동 중지 (Controller->StopMovement)
	 * 2. 로그 출력
	 * 3. 리소스 정리
	 */
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

protected:
	/**
	 * @brief 플레이어와의 거리 계산 함수
	 * 
	 * @details
	 * 보스와 플레이어 간의 거리를 계산하고 관련 변수들을 업데이트합니다.
	 * 
	 * @section calculation 계산 과정
	 * 1. 타겟 위치 가져오기
	 * 2. 소유자 위치 가져오기
	 * 3. 방향 벡터 계산
	 * 4. 거리 계산
	 * 
	 * @section variables 업데이트 변수
	 * - TargetLocation: 타겟(플레이어) 위치
	 * - OwnerLocation: 소유자(보스) 위치
	 * - DirectionToTarget: 보스에서 플레이어로의 방향 벡터
	 * - CurrentDistance: 현재 거리
	 */
	void PlayerDistanceCalculate();

	/**
	 * @brief 호 기반 이동 함수
	 * 
	 * @param DeltaTime 프레임 간 시간 간격
	 * 
	 * @details
	 * 플레이어 방향에 따라 호의 시작점 또는 끝점으로 이동하는 로직을 구현합니다.
	 * 보스가 플레이어를 바라보는 방향을 기준으로 좌우 30도씩의 호를 생성합니다.
	 * 
	 * @section arc_generation 호 생성 과정
	 * 1. 보스 전방 방향 계산
	 * 2. 보스 후방 방향 계산 (호의 기준 방향)
	 * 3. 호의 시작점과 끝점 계산 (-30도 ~ +30도)
	 * 4. 플레이어 방향에 따른 목표점 결정
	 * 
	 * @section direction_detection 방향 감지
	 * - 플레이어가 보스의 왼쪽에 있으면 → 호의 시작점 (ArcStart)
	 * - 플레이어가 보스의 오른쪽에 있으면 → 호의 끝점 (ArcEnd)
	 * - 기본값은 호의 시작점
	 */
	void MoveInOrbit(float DeltaTime);

	/**
	 * @brief 타겟을 바라보는 함수
	 * 
	 * @details
	 * 보스가 플레이어를 부드럽게 바라보도록 회전을 제어합니다.
	 * Yaw 회전만 적용하여 수직 회전은 제외합니다.
	 * 
	 * @section rotation_logic 회전 로직
	 * 1. 타겟 방향으로의 회전 계산
	 * 2. 현재 회전에서 목표 회전으로 보간
	 * 3. Yaw만 회전 (Pitch, Roll 고정)
	 * 4. 부드러운 회전 적용 (보간 속도: 5.0f)
	 */
	void LookAtTarget();

	/**
	 * @brief 궤도 디버그 시각화 함수
	 * 
	 * @details
	 * 거리 유지 시스템의 디버그 정보를 시각화합니다.
	 * 최소/최대 거리 구체, 목표 위치, 거리 선 등을 그립니다.
	 * 
	 * @section visualization 시각화 요소
	 * 1. 최소 거리 구체 (빨간색)
	 * 2. 최대 거리 구체 (초록색)
	 * 3. 목표 위치 구체 (노란색)
	 * 4. 보스에서 목표로의 선 (노란색)
	 * 5. 현재 거리 선 (거리에 따라 색상 변경)
	 * 6. 거리 텍스트 표시
	 * 7. 목표 위치 텍스트 표시
	 */
	void DrawOrbitDebug();

	/**
	 * @brief 호 디버그 시각화 함수
	 * 
	 * @details
	 * 호 기반 이동 시스템의 디버그 정보를 시각화합니다.
	 * 호 경로, 시작점/끝점, 방향 벡터 등을 그립니다.
	 * 
	 * @section visualization 시각화 요소
	 * 1. 호 경로 선분들 (청록색)
	 * 2. 호의 시작점과 끝점 선 (노란색)
	 * 3. 호의 중심점 (플레이어 위치, 빨간색)
	 * 4. 보스 전방 방향 (초록색)
	 * 5. 호의 기준 방향 (보라색)
	 * 6. 보스 위치 (파란색)
	 * 7. 궤도 위치 (주황색)
	 */
	void DrawArcDebug();

	// 거리 관련 변수들
	UPROPERTY(EditAnywhere, Category = "Distance")
	float MinDistance = 300.0f; ///< 최소 거리

	UPROPERTY(EditAnywhere, Category = "Distance")
	float MaxDistance = 800.0f; ///< 최대 거리

	// 내부 계산 변수들
	float CurrentDistance = 0.0f; ///< 현재 거리
	FVector TargetLocation; ///< 타겟 위치
	FVector OwnerLocation; ///< 소유자 위치
	FVector DirectionToTarget; ///< 타겟으로의 방향 벡터
	FVector ClosestPosition; ///< 가장 가까운 위치 (목표 위치)

	// 호 관련 변수들
	FVector BossForward; ///< 보스 전방 방향
	FVector BossBackward; ///< 보스 후방 방향 (호의 기준 방향)
	FVector ArcStart; ///< 호의 시작점
	FVector ArcEnd; ///< 호의 끝점
	float ArcRadius; ///< 호의 반지름

	// 궤도 관련 변수들
	float OrbitAngle = 0.0f; ///< 궤도 각도

	// 컴포넌트 참조들
	UPROPERTY()
	class ACharacter* Boss; ///< 보스 캐릭터 참조

	UPROPERTY()
	class AAIController* Controller; ///< AI 컨트롤러 참조

	UPROPERTY()
	class AActor* Target; ///< 타겟 (플레이어) 참조

	UPROPERTY()
	class AActor* Owner; ///< 소유자 (보스) 참조

	// 상태 태그 관련 변수들
	UPROPERTY(EditAnywhere, Category = "State Tags")
	FGameplayTag CurrentTargetStateTag; ///< 현재 타겟 상태 태그

	UPROPERTY(EditAnywhere, Category = "State Tags")
	struct FTargetStateTags
	{
		FGameplayTag Left; ///< 왼쪽 상태 태그
		FGameplayTag Right; ///< 오른쪽 상태 태그
	} TargetStateTag; ///< 타겟 상태 태그 구조체
};
