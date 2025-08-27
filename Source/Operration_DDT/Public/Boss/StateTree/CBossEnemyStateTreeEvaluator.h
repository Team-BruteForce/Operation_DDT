// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "CBossEnemyStateTreeEvaluator.generated.h"

// 전역 상수 정의
namespace BackstepConstants
{
	const int32 MAX_ANGLE_STEPS = 9;
	const float ANGLE_STEPS[MAX_ANGLE_STEPS] = { 0.0f, 15.0f, 30.0f, 45.0f, 60.0f, 90.0f, 120.0f, 150.0f, 180.0f };
	const FColor DEBUG_COLORS[MAX_ANGLE_STEPS] = { 
		FColor::Green, FColor::Yellow, FColor::Orange, FColor::Red, FColor::Purple, 
		FColor::Blue, FColor::Cyan, FColor::Magenta, FColor::White 
	};
}

USTRUCT(BlueprintType)
struct FBossRangeTags {
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.Range.Bucket")) FGameplayTag TooClose;
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.Range.Bucket")) FGameplayTag Melee;
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.Range.Bucket")) FGameplayTag Dash;
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.Range.Bucket")) FGameplayTag Ranged;
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.Range.Bucket")) FGameplayTag OutOfRange;
};

USTRUCT(BlueprintType)
struct FBossTargetState {
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.TargetDirection.Right")) FGameplayTag Right;
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.TargetDirection.Center")) FGameplayTag Center;
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.TargetDirection.Left")) FGameplayTag Left;
};
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

