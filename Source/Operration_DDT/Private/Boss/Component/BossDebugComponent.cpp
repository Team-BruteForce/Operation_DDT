/**
 * @file BossDebugComponent.cpp
 * @brief 보스 디버그 컴포넌트 구현 파일
 * 
 * @details
 * 보스의 디버그 정보를 표시하는 컴포넌트입니다.
 * 디버그 드로잉과 로그 출력을 통해 보스의 상태를 시각화합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Component/BossDebugComponent.h"
#include "DrawDebugHelpers.h"
#include "Boss/Component/CBossMovementComponent.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
UBossDebugComponent::UBossDebugComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBossDebugComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UBossDebugComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// ===== 거리 디버그 함수들 =====

/**
 * 거리 디버그 정보를 그립니다.
 */
void UBossDebugComponent::DrawDistanceDebug(const FVector& TargetLocation, const FVector& OwnerLocation, 
	float MinDistance, float MaxDistance, float CurrentDistance, const FVector& ClosestPosition)
{
	if (!GetWorld()) return;
	
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
		DistanceColor = FColor::White;
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

// ===== 궤도 디버그 함수들 =====

/**
 * 궤도 디버그 정보를 그립니다.
 */
void UBossDebugComponent::DrawOrbitDebug(const FVector& TargetLocation, const FVector& OwnerLocation,
	const FVector& ArcStart, const FVector& ArcEnd, float ArcRadius,
	const FVector& BossForward, const FVector& BossBackward, const FVector& ClosestPosition)
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

// ===== 백스탭 디버그 함수들 =====

/**
 * 백스탭 디버그 정보를 그립니다.
 */
void UBossDebugComponent::DrawBackstepDebug(const FVector& BackstepPosition, const FVector& PlayerLocation, int32 StepIndex)
{
	if (!GetWorld()) return;
	
	const FColor DebugColors[] = {FColor::Green, FColor::Blue, FColor::Yellow, FColor::Orange, FColor::Red, FColor::Purple};
	FColor Color = DebugColors[FMath::Clamp(StepIndex, 0, 5)];
	
	DrawDebugSphere(GetWorld(), BackstepPosition, 30.0f, 8, Color, false, -1.0f, 0, 3.0f);
	DrawDebugLine(GetWorld(), PlayerLocation, BackstepPosition, Color, false, -1.0f, 0, 2.0f);
}

/**
 * 백스탭 실패 시 디버그 정보를 그립니다.
 */
void UBossDebugComponent::DrawBackstepFailureDebug(const FVector& BossLocation)
{
	if (!GetWorld()) return;
	
	DrawDebugSphere(GetWorld(), BossLocation, 50.0f, 8, FColor::Red, false, -1.0f, 0, 3.0f);
}

void UBossDebugComponent::DrawOrbitDistanceDebug(const FVector& TargetLocation, const FVector& OwnerLocation, 
	float MinDistance, float MaxDistance, const FVector& ClosestPosition)
{
	if (!GetWorld()) return;
	
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
	float CurrentDistance = FVector::Dist(TargetLocation, OwnerLocation);
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

void UBossDebugComponent::DrawArcDebug(const FVector& TargetLocation, const FVector& OwnerLocation,
	const FVector& ArcStart, const FVector& ArcEnd, float ArcRadius,
	const FVector& BossForward, const FVector& BossBackward, const FVector& ClosestPosition)
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

void UBossDebugComponent::SetMovementComponent(class UCBossMovementComponent* InMovementComponent)
{
	MovementComponent = InMovementComponent;
}

void UBossDebugComponent::ExecuteDebugVisualization()
{
	if (!GetWorld() || !MovementComponent) return;
	
	// MovementComponent에서 디버그 데이터 가져오기
	// FVector TargetLocation = MovementComponent->GetDebugTargetLocation();
	// FVector OwnerLocation = MovementComponent->GetDebugOwnerLocation();
	// FVector ClosestPosition = MovementComponent->GetDebugClosestPosition();
	// FVector ArcStart = MovementComponent->GetDebugArcStart();
	// FVector ArcEnd = MovementComponent->GetDebugArcEnd();
	// float ArcRadius = MovementComponent->GetDebugArcRadius();
	// FVector BossForward = MovementComponent->GetDebugBossForward();
	// FVector BossBackward = MovementComponent->GetDebugBossBackward();
	// float CurrentDistance = MovementComponent->GetDebugCurrentDistance();
	
	// 거리 디버그 그리기
	// DrawOrbitDistanceDebug(TargetLocation, OwnerLocation, 200.0f, 400.0f, ClosestPosition);
	//
	// // 호 디버그 그리기
	// DrawArcDebug(TargetLocation, OwnerLocation, ArcStart, ArcEnd, ArcRadius, BossForward, BossBackward, ClosestPosition);
}

   