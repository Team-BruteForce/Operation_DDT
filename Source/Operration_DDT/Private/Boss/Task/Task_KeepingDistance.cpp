/**
 * @file Task_KeepingDistance.cpp
 * @brief 거리 유지 태스크 구현 파일
 * 
 * @details
 * StateTree에서 보스가 플레이어와의 거리를 유지하는 태스크입니다.
 * 이동 컴포넌트를 통해 적절한 거리를 유지하며 이동합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Task/Task_KeepingDistance.h"
#include "Global.h"
#include "Engine/World.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Boss/Component/CBossMovementComponent.h"
#include "Boss/Component/BossDebugComponent.h"



EStateTreeRunStatus UTask_KeepingDistance::EnterState(FStateTreeExecutionContext& Context,
                                                      const FStateTreeTransitionResult& Transition)
{
	if (!Target) return EStateTreeRunStatus::Failed;
	if (!Owner) return EStateTreeRunStatus::Failed;
	Boss=Cast<ACharacter>(Owner);
	
	// 컴포넌트 참조 가져오기
	MovementComponent = CHelpers::GetComponent<UCBossMovementComponent>(Boss);
	DebugComponent = CHelpers::GetComponent<UBossDebugComponent>(Boss);
	
	// DebugComponent에 MovementComponent 참조 설정
	if (DebugComponent && MovementComponent)
	{
		DebugComponent->SetMovementComponent(MovementComponent);
	}
	
	// 초기 궤도 각도 설정 (랜덤)
	OrbitAngle = FMath::RandRange(0.0f, 360.0f);
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus UTask_KeepingDistance::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if (!Target || !Owner) return EStateTreeRunStatus::Failed;
	
	// 컴포넌트에서 모든 계산 수행
	// MovementComponent->MaintainOptimalDistance(DeltaTime, 600, 1600);
	MovementComponent->ExecuteSmartMovement(DeltaTime, 400, 800);
	// 디버그 컴포넌트에서 시각화
	DebugComponent->ExecuteDebugVisualization();

	// 컴포넌트를 사용하여 백스탭 위치 계산
	if (MovementComponent) 
	{
		MovementComponent->FindBackstepPosition();
	}
	
	return EStateTreeRunStatus::Running;
}

void UTask_KeepingDistance::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
		Boss->GetCharacterMovement()->MaxWalkSpeed = 400;
	// 이동 중지
	if (Controller)
	{
		Controller->StopMovement();
	}
}


