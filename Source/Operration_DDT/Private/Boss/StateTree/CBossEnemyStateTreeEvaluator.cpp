// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/StateTree/CBossEnemyStateTreeEvaluator.h"
#include "Global.h"
#include "Boss/Component/BossStateComponent.h"
#include "Boss/Component/CBossStatusComponent.h"
#include "Boss/Component/CBossTargetingComponent.h"
#include "Boss/Component/CBossMovementComponent.h"
#include "Boss/Component/BossDebugComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Boss/Component/BossProjectileComponent.h"
#include "Boss/Component/CBossWeaponComponent.h"
#include "Boss/Component/FlyingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCBossEnemyStateTreeEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);
	// 컴포넌트 참조 가져오기
	if (!TargetingComponent) TargetingComponent = CHelpers::GetComponent<UCBossTargetingComponent>(Boss);
	if (!MovementComponent) MovementComponent = CHelpers::GetComponent<UCBossMovementComponent>(Boss);
	if (!DebugComponent) DebugComponent = CHelpers::GetComponent<UBossDebugComponent>(Boss);
	if (!State) State=CHelpers::GetComponent<UBossStateComponent>(Boss);
	if (!Status) Status=CHelpers::GetComponent<UCBossStatusComponent>(Boss);
	if (!Projectile) Projectile=CHelpers::GetComponent<UBossProjectileComponent>(Boss);
	if (!CharacterMovement) CharacterMovement=CHelpers::GetComponent<UCharacterMovementComponent>(Boss);
	if (!FlyingComponent) FlyingComponent=CHelpers::GetComponent<UFlyingComponent>(Boss);
	if (!WeaponComponent) WeaponComponent=CHelpers::GetComponent<UCBossWeaponComponent>(Boss);
	// 컴포넌트에서 계산 결과를 전역변수로 받아오기
	Get_Decision_Data(Context, DeltaTime);
	IsExitOrb= Projectile->ExitOrb;
	// 컴포넌트를 사용하여 타겟 이동 방향 계산
	if (TargetingComponent)
	{
		TargetingComponent->CalculateTargetMovementDirection(CurrentTargetDirectionState, DirectionTag);
	}
	
	// 컴포넌트를 사용하여 타겟 위치 예측
	if (TargetingComponent)
	{
		FVector TargetLoc;
		FRotator TargetRot;
		TargetingComponent->CalculatePredictedTargetLocation(TargetLoc, TargetRot, DeltaTime);
		TagetLocation = TargetLoc;
	}
	
	// 컴포넌트를 사용하여 백스탭 위치 계산
	if (MovementComponent)
	{
		DodgeLocation = MovementComponent->SafePosition;
	}
	
	// 현재 상태 태그 업데이트
	CurrentTag = State->GetStateTag();
	IsFly=FlyingComponent->IsFlying();
	CurrentPaseState=WeaponComponent->GetCurrentWeaponMode();
	CanTakeoff = FlyingComponent->bCanTakeoff;
	CanLanding = FlyingComponent->bCanLanding;
	LastActiveTag=State->LastActiveTag;
}

void UCBossEnemyStateTreeEvaluator::TreeStart(FStateTreeExecutionContext& Context)
{
	Super::TreeStart(Context);

}

/**
 * 의사결정 데이터를 수집하고 거리 계산을 수행합니다.
 */
void UCBossEnemyStateTreeEvaluator::Get_Decision_Data(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Target = GetWorld()->GetFirstPlayerController()->GetPawn();

	CheckNull(Owner);
	CheckNull(Target);
	
	// 기본 거리 계산
	player_ai_dist = Boss->GetDistanceTo(Target);
	
	// 컴포넌트를 사용하여 거리 상태 평가
	CurrentRangeTag = TargetingComponent->EvaluateDistanceState(player_ai_dist);
}


