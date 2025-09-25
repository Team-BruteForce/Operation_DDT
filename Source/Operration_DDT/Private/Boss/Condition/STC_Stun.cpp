/**
 * @file STC_Stun.cpp
 * @brief 스턴 상태 체크 조건 구현 파일
 * 
 * @details
 * StateTree에서 보스의 스턴 상태를 체크하는 조건입니다.
 * 보스 상태 컴포넌트를 통해 스턴 여부를 확인합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Condition/STC_Stun.h"

#include "Global.h"
#include "Boss/Component/CBossStatusComponent.h"

bool USTC_Stun::TestCondition(FStateTreeExecutionContext& Context) const
{
	UCBossStatusComponent* Status=CHelpers::GetComponent<UCBossStatusComponent>(Boss);
	return Status->BossCurrentStats.CurrentGroggyGauge>=Status->BossCurrentStats.MaxGroggyGauge;
}
