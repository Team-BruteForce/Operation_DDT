// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "CPatrolTask.generated.h"

/**
 * @brief StateTree에서 적 캐릭터의 순찰(Patrol) 행동을 처리하는 태스크(Task)입니다.
 */
UCLASS()
class OPERRATION_DDT_API UCPatrolTask : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:
	/** @brief 태스크가 시작될 때 한 번 호출되어 순찰 행동을 개시합니다. */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	
	/** @brief 태스크가 활성화된 동안 매 틱 호출됩니다. 이동 완료 여부 등을 확인할 수 있습니다. */
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

	/// @brief 이 태스크가 제어할 적 액터(Enemy)입니다. StateTree 에디터에서 Context Actor로 바인딩해야 합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	class AActor* Enemy;
};