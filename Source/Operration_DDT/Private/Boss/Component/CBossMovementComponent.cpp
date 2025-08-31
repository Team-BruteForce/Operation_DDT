// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file CBossMovementComponent.cpp
 * @brief 보스 이동 컴포넌트 구현 파일
 * 
 * 이 파일은 보스 캐릭터의 이동과 플레이어 추적 기능을 구현합니다.
 * 플레이어를 찾아서 부드럽게 회전하는 기능을 제공합니다.
 * 
 * 주요 기능:
 * - 플레이어 자동 탐지
 * - 부드러운 회전 보간
 */

#include "Boss/Component/CBossMovementComponent.h"

#include "AIController.h"
#include "Global.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem.h"
#include "DrawDebugHelpers.h"
#include "GameplayTagContainer.h"
#include "Boss/Component/CBossTargetingComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values for this component's properties
UCBossMovementComponent::UCBossMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCBossMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<APawn>(GetOwner());
	CheckNull(Owner);
	AIC = Cast<AAIController>(Owner->GetController());
	TargetingComp=CHelpers::GetComponent<UCBossTargetingComponent>(Owner);
}

// Called every frame
void UCBossMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	SafePosition=FindBackstepPosition();
}

void UCBossMovementComponent::RotateTowardsPlayer(float DeltaTime, float RotationSpeed)
{
	// 플레이어 찾기
	APawn* Player = FindPlayer();
	if (!Player || !Owner) return;
	
	// 플레이어 방향 계산
	FVector PlayerLocation = Player->GetActorLocation();
	FVector CurrentLocation = Owner->GetActorLocation();
	FVector Direction = (PlayerLocation - CurrentLocation).GetSafeNormal();
	
	// 목표 방향으로의 회전 계산
	FRotator TargetRotation = Direction.Rotation();
	FRotator CurrentRotation = Owner->GetActorRotation();
	
	// 부드러운 회전 보간
	float ClampedSpeed = FMath::Clamp(RotationSpeed, 0.0f, 180.0f);
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, ClampedSpeed);
	
	// Yaw만 회전 (수직 회전은 제한)
	NewRotation.Pitch = CurrentRotation.Pitch;
	NewRotation.Roll = CurrentRotation.Roll;
	
	// 회전 적용
	Owner->SetActorRotation(NewRotation);
}

FGameplayTag UCBossMovementComponent::GetPlayerMovementStateTag()
{
	
	static FVector PreviousTargetLocation = FindPlayer()->GetActorLocation();
	FVector CurrentTargetLocation = FindPlayer()->GetActorLocation();
	FVector TargetMovementDirection = (CurrentTargetLocation - PreviousTargetLocation).GetSafeNormal();
	
	FVector BossRight = Owner->GetActorRightVector();
	
	float DotProduct = FVector::DotProduct(TargetMovementDirection, BossRight);
	
	
	if (DotProduct > 0.1f)
	{
		PreviousTargetLocation = CurrentTargetLocation;
		return TargetStateTag.Right;
	}
	else if (DotProduct < -0.1f)
	{
		PreviousTargetLocation = CurrentTargetLocation;
		return TargetStateTag.Left;
	}
	else
	{
		PreviousTargetLocation = CurrentTargetLocation;
		return TargetStateTag.Center;
	}
	
}

APawn* UCBossMovementComponent::FindPlayer()
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

// ===== 백스탭 위치 찾기 함수들 =====

/**
 * 플레이어 기준 360도 영역에서 백스탭 가능한 위치를 찾습니다.
 */
