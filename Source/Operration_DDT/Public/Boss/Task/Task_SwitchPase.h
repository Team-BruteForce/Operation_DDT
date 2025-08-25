/**
 * @file Task_SwitchPase.h
 * @brief 보스 페이즈 전환 태스크 헤더 파일
 * 
 * @details
 * 보스의 페이즈를 전환하는 태스크를 정의합니다.
 * StateTree 기반 태스크 시스템을 사용하여 페이즈를 관리합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_SwitchPase.generated.h"

/**
 * @brief 보스 페이즈 전환 태스크
 * 
 * 보스의 페이즈를 전환하는 태스크입니다.
 * StateTree 기반 태스크 시스템을 사용하여 페이즈를 관리합니다.
 */
UCLASS()
class OPERRATION_DDT_API UTask_SwitchPase : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
	/**
	 * @brief 상태 진입 시 호출되는 함수
	 */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	
public:
	/**
	 * @brief 소유자 Pawn 참조
	 */
	UPROPERTY(EditAnywhere,Category="Owner")
	class APawn* Owner;
	
	/**
	 * @brief 소유 상태 태그
	 * 
	 * 현재 소유하고 있는 상태 태그입니다.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="State", meta=(AllowBinding))
	FGameplayTag OwningStateTag;

	/**
	 * @brief 시작 여부
	 * 
	 * 페이즈 전환이 시작되었는지 여부를 나타냅니다.
	 */
	UPROPERTY(VisibleAnywhere)
	bool bStarted=false;
};
