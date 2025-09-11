/**
 * @file Task_SwitchPase.cpp
 * @brief 보스 페이즈 전환 태스크 구현 파일
 * 
 * @details
 * 보스의 페이즈를 전환하는 태스크의 구현 파일입니다.
 * StateTree 기반 태스크 시스템을 사용하여 페이즈를 관리합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#include "Boss/Task/Task_SwitchPase.h"

#include "AIController.h"
#include "Global.h"
#include "StateTreeExecutionContext.h"
#include "Boss/Component/CBossWeaponComponent.h"

/**
 * @brief 상태 진입 시 호출되는 함수
 * 
 * 보스의 페이즈를 전환합니다.
 */
EStateTreeRunStatus UTask_SwitchPase::EnterState(FStateTreeExecutionContext& Context,
                                                 const FStateTreeTransitionResult& Transition)
{
	if(bStarted) return EStateTreeRunStatus::Running;
	bStarted=true;
	if (!Owner)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task_SwitchPase - Boss actor not found!"));
		return EStateTreeRunStatus::Running;
	}
		// 컴포넌트가 아직 초기화되지 않았을 수 있음 - 안전하게 체크
	UCBossWeaponComponent* Weapon = CHelpers::GetComponent<UCBossWeaponComponent>(Owner);
	if (!Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Task_SwitchPase - Weapon component not found!"));
		return EStateTreeRunStatus::Running;
	}
	CLog::Log("Pase Change Task");
	Weapon->SetMode(OwningStateTag);
	CLog::Log(OwningStateTag.ToString());
	
	return EStateTreeRunStatus::Running;
}
