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

#include <string>

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
#include "Boss/Component/FlyingComponent.h"
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
	Owner = Cast<ACharacter>(GetOwner());
	CheckNull(Owner);
	AIC = Cast<AAIController>(Owner->GetController());
	TargetingComp=CHelpers::GetComponent<UCBossTargetingComponent>(Owner);
	FlyingComponent=CHelpers::GetComponent<UFlyingComponent>(Owner);
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
	CLog::Log("=== 플레이어 행동 패턴 예측 시작 ===");
	APawn* Player = FindPlayer();
	if (!Player || !Owner) 
	{
		CLog::Log("Player 또는 Owner가 null - Center 반환");
		return TargetStateTag.Center;
	}
	
	FVector CurrentTargetLocation = Player->GetActorLocation();
	FVector BossLocation = Owner->GetActorLocation();
	
	// 보스에서 플레이어로의 방향
	FVector BossToPlayer = (CurrentTargetLocation - BossLocation).GetSafeNormal();
	FVector BossForward = Owner->GetActorForwardVector();
	FVector BossRight = Owner->GetActorRightVector();
	
	// 플레이어가 보스의 어느 쪽에 있는지 계산
	float PlayerSideDot = FVector::DotProduct(BossToPlayer, BossRight);
	
	// 플레이어와 보스 사이의 거리
	float DistanceToPlayer = FVector::Dist(BossLocation, CurrentTargetLocation);
	
	// 플레이어의 속도 계산 (움직임 강도)
	FVector PlayerVelocity = Player->GetVelocity();
	float PlayerSpeed = PlayerVelocity.Size();
	
	CLog::Log("플레이어 속도: " + FString::SanitizeFloat(PlayerSpeed));
	CLog::Log("플레이어 위치 (보스 기준): " + FString::SanitizeFloat(PlayerSideDot));
	CLog::Log("거리: " + FString::SanitizeFloat(DistanceToPlayer));
	
	// 플레이어 움직임 히스토리 (static으로 유지)
	static TArray<FVector> PlayerPositionHistory;
	static TArray<float> PlayerSpeedHistory;
	static int32 MaxHistorySize = 5;
	
	// 히스토리에 현재 정보 추가
	PlayerPositionHistory.Add(CurrentTargetLocation);
	PlayerSpeedHistory.Add(PlayerSpeed);
	
	// 히스토리 크기 제한
	if (PlayerPositionHistory.Num() > MaxHistorySize)
	{
		PlayerPositionHistory.RemoveAt(0);
		PlayerSpeedHistory.RemoveAt(0);
	}
	
	// 움직임 패턴 분석
	float AverageSpeed = 0.0f;
	FVector MovementTrend = FVector::ZeroVector;
	
	if (PlayerSpeedHistory.Num() >= 2)
	{
		// 평균 속도 계산
		for (float Speed : PlayerSpeedHistory)
		{
			AverageSpeed += Speed;
		}
		AverageSpeed /= PlayerSpeedHistory.Num();
		
		// 움직임 트렌드 계산 (최근 3프레임)
		if (PlayerPositionHistory.Num() >= 3)
		{
			FVector RecentMovement = PlayerPositionHistory.Last() - PlayerPositionHistory[PlayerPositionHistory.Num() - 3];
			MovementTrend = RecentMovement.GetSafeNormal();
		}
	}
	
	CLog::Log("평균 속도: " + FString::SanitizeFloat(AverageSpeed));
	CLog::Log("움직임 트렌드: " + MovementTrend.ToString());
	
	// 시각적 디버그
	if (GetWorld())
	{
		// 플레이어 속도에 따른 색상
		FColor SpeedColor = FColor::Green;
		if (PlayerSpeed > 400.0f) SpeedColor = FColor::Red;
		else if (PlayerSpeed > 200.0f) SpeedColor = FColor::Orange;
		else if (PlayerSpeed > 50.0f) SpeedColor = FColor::Yellow;
		else SpeedColor = FColor::Blue;
		
		// 플레이어 위치 표시 (속도에 따른 크기)
		float SphereSize = FMath::Clamp(PlayerSpeed * 0.5f, 30.0f, 150.0f);
		DrawDebugSphere(GetWorld(), CurrentTargetLocation, SphereSize, 8, SpeedColor, false, -1.0f, 0, 3.0f);
		
		// 플레이어 속도 벡터 표시
		if (PlayerSpeed > 10.0f)
		{
			FVector VelocityEnd = CurrentTargetLocation + PlayerVelocity * 0.1f;
			DrawDebugLine(GetWorld(), CurrentTargetLocation, VelocityEnd, SpeedColor, false, -1.0f, 0, 4.0f);
		}
		
		// 움직임 트렌드 표시
		if (MovementTrend.Size() > 0.1f)
		{
			FVector TrendEnd = CurrentTargetLocation + MovementTrend * 200.0f;
			DrawDebugLine(GetWorld(), CurrentTargetLocation, TrendEnd, FColor::Cyan, false, -1.0f, 0, 2.0f);
		}
		
		// 보스에서 플레이어로의 방향 표시
		DrawDebugLine(GetWorld(), BossLocation, CurrentTargetLocation, FColor::White, false, -1.0f, 0, 1.0f);
	}
	
	// 행동 패턴 예측 로직
	if (PlayerSpeed < 50.0f && AverageSpeed < 100.0f) // 정지 상태
	{
		CLog::Log("예측: 플레이어 정지 상태 - Center");
		return TargetStateTag.Center;
	}
	else if (PlayerSpeed > 400.0f || AverageSpeed > 350.0f) // 매우 빠른 이동 (대시, 회피)
	{
		// 빠른 이동 시 움직임 트렌드 기반 예측
		float TrendDot = FVector::DotProduct(MovementTrend, BossRight);
		if (TrendDot > 0.4f)
		{
			CLog::Log("예측: 플레이어 대시 이동 - Right");
			return TargetStateTag.Right;
		}
		else if (TrendDot < -0.4f)
		{
			CLog::Log("예측: 플레이어 대시 이동 - Left");
			return TargetStateTag.Left;
		}
		else
		{
			CLog::Log("예측: 플레이어 대시 이동 - Center");
			return TargetStateTag.Center;
		}
	}
	else if (PlayerSpeed > 200.0f || AverageSpeed > 150.0f) // 빠른 이동
	{
		// 빠른 이동 시 현재 위치와 트렌드 조합
		float CombinedDot = (PlayerSideDot + FVector::DotProduct(MovementTrend, BossRight)) * 0.5f;
		if (CombinedDot > 0.3f)
		{
			CLog::Log("예측: 플레이어 빠른 이동 - Right");
			return TargetStateTag.Right;
		}
		else if (CombinedDot < -0.3f)
		{
			CLog::Log("예측: 플레이어 빠른 이동 - Left");
			return TargetStateTag.Left;
		}
		else
		{
			CLog::Log("예측: 플레이어 빠른 이동 - Center");
			return TargetStateTag.Center;
		}
	}
	else // 보통 속도 이동
	{
		// 보통 속도에서는 현재 위치 기반
		if (PlayerSideDot > 0.25f)
		{
			CLog::Log("예측: 플레이어 보통 이동 - Right");
			return TargetStateTag.Right;
		}
		else if (PlayerSideDot < -0.25f)
		{
			CLog::Log("예측: 플레이어 보통 이동 - Left");
			return TargetStateTag.Left;
		}
		else
		{
			CLog::Log("예측: 플레이어 보통 이동 - Center");
			return TargetStateTag.Center;
		}
	}
}

