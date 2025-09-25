/**
 * @file STC_CheckPase.cpp
 * @brief 페이즈 체크 조건 구현 파일
 * 
 * @details
 * StateTree에서 보스의 페이즈를 체크하는 조건입니다.
 * 보스의 현재 페이즈 상태를 확인합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Condition/STC_CheckPase.h"
#include "Global.h"
#include "Boss/CBoss.h"

bool USTC_CheckPase::TestCondition(FStateTreeExecutionContext& Context) const
{
	ACBoss* Boss= Cast<ACBoss>(Owner);
	return Boss->IsPaseChange;
}
