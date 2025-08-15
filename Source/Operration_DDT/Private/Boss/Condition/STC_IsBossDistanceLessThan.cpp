// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file STC_IsBossDistanceLessThan.cpp
 * @brief 보스와 플레이어 간 거리가 임계값보다 작은지 확인하는 조건 구현 파일
 * 
 * 이 파일은 StateTree에서 사용되는 조건 클래스를 구현합니다.
 * 보스와 플레이어 간의 거리가 설정된 임계값보다 작거나 같은지 확인하여
 * StateTree의 상태 전환 조건으로 사용됩니다.
 * 
 * 주요 기능:
 * - 보스와 플레이어 간 거리 비교
 * - 임계값 기반 조건 판단
 * - StateTree 상태 전환 조건 제공
 */

#include "Boss/Condition/STC_IsBossDistanceLessThan.h"
#include "StateTreeExecutionContext.h"

/**
 * @brief 조건을 테스트하는 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @return true: 거리가 임계값 이하, false: 거리가 임계값보다 큼
 * 
 * 보스와 플레이어 간의 거리가 설정된 임계값보다 작거나 같은지 확인합니다.
 */
bool USTC_IsBossDistanceLessThan::TestCondition(FStateTreeExecutionContext& Context) const
{
	// return Super::TestCondition(Context);
	return BossToPlayerDistance<=DistanceThreshold;
}
