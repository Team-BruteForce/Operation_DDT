// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Enemy/APatrolPath.h"
#include "Enemy/CEnemyController.h"
#include "Enemy/Interface/IAIController.h"
#include "CEnemyMovement.generated.h"

/** @brief 적 캐릭터의 이동 로직(순찰 등)을 담당하는 액터 컴포넌트입니다. */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCEnemyMovement : public UActorComponent, public IIAIController
{
	GENERATED_BODY()
public: 
	UCEnemyMovement();

public:
	void SetOwnerController(ACEnemyController* InController) { Owner = InController; }
	
public:
	/** @brief 지정된 순찰 경로를 따라 다음 지점으로 이동을 시작시킵니다. (IIAIController 구현) */
	virtual void Patrol() override;
	
protected:
	virtual void BeginPlay() override;

public:
	/** @brief 다음 순찰 지점으로 인덱스를 증가시킵니다. 마지막 지점이면 처음으로 돌아갑니다. */
	void IncreasePatrolPoint();

public:
	/// @brief 적이 따라갈 순찰 경로(PatrolPath)입니다. 외부에서(주로 레벨 에디터에서) 지정해주어야 합니다.
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="PatrolPath")
	class AAPatrolPath* PatrolPath;

private:
	class ACEnemyController* Owner;
	class AAEnemy* Enemy;
	class ACEnemyController* EnemyController;


};