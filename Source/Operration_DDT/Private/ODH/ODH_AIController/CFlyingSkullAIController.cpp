// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_AIController/CFlyingSkullAIController.h"

ACFlyingSkullAIController::ACFlyingSkullAIController()
{
	// FlyingSkull 전용 설정들
	// 예: 더 넓은 감지 범위, 특별한 시각 각도 등
	SightRadius = 1200.0f;  // FlyingSkull은 더 넓은 감지 범위
	LoseSightRadius = 1400.0f;
	PeripheralVisionAngleDegrees = 120.0f;  // 더 넓은 시야각
}

void ACFlyingSkullAIController::BeginPlay()
{
	Super::BeginPlay();
	
	// FlyingSkull 전용 초기화 로직
	// 예: 특별한 AI 행동 패턴 설정 등
}

