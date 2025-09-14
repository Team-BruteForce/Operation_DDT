
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "Boss/Data/BossTagStructure.h"
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

	// ========================================
	// Component References
	// ========================================

	/** 보스 상태 컴포넌트 참조 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UBossStateComponent* State;
	/** 보스 능력치 컴포넌트 참조*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UCBossStatusComponent* Status;
	
	/** 타겟팅 컴포넌트 참조 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UCBossTargetingComponent* TargetingComponent;
	
	/** 이동 컴포넌트 참조 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UCBossMovementComponent* MovementComponent;
	
	/** 디버그 컴포넌트 참조 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UBossDebugComponent* DebugComponent;

	/** 비행 컴포넌트 참조 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UFlyingComponent* FlyingComponent;

	/** 웨폰 컴포넌트 참조 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UCBossWeaponComponent* WeaponComponent;

	/** 프로젝타일 컴포넌트 참조 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UBossProjectileComponent* Projectile;

	/** 프로젝타일 컴포넌트 참조 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Components")
	class UCharacterMovementComponent* CharacterMovement;

	/** 플레이어와 보스 간의 거리 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Distance", meta=(ToolTip="현재 플레이어와 보스 간의 실시간 거리입니다.\n이 값에 따라 CurrentRangeTag가 자동으로 결정됩니다."))
	float player_ai_dist = 0.0f;
	
	/** 거리별 범위 태그들 */
	UPROPERTY(EditAnywhere, Category="Tags")
	FBossRangeTags Range;
	
	/** 타겟 방향 태그들 */
	UPROPERTY(EditAnywhere, Category="Tags")
	FBossTargetState DirectionTag;
	
	

	
	/** 액션 실행 여부 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tag", meta=(ToolTip="보스가 오브를 소환한지 여부 트루면 더이상 오브를 실행하지 않습니다."))
	bool IsExitOrb = false;
	
	/** 현재 범위 태그 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="StateTree Variables", meta=(ToolTip="현재 플레이어와의 거리에 따라 결정된 범위 태그입니다.\nDistanceThresholds 배열의 값에 따라 자동으로 업데이트됩니다."))
	FGameplayTag CurrentRangeTag;

	/** 현재 상태 태그 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tag", meta=(ToolTip="보스의 현재 상태를 나타내는 태그입니다.\nBossStateComponent에서 가져온 상태 정보입니다."))
	FGameplayTag CurrentTag;
	
	/** 현재 타겟 방향 상태 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tag", meta=(ToolTip="플레이어의 이동 방향을 보스 기준으로 분석한 결과입니다.\nLeft: 보스 왼쪽으로 이동\nCenter: 보스 정면으로 이동\nRight: 보스 오른쪽으로 이동"))
	FGameplayTag CurrentTargetDirectionState;

	/** 현재 페이즈 상태 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tag", meta=(ToolTip="플레이어의 이동 방향을 보스 기준으로 분석한 결과입니다.\nLeft: 보스 왼쪽으로 이동\nCenter: 보스 정면으로 이동\nRight: 보스 오른쪽으로 이동"))
	FGameplayTag CurrentPaseState;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tag", meta=(ToolTip="보스가 회피 할 수있는 안전한 좌표입니다."))
	FVector SafePosition;
	
	/** 회피 위치 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tag", meta=(ToolTip="보스가 회피할 위치입니다. 모션워핑에서 사용됩니다."))
	FVector DodgeLocation;

	/** 타겟 예측 위치 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tag", meta=(ToolTip="보스가 공격할 예측한 위치입니다. 모션워핑에서 사용됩니다."))
	FVector TagetLocation;
	/** 비행 상태*/
	UPROPERTY(EditDefaultsOnly, Category="Fly")
	bool IsFly=false;

	UPROPERTY(EditDefaultsOnly, Category="Fly")
	bool CanTakeoff = true;
	
	UPROPERTY(EditDefaultsOnly, Category="Fly")
	bool CanLanding = true;

	UPROPERTY(EditDefaultsOnly, Category="percent")
	float ActionPercent=0.0f;
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
	
};
