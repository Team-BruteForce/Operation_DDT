// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/StateTree/CBossEnemyStateTreeEvaluator.h"
#include "Global.h"
#include "MotionWarpingComponent.h"
#include "Boss/CBoss.h"
#include "Boss/Component/BossStateComponent.h"
#include "Boss/Component/CBossStatusComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "NavigationSystem.h"

void UCBossEnemyStateTreeEvaluator::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);

	UBossStateComponent* State=CHelpers::GetComponent<UBossStateComponent>(Boss);
	UCBossStatusComponent* Status=CHelpers::GetComponent<UCBossStatusComponent>(Boss);
	Get_Decision_Data(Context,DeltaTime);
	CLog::Print( State->GetStateTag().ToString(), 1);
	CLog::Print(FString::Printf(TEXT("Distance : %f"), player_ai_dist), 2);
	CLog::Print("Target : "+Target->GetName(), 3);
	CLog::Print(Status->BossCurrentStats.CurrentAP, 4);
	
	CLog::Print("Current Range Tag: " + CurrentRangeTag.ToString(), 5);
	CalculateTargetMovementDirection();
	CurrentTag=State->GetStateTag();

	FVector TargetLoc;
	FRotator TargetRot;
	CalculatePredictedTargetLocation(TargetLoc, TargetRot, DeltaTime);
	TagetLocation=TargetLoc;
	
	// 백스탭 위치 찾기 (디버그용)
	FVector BackstepPos = FindBackstepPosition(DistanceThresholds.Num() >= 3 ? DistanceThresholds[2] : 1200.0f);

	DodgeLocation = BackstepPos;
	DrawBackstepDebugInfo(BackstepPos);
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
	
	player_ai_dist = Boss->GetDistanceTo(Target);
	CurrentRangeTag = EvaluateDistanceState(player_ai_dist);
}

/**
 * 타겟의 이동 방향을 계산하고 보스 기준으로 좌우 판단합니다.
 */
void UCBossEnemyStateTreeEvaluator::CalculateTargetMovementDirection()
{
	if (!Target || !Owner || !GetWorld()) return;
	
	static FVector PreviousTargetLocation = Target->GetActorLocation();
	FVector CurrentTargetLocation = Target->GetActorLocation();
	FVector TargetMovementDirection = (CurrentTargetLocation - PreviousTargetLocation).GetSafeNormal();
	
	FVector BossRight = Boss->GetActorRightVector();
	
	float DotProduct = FVector::DotProduct(TargetMovementDirection, BossRight);
	
	FString MovementDirection;
	FColor DirectionColor;
	
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
	
	PreviousTargetLocation = CurrentTargetLocation;
}

/**
 * 플레이어의 속도와 가속도를 기반으로 미래 위치를 예측합니다.
 */
void UCBossEnemyStateTreeEvaluator::CalculatePredictedTargetLocation(FVector& OutTargetLocation, FRotator& OutTargetRotation, const float DeltaTime)
{
	if (!Target || !Boss) return;
	
	FVector PlayerVelocity = Target->GetVelocity();
	float PlayerSpeed = PlayerVelocity.Size();
	
	static FVector PreviousPlayerVelocity = PlayerVelocity;
	FVector PlayerAcceleration = (PlayerVelocity - PreviousPlayerVelocity) / DeltaTime;
	
	FVector PredictedPlayerLocation = CalculateAdvancedPrediction(PlayerVelocity, PlayerAcceleration, DeltaTime);
	
	PreviousPlayerVelocity = PlayerVelocity;
	
	FRotator PredictedRotation = CalculatePredictedRotation(PlayerVelocity, PlayerSpeed);
	
	OutTargetLocation = PredictedPlayerLocation - PredictedRotation.Vector() * 137.f;
	OutTargetRotation = Target->GetActorRotation();
}

/**
 * 4가지 예측 방법을 가중 평균으로 조합하여 최종 예측 위치를 계산합니다.
 */
FVector UCBossEnemyStateTreeEvaluator::CalculateAdvancedPrediction(const FVector& PlayerVelocity, const FVector& PlayerAcceleration, const float DeltaTime)
{
	FVector CurrentLocation = Target->GetActorLocation();
	float PlayerSpeed = PlayerVelocity.Size();
	
	float BasePredictionTime = FMath::Clamp(PlayerSpeed / 600.0f, 0.3f, 1.5f);
	FVector BasicPrediction = CurrentLocation + PlayerVelocity * BasePredictionTime;
	
	FVector AccelerationPrediction = CurrentLocation + 
		PlayerVelocity * BasePredictionTime + 
		0.5f * PlayerAcceleration * BasePredictionTime * BasePredictionTime;
	
	FVector PatternPrediction = CalculatePatternBasedPrediction(PlayerVelocity, PlayerSpeed);
	
	float DistanceBasedTime = CalculateDistanceBasedPredictionTime();
	FVector DistancePrediction = CurrentLocation + PlayerVelocity * DistanceBasedTime;
	
	float WeightBasic = 0.3f;
	float WeightAccel = 0.4f;
	float WeightPattern = 0.2f;
	float WeightDistance = 0.1f;
	
	FVector FinalPrediction = BasicPrediction * WeightBasic + 
		AccelerationPrediction * WeightAccel + 
		PatternPrediction * WeightPattern + 
		DistancePrediction * WeightDistance;
	
	return FinalPrediction;
}