FVector UCBossMovementComponent::FindBackstepPosition()
{
	
	// 게임 시작 시 플레이어가 아직 준비되지 않았을 수 있음
	APawn* Player = FindPlayer();
	if (!Player) return Owner->GetActorLocation();
	
	if (!TargetingComp) return Owner->GetActorLocation();
	
	FVector BossLocation = Owner->GetActorLocation();
	FVector PlayerLocation = Player->GetActorLocation();
	
	// 보스에서 플레이어로의 방향을 뒤집어서 보스 뒤쪽 방향으로 설정 (1순위 방향)
	FVector BossBackwardDirection = (BossLocation - PlayerLocation).GetSafeNormal();
	
	// 각도 단계별로 검사 (0도, 15도, 30도, 45도, 60도, 90도, 120도, 150도, 180도, 210도)
	for (int32 i = 0; i < BackstepConstants::MAX_ANGLE_STEPS; i++)
	{
		// 양쪽 방향 모두 검사 (시계방향, 반시계방향)
		for (int32 direction = -1; direction <= 1; direction += 2)
		{
			float CurrentAngle = BackstepConstants::ANGLE_STEPS[i] * direction;
			FVector SearchDirection = BossBackwardDirection.RotateAngleAxis(CurrentAngle, FVector::UpVector);
			
			// 플레이어로부터 지정된 거리에 있는 위치 계산
			FVector TargetPosition = PlayerLocation + SearchDirection * TargetingComp->DistanceThresholds[3];
			
			// Nav Mesh에서 안전한 위치 찾기
			SafePosition = FindSafePositionOnNavMesh(SearchDirection, FVector::Dist(BossLocation, TargetPosition));
			
			if (SafePosition != BossLocation && IsPositionFarFromPlayer(SafePosition, 0.0f))
			{
				// 디버그 시각화
				DrawDebugSphere(GetWorld(), SafePosition, 30.0f, 8, BackstepConstants::DEBUG_COLORS[i], false, -1.0f, 0, 3.0f);
				DrawDebugLine(GetWorld(), PlayerLocation, SafePosition, BackstepConstants::DEBUG_COLORS[i], false, -1.0f, 0, 2.0f);
				return SafePosition;
			}
		}
	}
	
	// 모든 방향에서 안전한 위치를 찾지 못한 경우
	DrawDebugSphere(GetWorld(), BossLocation, 50.0f, 8, FColor::Red, false, -1.0f, 0, 3.0f);
	return BossLocation;
}

/**
 * Nav Mesh를 사용하여 특정 방향에서 안전한 위치를 찾습니다.
 */
