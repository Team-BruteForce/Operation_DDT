// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ODH/ODH_AIController/CEnemyAIController.h"
#include "CSkeletonEnemyAIController.generated.h"

/**
 * SkeletonEnemy 전용 AI Controller
 * CEnemyAIController를 상속하여 기본 기능을 사용하고
 * SkeletonEnemy 특화 기능을 추가할 수 있음
 */
UCLASS()
class OPERRATION_DDT_API ACSkeletonEnemyAIController : public ACEnemyAIController
{
	GENERATED_BODY()

public:
	ACSkeletonEnemyAIController();

protected:
	virtual void BeginPlay() override;

public:
	// SkeletonEnemy 전용 기능들을 여기에 추가할 수 있음
	// 예: 특별한 공격 패턴, 이동 로직 등
};