public:
	// ========================================
	// Public Functions
	// ========================================

	/**
	 * @brief 매 프레임 호출되는 틱 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @param DeltaTime 델타 타임
	 * 
	 * StateTree가 실행될 때마다 호출되어 의사결정 데이터를 업데이트합니다.
	 */
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

	/**
	 * @brief StateTree 시작 시 호출되는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 */
	virtual void TreeStart(FStateTreeExecutionContext& Context) override;

	// ========================================
	// Public Variables
	// ========================================
	
	/** AI 컨트롤러 참조 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Owner")
	class AAIController* Owner;
	
	/** 보스 캐릭터 참조 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Boss")
	class APawn* Boss;
	
	/** 타겟(플레이어) 참조 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Target")
	class APawn* Target;

	/** 플레이어와 보스 간의 거리 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Distance", meta=(ToolTip="현재 플레이어와 보스 간의 실시간 거리입니다.\n이 값에 따라 CurrentRangeTag가 자동으로 결정됩니다."))
	float player_ai_dist = 0.0f;
	
	/** 거리별 범위 태그들 */
	UPROPERTY(EditAnywhere, Category="Tags")
	FBossRangeTags Range;
	
	/** 타겟 방향 태그들 */
	UPROPERTY(EditAnywhere, Category="Tags")
	FBossTargetState DirectionTag;
	
	/** 거리별 범위 기준값들 (에디터에서 각각 설정 가능) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Distance Settings", meta=(ClampMin="100.0", ClampMax="1000.0", ToolTip="거리별 범위 기준값들입니다.\n[0]: TooClose 범위 (300)\n[1]: Melee 범위 (400)\n[2]: Dash 범위 (1200) - 백스탭 거리와 연동\n[3]: Ranged 범위 (1600)\n[4]: OutOfRange 범위 (2000)"))
	TArray<float> DistanceThresholds = { 200.0f, 600.0f, 1600.0f, 2000.0f, 2000.0f };
	
	/** 액션 실행 여부 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tag", meta=(ToolTip="보스가 현재 액션을 실행 중인지 여부를 나타냅니다.\nTrue일 때 새로운 액션을 시작하지 않습니다."))
	bool IsAction = false;
	
	/** 현재 범위 태그 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="StateTree Variables", meta=(ToolTip="현재 플레이어와의 거리에 따라 결정된 범위 태그입니다.\nDistanceThresholds 배열의 값에 따라 자동으로 업데이트됩니다."))
	FGameplayTag CurrentRangeTag;

	/** 현재 상태 태그 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tag", meta=(ToolTip="보스의 현재 상태를 나타내는 태그입니다.\nBossStateComponent에서 가져온 상태 정보입니다."))
	FGameplayTag CurrentTag;
	
	/** 현재 타겟 방향 상태 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tag", meta=(ToolTip="플레이어의 이동 방향을 보스 기준으로 분석한 결과입니다.\nLeft: 보스 왼쪽으로 이동\nCenter: 보스 정면으로 이동\nRight: 보스 오른쪽으로 이동"))
	FGameplayTag CurrentTargetDirectionState;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tag", meta=(ToolTip="보스가 회피 할 수있는 안전한 좌표입니다."))
	FVector SafePosition;
	
	/** 회피 위치 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tag", meta=(ToolTip="보스가 회피할 위치입니다. 모션워핑에서 사용됩니다."))
	FVector DodgeLocation;

	/** 타겟 예측 위치 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tag", meta=(ToolTip="보스가 공격할 예측한 위치입니다. 모션워핑에서 사용됩니다."))
	FVector TagetLocation;

private:
	// ========================================
	// Private Functions
	// ========================================
	
	/**
	 * @brief 의사결정에 필요한 데이터를 수집하는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @param DeltaTime 델타 타임
	 * 
	 * 플레이어와 보스 간의 거리 등 의사결정에 필요한 데이터를 수집합니다.
	 */
	void Get_Decision_Data(FStateTreeExecutionContext& Context, const float DeltaTime);
	
	/**
	 * @brief 타겟의 이동 방향을 계산하고 보스 기준으로 좌우 판단합니다.
	 */
	void CalculateTargetMovementDirection();
	
	/**
	 * @brief 예측된 타겟 위치를 계산하는 함수
	 * 
	 * @param OutTargetLocation 계산된 목표 위치
	 * @param OutTargetRotation 계산된 목표 회전
	 * @param DeltaTime 델타 타임
	 * 
	 * 플레이어의 속도, 가속도, 움직임 패턴을 분석하여 미래 위치를 예측합니다.
	 */
	void CalculatePredictedTargetLocation(FVector& OutTargetLocation, FRotator& OutTargetRotation, const float DeltaTime);
	
	// 고급 예측 함수들
	FVector CalculateAdvancedPrediction(const FVector& PlayerVelocity, const FVector& PlayerAcceleration, const float DeltaTime);
	FVector CalculatePatternBasedPrediction(const FVector& PlayerVelocity, float PlayerSpeed);
	float CalculateDistanceBasedPredictionTime();
	FRotator CalculatePredictedRotation(const FVector& PlayerVelocity, float PlayerSpeed);
	
	/**
	 * 플레이어 기준 360도 영역에서 백스탭 가능한 위치를 찾습니다.
	 * Nav Mesh를 사용하여 안전한 위치를 찾습니다.
	 */
	FVector FindBackstepPosition(float BackstepDistance = 1200.0f);
	
	/**
	 * Nav Mesh를 사용하여 특정 방향에서 안전한 위치를 찾습니다.
	 */
	FVector FindSafePositionOnNavMesh(const FVector& Direction, float Distance);
	
	/**
	 * 위치가 플레이어로부터 충분히 떨어져 있는지 확인합니다.
	 */
	bool IsPositionFarFromPlayer(const FVector& Position, float MinDistanceFromPlayer = 1200.0f);
	
	/**
	 * 백스탭 디버그 정보를 출력합니다.
	 */
	void DrawBackstepDebugInfo(const FVector& BackstepPos);
	
	/**
	 * 백스탭 위치 디버그 시각화를 수행합니다.
	 */
	void DrawBackstepPositionDebug(const FVector& SafePosition, const FVector& PlayerLocation, int32 StepIndex);
	
	/**
	 * 백스탭 실패 시 디버그 시각화를 수행합니다.
	 */
	void DrawBackstepFailureDebug(const FVector& BossLocation);
	
	/**
	 * 거리에 따라 적절한 범위 태그를 반환합니다.
	 */
	FGameplayTag EvaluateDistanceState(float Distance);
	
};
