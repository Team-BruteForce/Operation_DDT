// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file CBossEnemyStateTreeEvaluator.cpp
 * @brief 보스 적 StateTree 평가자 구현 파일
 * 
 * 이 파일은 보스 적의 StateTree에서 사용되는 평가자를 구현합니다.
 * 매 프레임 호출되어 보스와 플레이어 간의 거리 계산 및 의사결정 데이터를 수집하고,
 * StateTree의 상태 정보를 로그로 출력합니다.
 * 
 * 주요 기능:
 * - 보스와 플레이어 간 거리 계산
 * - 의사결정 데이터 수집 및 업데이트
 * - StateTree 상태 정보 로그 출력
 * - 타겟(플레이어) 관리
 */

#include "Boss/StateTree/CBossEnemyStateTreeEvaluator.h"
#include "AIController.h"
#include "Global.h"
#include "StateTreeExecutionContext.h"
#include "Boss/CBoss.h"
#include "GameFramework/GameSession.h"

/**
 * @brief 매 프레임 호출되는 틱 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @param DeltaTime 델타 타임
 * 
 * StateTree가 실행될 때마다 호출되어 의사결정 데이터를 업데이트합니다.
 * 현재는 로그 출력과 의사결정 데이터 수집을 수행합니다.
 */
void UCBossEnemyStateTreeEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);

	Get_Decision_Data(Context,DeltaTime);
	CLog::Print( "State : "+Context.GetActiveStateName(), 1);
	CLog::Print(FString::Printf(TEXT("Distance : %f"), player_ai_dist), 2);
	CLog::Print("Target : "+Target->GetName(), 3);
	if (SelectedTag.IsValid())
		CLog::Print("AttackState : "+SelectedTag.ToString(), 3);
		CLog::Print(IsAction, 4);
	
}

/**
 * @brief 의사결정에 필요한 데이터를 수집하는 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @param DeltaTime 델타 타임
 * 
 * 플레이어와 보스 간의 거리 등 의사결정에 필요한 데이터를 수집합니다.
 * 현재는 플레이어를 타겟으로 설정하고 거리를 계산합니다.
 */
void UCBossEnemyStateTreeEvaluator::Get_Decision_Data(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Target = Target = GetWorld()->GetFirstPlayerController()->GetPawn();

	CheckNull(Owner);
	CheckNull(Target);
	
	player_ai_dist=Boss->GetDistanceTo(Target);
}
