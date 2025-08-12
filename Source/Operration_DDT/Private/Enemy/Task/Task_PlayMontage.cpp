// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Task/Task_PlayMontage.h"

#include "Global.h"
#include "Enemy/CBoss.h"

/**
 * @brief 상태 진입 시 호출되는 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @param Transition 상태 전환 결과
 * @return EStateTreeRunStatus::Running 상태가 계속 실행됨을 나타냄
 * 
 * 상태에 진입할 때 보스의 공격 애니메이션을 시작합니다.
 * 현재는 기본 공격 태그(FGameplayTag::A)를 사용하여 테스트 공격을 수행합니다.
 */
EStateTreeRunStatus UTask_PlayMontage::EnterState(FStateTreeExecutionContext& Context,
                                                  const FStateTreeTransitionResult& Transition)
{
	// CHelpers::GetComponent<>()
	ACBoss* Boss=Cast<ACBoss>(Owner);
	// Boss->AttackTest(FGameplayTag::A);
	return EStateTreeRunStatus::Running;
}

/**
 * @brief 상태 실행 중 매 프레임 호출되는 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @param DeltaTime 델타 타임
 * @return EStateTreeRunStatus::Running 상태가 계속 실행됨을 나타냄
 * 
 * 공격 애니메이션이 재생되는 동안 상태를 유지합니다.
 * 현재는 단순히 Running 상태를 반환하여 애니메이션이 완료될 때까지 대기합니다.
 */
EStateTreeRunStatus UTask_PlayMontage::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
		return EStateTreeRunStatus::Running;
}