/**
 * 최근 10프레임의 속도 데이터를 분석하여 움직임 패턴을 예측합니다.
 */
FVector UCBossEnemyStateTreeEvaluator::CalculatePatternBasedPrediction(const FVector& PlayerVelocity, float PlayerSpeed)
{
	static TArray<FVector> RecentVelocities;
	static const int32 MaxHistorySize = 10;
	
	RecentVelocities.Add(PlayerVelocity);
	if (RecentVelocities.Num() > MaxHistorySize) {
		RecentVelocities.RemoveAt(0);
	}
	
	FVector AverageVelocity = FVector::ZeroVector;
	for (const FVector& Vel : RecentVelocities) {
		AverageVelocity += Vel;
	}
	AverageVelocity /= RecentVelocities.Num();
	
	FVector VelocityTrend = FVector::ZeroVector;
	if (RecentVelocities.Num() >= 2) {
		VelocityTrend = (RecentVelocities.Last() - RecentVelocities[0]) / RecentVelocities.Num();
	}
	
	float PredictionTime = FMath::Clamp(PlayerSpeed / 600.0f, 0.3f, 1.5f);
	FVector PatternPrediction = Target->GetActorLocation() + 
		AverageVelocity * PredictionTime + 
		VelocityTrend * PredictionTime * 0.5f;
	
	return PatternPrediction;
}

/**
 * 보스와 플레이어 간의 거리에 따라 예측 시간을 조정합니다.
 */
float UCBossEnemyStateTreeEvaluator::CalculateDistanceBasedPredictionTime()
{
	if (!Target || !Boss) return 0.5f;
	
	float Distance = Boss->GetDistanceTo(Target);
	
	if (Distance > 1000.0f) return 1.2f;
	else if (Distance > 600.0f) return 1.0f;
	else if (Distance > 300.0f) return 0.7f;
	else return 0.4f;
}

/**
 * 플레이어의 이동 방향을 기반으로 회전을 예측하고 부드럽게 보간합니다.
 */
FRotator UCBossEnemyStateTreeEvaluator::CalculatePredictedRotation(const FVector& PlayerVelocity, float PlayerSpeed)
{
	FRotator PredictedRotation = Target->GetActorRotation();
	
	if (PlayerSpeed > 50.0f) {
		FVector MoveDirection = PlayerVelocity.GetSafeNormal();
		PredictedRotation = MoveDirection.Rotation();
		
		FRotator CurrentRotation = Target->GetActorRotation();
		PredictedRotation = FMath::RInterpTo(CurrentRotation, PredictedRotation, GetWorld()->GetDeltaSeconds(), 3.0f);
	}
	
	return PredictedRotation;
}

/**
 * 거리에 따라 적절한 범위 태그를 반환합니다.
 */
FGameplayTag UCBossEnemyStateTreeEvaluator::EvaluateDistanceState(float Distance)
{
	// 거리 임계값 배열이 비어있거나 크기가 맞지 않으면 기본값 반환
	if (DistanceThresholds.Num() < 4)
	{
		return Range.OutOfRange;
	}
	
	// 거리에 따라 적절한 태그 반환
	if (Distance <= DistanceThresholds[0]) // TooClose
	{
		return Range.TooClose;
	}
	else if (Distance <= DistanceThresholds[1]) // Melee
	{
		return Range.Melee;
	}
	else if (Distance <= DistanceThresholds[2]) // Dash
	{
		return Range.Dash;
	}
	else if (Distance <= DistanceThresholds[3]) // Ranged
	{
		return Range.Ranged;
	}
	else // OutOfRange
	{
		return Range.OutOfRange;
	}
}

/**
 * 플레이어 기준 360도 영역에서 백스탭 가능한 위치를 찾습니다.
 * 플레이어로부터 지정된 거리에 있는 안전한 위치를 찾습니다.
 * 보스 뒤쪽 방향을 1순위로 하고 점점 각도를 넓혀가면서 찾습니다.
 * Nav Mesh를 사용하여 안전한 위치를 찾습니다.
 */
