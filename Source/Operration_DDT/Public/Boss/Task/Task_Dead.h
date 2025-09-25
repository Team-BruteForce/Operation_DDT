/**
 * @file Task_Dead.h
 * @brief 보스 사망 처리 태스크 헤더 파일
 * 
 * @details
 * StateTree에서 보스의 사망 상태를 처리하는 태스크입니다.
 * 보스의 콜리전을 비활성화하고 사망 애니메이션을 실행합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_Dead.generated.h"

/**
 * @brief 보스 사망 처리 태스크
 * 
 * StateTree에서 보스의 사망 상태를 처리하는 태스크입니다.
 * 보스의 콜리전을 비활성화하고 사망 애니메이션을 실행합니다.
 */
UCLASS()
class OPERRATION_DDT_API UTask_Dead : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tag")
	class AActor* Boss;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tag")
	FGameplayTag SelectedTag;
};
