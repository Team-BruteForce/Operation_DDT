// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file CBossEnemyStateTreeEvaluator.cpp
 * @brief 보스 적 StateTree 평가자 구현 파일
 * 
 * 이 파일은 보스 적의 StateTree에서 사용되는 평가자를 구현합니다.
 * 매 프레임 호출되어 보스와 플레이어 간의 거리 계산 및 의사결정 데이터를 수집하고,
 * StateTree의 상태 정보를 로그로 출력합니다.
 * 
 * 주요 기능:
 * - 보스와 플레이어 간 거리 계산
 * - 의사결정 데이터 수집 및 업데이트
 * - StateTree 상태 정보 로그 출력
 * - 타겟(플레이어) 관리
 */

#include "Boss/StateTree/CBossEnemyStateTreeEvaluator.h"
#include "Global.h"
#include "Boss/CBoss.h"
#include "Boss/Component/BossStateComponent.h"

/**
 * @brief 매 프레임 호출되는 틱 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @param DeltaTime 델타 타임
 * 
 * StateTree가 실행될 때마다 호출되어 의사결정 데이터를 업데이트합니다.
 * 현재는 로그 출력과 의사결정 데이터 수집을 수행합니다.
 */
void UCBossEnemyStateTreeEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);

	UBossStateComponent* State=CHelpers::GetComponent<UBossStateComponent>(Boss);
	Get_Decision_Data(Context,DeltaTime);
	CLog::Print( State->GetStateTag().ToString(), 1);
	CLog::Print(FString::Printf(TEXT("Distance : %f"), player_ai_dist), 2);
	CLog::Print("Target : "+Target->GetName(), 3);
	
	// 거리 상태 정보 출력
	CLog::Print("Current Range Tag: " + CurrentRangeTag.ToString(), 4);
	CalculateTargetMovementDirection();
	CurrentTag=State->GetStateTag();
}

void UCBossEnemyStateTreeEvaluator::TreeStart(FStateTreeExecutionContext& Context)
{
	Super::TreeStart(Context);
}

/**
 * @brief 의사결정에 필요한 데이터를 수집하는 함수
 * 
 * @param Context StateTree 실행 컨텍스트
 * @param DeltaTime 델타 타임
 * 
 * 플레이어와 보스 간의 거리 등 의사결정에 필요한 데이터를 수집합니다.
 * 현재는 플레이어를 타겟으로 설정하고 거리를 계산합니다.
 */
void UCBossEnemyStateTreeEvaluator::Get_Decision_Data(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Target = GetWorld()->GetFirstPlayerController()->GetPawn();

	CheckNull(Owner);
	CheckNull(Target);
	
	player_ai_dist = Boss->GetDistanceTo(Target);
	CurrentRangeTag = EvaluateDistanceState(player_ai_dist);
}

void UCBossEnemyStateTreeEvaluator::CalculateTargetMovementDirection()
{
	if (!Target || !Owner || !GetWorld()) return;
	
	// 타겟의 현재 위치와 이전 프레임 위치를 비교하여 이동 방향 계산
	static FVector PreviousTargetLocation = Target->GetActorLocation();
	FVector CurrentTargetLocation = Target->GetActorLocation();
	FVector TargetMovementDirection = (CurrentTargetLocation - PreviousTargetLocation).GetSafeNormal();
	
	// 보스의 오른쪽 방향 벡터
	FVector BossRight = Boss->GetActorRightVector();
	
	// 타겟 이동 방향과 보스 오른쪽 방향의 내적 계산
	float DotProduct = FVector::DotProduct(TargetMovementDirection, BossRight);
	
	// 이동 방향 판단
	FString MovementDirection;
	FColor DirectionColor;
		CLog::Log("CalculateTargetMovementDirection : "+CurrentTargetDirectionState.ToString());
	
	if (DotProduct > 0.1f)
	{
		CurrentTargetDirectionState=DirectionTag.Right;
		DirectionColor = FColor::Green;
	}
	else if (DotProduct < -0.1f)
	{
		CurrentTargetDirectionState=DirectionTag.Left;
		DirectionColor = FColor::Red;
	}
	else
	{
		CurrentTargetDirectionState=DirectionTag.Center;
		DirectionColor = FColor::Yellow;
	}
	
	// 디버그 텍스트 표시
	FString DebugText = FString::Printf(TEXT("Target Movement: %s (Dot: %.2f)"), 
		*MovementDirection, DotProduct);
	DrawDebugString(
		GetWorld(),
		Target->GetActorLocation() + FVector(0, 0, 150),
		DebugText,
		nullptr,
		DirectionColor,
		0.0f
	);
	
	// 타겟 이동 방향 시각화
	if (TargetMovementDirection.Size() > 0.1f)
	{
		DrawDebugLine(
			GetWorld(),
			CurrentTargetLocation,
			CurrentTargetLocation + TargetMovementDirection * 100.0f,
			DirectionColor,
			false,
			-1.0f,
			0,
			3.0f
		);
	}
	
	// 보스의 오른쪽 방향 시각화
	DrawDebugLine(
		GetWorld(),
		Boss->GetActorLocation(),
		Boss->GetActorLocation() + BossRight * 100.0f,
		FColor::Blue,
		false,
		-1.0f,
		0,
		2.0f
	);
	
	// 이전 위치 업데이트
	PreviousTargetLocation = CurrentTargetLocation;
}

FGameplayTag UCBossEnemyStateTreeEvaluator::EvaluateDistanceState(float Distance)
{
	// 거리 300 기준으로 태그별 if문
	if (Distance <= DistanceThreshold) // 300 이하
	{
		return Range.TooClose;
	}
	else if (Distance <= DistanceThreshold * 2) // 600 이하
	{
		return Range.Melee;
	}
	else if (Distance <= DistanceThreshold * 3) // 900 이하
	{
		return Range.Dash;
	}
	else if (Distance <= DistanceThreshold * 4) // 1200 이하
	{
		return Range.Ranged;
	}
	else // 1200 초과
	{
		return Range.OutOfRange;
	}
}
