// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file Task_PlayMontage.cpp
 * @brief StateTree 몽타주 재생 태스크 구현 파일
 * 
 * 이 파일은 StateTree에서 보스 캐릭터의 공격 애니메이션 몽타주를 재생하는 태스크를 구현합니다.
 * 상태 진입 시 공격 애니메이션을 시작하고, 애니메이션이 완료될 때까지 상태를 유지합니다.
 * 노티파이와 연동하여 몽타주 완료를 감지할 수 있습니다.
 * 
 * 주요 기능:
 * - 공격 애니메이션 몽타주 재생
 * - 몽타주 완료 감지 (노티파이 기반)
 * - StateTree 상태 관리
 */

#include "Enemy/Task/Task_PlayMontage.h"

#include "Global.h"
#include "StateTreeExecutionContext.h"
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
	Super::EnterState(Context, Transition);

	
	// 몽타주 실행
	// if(bStarted) return EStateTreeRunStatus::Running;
	// bStarted=true;
	if (ACBoss* Boss = Cast<ACBoss>(Owner))
	Boss->AttackTest(SelectedTag);


	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus UTask_PlayMontage::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	// 몽타주가 재생 중이면 계속 실행
	// 실제로는 노티파이에서 StateTree 이벤트를 보내서 상태를 종료해야 합니다
	return EStateTreeRunStatus::Running;
}
