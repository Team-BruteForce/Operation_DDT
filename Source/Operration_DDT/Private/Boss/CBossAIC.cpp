// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file CBossAIC.cpp
 * @brief 보스 AI 컨트롤러 구현 파일
 * 
 * 이 파일은 보스 캐릭터의 AI 컨트롤러를 구현합니다.
 * StateTree 컴포넌트를 통해 보스의 행동을 제어하며,
 * Pawn 소유 시 초기화 작업을 수행합니다.
 * 
 * 주요 기능:
 * - 보스 Pawn 소유 시 초기화
 * - StateTree 컴포넌트 관리
 * - AI 행동 제어 (StateTree 기반)
 */

#include "Boss/CBossAIC.h"

#include "Global.h"

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

	CLog::Log("Possess Success");
}
