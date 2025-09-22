// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_AIController/CFlowerEnemyAIController.h"

ACFlowerEnemyAIController::ACFlowerEnemyAIController()
{
	// FlowerEnemy 전용 설정들
	// 예: 중간 감지 범위, 적당한 시각 각도 등
	SightRadius = 1000.0f;  // FlowerEnemy는 중간 감지 범위
	LoseSightRadius = 1200.0f;
	PeripheralVisionAngleDegrees = 90.0f;  // 적당한 시야각
}

void ACFlowerEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	// FlowerEnemy 전용 초기화 로직
	// 예: 특별한 AI 행동 패턴 설정 등
}
