// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/StateTree/CBossEnemyStateTreeEvaluator.h"
#include "AIController.h"
#include "Global.h"
#include "StateTreeExecutionContext.h"
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
	CLog::Log(Context.GetOwner()->GetName());

	CLog::Log("Tick");
	Get_Decision_Data(Context,DeltaTime);
	
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
