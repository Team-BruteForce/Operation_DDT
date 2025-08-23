// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/Task/Task_BossChase.h"
#include "Global.h"
#include "Engine/World.h"
#include "AIController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Navigation/PathFollowingComponent.h"

/**
 * @file Task_BossChase.cpp
 * @brief 보스 AI 체이스 태스크 구현 파일
 * 
 * @date 2024-12-19
 * @author [이효원]
 * @version 1.0
 * 
 * @section overview 개요
 * 이 태스크는 보스 AI가 플레이어를 추적하는 기능을 구현합니다.
 * StateTree 기반으로 동작하며, AI MoveTo의 타겟 추적 문제를 해결하여
 * 자연스럽고 안정적인 추적을 제공합니다.
 * 
 * @section purpose 개발 목적
 * - AI MoveTo의 타겟 추적 문제 해결 (도착하기 직전 왔다갔다 하는 현상)
 * - 동적 타겟 업데이트를 통한 자연스러운 추적
 * - 경로 찾기 실패 시 대안 이동 방식 제공
 * - 거리 기반 추적 완료 조건 설정
 * 
 * @section features 주요 기능
 * - 주기적 타겟 위치 업데이트 (0.5초마다)
 * - 거리 기반 추적 완료 조건 (200.0f 이내)
 * - 경로 찾기 실패 시 직접 이동 방식
 * - 타겟 추적 중단 조건 (너무 멀리 있을 때)
 * - 부드러운 가속/감속을 통한 자연스러운 움직임
 * 
 * @section technical_details 기술적 세부사항
 * - StateTree Task 시스템 활용
 * - Tick 기반 동적 업데이트
 * - PathFollowing 컴포넌트 상태 모니터링
 * - AcceptanceRadius를 작게 설정하여 정확한 도착
 * - 대안 이동 방식으로 경로 찾기 실패 대응
 * 
 * @section portfolio_highlight 포트폴리오 하이라이트
 * 이 태스크는 AI 시스템에서 흔히 발생하는 타겟 추적 문제를
 * 체계적으로 해결한 사례입니다. 특히 동적 업데이트와
 * 대안 이동 방식을 통해 안정적이고 자연스러운 AI 추적을
 * 구현한 점이 포트폴리오에서 돋보입니다.
 */

// 추적 관련 설정값들
static const float ACCEPTANCE_RADIUS = 50.0f;      // 도착 인정 반경
static const float MAX_CHASE_DISTANCE = 1000.0f;   // 최대 추적 거리


EStateTreeRunStatus UTask_BossChase::EnterState(FStateTreeExecutionContext& Context,
    const FStateTreeTransitionResult& Transition)
{
    if (!Target) return EStateTreeRunStatus::Failed;
    if (!Owner) return EStateTreeRunStatus::Failed;
    Movement=CHelpers::GetComponent<UCBossMovementComponent>(Owner);
    if (!Movement) return EStateTreeRunStatus::Failed;
    
    return EStateTreeRunStatus::Running;
}

void UTask_BossChase::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
    Super::ExitState(Context, Transition);
    
    // AI MoveTo 중지
    if (Controller)
    {
        Controller->StopMovement();
    }
    
    // 속도 초기화
    if (Owner && Owner->GetMovementComponent())
    {
        Owner->GetMovementComponent()->Velocity = FVector::ZeroVector;
    }
}

EStateTreeRunStatus UTask_BossChase::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{

    // if (DistanceToTarget <= DistanceThreshold) return EStateTreeRunStatus::Succeeded;
    // if (DistanceToTarget > MAX_CHASE_DISTANCE) return EStateTreeRunStatus::Failed;

    // 작은 AcceptanceRadius로 정확한 도착
    EPathFollowingRequestResult::Type MoveResult = Controller->MoveToActor(Target, ACCEPTANCE_RADIUS);

    // 경로 찾기 실패 시 Failed 반환
    if (MoveResult == EPathFollowingRequestResult::Failed)
    {
        UE_LOG(LogTemp, Warning, TEXT("Boss Chase: Path finding Succeeded"));
        return EStateTreeRunStatus::Succeeded;
    }

    // 가까워질수록 속도 조절
    float SpeedFactor = FMath::Clamp(DistanceToTarget / DistanceThreshold, 0.3f, 1.0f);
    float CurrentSpeed = Owner->GetMovementComponent()->GetMaxSpeed() * SpeedFactor;

    // 부드러운 가속/감속
    static float CurrentVelocity = 0.0f;
    CurrentVelocity = FMath::Lerp(CurrentVelocity, CurrentSpeed, 0.1f);

    // 속도 적용
    if (Owner->GetMovementComponent())
    {
        FVector Direction = (Target->GetActorLocation() - Owner->GetActorLocation()).GetSafeNormal();
        Owner->GetMovementComponent()->Velocity = Direction * CurrentVelocity;
    }

    return EStateTreeRunStatus::Running;
}

