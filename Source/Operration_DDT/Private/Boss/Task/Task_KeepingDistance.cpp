// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/Task/Task_KeepingDistance.h"
#include "Global.h"
#include "Engine/World.h"
#include "AIController.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"


EStateTreeRunStatus UTask_KeepingDistance::EnterState(FStateTreeExecutionContext& Context,
                                                      const FStateTreeTransitionResult& Transition)
{
	if (!Target) return EStateTreeRunStatus::Failed;
	if (!Owner) return EStateTreeRunStatus::Failed;
	Boss=Cast<ACharacter>(Owner);
	
	// 초기 궤도 각도 설정 (랜덤)
	OrbitAngle = FMath::RandRange(0.0f, 360.0f);
	
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus UTask_KeepingDistance::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if (!Target || !Owner) return EStateTreeRunStatus::Failed;
	
	// 거리 계산
	PlayerDistanceCalculate();
	
	// 보스가 적절한 거리에 있으면 원형 궤도로 움직임
	if (CurrentDistance <= MaxDistance &&CurrentDistance >= MinDistance)
	{
		MoveInOrbit(DeltaTime);
		Boss->GetCharacterMovement()->MaxWalkSpeed=150;
	}
	else
	{
		Boss->GetCharacterMovement()->MaxWalkSpeed=400;
		
		// 거리가 맞지 않으면 목표 위치로 이동
		if (CurrentDistance < MinDistance)
		{
			ClosestPosition = TargetLocation - DirectionToTarget * MinDistance;
		}
		else if (CurrentDistance > MaxDistance)
		{
			ClosestPosition = TargetLocation - DirectionToTarget * MaxDistance;
		}
	}
	if (Controller)
	{
		Controller->MoveToLocation(ClosestPosition);
		LookAtTarget();
	}
	
	// 타겟 주변에 원형 영역 디버그 그리기
	DrawOrbitDebug();
	
	// 호 디버그 그리기
	DrawArcDebug();
	
	return EStateTreeRunStatus::Running;
}

void UTask_KeepingDistance::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
	CLog::Log("UTask_KeepingDistance : ExitState");
	
	// 이동 중지
	if (Controller)
	{
		Controller->StopMovement();
	}
}


void UTask_KeepingDistance::PlayerDistanceCalculate()
{
	TargetLocation = Target->GetActorLocation();
	OwnerLocation = Owner->GetActorLocation();
	// 보스에서 플레이어로의 방향 벡터
	DirectionToTarget = (TargetLocation - OwnerLocation).GetSafeNormal();
	CurrentDistance = FVector::Dist(OwnerLocation, TargetLocation);
}

void UTask_KeepingDistance::MoveInOrbit(float DeltaTime)
{
	if (!Controller || !Owner || !Target) return;
	 
	// 보스에서 플레이어로의 방향 벡터
	FVector BossToTarget = (TargetLocation - OwnerLocation).GetSafeNormal();
	
	// 보스의 오른쪽 방향
	FVector BossRight = Owner->GetActorRightVector();
	
	// 보스가 실제로 플레이어를 바라보는 방향 (보스의 전방 방향)
	BossForward = Owner->GetActorForwardVector();
	
	// 플레이어를 중심으로 호 그리기
	// 보스가 플레이어를 바라보는 방향의 반대 방향을 기준으로 좌우 30도씩 (총 60도)
	BossBackward = -BossForward; // 보스 전방의 반대 방향
	float StartAngle = -30.0f; // 보스 방향 기준 왼쪽 30도
	float EndAngle = 30.0f;    // 보스 방향 기준 오른쪽 30도
	
	// 호의 반지름을 보스의 현재 위치 거리로 설정
	ArcRadius = CurrentDistance;
	
	// 호의 시작점과 끝점 계산
	ArcStart = TargetLocation + BossBackward.RotateAngleAxis(StartAngle, FVector::UpVector) * ArcRadius;
	ArcEnd = TargetLocation + BossBackward.RotateAngleAxis(EndAngle, FVector::UpVector) * ArcRadius;

		// 궤도 위치를 호의 오른쪽 끝점으로 설정 (보스가 오른쪽으로 회전)
		if (TargetStateTag.Left == CurrentTargetStateTag){
			ClosestPosition = ArcStart;
			CLog::Print(TEXT("Changed to ArcStart (Left)"), 1);
		}
		else if (TargetStateTag.Right == CurrentTargetStateTag){
			ClosestPosition = ArcEnd;
			CLog::Print(TEXT("Changed to ArcEnd (Right)"), 2);
		}
		else
		{
			ClosestPosition = ArcStart;
			CLog::Print(TEXT("Changed to ArcStart (Default)"), 3);
		}
}

