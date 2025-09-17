/**
 * @file CBossAIC.cpp
 * @brief 보스 AI 컨트롤러 구현 파일
 * 
 * @details
 * 보스 캐릭터의 AI 컨트롤러를 구현합니다.
 * StateTree 컴포넌트를 통해 보스의 행동을 제어하며,
 * Pawn 소유 시 초기화 작업을 수행합니다.
 * 
 * @author [이효원]
 * @date 2024-12-19
 */

#include "Boss/CBossAIC.h"

#include "Global.h"
#include "StateTree.h"
#include "Boss/CBoss.h"
#include "Components/StateTreeAIComponent.h"
#include <Boss/CBoss.h>

/**
 * @brief 보스 AI 컨트롤러 생성자
 * 
 * StateTree 컴포넌트를 생성하고 기본 설정을 적용합니다.
 */
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
 */
void ACBossAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	if (!StateTreeComp) return;
	
	// 보스 초기 위치 저장 (레벨에 배치된 위치)
        ACBoss* Boss=Cast<ACBoss>(InPawn);
        CheckNull(Boss);
        Boss->InitialLocation = Boss->GetActorLocation();
        UE_LOG(LogTemp, Warning, TEXT("CBoss: 초기 위치 저장 완료 - %s"), 
            *Boss->InitialLocation.ToString());
}
