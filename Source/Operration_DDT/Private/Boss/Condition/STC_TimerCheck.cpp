/**
 * @file STC_TimerCheck.cpp
 * @brief 타이머 체크 조건 구현 파일
 * 
 * @details
 * StateTree에서 타이머 기반 조건을 체크하는 조건입니다.
 * 델타타임을 사용하여 시간 기반 조건을 확인합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Condition/STC_TimerCheck.h"

#include "StateTreeExecutionContext.h"
#include "Math/UnrealMathUtility.h"
#include "Kismet/GameplayStatics.h"

bool USTC_TimerCheck::TestCondition(FStateTreeExecutionContext& Context) const
{
	// 델타타임 가져오기
	float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(Context.GetWorld());
	
	// 타이머 업데이트
	CurrentTimer += DeltaTime;
	
	// 목표 시간 도달했는지 체크
	if (CurrentTimer >= TargetTime)
	{
	// 타이머 리셋 (다음 랜덤 시간 설정)
	const_cast<USTC_TimerCheck*>(this)->ResetTimer();
	return true;
	}
	
	return false;
}

void USTC_TimerCheck::ResetTimer()
{
	// 랜덤 시간 생성
	TargetTime = FMath::RandRange(MinTime, MaxTime);
	CurrentTimer = 0.0f;
}
