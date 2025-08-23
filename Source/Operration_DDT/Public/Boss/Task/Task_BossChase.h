// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file Task_BossChase.h
 * @brief 보스 추적 태스크 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 보스가 플레이어를 추적하는 태스크를 정의합니다.
 * StateTree 기반으로 플레이어 추적 및 이동을 처리합니다.
 * 
 * @section features 주요 기능
 * - 플레이어 추적 및 이동
 * - 거리 기반 추적 로직
 * - StateTree 연동
 * - 블루프린트 기반 태스크
 * 
 * @author [개발자명]
 * @date [작성일]
 * @version 1.0
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_BossChase.generated.h"

/**
 * @brief 보스 추적 태스크 클래스
 * 
 * @details
 * 보스가 플레이어를 추적하는 StateTree 태스크입니다.
 * 플레이어와의 거리를 계산하고 적절한 이동을 수행합니다.
 */
UCLASS()
class OPERRATION_DDT_API UTask_BossChase : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
	/**
	 * @brief 태스크 실행 중 매 프레임 호출되는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @param DeltaTime 프레임 간 시간 간격
	 * @return EStateTreeRunStatus::Running 또는 Failed
	 * 
	 * @details
	 * 매 프레임마다 플레이어 추적 로직을 수행합니다.
	 * 거리 계산, 이동 처리, 목표 도달 확인을 담당합니다.
	 */
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
	
	/**
	 * @brief 태스크 진입 시 호출되는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @param Transition 상태 전환 정보
	 * @return EStateTreeRunStatus::Running 또는 Failed
	 * 
	 * @details
	 * 태스크가 시작될 때 초기화 작업을 수행합니다.
	 * 타겟과 소유자 확인, 이동 컴포넌트 설정을 담당합니다.
	 */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	
	/**
	 * @brief 태스크 종료 시 호출되는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @param Transition 상태 전환 정보
	 * 
	 * @details
	 * 태스크가 종료될 때 정리 작업을 수행합니다.
	 * 이동 중지 및 로그 출력을 담당합니다.
	 */
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	/**
	 * @brief 보스 이동 컴포넌트
	 * 
	 * @details
	 * 보스의 이동을 담당하는 컴포넌트입니다.
	 * 플레이어 추적 및 이동 로직을 처리합니다.
	 */
	UPROPERTY()
	class UCBossMovementComponent* Movement;
	
public:
	/**
	 * @brief 추적할 타겟 (플레이어)
	 * 
	 * @details
	 * 보스가 추적할 타겟 액터입니다.
	 * 보통 플레이어 캐릭터가 됩니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowBinding))
	class APawn* Target;
	
	/**
	 * @brief 보스 소유자
	 * 
	 * @details
	 * 이 태스크를 실행하는 보스 액터입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowBinding))
	class APawn* Owner;
	
	/**
	 * @brief AI 컨트롤러
	 * 
	 * @details
	 * 보스를 제어하는 AI 컨트롤러입니다.
	 * 이동 명령을 처리합니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowBinding))
	class AAIController* Controller;
	
	/**
	 * @brief 타겟까지의 거리
	 * 
	 * @details
	 * 보스와 타겟(플레이어) 간의 현재 거리입니다.
	 * 실시간으로 업데이트됩니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowBinding))
	float DistanceToTarget;
	
	/**
	 * @brief 거리 임계값
	 * 
	 * @details
	 * 추적을 중단할 거리 임계값입니다.
	 * 이 거리 이내에 도달하면 추적을 완료합니다.
	 * 
	 * @section default_value 기본값
	 * - 기본값: 0.0f
	 * - 설정 가능한 값으로 조정 가능
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowBinding))
	float DistanceThreshold = 0.0f;
};
