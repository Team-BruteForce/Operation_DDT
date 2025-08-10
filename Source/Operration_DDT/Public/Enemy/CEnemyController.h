// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CEnemyController.generated.h"

/** @brief 적 캐릭터의 행동을 제어하는 AI 컨트롤러입니다. */
UCLASS()
class OPERRATION_DDT_API ACEnemyController : public AAIController
{
	GENERATED_BODY()
public:
	ACEnemyController(const FObjectInitializer& ObjectInitializer);
	
	virtual void BeginPlay() override;
	
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	virtual void OnPossess(APawn* InPawn) override;

public:
	/// @brief StateTree AI 로직을 담고 있는 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class UCEnemyStateTreeAIComponent* EnemyStateTreeAIComponent;
	
	/// @brief 적의 이동을 담당하는 커스텀 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	class UCEnemyMovement* MoveComp;

	/// @brief 현재 제어 중인 적 캐릭터(Pawn)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAEnemy* Enemy;

	/// @brief 순찰 경로 등으로 사용될 스플라인
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class USplineComponent* Spline;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 TotalPatrolPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	int32 CurrentPatrolPoints;
};