FVector UCBossEnemyStateTreeEvaluator::FindBackstepPosition(float BackstepDistance)
{
	if (!Boss || !Target || !GetWorld()) return Boss->GetActorLocation();
	
	FVector BossLocation = Boss->GetActorLocation();
	FVector PlayerLocation = Target->GetActorLocation();
	
	// 보스에서 플레이어로의 방향을 뒤집어서 보스 뒤쪽 방향으로 설정 (1순위 방향)
	FVector BossBackwardDirection = (BossLocation - PlayerLocation).GetSafeNormal();
	
	for (int32 i = 0; i < BackstepConstants::MAX_ANGLE_STEPS; i++)
	{
		// 양쪽 방향 모두 검사 (시계방향, 반시계방향)
		for (int32 direction = -1; direction <= 1; direction += 2)
		{
			float CurrentAngle = BackstepConstants::ANGLE_STEPS[i] * direction;
			FVector SearchDirection = BossBackwardDirection.RotateAngleAxis(CurrentAngle, FVector::UpVector);
			
			// 플레이어로부터 지정된 거리에 있는 위치 계산
			FVector TargetPosition = PlayerLocation + SearchDirection * BackstepDistance;
			
			// Nav Mesh에서 안전한 위치 찾기
			SafePosition = FindSafePositionOnNavMesh(SearchDirection, FVector::Dist(BossLocation, TargetPosition));
			
			if (SafePosition != BossLocation && IsPositionFarFromPlayer(SafePosition, 0.0f))
			{
				// 디버그 시각화
				DrawBackstepPositionDebug(SafePosition, PlayerLocation, i);
				return SafePosition;
			}
		}
	}
	
	// 모든 방향에서 안전한 위치를 찾지 못한 경우
	DrawBackstepFailureDebug(BossLocation);
	return BossLocation;
}

/**
 * Nav Mesh를 사용하여 특정 방향에서 안전한 위치를 찾습니다.
 */
FVector UCBossEnemyStateTreeEvaluator::FindSafePositionOnNavMesh(const FVector& Direction, float Distance)
{
	if (!Boss || !GetWorld()) return Boss->GetActorLocation();
	
	FVector BossLocation = Boss->GetActorLocation();
	FVector TargetLocation = BossLocation + Direction * Distance;
	
	// Nav Mesh에서 가장 가까운 안전한 위치 찾기
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	if (NavSystem)
	{
		FNavLocation ProjectedLocation;
		if (NavSystem->ProjectPointToNavigation(TargetLocation, ProjectedLocation))
		{
			// 디버그: Nav Mesh 위치 (파란색)
			DrawDebugLine(GetWorld(), BossLocation, ProjectedLocation.Location, FColor::Blue, false, -1.0f, 0, 2.0f);
			return ProjectedLocation.Location;
		}
		else
		{
			// Nav Mesh 프로젝션 실패 시 보스 위치 반환 (낭떨어지 방지)
			DrawDebugSphere(GetWorld(), TargetLocation, 50.0f, 8, FColor::Red, false, -1.0f, 0, 3.0f);
			return BossLocation; // 보스 현재 위치 반환
		}
	}
	
	// Nav System이 없을 때도 보스 위치 반환
	return BossLocation;
}

/**
 * 위치가 플레이어로부터 충분히 떨어져 있는지 확인합니다.
 */
bool UCBossEnemyStateTreeEvaluator::IsPositionFarFromPlayer(const FVector& Position, float MinDistanceFromPlayer)
{
	if (!Boss || !Target) return false;
	
	// MinDistanceFromPlayer가 0이면 DistanceThresholds[2] 사용
	if (MinDistanceFromPlayer <= 0.0f && DistanceThresholds.Num() >= 3)
	{
		MinDistanceFromPlayer = DistanceThresholds[2];
	}
	
	// 플레이어와의 거리 확인
	float DistanceToPlayer = FVector::Dist(Position, Target->GetActorLocation());
	return DistanceToPlayer >= MinDistanceFromPlayer;
}

/**
 * 백스탭 디버그 정보를 출력합니다.
 */
void UCBossEnemyStateTreeEvaluator::DrawBackstepDebugInfo(const FVector& BackstepPos)
{
	// 플레이어와의 거리도 함께 출력
	float DistanceToBackstep = FVector::Dist(BackstepPos, Target->GetActorLocation());
}

/**
 * 백스탭 위치 디버그 시각화를 수행합니다.
 */
void UCBossEnemyStateTreeEvaluator::DrawBackstepPositionDebug(const FVector& SafePositions, const FVector& PlayerLocation, int32 StepIndex)
{
	DrawDebugSphere(GetWorld(), SafePositions, 30.0f, 8, BackstepConstants::DEBUG_COLORS[StepIndex], false, -1.0f, 0, 3.0f);
	DrawDebugLine(GetWorld(), PlayerLocation, SafePosition, BackstepConstants::DEBUG_COLORS[StepIndex], false, -1.0f, 0, 2.0f);
}

/**
 * 백스탭 실패 시 디버그 시각화를 수행합니다.
 */
void UCBossEnemyStateTreeEvaluator::DrawBackstepFailureDebug(const FVector& BossLocation)
{
	DrawDebugSphere(GetWorld(), BossLocation, 50.0f, 8, FColor::Red, false, -1.0f, 0, 3.0f);
}
