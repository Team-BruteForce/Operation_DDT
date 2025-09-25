/**
 * @file Task_AroundAttack.h
 * @brief 주변 공격 태스크 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 StateTree에서 보스가 주변을 돌면서 공격하는 태스크를 정의합니다.
 * 스플라인 반원 이동과 연속 프로젝타일 발사를 실행합니다.
 * 
 * @section architecture 아키텍처
 * - UTask_AroundAttack: 주변 공격 관리 메인 태스크
 * - 스플라인 기반 이동 시스템
 * - 프로젝타일 발사 시스템
 * - StateTree 태스크 시스템
 * 
 * @section features 주요 기능
 * - 스플라인 반원 이동
 * - 연속 프로젝타일 발사
 * - StateTree 연동
 * - 블루프린트 기반 태스크
 * 
 * @section attack_system 공격 시스템
 * - 스플라인 이동: 반원 경로 이동
 * - 프로젝타일 발사: 연속 발사
 * - StateTree 연동: 태스크 실행
 * 
 * @author 이효원
 * @date 2024-12-19
 * @version 1.0
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_AroundAttack.generated.h"

/**
 * @brief 주변 공격 태스크 클래스
 * 
 * @details
 * StateTree에서 보스가 주변을 돌면서 공격하는 태스크입니다.
 * 스플라인 반원 이동과 연속 프로젝타일 발사를 실행합니다.
 * 
 * @section design_patterns 설계 패턴
 * - 태스크 패턴: StateTree 태스크 시스템 활용
 * - 전략 패턴: 다양한 공격 전략 지원
 * - 컴포지트 패턴: 이동과 공격 조합
 * 
 * @section responsibilities 책임
 * - 스플라인 반원 이동 실행
 * - 연속 프로젝타일 발사
 * - StateTree 연동
 * - 공격 패턴 관리
 * 
 * @section integration 연동 시스템
 * - StateTree: 태스크 실행 관리
 * - 스플라인 시스템: 이동 경로 관리
 * - 프로젝타일 시스템: 공격 실행
 * - 보스 캐릭터: 공격 명령 실행
 * 
 * @section usage 사용법
 * 1. StateTree에 태스크 추가
 * 2. 스플라인 경로 설정
 * 3. 프로젝타일 발사 설정
 * 4. 태스크 실행
 */
{
	GENERATED_BODY()
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;


	UPROPERTY(EditAnywhere)
	class APawn* Boss;
};
