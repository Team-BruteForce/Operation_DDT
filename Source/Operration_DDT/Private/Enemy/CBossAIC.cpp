// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CBossAIC.h"

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
