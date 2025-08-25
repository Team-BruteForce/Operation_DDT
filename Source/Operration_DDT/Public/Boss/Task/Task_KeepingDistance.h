/**
 * @file Task_KeepingDistance.h
 * @brief 보스 거리 유지 태스크 헤더 파일
 * 
 * @details
 * 보스가 플레이어와 적절한 거리를 유지하면서 이동하는 태스크를 정의합니다.
 * 플레이어 방향에 따라 호의 시작점 또는 끝점으로 이동하는 고급 AI 행동을 구현합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "Engine/World.h"
#include "AIController.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Boss/StateTree/CBossEnemyStateTreeEvaluator.h"
#include "GameFramework/Character.h"
#include "Task_KeepingDistance.generated.h"

/**
 * @brief 보스 거리 유지 태스크
 * 
 * 보스가 플레이어와 적절한 거리를 유지하면서 이동하는 태스크입니다.
 * StateTree 기반 태스크 시스템을 사용하여 플레이어 방향 감지 및 호 기반 이동을 구현합니다.
 */
UCLASS()
class OPERRATION_DDT_API UTask_KeepingDistance : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	/**
	 * @brief 상태 진입 시 호출되는 함수
	 */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	
	/**
	 * @brief 매 프레임 호출되는 틱 함수
	 */
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
	
	/**
	 * @brief 상태 종료 시 호출되는 함수
	 */
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

protected:
	/**
	 * @brief 플레이어 거리 계산
	 * 
	 * 플레이어와의 거리를 계산합니다.
	 */
	void PlayerDistanceCalculate();
	
	/**
	 * @brief 궤도 이동
	 * 
	 * @param DeltaTime 델타 타임
	 * 
	 * 플레이어를 중심으로 한 궤도를 따라 이동합니다.
	 */
	void MoveInOrbit(float DeltaTime);
	
	/**
	 * @brief 타겟 바라보기
	 * 
	 * 플레이어를 향해 회전합니다.
	 */
	void LookAtTarget();
	
	/**
	 * @brief 궤도 디버그 그리기
	 * 
	 * 궤도 이동을 시각적으로 표시합니다.
	 */
	void DrawOrbitDebug();
	
	/**
	 * @brief 호 디버그 그리기
	 * 
	 * 호 이동을 시각적으로 표시합니다.
	 */
	void DrawArcDebug();

	// 거리 관련 변수들
	/**
	 * @brief 최소 거리
	 */
	UPROPERTY(EditAnywhere, Category = "Distance")
	float MinDistance = 300.0f;

	/**
	 * @brief 최대 거리
	 */
	UPROPERTY(EditAnywhere, Category = "Distance")
	float MaxDistance = 800.0f;

	// 내부 계산 변수들
	/**
	 * @brief 현재 거리
	 */
	float CurrentDistance = 0.0f; 
	
	/**
	 * @brief 타겟 위치
	 */
	FVector TargetLocation; 
	
	/**
	 * @brief 소유자 위치
	 */
	FVector OwnerLocation;
	
	/**
	 * @brief 타겟 방향
	 */
	FVector DirectionToTarget;
	
	/**
	 * @brief 가장 가까운 위치
	 */
	FVector ClosestPosition; 

	// 호 관련 변수들
	/**
	 * @brief 보스 전방 방향
	 */
	FVector BossForward;
	
	/**
	 * @brief 보스 후방 방향 (호의 기준 방향)
	 */
	FVector BossBackward;
	
	/**
	 * @brief 호의 시작점
	 */
	FVector ArcStart;
	
	/**
	 * @brief 호의 끝점
	 */
	FVector ArcEnd;
	
	/**
	 * @brief 호의 반지름
	 */
	float ArcRadius;

	/**
	 * @brief 궤도 각도
	 */
	float OrbitAngle = 0.0f;

	/**
	 * @brief 보스 캐릭터 참조
	 */
	UPROPERTY()
	class ACharacter* Boss;

	/**
	 * @brief AI 컨트롤러
	 */
	UPROPERTY()
	class AAIController* Controller; 

	/**
	 * @brief 타겟 액터
	 */
	UPROPERTY()
	class AActor* Target;

	/**
	 * @brief 소유자 액터
	 */
	UPROPERTY()
	class AActor* Owner;

	// 상태 태그 관련 변수들
	/**
	 * @brief 현재 타겟 상태 태그
	 */
	UPROPERTY(EditAnywhere, Category = "State Tags")
	FGameplayTag CurrentTargetStateTag;
	
	/**
	 * @brief 타겟 상태 태그
	 */
	UPROPERTY(EditAnywhere)
	FBossTargetState TargetStateTag;
	
};