void UTask_KeepingDistance::LookAtTarget()
{
	if (!Owner || !Target) return;
	
	// 타겟을 바라보는 회전 계산
	FRotator TargetRotation = DirectionToTarget.Rotation();
	
	// 부드러운 회전을 위해 현재 회전에서 목표 회전으로 보간
	FRotator CurrentRotation = Owner->GetActorRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->GetDeltaSeconds(), 5.0f);
	
	// Yaw만 회전 (수직 회전은 제외)
	NewRotation.Pitch = CurrentRotation.Pitch;
	NewRotation.Roll = CurrentRotation.Roll;
	
	Owner->SetActorRotation(NewRotation);
}
void UTask_KeepingDistance::DrawOrbitDebug()
{
	if (!Target || !GetWorld() || !Owner) return;
	// 1. 최소 거리 구체 (빨간색)
	DrawDebugSphere(
		GetWorld(),
		TargetLocation,
		MinDistance,
		12, // 세그먼트 수
		FColor::Red,
		false, // 지속 시간 (false = 매 프레임 그리기)
		-1.0f, // 라이프타임
		0, // 우선순위
		2.0f // 두께
	);
	
	// 2. 최대 거리 구체 (초록색)
	DrawDebugSphere(
		GetWorld(),
		TargetLocation,
		MaxDistance,
		12,
		FColor::Green,
		false,
		-1.0f,
		0,
		2.0f
	);
	
	// 3. 보스가 이동할 목표 위치 표시 (노란색 구체)
	DrawDebugSphere(
		GetWorld(),
		ClosestPosition,
		50.0f, // 크기
		8,
		FColor::Yellow,
		false,
		-1.0f,
		0,
		3.0f
	);
	
	// 4. 보스에서 목표 위치로의 선 그리기 (노란색)
	DrawDebugLine(
		GetWorld(),
		OwnerLocation,
		ClosestPosition,
		FColor::Yellow,
		false,
		-1.0f,
		0,
		3.0f
	);
	
	// 5. 현재 거리 선 그리기
	FColor DistanceColor = FColor::White;
	if (CurrentDistance < MinDistance)
		DistanceColor = FColor::Red;
	else if (CurrentDistance > MaxDistance)
		DistanceColor = FColor::Green;
	else
		DistanceColor = FColor::Blue;
	
	DrawDebugLine(
		GetWorld(),
		TargetLocation,
		OwnerLocation,
		DistanceColor,
		false,
		-1.0f,
		0,
		2.0f
	);
	
	// 6. 거리 텍스트 표시
	FString DistanceText = FString::Printf(TEXT("Distance: %.0f"), CurrentDistance);
	DrawDebugString(
		GetWorld(),
		OwnerLocation + FVector(0, 0, 100),
		DistanceText,
		nullptr,
		DistanceColor,
		0.0f
	);
	
	// 7. 목표 위치 텍스트 표시
	FString TargetText = FString::Printf(TEXT("Target Pos: %.0f, %.0f, %.0f"), 
		ClosestPosition.X, ClosestPosition.Y, ClosestPosition.Z);
	DrawDebugString(
		GetWorld(),
		ClosestPosition + FVector(0, 0, 50),
		TargetText,
		nullptr,
		FColor::Yellow,
		0.0f
	);
}

void UTask_KeepingDistance::DrawArcDebug()
{
	if (!GetWorld()) return;
	
	// 호를 여러 점으로 나누어 그리기
	const int32 NumSegments = 15; // 더 부드러운 호를 위해 세그먼트 수 증가
	float StartAngle = -30.0f; // 보스 방향 기준 왼쪽 30도
	float EndAngle = 30.0f;    // 보스 방향 기준 오른쪽 30도
	
	for (int32 i = 0; i < NumSegments; i++)
	{
		float CurrentAngle = FMath::Lerp(StartAngle, EndAngle, (float)i / (NumSegments - 1));
		float NextAngle = FMath::Lerp(StartAngle, EndAngle, (float)(i + 1) / (NumSegments - 1));
		
		// 보스가 있는 방향을 기준으로 회전한 방향 계산
		FVector CurrentDirection = BossBackward.RotateAngleAxis(CurrentAngle, FVector::UpVector);
		FVector NextDirection = BossBackward.RotateAngleAxis(NextAngle, FVector::UpVector);
		
		FVector StartPoint = TargetLocation + CurrentDirection * ArcRadius;
		FVector EndPoint = TargetLocation + NextDirection * ArcRadius;
		
		// 호 선분 그리기
		DrawDebugLine(
			GetWorld(),
			StartPoint,
			EndPoint,
			FColor::Cyan,
			false,
			-1.0f,
			0,
			3.0f
		);
	}
	
	// 호의 시작점과 끝점 표시
	DrawDebugLine(
		GetWorld(),
		TargetLocation,
		ArcStart,
		FColor::Yellow,
		false,
		-1.0f,
		0,
		2.0f
	);
	
	DrawDebugLine(
		GetWorld(),
		TargetLocation,
		ArcEnd,
		FColor::Yellow,
		false,
		-1.0f,
		0,
		2.0f
	);
	
	// 호의 중심점 표시 (플레이어 위치)
	DrawDebugSphere(
		GetWorld(),
		TargetLocation,
		20.0f,
		8,
		FColor::Red,
		false,
		-1.0f,
		0,
		2.0f
	);
	
	// 보스의 전방 방향 표시
	DrawDebugLine(
		GetWorld(),
		OwnerLocation,
		OwnerLocation + BossForward * 200.0f,
		FColor::Green,
		false,
		-1.0f,
		0,
		3.0f
	);
	
	// 보스가 있는 방향 표시 (호의 기준 방향)
	DrawDebugLine(
		GetWorld(),
		TargetLocation,
		TargetLocation + BossBackward * 200.0f,
		FColor::Purple,
		false,
		-1.0f,
		0,
		3.0f
	);
	
	// 보스 위치 표시
	DrawDebugSphere(
		GetWorld(),
		OwnerLocation,
		15.0f,
		8,
		FColor::Blue,
		false,
		-1.0f,
		0,
		2.0f
	);
	
	// 궤도 위치 표시 (ClosestPosition 사용)
	DrawDebugSphere(
		GetWorld(),
		ClosestPosition,
		25.0f,
		8,
		FColor::Orange,
		false,
		-1.0f,
		0,
		2.0f
	);
}