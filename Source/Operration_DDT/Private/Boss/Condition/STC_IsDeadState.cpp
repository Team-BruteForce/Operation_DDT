/**
 * @file STC_IsDeadState.cpp
 * @brief 사망 상태 체크 조건 구현 파일
 * 
 * @details
 * StateTree에서 보스의 사망 상태를 체크하는 조건입니다.
 * 보스 상태 컴포넌트를 통해 사망 여부를 확인합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Condition/STC_IsDeadState.h"
#include "Global.h"
#include "StateTreeExecutionContext.h"
#include "Boss/Component/CBossStatusComponent.h"
#include "GameFramework/Character.h"

bool USTC_IsDeadState::TestCondition(FStateTreeExecutionContext& Context) const
{
	ACharacter* Owner=Cast<ACharacter>(Context.GetOwner());
	if (Owner)
		CLog::Log("USTC_IsDeadState : "+Owner->GetName());
	UCBossStatusComponent* Status= CHelpers::GetComponent<UCBossStatusComponent>(Owner);
	return Status->BossCurrentStats.CurrentHP<=0;
}

