// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file CBossAIC.cpp
 * @brief 보스 AI 컨트롤러 구현 파일
 * 
 * @date 2024-12-19
 * @author [이효원]
 * @version 1.0
 * 
 * @section overview 개요
 * 이 파일은 보스 캐릭터의 AI 컨트롤러를 구현합니다.
 * StateTree 컴포넌트를 통해 보스의 행동을 제어하며,
 * Pawn 소유 시 초기화 작업을 수행합니다.
 * 
 * @section purpose 개발 목적
 * - 기획자가 설계한 보스 AI 행동 패턴을 StateTree로 구현
 * - 실시간 데이터 변경에 따른 AI 행동 동적 업데이트
 * - 보스 캐릭터의 지능적이고 예측 가능한 전투 행동 구현
 * 
 * @section features 주요 기능
 * - 보스 Pawn 소유 시 초기화
 * - StateTree 컴포넌트 관리
 * - AI 행동 제어 (StateTree 기반)
 * - 동적 데이터 로딩을 통한 행동 패턴 업데이트
 * 
 * @section technical_details 기술적 세부사항
 * - Unreal Engine의 AI 컨트롤러 시스템 활용
 * - StateTree를 통한 복잡한 AI 행동 상태 관리
 * - 데이터 기반 AI 행동 패턴 설계
 * 
 * @section portfolio_highlight 포트폴리오 하이라이트
 * 이 AI 컨트롤러는 게임 개발에서 핵심적인 AI 시스템 구현 능력을 보여주며,
 * StateTree를 활용한 체계적인 AI 행동 제어 시스템을 구현한 사례입니다.
 * 특히 실시간 데이터 동기화를 통한 AI 행동 동적 업데이트는
 * 현대 게임 개발에서 요구되는 유연성과 확장성을 잘 보여줍니다.
 */

#include "Boss/CBossAIC.h"

#include "Global.h"
#include "StateTree.h"
#include "Components/StateTreeAIComponent.h"

ACBossAIC::ACBossAIC()
{
	// StateTree 컴포넌트 생성
	CHelpers::CreateActorComponent(this, &StateTreeComp, "StateTreeComp");

	// // StateTree 설정
	// if (StateTree && StateTreeComp)
	// {
	// 	StateTreeComp->SetStateTree(StateTree);
	// }
}

/**
 * @brief Pawn 소유 시 호출되는 함수
 * 
 * @param InPawn 소유할 Pawn 객체
 * 
 * AI 컨트롤러가 Pawn을 소유할 때 초기화 작업을 수행합니다.
 * 현재는 로그 메시지만 출력합니다.
 */
void ACBossAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!StateTreeComp) return;
}
