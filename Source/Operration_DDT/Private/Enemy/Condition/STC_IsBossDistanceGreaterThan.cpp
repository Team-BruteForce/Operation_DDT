// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Condition/STC_IsBossDistanceGreaterThan.h"

/**
 * @brief 조건을 테스트하는 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @return true: 거리가 임계값보다 큼, false: 거리가 임계값 이하
 * 
 * 보스와 플레이어 간의 거리가 설정된 임계값보다 큰지 확인합니다.
 */
bool USTC_IsBossDistanceGreaterThan::TestCondition(FStateTreeExecutionContext& Context) const
{
	return BossToPlayerDistance>DistanceThreshold;
}
