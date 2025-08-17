// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "Task_PlayMontage.generated.h"

USTRUCT()
struct FPlayMontageInstanceData
{
	GENERATED_BODY()

	// StateTree 파라미터 'IsAction'에 바인딩
	UPROPERTY(EditAnywhere, Category="Params")
	bool IsAction;
};
/**
 * @brief 애니메이션 몽타주를 재생하는 StateTree 태스크 클래스
 * 
 * StateTree에서 보스 캐릭터의 공격 애니메이션을 재생하는 태스크입니다.
 * 상태 진입 시 공격 애니메이션을 시작하고, 애니메이션이 완료될 때까지 상태를 유지합니다.
 */
UCLASS()
class OPERRATION_DDT_API UTask_PlayMontage : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:
	typedef FPlayMontageInstanceData FInstanceDataType;
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
	 * 에디터에서 수정 가능하며 블루프린트에서 읽기/쓰기가 가능합니다.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="MontageData")
	class APawn* Owner;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Tag")
	FGameplayTag SelectedTag;

	UPROPERTY(VisibleAnywhere)
	bool bStarted=false;
	
};
