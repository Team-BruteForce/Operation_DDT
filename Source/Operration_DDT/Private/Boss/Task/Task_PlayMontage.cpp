/**
 * @file Task_PlayMontage.cpp
 * @brief 보스 몽타주 재생 태스크 구현 파일
 * 
 * @details
 * StateTree에서 보스 캐릭터의 공격 애니메이션 몽타주를 재생하는 태스크의 구현 파일입니다.
 * 상태 진입 시 공격 애니메이션을 시작하고, 애니메이션이 완료될 때까지 상태를 유지합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#include "Boss/Task/Task_PlayMontage.h"

#include "Global.h"
#include "StateTreeExecutionContext.h"
#include "Boss/CBoss.h"
#include "Boss/Component/BossProjectileComponent.h"
#include "Boss/Component/CBossTargetingComponent.h"
#include "Boss/Component/CBossWeaponComponent.h"

/**
 * @brief 상태 진입 시 호출되는 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @param Transition 상태 전환 결과
 * @return EStateTreeRunStatus::Running 상태가 계속 실행됨을 나타냄
 * 
 * 상태에 진입할 때 보스의 공격 애니메이션을 시작합니다.
 */
EStateTreeRunStatus UTask_PlayMontage::EnterState(FStateTreeExecutionContext& Context,
                                                  const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	
	// 몽타주 실행
	if(bStarted) return EStateTreeRunStatus::Running;
	bStarted=true;
	UCBossWeaponComponent* WeaponComp=CHelpers::GetComponent<UCBossWeaponComponent>(Owner);
	UBossProjectileComponent* ProjectileComp=CHelpers::GetComponent<UBossProjectileComponent>(Owner);
	UCBossTargetingComponent* TargetingComp=CHelpers::GetComponent<UCBossTargetingComponent>(Owner);
	
	if (WeaponComp)
	{
		WeaponComp->BossDoAction(SelectedTag);
		// ProjectileComp->ShotProjectile();
		ProjectileComp->ShotProjectileToLocation(TargetingComp->FindPlayer(), 1.0f);
	}
	
	return EStateTreeRunStatus::Running;
}

/**
 * @brief 상태 실행 중 매 프레임 호출되는 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @param DeltaTime 델타 타임
 * @return EStateTreeRunStatus 상태 실행 상태
 * 
 * 공격 애니메이션이 재생되는 동안 상태를 유지합니다.
 */
EStateTreeRunStatus UTask_PlayMontage::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	// 몽타주가 재생 중이면 계속 실행
	// 실제로는 노티파이에서 StateTree 이벤트를 보내서 상태를 종료해야 합니다
	return EStateTreeRunStatus::Running;
}