FVector UCBossMovementComponent::FindSafePositionOnNavMesh(const FVector& Direction, float Distance)
{
	if (!Owner || !GetWorld()) return Owner->GetActorLocation();
	
	FVector BossLocation = Owner->GetActorLocation();
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
bool UCBossMovementComponent::IsPositionFarFromPlayer(const FVector& Position, float MinDistanceFromPlayer)
{
	if (!Owner) return false;
	
	APawn* Player = FindPlayer();
	if (!Player) return false;
	
	// MinDistanceFromPlayer가 0이면 기본값 사용
	// if (MinDistanceFromPlayer <= 0.0f)
	// {
	// 	MinDistanceFromPlayer = 600.0f; // 기본 최소 거리
	// }
	
	// 플레이어와의 거리 확인
	float DistanceToPlayer = FVector::Dist(Position, Player->GetActorLocation());
	return DistanceToPlayer >= MinDistanceFromPlayer;
}

/**
 * 플레이어와의 거리를 계산합니다.
 */
void UCBossMovementComponent::CalculatePlayerDistance(FVector& OutTargetLocation, FVector& OutOwnerLocation, 
	FVector& OutDirectionToTarget, float& OutCurrentDistance)
{
	if (!Owner) return;
	
	APawn* Player = FindPlayer();
	if (!Player) return;
	
	OutTargetLocation = Player->GetActorLocation();
	OutOwnerLocation = Owner->GetActorLocation();
	OutDirectionToTarget = (OutTargetLocation - OutOwnerLocation).GetSafeNormal();
	OutCurrentDistance = FVector::Dist(OutOwnerLocation, OutTargetLocation);
}

void UCBossMovementComponent::LookAtTarget(const FVector& DirectionToTarget)
{
	if (!Owner) return;
	
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

// ===== 궤도 이동 함수들 =====
void UCBossMovementComponent::ExecuteOrbitMovement(float DeltaTime, float MinDistance, float MaxDistance)
{
	if (!Owner) return;
	
	// 거리 계산
	FVector TargetLocation, OwnerLocation, DirectionToTarget;
	float CurrentDistance;
	CalculatePlayerDistance(TargetLocation, OwnerLocation, DirectionToTarget, CurrentDistance);
	
	// 거리 허용 오차 설정
	float DistanceTolerance = 50.0f;
	float AdjustedMinDistance = MinDistance - DistanceTolerance;
	float AdjustedMaxDistance = MaxDistance + DistanceTolerance;
	
	// 목표 위치 변수
	FVector ClosestPosition;
	
	// 보스가 적절한 거리에 있으면 원형 궤도로 움직임
	if (CurrentDistance <= AdjustedMaxDistance or CurrentDistance >= AdjustedMinDistance)
	{
		// 플레이어 움직임 방향 감지 및 태그 결정
		FGameplayTag PlayerStateTag = GetPlayerMovementStateTag();
		// 플레이어 상태에 따른 궤도 위치 계산
		CalculateOrbitPosition(DeltaTime, MinDistance, MaxDistance, PlayerStateTag, ClosestPosition);
		
		// 속도 조정
		if (ACharacter* BossChar = Cast<ACharacter>(Owner))
		{
			BossChar->GetCharacterMovement()->MaxWalkSpeed = 200;
		}
	}
	else
	{
		// 속도 조정
		if (ACharacter* BossChar = Cast<ACharacter>(Owner))
		{
			BossChar->GetCharacterMovement()->MaxWalkSpeed = 400;
		}
		
		if (CurrentDistance < AdjustedMinDistance)
		{
			ClosestPosition = TargetLocation - DirectionToTarget * MinDistance;
		}
		else if (CurrentDistance > AdjustedMaxDistance)
		{
			ClosestPosition = TargetLocation - DirectionToTarget * MaxDistance;
		}
		// 거리가 맞지 않으면 목표 위치로 이동

	}
	// AI 컨트롤러로 이동
	if (AIC)
	{
		AIC->MoveToLocation(ClosestPosition, 0);
		
		// 타겟 바라보기
		// LookAtTarget(DirectionToTarget);
	}
	
	// 디버그 데이터 저장
	DebugTargetLocation = TargetLocation;
	DebugOwnerLocation = OwnerLocation;
	DebugClosestPosition = ClosestPosition;
	DebugCurrentDistance = CurrentDistance;
	
	// 호 정보 계산 및 저장
	FVector ArcStart, ArcEnd;
	float ArcRadius;
	FVector BossForward, BossBackward;
	
	if (CurrentDistance <= AdjustedMaxDistance && CurrentDistance >= AdjustedMinDistance)
	{
		FGameplayTag PlayerStateTag = GetPlayerMovementStateTag();
		MoveInOrbit(DeltaTime, MinDistance, MaxDistance, PlayerStateTag,
			ClosestPosition, ArcStart, ArcEnd, ArcRadius, BossForward, BossBackward);
		
		DebugArcStart = ArcStart;
		DebugArcEnd = ArcEnd;
		DebugArcRadius = ArcRadius;
		DebugBossForward = BossForward;
		DebugBossBackward = BossBackward;
	}
}

/**
 * 플레이어 주변에서 궤도 이동 위치를 계산합니다.
 */
void UCBossMovementComponent::CalculateOrbitPosition(float DeltaTime, float MinDistance, float MaxDistance, 
	const FGameplayTag& CurrentTargetStateTag, FVector& OutClosestPosition)
{
	if (!Owner) return;
	
	APawn* Player = FindPlayer();
	if (!Player) return;
	
	FVector TargetLocation = Player->GetActorLocation();
	FVector OwnerLocation = Owner->GetActorLocation();
	FVector DirectionToTarget = (TargetLocation - OwnerLocation).GetSafeNormal();
	float CurrentDistance = FVector::Dist(OwnerLocation, TargetLocation);
	
	// 보스에서 플레이어로의 방향 벡터
	FVector BossToTarget = (TargetLocation - OwnerLocation).GetSafeNormal();
	
	// 보스의 오른쪽 방향
	FVector BossRight = Owner->GetActorRightVector();
	
	// 보스가 실제로 플레이어를 바라보는 방향 (보스의 전방 방향)
	FVector BossForward = Owner->GetActorForwardVector();
	
	// 플레이어를 중심으로 호 그리기
	// 보스가 플레이어를 바라보는 방향의 반대 방향을 기준으로 좌우 30도씩 (총 60도)
	FVector BossBackward = -BossForward; // 보스 전방의 반대 방향
	float StartAngle = -15.0f; // 보스 방향 기준 왼쪽 30도
	float EndAngle = 15.0f;    // 보스 방향 기준 오른쪽 30도
	
	// 호의 반지름을 적절한 거리로 설정
	float ArcRadius;
	if (CurrentDistance < MinDistance)
	{
		ArcRadius = MinDistance; // 너무 가까우면 최소 거리로
	}
	else if (CurrentDistance > MaxDistance)
	{
		ArcRadius = MaxDistance; // 너무 멀면 최대 거리로
	}
	else
	{
		ArcRadius = CurrentDistance; // 적절한 거리면 현재 거리 사용

	}
	// 호의 시작점과 끝점 계산
	FVector ArcStart = TargetLocation + BossBackward.RotateAngleAxis(StartAngle, FVector::UpVector) * ArcRadius;
	FVector ArcEnd = TargetLocation + BossBackward.RotateAngleAxis(EndAngle, FVector::UpVector) * ArcRadius;

	// 궤도 위치를 호의 오른쪽 끝점으로 설정 (보스가 오른쪽으로 회전)
	if (TargetStateTag.Left==CurrentTargetStateTag)
		OutClosestPosition = ArcStart;
	else if (TargetStateTag.Right==CurrentTargetStateTag)
		OutClosestPosition = ArcEnd;
	else
		OutClosestPosition = ArcStart;

	
}

void UCBossMovementComponent::MoveInOrbit(float DeltaTime, float MinDistance, float MaxDistance, 
	const FGameplayTag& CurrentTargetStateTag,
	FVector& OutClosestPosition, FVector& OutArcStart, FVector& OutArcEnd, 
	float& OutArcRadius, FVector& OutBossForward, FVector& OutBossBackward)
{
	if (!Owner) return;
	
	FVector TargetLocation =  FindPlayer()->GetActorLocation();
	FVector OwnerLocation = Owner->GetActorLocation();
	
	// 보스에서 플레이어로의 방향 벡터
	FVector BossToTarget = (TargetLocation - OwnerLocation).GetSafeNormal();
	
	// 보스의 오른쪽 방향
	FVector BossRight = Owner->GetActorRightVector();
	
	// 보스가 실제로 플레이어를 바라보는 방향 (보스의 전방 방향)
	OutBossForward = Owner->GetActorForwardVector();
	
	// 플레이어를 중심으로 호 그리기
	// 보스가 플레이어를 바라보는 방향의 반대 방향을 기준으로 좌우 30도씩 (총 60도)
	OutBossBackward = -OutBossForward; // 보스 전방의 반대 방향
	float StartAngle = -30.0f; // 보스 방향 기준 왼쪽 30도
	float EndAngle = 30.0f;    // 보스 방향 기준 오른쪽 30도
	
	// 호의 반지름을 적절한 거리로 설정
	float CurrentDistance = FVector::Dist(OwnerLocation, TargetLocation);
	if (CurrentDistance < MinDistance)
	{
		OutArcRadius = MinDistance; // 너무 가까우면 최소 거리로
	}
	else if (CurrentDistance > MaxDistance)
	{
		OutArcRadius = MaxDistance; // 너무 멀면 최대 거리로
	}
	else
	{
		OutArcRadius = CurrentDistance; // 적절한 거리면 현재 거리 사용
	}
	
	// 호의 시작점과 끝점 계산
	OutArcStart = TargetLocation + OutBossBackward.RotateAngleAxis(StartAngle, FVector::UpVector) * OutArcRadius;
	OutArcEnd = TargetLocation + OutBossBackward.RotateAngleAxis(EndAngle, FVector::UpVector) * OutArcRadius;

	// 궤도 위치를 호의 오른쪽 끝점으로 설정 (보스가 오른쪽으로 회전)
	if (TargetStateTag.Left == CurrentTargetStateTag){
		OutClosestPosition = OutArcStart;
		CLog::Print(TEXT("Changed to ArcStart (Left)"), 1);
	}
	else if (TargetStateTag.Right == CurrentTargetStateTag){
		OutClosestPosition = OutArcEnd;
		CLog::Print(TEXT("Changed to ArcEnd (Right)"), 2);
	}
	else
	{
		OutClosestPosition = OutArcStart;
		CLog::Print(TEXT("Changed to ArcStart (Default)"), 3);
	}
}

// ===== 디버그 데이터 Getter 함수들 =====

FVector UCBossMovementComponent::GetDebugTargetLocation() const
{
	return DebugTargetLocation;
}

FVector UCBossMovementComponent::GetDebugOwnerLocation() const
{
	return DebugOwnerLocation;
}

FVector UCBossMovementComponent::GetDebugClosestPosition() const
{
	return DebugClosestPosition;
}

FVector UCBossMovementComponent::GetDebugArcStart() const
{
	return DebugArcStart;
}

FVector UCBossMovementComponent::GetDebugArcEnd() const
{
	return DebugArcEnd;
}

float UCBossMovementComponent::GetDebugArcRadius() const
{
	return DebugArcRadius;
}

FVector UCBossMovementComponent::GetDebugBossForward() const
{
	return DebugBossForward;
}

FVector UCBossMovementComponent::GetDebugBossBackward() const
{
	return DebugBossBackward;
}

float UCBossMovementComponent::GetDebugCurrentDistance() const
{
	return DebugCurrentDistance;
}
