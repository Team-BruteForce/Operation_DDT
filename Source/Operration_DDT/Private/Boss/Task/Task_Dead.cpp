/**
 * @file Task_Dead.cpp
 * @brief 보스 사망 처리 태스크 구현 파일
 * 
 * @details
 * StateTree에서 보스의 사망 상태를 처리하는 태스크입니다.
 * 보스의 콜리전을 비활성화하고 사망 애니메이션을 실행합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Task/Task_Dead.h"
#include "StateTreeExecutionContext.h"
#include "Boss/Component/CBossWeaponComponent.h"
#include "Global.h"
#include "Boss/CBoss.h"
#include "GameFramework/Character.h"

/**
 * @brief 사망 상태 진입 시 호출되는 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @param Transition 상태 전환 결과
 * @return EStateTreeRunStatus 실행 상태
 * 
 * 보스의 콜리전을 비활성화하고 사망 애니메이션을 실행합니다.
 */
EStateTreeRunStatus UTask_Dead::EnterState(FStateTreeExecutionContext& Context,
                                           const FStateTreeTransitionResult& Transition)
{
	UCBossWeaponComponent* Weapon= CHelpers::GetComponent<UCBossWeaponComponent>(Boss);
	ACBoss* Owner=Cast<ACBoss>(Boss);
	Owner->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (Weapon){
		Weapon->BossHitAction(SelectedTag);
		Context.Stop();
	}
	return EStateTreeRunStatus::Succeeded;
}