void UCBossMovementComponent::SetMovementStateWalk()
{
	Owner->GetCharacterMovement()->SetMovementMode(MOVE_NavWalking);
	IsFlying=false;
	
	// 비행 컴포넌트가 있으면 착륙
	if (FlyingComponent)
	{
		FlyingComponent->StartLanding();
	}
}

void UCBossMovementComponent::SetMovementStateFly()
{
	Owner->GetCharacterMovement()->SetMovementMode(MOVE_Flying);
	IsFlying=true;
	
	// 비행 컴포넌트가 있으면 이륙
	if (FlyingComponent)
	{
		FlyingComponent->StartTakeoff(300.0f); // 300 유닛 높이로 이륙
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
			FVector TargetPosition = PlayerLocation + SearchDirection * TargetingComp->DistanceThresholds[2];
			
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

/**
 * @brief 거리 유지와 궤도 이동을 상황에 맞게 자동으로 전환하는 통합 함수
 */
void UCBossMovementComponent::ExecuteSmartMovement(float DeltaTime, float MinDistance, float MaxDistance)
{
	if (!Owner) return;
	
	// 최소 거리 도착 여부를 추적하는 static 변수
	static bool bReachedMinDistance = false;
	
	// 목표 위치 도착 여부를 추적하는 static 변수
	static bool bReachedTargetPosition = false;
	
	// 시간 기반 전환을 위한 변수들
	static float TimeInOptimalRange = 0.0f;
	static bool bWasInOptimalRange = false;
	
			// 플레이어 방향 추적을 위한 static 변수들
		static FVector PreviousPlayerLocation = FVector::ZeroVector;
		static bool bPlayerLocationInitialized = false;
		static FVector LastMovementDirection = FVector::ZeroVector; // 이전 이동 방향 저장
	
	// 거리 계산
	FVector TargetLocation, OwnerLocation, DirectionToTarget;
	float CurrentDistance;
	CalculatePlayerDistance(TargetLocation, OwnerLocation, DirectionToTarget, CurrentDistance);
	
	// 거리 허용 오차 설정
	float DistanceTolerance = 50.0f;
	float AdjustedMinDistance = MinDistance - DistanceTolerance;
	float AdjustedMaxDistance = MaxDistance + DistanceTolerance;
	
	// 최소 거리에 도착했는지 확인 (시간 기반 전환)
	// 적정 거리 범위에서 1.5초 동안 유지되면 궤도 이동 모드로 전환
	if (!bReachedMinDistance)
	{
		// 거리 조건 확인
		bool bInOptimalRange = (CurrentDistance >= MinDistance && CurrentDistance <= MaxDistance);
		
		if (bInOptimalRange) 
		{
			if (!bWasInOptimalRange) 
			{
				TimeInOptimalRange = 0.0f;  // 처음 진입 시 타이머 리셋
			}
			TimeInOptimalRange += DeltaTime;  // 시간 누적
			
			// 1.5초 동안 유지되면 전환
			if (TimeInOptimalRange >= 1.5f) 
			{
				bReachedMinDistance = true;
			}
		} 
		else 
		{
			TimeInOptimalRange = 0.0f;  // 범위 벗어나면 타이머 리셋
		}
		
		bWasInOptimalRange = bInOptimalRange;
	}
	
	// 플레이어 방향 추적 초기화
	if (!bPlayerLocationInitialized)
	{
		PreviousPlayerLocation = TargetLocation;
		bPlayerLocationInitialized = true;
	}
	
			// 플레이어 이동 방향 계산
		FVector PlayerMovementDirection = FVector::ZeroVector;
		if (bPlayerLocationInitialized)
		{
			PlayerMovementDirection = (TargetLocation - PreviousPlayerLocation).GetSafeNormal();
			PreviousPlayerLocation = TargetLocation;
			
			// 이동 방향이 유효하면 저장
			if (PlayerMovementDirection.Size() > 0.1f)
			{
				LastMovementDirection = PlayerMovementDirection;
			}
		}
	
	// 최소 거리에 도착했으면 더 이상 거리 유지하지 않음
	
	// 목표 위치 변수
	FVector TargetPosition;
	
	// 항상 타겟을 바라보기 (기존 함수 사용)
	RotateTowardsPlayer(DeltaTime, 5.0f);
	
	// 최소 거리에 도착했으면 목표 위치로 이동 후 방향 추적 시작
	if (bReachedMinDistance)
	{
		// 목표 위치에 도착했는지 확인 (한번만 체크)
		if (!bReachedTargetPosition)
		{
			FVector CurrentTargetPosition = TargetLocation - DirectionToTarget * MinDistance;
			float DistanceToTarget = FVector::Dist(OwnerLocation, CurrentTargetPosition);
			
			if (DistanceToTarget <= 50.0f)
			{
				bReachedTargetPosition = true;
			}
		}
		
		if (bReachedTargetPosition)
		{
			// 목표 위치에 도착했으면 플레이어 방향에 따른 지능형 이동
			FVector BossRight = Owner->GetActorRightVector();
			FVector BossForward = Owner->GetActorForwardVector();
			
			// 플레이어 이동 방향을 보스 기준 좌표계로 변환
			float RightDot = FVector::DotProduct(PlayerMovementDirection, BossRight);
			float ForwardDot = FVector::DotProduct(PlayerMovementDirection, BossForward);
			
			// 플레이어가 정지했는지 확인
			bool bPlayerStopped = (PlayerMovementDirection.Size() < 0.1f);
			
			TargetPosition = OwnerLocation; // 기본값: 현재 위치 유지
			
			// 플레이어가 정지했고 이전 이동 방향이 있으면 이전 방향으로 계속 이동
			if (bPlayerStopped && LastMovementDirection.Size() > 0.1f)
			{
				// 이전 이동 방향을 보스 기준 좌표계로 변환
				float LastRightDot = FVector::DotProduct(LastMovementDirection, BossRight);
				float LastForwardDot = FVector::DotProduct(LastMovementDirection, BossForward);
				
				// 이전 방향에 따라 이동
				if (FMath::Abs(LastRightDot) > FMath::Abs(LastForwardDot))
				{
					// 이전에 좌우로 움직였다면 반대 방향으로 계속
					FVector SideDirection = (LastRightDot > 0) ? -BossRight : BossRight;
					TargetPosition = OwnerLocation + (SideDirection * 200.0f);
				}
				else if (LastForwardDot > 0.1f)
				{
					// 이전에 앞으로 움직였다면 앞으로 계속
					FVector ForwardDirection = BossForward;
					TargetPosition = OwnerLocation + (ForwardDirection * 200.0f);
				}
				else if (LastForwardDot < -0.1f)
				{
					// 이전에 뒤로 움직였다면 뒤로 계속
					FVector BackwardDirection = -BossForward;
					TargetPosition = OwnerLocation + (BackwardDirection * 200.0f);
				}
			}
			// 플레이어가 움직이고 있으면 기존 로직 사용
			else if (!bPlayerStopped)
			{
				// 좌우 방향 처리 (반대 방향으로 이동)
				if (FMath::Abs(RightDot) > FMath::Abs(ForwardDot))
				{
					// 좌우 이동이 더 강함
					FVector SideDirection = (RightDot > 0) ? -BossRight : BossRight; // 반대 방향
					TargetPosition = OwnerLocation + (SideDirection * 200.0f);
				}
				// 앞뒤 방향 처리 (좌우보다 앞뒤가 더 강할 때)
				else if (ForwardDot > 0.1f) // 앞으로 이동 (양수)
				{
					// 플레이어가 앞으로 가면 보스도 앞으로 이동
					FVector ForwardDirection = BossForward;
					TargetPosition = OwnerLocation + (ForwardDirection * 200.0f);
				}
				else if (ForwardDot < -0.1f) // 뒤로 이동 (음수)
				{
					// 플레이어가 뒤로 가면 보스도 뒤로 이동
					FVector BackwardDirection = -BossForward;
					TargetPosition = OwnerLocation + (BackwardDirection * 200.0f);
				}
			}
			// 플레이어가 정지하고 이전 방향도 없으면 그대로 유지
			
			// AI MoveTo로 목표 좌표로 이동
			if (AIC)
			{
					AIC->MoveToLocation(TargetPosition, 0);
			}
		}
		else
		{
			// 아직 목표 위치에 도착하지 않았으면 계속 목표 위치로 이동
			FVector CurrentTargetPosition = TargetLocation - DirectionToTarget * MinDistance;
			TargetPosition = CurrentTargetPosition;
			
			// AI 컨트롤러로 이동
			if (AIC)
			{
					AIC->MoveToLocation(TargetPosition, 0);
			}
		}
		
		// 속도 조정 (궤도 이동)
		if (ACharacter* BossChar = Cast<ACharacter>(Owner))
		{
			BossChar->GetCharacterMovement()->MaxWalkSpeed = 150;
		}
	}
	else
	{
		
		// 아직 최소 거리에 도착하지 않았으면 거리 유지
		if (CurrentDistance < AdjustedMinDistance)
		{
			TargetPosition = TargetLocation - DirectionToTarget * MinDistance;
			
			// 속도 조정 (거리 유지 - 빠른 이동)
			if (ACharacter* BossChar = Cast<ACharacter>(Owner))
			{
				BossChar->GetCharacterMovement()->MaxWalkSpeed = 300;
			}
			
			// AI 컨트롤러로 이동
			if (AIC)
			{
					AIC->MoveToLocation(TargetPosition, 0);
			}
		}
		else if (CurrentDistance > AdjustedMaxDistance)
		{
			TargetPosition = TargetLocation - DirectionToTarget * MaxDistance;
			
			// 속도 조정 (거리 유지 - 빠른 이동)
			if (ACharacter* BossChar = Cast<ACharacter>(Owner))
			{
				BossChar->GetCharacterMovement()->MaxWalkSpeed = 300;
			}
			
			// AI 컨트롤러로 이동
			if (AIC)
			{
					AIC->MoveToLocation(TargetPosition, 0);
			}
		}
		else
		{
			// 적정 거리면 최소 거리로 접근
			TargetPosition = TargetLocation - DirectionToTarget * MinDistance;
			
			// 속도 조정 (거리 유지 - 부드러운 접근)
			if (ACharacter* BossChar = Cast<ACharacter>(Owner))
			{
				BossChar->GetCharacterMovement()->MaxWalkSpeed = 300;
			}
			
			// AI 컨트롤러로 이동
			if (AIC)
			{
					AIC->MoveToLocation(TargetPosition, 0);
			}
		}
	}
	
	// 디버그 데이터 저장
	DebugTargetLocation = TargetLocation;
	DebugOwnerLocation = OwnerLocation;
	DebugClosestPosition = TargetPosition;
	DebugCurrentDistance = CurrentDistance;
	
	// 시각적 디버그 - 거리 범위 표시
	if (GetWorld())
	{
		// 플레이어 위치 (중앙)
		DrawDebugSphere(GetWorld(), TargetLocation, 50.0f, 8, FColor::Yellow, false, -1.0f, 0, 3.0f);
		
		// 보스 위치
		DrawDebugSphere(GetWorld(), OwnerLocation, 40.0f, 8, FColor::Magenta, false, -1.0f, 0, 3.0f);
		
		// 최소 거리 원 (빨간색)
		DrawDebugCircle(GetWorld(), TargetLocation, MinDistance, 32, FColor::Red, false, -1.0f, 0, 2.0f);
		
		// 최대 거리 원 (파란색)
		DrawDebugCircle(GetWorld(), TargetLocation, MaxDistance, 32, FColor::Blue, false, -1.0f, 0, 2.0f);
		
		// 현재 거리 원 (초록색)
		DrawDebugCircle(GetWorld(), TargetLocation, CurrentDistance, 32, FColor::Green, false, -1.0f, 0, 1.0f);
		
		// 목표 위치 표시
		DrawDebugSphere(GetWorld(), TargetPosition, 60.0f, 12, FColor::Cyan, false, -1.0f, 0, 4.0f);
		
		// 보스에서 목표 위치로의 선
		DrawDebugLine(GetWorld(), OwnerLocation, TargetPosition, FColor::Cyan, false, -1.0f, 0, 3.0f);
		
		// 시각적 디버그만 유지 (텍스트 로그 제거)
		if (bReachedMinDistance && bReachedTargetPosition)
		{
			// 플레이어 이동 방향 표시 (빨간색)
			if (PlayerMovementDirection.Size() > 0.1f)
			{
				FVector PlayerDirectionEnd = TargetLocation + (PlayerMovementDirection * 150.0f);
				DrawDebugLine(GetWorld(), TargetLocation, PlayerDirectionEnd, FColor::Red, false, -1.0f, 0, 3.0f);
			}
			
			// 보스 지능형 이동 방향 표시
			FVector BossRight = Owner->GetActorRightVector();
			FVector BossForward = Owner->GetActorForwardVector();
			
			float RightDot = FVector::DotProduct(PlayerMovementDirection, BossRight);
			float ForwardDot = FVector::DotProduct(PlayerMovementDirection, BossForward);
			
			FColor MovementColor = FColor::Blue;
			FVector MovementEnd = OwnerLocation;
			
			// 좌우 방향 처리
			if (FMath::Abs(RightDot) > FMath::Abs(ForwardDot))
			{
				// 좌우 이동 (반대 방향)
				FVector SideDirection = (RightDot > 0) ? -BossRight : BossRight;
				MovementEnd = OwnerLocation + (SideDirection * 150.0f);
				MovementColor = FColor::Blue;
			}
			// 앞뒤 방향 처리
			else if (ForwardDot < -0.1f) // 뒤로 이동
			{
				// 뒤로 따라가기
				FVector BackwardDirection = -BossForward;
				MovementEnd = OwnerLocation + (BackwardDirection * 150.0f);
				MovementColor = FColor::Green;
			}
			else
			{
				// 앞으로 이동하거나 정지 (그대로 유지)
				MovementColor = FColor::Yellow;
			}
			
			// 보스 이동 방향 표시
			if (MovementEnd != OwnerLocation)
			{
				DrawDebugLine(GetWorld(), OwnerLocation, MovementEnd, MovementColor, false, -1.0f, 0, 3.0f);
			}
		}
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

float UCBossMovementComponent::GetDebugCurrentDistance() const
{
	return DebugCurrentDistance;
}

/**
 * @brief 보스 이동 시스템 완전 초기화 (매니저용)
 * 
 * 모든 이동 관련 상태를 초기화하고 기본 이동 상태로 리셋합니다.
 * 보스 매니저에서 보스 리셋 시 사용됩니다.
 */
void UCBossMovementComponent::ResetMovementSystem()
{
	// 1. 안전한 위치 초기화
	SafePosition = FVector::ZeroVector;

	// 2. 비행 상태 초기화 (지상 모드로)
	IsFlying = false;

	// 3. 디버그 데이터 초기화
	DebugTargetLocation = FVector::ZeroVector;
	DebugOwnerLocation = FVector::ZeroVector;
	DebugClosestPosition = FVector::ZeroVector;
	DebugCurrentDistance = 0.0f;

	// 4. 이동 상태를 걷기로 설정
	SetMovementStateWalk();

	// 5. 타겟 상태 태그 초기화 (필요시)
	// TargetStateTag 초기화는 구조체의 기본값으로 설정됨

	// 6. 소유자 캐릭터가 있다면 위치 관련 초기화
	if (Owner)
	{
		// 캐릭터 이동 컴포넌트를 걷기 모드로 설정
		if (UCharacterMovementComponent* MovementComp = Owner->GetCharacterMovement())
		{
			MovementComp->SetMovementMode(MOVE_Walking);
		}

		// 현재 위치를 디버그 데이터로 설정
		DebugOwnerLocation = Owner->GetActorLocation();
		
		// 플레이어 찾기 및 디버그 데이터 업데이트
		if (APawn* Player = FindPlayer())
		{
			DebugTargetLocation = Player->GetActorLocation();
			FVector OwnerLoc, TargetLoc, DirectionToTarget;
			float CurrentDistance;
			CalculatePlayerDistance(TargetLoc, OwnerLoc, DirectionToTarget, CurrentDistance);
			DebugCurrentDistance = CurrentDistance;
		}
	}

	// 7. 비행 컴포넌트가 있다면 걷기 상태로 설정
	if (FlyingComponent)
	{
		FlyingComponent->bIsFlying = false;
	}

	UE_LOG(LogTemp, Warning, TEXT("보스 이동 시스템 완전 초기화 완료 - 걷기 모드로 리셋"));
}
