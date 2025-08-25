/**
 * @file Task_PlayMontage.h
 * @brief 보스 몽타주 재생 태스크 헤더 파일
 * 
 * @details
 * StateTree에서 보스 캐릭터의 공격 애니메이션을 재생하는 태스크를 정의합니다.
 * 상태 진입 시 공격 애니메이션을 시작하고, 애니메이션이 완료될 때까지 상태를 유지합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_PlayMontage.generated.h"

/**
 * @brief 보스 몽타주 재생 태스크
 * 
 * StateTree에서 보스 캐릭터의 공격 애니메이션을 재생하는 태스크입니다.
 * 상태 진입 시 공격 애니메이션을 시작하고, 애니메이션이 완료될 때까지 상태를 유지합니다.
 */
UCLASS()
class OPERRATION_DDT_API UTask_PlayMontage : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

protected:
	/**
	 * @brief 상태 진입 시 호출되는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @param Transition 상태 전환 결과
	 * @return EStateTreeRunStatus 상태 실행 상태
	 * 
	 * 상태에 진입할 때 보스의 공격 애니메이션을 시작합니다.
	 */
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	/**
	 * @brief 상태 실행 중 매 프레임 호출되는 함수
	 * 
	 * @param Context StateTree 실행 컨텍스트
	 * @param DeltaTime 델타 타임
	 * @return EStateTreeRunStatus 상태 실행 상태
	 * 
	 * 공격 애니메이션이 재생되는 동안 상태를 유지합니다.
	 */
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;

public:
	/**
	 * @brief 태스크를 소유하는 Pawn 객체
	 * 
	 * 애니메이션을 재생할 보스 캐릭터의 Pawn 객체입니다.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="MontageData")
	class APawn* Owner;

	/**
	 * @brief 선택된 태그
	 * 
	 * 재생할 몽타주를 식별하는 게임플레이 태그입니다.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tag")
	FGameplayTag SelectedTag;

	/**
	 * @brief 시작 여부
	 * 
	 * 몽타주가 시작되었는지 여부를 나타냅니다.
	 */
	UPROPERTY(VisibleAnywhere)
	bool bStarted=false;
	
};
