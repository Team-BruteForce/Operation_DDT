// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file CBossTargetingComponent.cpp
 * @brief 보스 타겟팅 컴포넌트 구현 파일
 * 
 * 이 파일은 보스 캐릭터의 타겟팅 로직을 구현합니다.
 * 플레이어 위치 예측, 이동 방향 분석, 거리 상태 평가 등을 처리합니다.
 * 
 * 주요 기능:
 * - 플레이어 위치 예측 (속도, 가속도 기반)
 * - 플레이어 이동 방향 분석 (좌/우/중앙)
 * - 거리 상태 평가 (TooClose, Melee, Dash, Ranged, OutOfRange)
 * - 고급 예측 알고리즘 (패턴 기반, 거리 기반)
 */

#include "Boss/Component/CBossTargetingComponent.h"
#include "Global.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "DrawDebugHelpers.h"
#include "Boss/Component/CBossStatusComponent.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties

UCBossTargetingComponent::UCBossTargetingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCBossTargetingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Owner = Cast<APawn>(GetOwner());
	CheckNull(Owner);
	Target = FindPlayer();
}

// Called every frame
void UCBossTargetingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// 타겟이 없으면 다시 찾기
	if (!Target)
	{
		Target = FindPlayer();
	}
}

/**
 * 플레이어를 찾는 함수
 */
APawn* UCBossTargetingComponent::FindPlayer()
{
	if (!GetWorld()) return nullptr;
	
	// 첫 번째 플레이어 컨트롤러 찾기
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (!PlayerController) return nullptr;
	
	// 플레이어 컨트롤러의 Pawn 가져오기
	APawn* PlayerPawn = PlayerController->GetPawn();
	if (!PlayerPawn) return nullptr;
	
	return PlayerPawn;
}

/**
 * 플레이어의 속도와 가속도를 기반으로 미래 위치를 예측합니다.
 */
void UCBossTargetingComponent::CalculatePredictedTargetLocation(FVector& OutTargetLocation, FRotator& OutTargetRotation, const float DeltaTime)
{
	if (!Target || !Owner) return;
	
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
FVector UCBossTargetingComponent::CalculateAdvancedPrediction(const FVector& PlayerVelocity, const FVector& PlayerAcceleration, const float DeltaTime)
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
FVector UCBossTargetingComponent::CalculatePatternBasedPrediction(const FVector& PlayerVelocity, float PlayerSpeed)
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
float UCBossTargetingComponent::CalculateDistanceBasedPredictionTime()
{
	if (!Target || !Owner) return 0.5f;
	
	float Distance = Owner->GetDistanceTo(Target);
	
	if (Distance > 1000.0f) return 1.2f;
	else if (Distance > 600.0f) return 1.0f;
	else if (Distance > 300.0f) return 0.7f;
	else return 0.4f;
}

/**
 * 플레이어의 이동 방향을 기반으로 회전을 예측하고 부드럽게 보간합니다.
 */
FRotator UCBossTargetingComponent::CalculatePredictedRotation(const FVector& PlayerVelocity, float PlayerSpeed)
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
 * 타겟의 이동 방향을 계산하고 보스 기준으로 좌우 판단합니다. (매개변수 버전)
 */
void UCBossTargetingComponent::CalculateTargetMovementDirection(FGameplayTag& OutDirectionState, const FBossTargetState& DirectionTags)
{
	if (!Target || !Owner || !GetWorld()) return;
	
	static FVector PreviousTargetLocation = Target->GetActorLocation();
	FVector CurrentTargetLocation = Target->GetActorLocation();
	FVector TargetMovementDirection = (CurrentTargetLocation - PreviousTargetLocation).GetSafeNormal();
	
	FVector BossRight = Owner->GetActorRightVector();
	
	float DotProduct = FVector::DotProduct(TargetMovementDirection, BossRight);
	
	FString MovementDirection;
	FColor DirectionColor;
	
	if (DotProduct > 0.1f)
	{
		OutDirectionState = DirectionTags.Right;
		DirectionColor = FColor::Green;
	}
	else if (DotProduct < -0.1f)
	{
		OutDirectionState = DirectionTags.Left;
		DirectionColor = FColor::Red;
	}
	else
	{
		OutDirectionState = DirectionTags.Center;
		DirectionColor = FColor::Yellow;
	}
	
	
	PreviousTargetLocation = CurrentTargetLocation;
}

/**
 * 거리에 따라 적절한 범위 태그를 반환합니다.
 */
FGameplayTag UCBossTargetingComponent::EvaluateDistanceState(float Distance)
{
	// 거리 임계값 배열이 비어있거나 크기가 맞지 않으면 기본값 반환
	if (DistanceThresholds.Num() < 4)
	{
		// 기본 태그 반환
		return RangeTags.OutOfRange;
	}
	
	// 거리에 따라 적절한 태그 반환
	if (Distance <= DistanceThresholds[0]) // TooClose
	{
		return RangeTags.TooClose;
	}
	else if (Distance <= DistanceThresholds[1]) // Melee
	{
		return RangeTags.Melee;
	}
	else if (Distance <= DistanceThresholds[2]) // Dash
	{
		return RangeTags.Dash;
	}
	else if (Distance <= DistanceThresholds[3]) // Ranged
	{
		return RangeTags.Ranged;
	}
	else // OutOfRange
	{
		return RangeTags.OutOfRange;
	}
}