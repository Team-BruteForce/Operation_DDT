/**
 * @file Task_AroundAttack.cpp
 * @brief 주변 공격 태스크 구현 파일
 * 
 * @details
 * StateTree에서 보스가 주변을 돌면서 공격하는 태스크입니다.
 * 스플라인 반원 이동과 연속 프로젝타일 발사를 실행합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Task/Task_AroundAttack.h"
#include "Global.h"
#include "Boss/Component/BossProjectileComponent.h"
#include "Boss/Component/FlyingComponent.h"

EStateTreeRunStatus UTask_AroundAttack::EnterState(FStateTreeExecutionContext& Context,
                                                   const FStateTreeTransitionResult& Transition)
{
	UFlyingComponent* FlyingComponent=CHelpers::GetComponent<UFlyingComponent>(Boss);
	UBossProjectileComponent* Projectile=CHelpers::GetComponent<UBossProjectileComponent>(Boss);
	Projectile->SpawnProjectileContinuously(0.5f);
	FlyingComponent->StartSplineSemicircleMovement();
	return EStateTreeRunStatus::Running;
}

void UTask_AroundAttack::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	UFlyingComponent* FlyingComponent = CHelpers::GetComponent<UFlyingComponent>(Boss);
	UBossProjectileComponent* Projectile = CHelpers::GetComponent<UBossProjectileComponent>(Boss);
	
	if (FlyingComponent)
	{
		FlyingComponent->ResetSplineSemicircleMovement();
	}
	 
	if (Projectile)
	{
		Projectile->CancelProjectileContinuousSpawning();
	}
}
