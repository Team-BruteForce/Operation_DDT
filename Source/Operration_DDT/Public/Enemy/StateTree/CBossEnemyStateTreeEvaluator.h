// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "CBossEnemyStateTreeEvaluator.generated.h"

/**
 * @brief 보스 적 StateTree 평가자 클래스
 * 
 * 보스 적의 StateTree에서 사용되는 평가자 클래스입니다.
 * 보스와 플레이어 간의 거리 계산 및 의사결정 데이터를 관리합니다.
 */
UCLASS()
class OPERRATION_DDT_API UCBossEnemyStateTreeEvaluator : public UStateTreeEvaluatorBlueprintBase
{
	GENERATED_BODY()
	
	/**
	 * @brief 매 프레임 호출되는 틱 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @param DeltaTime 델타 타임
	 * 
	 * StateTree가 실행될 때마다 호출되어 의사결정 데이터를 업데이트합니다.
	 */
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

private:
	/**
	 * @brief 의사결정에 필요한 데이터를 수집하는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @param DeltaTime 델타 타임
	 * 
	 * 플레이어와 보스 간의 거리 등 의사결정에 필요한 데이터를 수집합니다.
	 */
	void Get_Decision_Data(FStateTreeExecutionContext& Context, const float DeltaTime);

public:
	/**
	 * @brief AI 컨트롤러 소유자
	 * 
	 * 이 평가자를 소유하는 AI 컨트롤러입니다.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Owner")
	class AAIController* Owner;
	
	/**
	 * @brief 보스 액터
	 * 
	 * 보스 캐릭터의 액터입니다.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Owner")
	class AActor* Boss;
	
	/**
	 * @brief 타겟 액터
	 * 
	 * 보스가 추적할 타겟(플레이어) 액터입니다.
	 */
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadOnly,Category="Target")
	class AActor* Target;

public:
	/**
	 * @brief 플레이어와 AI 간의 거리
	 * 
	 * 현재 플레이어와 AI 간의 거리를 저장합니다.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Distance")
	float player_ai_dist=0.0f;
	
};
