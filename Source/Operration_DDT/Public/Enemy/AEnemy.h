// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/EnemyInterface.h"
#include "AEnemy.generated.h"

/**
 * @brief 게임에 등장하는 모든 적 캐릭터의 기반이 되는 클래스입니다.
 * @details IEnemyInterface를 구현하여 AI 컨트롤러와 상호작용합니다.
 */
UCLASS(BlueprintType)
class OPERRATION_DDT_API AAEnemy : public ACharacter, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAEnemy();

public:
	/**
	 * @brief 적이 순찰할 경로(PatrolPath)를 반환합니다. (IEnemyInterface 구현)
	 * @return AAPatrolPath* 순찰 경로 액터에 대한 포인터
	 */
	virtual AAPatrolPath* GetPatrolPath() override;

protected:
	virtual void BeginPlay() override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/// @brief 캐릭터의 실제 이동을 처리하는 커스텀 무브먼트 컴포넌트입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MoveMent")
	class UCEnemyMovement* EnemyMovementComponent;

public:
	/// @brief 이 적이 따라갈 순찰 경로입니다. 레벨 에디터에서 지정해야 합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PatrolPath")
	class AAPatrolPath* PatrolPath;
};