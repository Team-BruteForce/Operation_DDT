// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/Component/FlyingComponent.h"

#include "AIController.h"
#include "Global.h"
#include "DrawDebugHelpers.h"
#include "Boss/Actor/FlySpline.h"
#include "Components/SplineComponent.h"
#include "Components/StateTreeComponent.h"

UFlyingComponent::UFlyingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CHelpers::GetClass<AFlySpline>(&SplineActorClass,AssetPaths::Boss_FlySpline);
}

void UFlyingComponent::BeginPlay()
{
	Super::BeginPlay();
	
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		CharacterMovement = OwnerCharacter->GetCharacterMovement(); 
		StartLocation = OwnerCharacter->GetActorLocation();
		CurrentAltitude = StartLocation.Z;
		TargetAltitude = StartLocation.Z + DefaultFlyingHeight;
		
		// 스플라인 액터 스폰 (보스 현재 위치)
		SpawnSplineActor(StartLocation);
	}
}

void UFlyingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!OwnerCharacter || !CharacterMovement) return;
	
	// 이륙 중이면 이륙 로직 실행
	if (bIsTakingOff)
	{
		UpdateTakeoff(DeltaTime);
	}
	// 착륙 중이면 착륙 로직 실행
	else if (bIsLanding)
	{
		UpdateLanding(DeltaTime);
	}
	// 비행 중이면 고도 변화 및 궤도 이동 실행
	else if (bIsFlying)
	{
		// 호버링 모드 업데이트 (호버링 중에는 고도 변화 중지)
		if (bIsHovering)
		{
			UpdateHovering(DeltaTime);
		}
		// 스플라인 기반 궤도 이동 업데이트
		else if (bIsOrbitingWithSpline)
		{
			UpdateSplineOrbit(DeltaTime);
		}
		else if (bIsOrbiting)
		{
			UpdateOrbit(DeltaTime);
		}
		else
		{
			UpdateAltitudeVariation(DeltaTime);
		}
	}
	
	// 쿨타임 업데이트
	UpdateCooldowns(DeltaTime);
}

void UFlyingComponent::StartTakeoff(float TargetHeight, float Speed)
{
	if (!OwnerCharacter || bIsFlying) return;
	
	bIsTakingOff = true;
	bIsLanding = false;
	bIsOrbiting = false;
	
	StartLocation = OwnerCharacter->GetActorLocation();
	TargetAltitude = StartLocation.Z + TargetHeight;
	TakeoffProgress = 0.0f;
	
	// 이륙 속도 설정
	TakeoffSpeed = Speed;
	
	// 비행 모드로 전환
	CharacterMovement->SetMovementMode(MOVE_Flying);
	
}

void UFlyingComponent::StartLanding(FVector LandLocation)
{
	if (!OwnerCharacter || !bIsFlying) return;
	
	bIsLanding = true;
	bIsTakingOff = false;
	bIsOrbiting = false;
	
	// 착륙 위치가 지정되지 않았으면 현재 위치 아래로 설정
	if (LandLocation == FVector::ZeroVector)
	{
		LandLocation = OwnerCharacter->GetActorLocation();
		LandLocation.Z = StartLocation.Z; // 원래 지상 높이로
	}
	
	this->LandingLocation = LandLocation;
	LandingProgress = 0.0f;
	
}



void UFlyingComponent::StopFlying()
{
	if (!OwnerCharacter) return;
	
	bIsFlying = false;
	bIsTakingOff = false;
	bIsLanding = false;
	bIsOrbiting = false;
	
	// 지상 이동 모드로 전환
	CharacterMovement->SetMovementMode(MOVE_NavWalking);
	
}

void UFlyingComponent::UpdateTakeoff(float DeltaTime)
{
	if (!OwnerCharacter) return;
	
	// 이륙 진행도 업데이트
	TakeoffProgress += DeltaTime * TakeoffSpeed / 100.0f; // 1초에 3%씩 진행
	TakeoffProgress = FMath::Clamp(TakeoffProgress, 0.0f, 1.0f);
	
	// 현재 위치에서 목표 고도까지 부드럽게 상승
	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	FVector TargetLocation = StartLocation;
	TargetLocation.Z = FMath::Lerp(StartLocation.Z, TargetAltitude, TakeoffProgress);
	
	// 부드러운 곡선으로 이륙 (EaseOut)
	float SmoothProgress = 1.0f - FMath::Pow(1.0f - TakeoffProgress, 3.0f);
	TargetLocation.Z = FMath::Lerp(StartLocation.Z, TargetAltitude, SmoothProgress);
	
	MoveToLocation(TargetLocation, TakeoffSpeed);
	
	// 이륙 완료 체크
	if (TakeoffProgress >= 1.0f)
	{
		bIsTakingOff = false;
		bIsFlying = true;
		CurrentAltitude = TargetAltitude;
		
		// 이륙 완료 후 착륙 쿨타임 시작
		bCanLanding = false;
		LandingCooldownTimer = LandingCooldownTime;
	}
	
	// 디버그 시각화
	if (GetWorld())
	{
		DrawDebugLine(GetWorld(), StartLocation, TargetLocation, FColor::Green, false, -1.0f, 0, 2.0f);
		DrawDebugSphere(GetWorld(), TargetLocation, 30.0f, 8, FColor::Green, false, -1.0f, 0, 3.0f);
	}
}

void UFlyingComponent::UpdateLanding(float DeltaTime)
{
	if (!OwnerCharacter) return;
	
	// 착륙 진행도 업데이트
	LandingProgress += DeltaTime * LandingSpeed / 100.0f; // 1초에 2%씩 진행
	LandingProgress = FMath::Clamp(LandingProgress, 0.0f, 1.0f);
	
	// 현재 위치에서 착륙 위치까지 부드럽게 하강
	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	FVector TargetLocation = FMath::Lerp(CurrentLocation, LandingLocation, LandingProgress);
	
	// 부드러운 곡선으로 착륙 (EaseIn)
	float SmoothProgress = FMath::Pow(LandingProgress, 2.0f);
	TargetLocation = FMath::Lerp(CurrentLocation, LandingLocation, SmoothProgress);
	
	MoveToLocation(TargetLocation, LandingSpeed);
	
	// 착륙 중 회전 조정 (Z축 각도를 0으로)
	FRotator CurrentRotation = OwnerCharacter->GetActorRotation();
	FRotator TargetRotation = CurrentRotation;
	TargetRotation.Pitch = FMath::Lerp(CurrentRotation.Pitch, 0.0f, LandingProgress);
	
	// 부드러운 회전 적용
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
	OwnerCharacter->SetActorRotation(NewRotation);
	
	// 착륙 완료 체크
	if (LandingProgress >= 1.0f)
	{
		bIsLanding = false;
		bIsFlying = false;
		
		// 지상 이동 모드로 전환
		CharacterMovement->SetMovementMode(MOVE_NavWalking);
		
		// 최종 회전 보정 (완전히 수평으로)
		FRotator FinalRotation = OwnerCharacter->GetActorRotation();
		FinalRotation.Pitch = 0.0f;
		OwnerCharacter->SetActorRotation(FinalRotation);
		
		// 착륙 완료 후 이륙 쿨타임 시작
		bCanTakeoff = false;
		TakeoffCooldownTimer = TakeoffCooldownTime;
	}
	
	// 디버그 시각화
	if (GetWorld())
	{
		DrawDebugLine(GetWorld(), CurrentLocation, LandingLocation, FColor::Red, false, -1.0f, 0, 2.0f);
		DrawDebugSphere(GetWorld(), LandingLocation, 30.0f, 8, FColor::Red, false, -1.0f, 0, 3.0f);
	}
}

void UFlyingComponent::UpdateOrbit(float DeltaTime)
{
	if (!OwnerCharacter) return;
	
	// 궤도 각도 업데이트
	CurrentOrbitAngle += DeltaTime * OrbitSpeed;
	if (CurrentOrbitAngle >= 360.0f)
	{
		CurrentOrbitAngle -= 360.0f;
	}
	
	// 궤도 위치 계산
	float RadianAngle = FMath::DegreesToRadians(CurrentOrbitAngle);
	FVector OrbitOffset = FVector(
		FMath::Cos(RadianAngle) * OrbitRadius,
		FMath::Sin(RadianAngle) * OrbitRadius,
		0.0f
	);
	
	FVector TargetLocation = OrbitCenter + OrbitOffset;
	TargetLocation.Z = CurrentAltitude + AltitudeVariationOffset; // 고도 변화 적용
	
	MoveToLocation(TargetLocation, FlyingSpeed);
	
	// 디버그 시각화
	if (GetWorld())
	{
		// 궤도 원 그리기
		DrawDebugCircle(GetWorld(), OrbitCenter, OrbitRadius, 32, FColor::Cyan, false, -1.0f, 0, 2.0f);
		
		// 현재 위치와 목표 위치 표시
		DrawDebugSphere(GetWorld(), TargetLocation, 40.0f, 12, FColor::Cyan, false, -1.0f, 0, 3.0f);
		DrawDebugLine(GetWorld(), OwnerCharacter->GetActorLocation(), TargetLocation, FColor::Cyan, false, -1.0f, 0, 2.0f);
	}
}


void UFlyingComponent::UpdateAltitudeVariation(float DeltaTime)
{
	// 고도 변화 타이머 업데이트
	AltitudeVariationTimer += DeltaTime * AltitudeChangeSpeed;
	
	// 사인파를 사용한 자연스러운 고도 변화
	AltitudeVariationOffset = FMath::Sin(AltitudeVariationTimer) * AltitudeVariationRange;
	
	// 추가적인 랜덤 변화 (더 자연스럽게)
	static float RandomVariationTimer = 0.0f;
	RandomVariationTimer += DeltaTime;
	if (RandomVariationTimer >= 2.0f) // 2초마다 랜덤 변화
	{
		AltitudeVariationOffset += FMath::RandRange(-20.0f, 20.0f);
		RandomVariationTimer = 0.0f;
	}
	
	// 고도 변화 범위 제한
	AltitudeVariationOffset = FMath::Clamp(AltitudeVariationOffset, -AltitudeVariationRange, AltitudeVariationRange);
	
	// 디버그 시각화
	if (GetWorld())
	{
		FVector CurrentLocation = OwnerCharacter->GetActorLocation();
		FVector AltitudeLineStart = CurrentLocation;
		AltitudeLineStart.Z = TargetAltitude;
		FVector AltitudeLineEnd = AltitudeLineStart;
		AltitudeLineEnd.Z += AltitudeVariationOffset;
		
		DrawDebugLine(GetWorld(), AltitudeLineStart, AltitudeLineEnd, FColor::Yellow, false, -1.0f, 0, 1.0f);
	}
}

bool UFlyingComponent::MoveToLocation(const FVector& TargetLocation, float Speed, float ReachDistance)
{
	if (!OwnerCharacter || !CharacterMovement) return false;
	
	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
	float Distance = FVector::Dist(CurrentLocation, TargetLocation);
	
	// 도달했는지 확인
	bool bReached = Distance <= ReachDistance;
	
	// 도달했으면 이동 중지
	if (bReached)
	{
		return true;
	}
	
	// 비행 중에는 Velocity 직접 설정 (AI용)
	if (bIsFlying || bIsTakingOff || bIsLanding)
	{
		// 비행 모드로 전환
		CharacterMovement->SetMovementMode(MOVE_Flying);
		
		// 목표 위치로의 방향 벡터 계산
		FVector MoveDirection = (TargetLocation - CurrentLocation).GetSafeNormal();
		
		// 스플라인 궤도 이동 중에는 플레이어를 바라보면서 이동
		if (bIsOrbitingWithSpline)
		{
			// 플레이어 찾기
			APawn* TargetPawn = nullptr;
			for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
			{
				APlayerController* PC = Iterator->Get();
				if (PC && PC->GetPawn())
				{
					TargetPawn = PC->GetPawn();
					break;
				}
			}
			
			// 플레이어를 바라보도록 회전 설정
			if (TargetPawn)
			{
				FVector PlayerLocation = TargetPawn->GetActorLocation();
				FVector LookDirection = (PlayerLocation - CurrentLocation).GetSafeNormal();
				FRotator TargetRotation = FRotationMatrix::MakeFromX(LookDirection).Rotator();
				
				// Yaw만 업데이트 (비행 중 고도 유지)
				FRotator CurrentRotation = OwnerCharacter->GetActorRotation();
				TargetRotation.Pitch = CurrentRotation.Pitch;
				TargetRotation.Roll = CurrentRotation.Roll;
				
				OwnerCharacter->SetActorRotation(TargetRotation);
			}
		}
		
		// 속도 적용
		FVector NewVelocity = MoveDirection * Speed;
		
		// CharacterMovement의 Velocity 직접 설정
		CharacterMovement->Velocity = NewVelocity;
		
	}
	else
	{
		// 지상에서는 기존 방식 사용 (NavMesh 기반)
		CharacterMovement->AddInputVector(Direction * Speed);
	}
	
	return bReached;
}

float UFlyingComponent::GetCurrentAltitude() const
{
	if (!OwnerCharacter) return 0.0f;
	return OwnerCharacter->GetActorLocation().Z;
}

void UFlyingComponent::SetTargetAltitude(float NewAltitude)
{
	TargetAltitude = NewAltitude;
}

AFlySpline* UFlyingComponent::SpawnSplineActor(FVector SpawnLocation)
{
	if (!GetWorld())
	{
		return nullptr;
	}

	// 기존 스플라인 액터가 있으면 제거
	if (SpawnedSplineActor)
	{
		SpawnedSplineActor->Destroy();
		SpawnedSplineActor = nullptr;
	}

	// 스플라인 액터 클래스가 설정되지 않았으면 기본 클래스 사용
	if (!SplineActorClass)
	{
		SplineActorClass = AFlySpline::StaticClass();
	}

	// 스폰 파라미터 설정
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	SpawnParams.Instigator = Cast<APawn>(GetOwner());
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// 스플라인 액터 스폰
	SpawnedSplineActor = GetWorld()->SpawnActor<AFlySpline>(SplineActorClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);

	return SpawnedSplineActor;
}

USplineComponent* UFlyingComponent::FindHighestSpline() const
{
	if (!SpawnedSplineActor)
	{
		return nullptr;
	}

	TArray<USplineComponent*> Splines = SpawnedSplineActor->GetHorizontalSplines();
	if (Splines.Num() == 0)
	{
		return nullptr;
	}

	// 가장 높은 스플라인 찾기 (인덱스 3이 가장 높음)
	USplineComponent* HighestSpline = nullptr;
	float HighestZ = -FLT_MAX;

	for (int32 i = 0; i < Splines.Num(); i++)
	{
		if (Splines[i])
		{
			// 스플라인의 첫 번째 포인트의 Z 좌표 확인
			FVector FirstPoint = Splines[i]->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
			if (FirstPoint.Z > HighestZ)
			{
				HighestZ = FirstPoint.Z;
				HighestSpline = Splines[i];
			}
		}
	}

	return HighestSpline;
}


void UFlyingComponent::StartSplineOrbit()
{
	// 자동으로 가장 높은 스플라인 찾기
	USplineComponent* HighestSpline = FindHighestSpline();
	if (!HighestSpline)
	{
		return;
	}

	// 비행 상태 설정 (핵심!)
	bIsFlying = true;
	
	// 스플라인 궤도 이동 시작
	bIsOrbitingWithSpline = true;
	CurrentSplineIndex = 0; // 첫 번째 포인트부터 시작
	SplineProgress = 0.0f;
	SplineOrbitSpeed = 1.0f;
	
	// 초기 랜덤 오프셋 생성
	CurrentRandomOffset = FVector(
		FMath::RandRange(0.0f, 300.0f),   // X축 랜덤 (앞으로만)
		0.0f,  // Y축은 스플라인 그대로
		FMath::RandRange(-100.0f, 100.0f)   // Z축 랜덤 (고도 변화)
	);

}

void UFlyingComponent::StopSplineOrbit()
{
	bIsOrbitingWithSpline = false;
	CurrentSplineIndex = 0;
	SplineProgress = 0.0f;
	
}

void UFlyingComponent::UpdateSplineOrbit(float DeltaTime)
{
	if (!bIsOrbitingWithSpline || !SpawnedSplineActor)
		return;

	TArray<USplineComponent*> Splines = SpawnedSplineActor->GetHorizontalSplines();
	if (Splines.Num() == 0)
		return;

	// 가장 높은 스플라인 (인덱스 3)에서만 궤도 이동
	USplineComponent* CurrentSpline = Splines[3]; // 가장 높은 스플라인
	if (!CurrentSpline)
		return;

	// 스플라인 포인트 개수
	int32 NumPoints = CurrentSpline->GetNumberOfSplinePoints();
	
	// 현재 목표 위치 (저장된 랜덤 오프셋 사용)
	FVector BaseLocation = CurrentSpline->GetLocationAtSplinePoint(CurrentSplineIndex, ESplineCoordinateSpace::World);
	FVector TargetLocation = BaseLocation + CurrentRandomOffset;
	
	// 스플라인 궤도 이동 중에는 이동만 중지 (StateTree는 유지)
	if (OwnerCharacter->GetController())
	{
		OwnerCharacter->GetController()->StopMovement();
		if (AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController()))
		{
			AIController->StopMovement();
			// StateTree는 유지하고 이동만 중지
		}
	}
	
	// MoveToLocation으로 이동하고 도달 여부 확인
	bool bReached = MoveToLocation(TargetLocation, FlyingSpeed, 80.0f);
	
	
	// 도달했으면 다음 포인트로 이동
	if (bReached)
	{
		int32 PreviousIndex = CurrentSplineIndex;
		
		// 랜덤 방향으로 이동 (시계방향 또는 반시계방향)
		int32 Direction = FMath::RandBool() ? 1 : -1;
		int32 NewIndex = (CurrentSplineIndex + Direction + NumPoints) % NumPoints;
		CurrentSplineIndex = NewIndex;
		
		// 인덱스 올릴 때마다 랜덤 오프셋 생성
		CurrentRandomOffset = FVector(
			FMath::RandRange(0.0f, 500.0f),   // X축 랜덤 (앞으로만)
			0.0f,  // Y축은 스플라인 그대로
			FMath::RandRange(-300.0f, 300.0f)   // Z축 랜덤 (고도 변화)
		);
		
	}
	
	// 디버그 시각화
	if (GetWorld())
	{
		// 현재 스플라인 위치 표시
		DrawDebugSphere(GetWorld(), TargetLocation, 20.0f, 8, FColor::Green, false, 0.1f);
		
		// 가장 높은 스플라인 (활성) 표시
		FVector ActiveSplineStart = Splines[3]->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
		DrawDebugSphere(GetWorld(), ActiveSplineStart, 15.0f, 6, FColor::Red, false, 0.1f);
		
		// 다른 스플라인들 표시
		for (int32 i = 0; i < 3; i++)
		{
			FVector SplineStart = Splines[i]->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
			DrawDebugSphere(GetWorld(), SplineStart, 10.0f, 4, FColor::Blue, false, 0.1f);
		}
	}
}

void UFlyingComponent::StartHovering(FVector HoverLocation)
{
	if (!OwnerCharacter || !bIsFlying) return;
	
	// 다른 비행 모드들 중지
	bIsOrbiting = false;
	bIsOrbitingWithSpline = false;
	
	// 호버링 모드 시작
	bIsHovering = true;
	
	// 호버링 위치 설정
	if (HoverLocation == FVector::ZeroVector)
	{
		HoverLocation = OwnerCharacter->GetActorLocation();
	}
	// 현재 높이 그대로 호버링 (상승은 이륙에서 처리)
	this->HoveringLocation = HoverLocation;
	HoveringProgress = 0.0f;
	
	// 주기적 이동 타이머 초기화
	PeriodicMovementTimer = 0.0f;
	NextPeriodicMovementTime = FMath::RandRange(PeriodicMovementMinTime, PeriodicMovementMaxTime);
	
}

void UFlyingComponent::StopHovering()
{
	if (!bIsHovering) return;
	
	bIsHovering = false;
	bIsMovingToRandomPoint = false;
	bIsMovingSideways = false;
	bIsMovingSplineSemicircle = false;
	HoveringProgress = 0.0f;
	RandomTargetLocation = FVector::ZeroVector;
	SideTargetLocation = FVector::ZeroVector;
	CurrentSplineSemicircleIndex = 0;
	TargetSplineSemicircleIndex = 0;
	CurrentSplineSemicircleTarget = FVector::ZeroVector;
	
}

void UFlyingComponent::UpdateHovering(float DeltaTime)
{
	if (!OwnerCharacter || !bIsHovering) return;
	
	// 완료 딜레이 대기 중인지 확인
	if (bWaitingForRandomCompletion || bWaitingForSideCompletion || bWaitingForSplineCompletion)
	{
		CompletionDelayTimer -= DeltaTime;
		if (CompletionDelayTimer <= 0.0f)
		{
			CompletionDelayTimer = 0.0f;
			
			// 어떤 이동이 완료 대기 중인지 확인하고 해당 이벤트 전송
			if (bWaitingForRandomCompletion)
			{
				bWaitingForRandomCompletion = false;
				CLog::Log("0.5초 딜레이 완료 - 랜덤 이동 이벤트 전송!");
				OnRandomMovementCompleted();
			}
			else if (bWaitingForSideCompletion)
			{
				bWaitingForSideCompletion = false;
				CLog::Log("0.5초 딜레이 완료 - 사이드 이동 이벤트 전송!");
				OnSideMovementCompleted();
			}
			else if (bWaitingForSplineCompletion)
			{
				bWaitingForSplineCompletion = false;
				CLog::Log("0.5초 딜레이 완료 - 스플라인 반원 이동 이벤트 전송!");
				OnSplineSemicircleMovementCompleted();
			}
		}
		
		// 딜레이 중에도 호버링 계속 실행 (return 제거)
	}
	
	// 딜레이 중에는 새로운 이동 시작하지 않음
	if (bWaitingForRandomCompletion || bWaitingForSideCompletion || bWaitingForSplineCompletion)
	{
		// 호버링만 실행하고 새로운 이동은 시작하지 않음
		bool bReached = MoveToLocation(HoveringLocation, HoveringSpeed, 100.0f);
		if (bReached && OwnerCharacter->GetController())
		{
			OwnerCharacter->GetController()->StopMovement();
		}
		return;
	}
	
	// 랜덤 이동 중인지 확인
	if (bIsMovingToRandomPoint)
	{
		// 첫 프레임은 스킵
		if (bJustStartedMovement)
		{
			bJustStartedMovement = false;
			MoveToLocation(RandomTargetLocation, RandomMovementSpeed, 50.0f); // 이동만 시작
			return;
		}
		
		// 랜덤 위치로 이동
		bool bReached = MoveToLocation(RandomTargetLocation, RandomMovementSpeed, 50.0f);
		
		if (bReached)
		{
			CLog::Log("랜덤 이동 완료! 호버링 전환 후 0.5초 딜레이");
			
			// 1단계: 이동 완료
			bIsMovingToRandomPoint = false;
			
			// 2단계: 호버링으로 전환
			HoveringLocation = RandomTargetLocation; // 새로운 호버링 위치로 설정
			// bIsHovering = true; // 이미 true 상태
			
			// 3단계: 0.5초 후 이벤트 전송 예약
			bWaitingForRandomCompletion = true;
			CompletionDelayTimer = CompletionDelay;
		}
	}
	// 사이드 이동 중인지 확인 (곡선 이동으로 개선)
	else if (bIsMovingSideways)
	{
		// 첫 프레임: 곡선 이동 초기화
		if (bJustStartedMovement)
		{
			bJustStartedMovement = false;
			SideMovementProgress = 0.0f; // 곡선 진행도 초기화
			SideStartLocation = OwnerCharacter->GetActorLocation(); // 시작 위치 저장
			
			// 중간점 계산 (곡선을 위한 제어점)
			FVector ToTarget = SideTargetLocation - SideStartLocation;
			FVector Perpendicular = FVector::CrossProduct(ToTarget, FVector::UpVector).GetSafeNormal();
			SideMidPoint = SideStartLocation + ToTarget * 0.5f + Perpendicular * 150.0f; // 곡선의 높이
			return;
		}
		
		// 곡선 이동 업데이트 (이징 적용)
		float BaseSideMovementSpeed = FlyingSpeed * 0.8f; // 1.5f에서 0.8f로 감소
		
		// 이징 함수: 천천히 → 정상 → 천천히 (SmoothStep)
		float EaseMultiplier = CalculateEaseInOutMultiplier(SideMovementProgress);
		float AdjustedSpeed = BaseSideMovementSpeed * EaseMultiplier;
		
		SideMovementProgress += DeltaTime * (AdjustedSpeed / 1000.0f); // 진행도 업데이트
		SideMovementProgress = FMath::Clamp(SideMovementProgress, 0.0f, 1.0f);
		
		// 베지어 곡선으로 부드러운 경로 계산
		FVector CurrentPosition = CalculateBezierPoint(SideStartLocation, SideMidPoint, SideTargetLocation, SideMovementProgress);
		
		// 항상 플레이어를 바라보도록 회전 설정
		APawn* TargetPawn = nullptr;
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PC = Iterator->Get();
			if (PC && PC->GetPawn())
			{
				TargetPawn = PC->GetPawn();
				break;
			}
		}
		
		if (TargetPawn)
		{
			FVector PlayerLocation = TargetPawn->GetActorLocation();
			FVector CurrentPos = OwnerCharacter->GetActorLocation();
			FVector LookDirection = (PlayerLocation - CurrentPos).GetSafeNormal();
			FRotator TargetRotation = FRotationMatrix::MakeFromX(LookDirection).Rotator();
			
			// Yaw만 업데이트 (비행 중 고도 유지)
			FRotator CurrentRotation = OwnerCharacter->GetActorRotation();
			TargetRotation.Pitch = CurrentRotation.Pitch;
			TargetRotation.Roll = CurrentRotation.Roll;
			
			// 부드러운 회전 적용
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 3.0f);
			OwnerCharacter->SetActorRotation(NewRotation);
		}
		
		// 곡선 경로로 이동
		bool bReached = MoveToLocation(CurrentPosition, BaseSideMovementSpeed, 50.0f);
		
		// 곡선 이동 완료 확인 (진행도 기반)
		if (SideMovementProgress >= 1.0f)
		{
			CLog::Log("곡선 사이드 이동 완료! 호버링 전환 후 0.5초 딜레이");
			
			// 1단계: 이동 완료
			bIsMovingSideways = false;
			
			// 2단계: 호버링으로 전환
			HoveringLocation = SideTargetLocation; // 새로운 호버링 위치로 설정
			// bIsHovering = true; // 이미 true 상태
			
			// 3단계: 0.5초 후 이벤트 전송 예약
			bWaitingForSideCompletion = true;
			CompletionDelayTimer = CompletionDelay;
		}
	}
	// 스플라인 반원 이동 중인지 확인
	else if (bIsMovingSplineSemicircle)
	{
		// 첫 프레임은 스킵
		if (bJustStartedMovement)
		{
			bJustStartedMovement = false;
			float SplineMovementSpeed = FlyingSpeed * 2.0f;
			MoveToLocation(CurrentSplineSemicircleTarget, SplineMovementSpeed, 50.0f); // 이동만 시작
			return;
		}
		
		// 현재 스플라인 포인트로 이동 (속도 증가)
		float SplineMovementSpeed = FlyingSpeed * 2.0f;
		bool bReached = MoveToLocation(CurrentSplineSemicircleTarget, SplineMovementSpeed, 50.0f);
		
		if (bReached)
		{
		// 이전 인덱스 저장
		int32 PreviousIndex = CurrentSplineSemicircleIndex;
		
		// 다음 스플라인 포인트로 이동
		CurrentSplineSemicircleIndex++;
		
		// 카운터 증가
		SplineMoveCount++;
		
		// 각도 계산 (스플라인 포인트 간 각도)
		if (SpawnedSplineActor)
		{
			TArray<USplineComponent*> Splines = SpawnedSplineActor->GetHorizontalSplines();
			if (Splines.Num() > 2 && Splines[2])
			{
				USplineComponent* TargetSpline = Splines[2];
				int32 NumPoints = TargetSpline->GetNumberOfSplinePoints();
				
				// 스플라인이 원형이므로 각 포인트 간 각도는 360도 / 포인트 수
				float AnglePerPoint = 360.0f / NumPoints;
				MovedAngle += AnglePerPoint;
			}
		}
		
		// 반원(180도) 완료 체크 (각도 기반)
		if (MovedAngle >= 180.0f)
		{
			CLog::Log("스플라인 반원 이동 완료! 호버링 전환 후 0.5초 딜레이");
			
			// 1단계: 스플라인 반원 이동 완료
			bIsMovingSplineSemicircle = false;
			CurrentSplineSemicircleIndex = 0;
			TargetSplineSemicircleIndex = 0;
			StartSplineSemicircleIndex = 0;
			CurrentSplineSemicircleTarget = FVector::ZeroVector;
			SplineMoveCount = 0; // 카운터 리셋
			MovedAngle = 0.0f; // 각도 리셋
			
			// 2단계: 호버링으로 전환 (현재 위치에서 호버링)
			HoveringLocation = OwnerCharacter->GetActorLocation(); // 현재 위치에서 호버링
			// bIsHovering = true; // 이미 true 상태
			
			// 3단계: 0.5초 후 이벤트 전송 예약
			bWaitingForSplineCompletion = true;
			CompletionDelayTimer = CompletionDelay;
		}
		else
		{
			// 다음 스플라인 포인트 설정
			UpdateSplineSemicircleTarget();
		}
		}
	}
	else
	{
		// 수동 제어 모드: 단순히 호버링 위치로만 이동
		bool bReached = MoveToLocation(HoveringLocation, HoveringSpeed, 100.0f);
		
		if (bReached)
		{
			if (OwnerCharacter && OwnerCharacter->GetController())
			{
				OwnerCharacter->GetController()->StopMovement();
			}
		}
	}
	
	// 회전 로직 (랜덤 이동 중이면 이동 방향, 아니면 플레이어 방향)
	if (bIsMovingToRandomPoint)
	{
		// 랜덤 이동 중: 이동 방향을 바라보기
		FVector CurrentLocation = OwnerCharacter->GetActorLocation();
		FVector MoveDirection = (RandomTargetLocation - CurrentLocation).GetSafeNormal();
		
		if (MoveDirection.Size() > 0.1f)
		{
			FRotator TargetRotation = FRotationMatrix::MakeFromX(MoveDirection).Rotator();
			
			// 부드러운 회전 적용
			FRotator CurrentRotation = OwnerCharacter->GetActorRotation();
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed * 2.0f); // 더 빠른 회전
			
			OwnerCharacter->SetActorRotation(NewRotation);
		}
	}
	else
	{
		// 호버링 중: 플레이어를 내려다보기
		APawn* TargetPawn = nullptr;
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			APlayerController* PC = Iterator->Get();
			if (PC && PC->GetPawn())
			{
				TargetPawn = PC->GetPawn();
				break;
			}
		}
		
		if (TargetPawn)
		{
			FVector CurrentLocation = OwnerCharacter->GetActorLocation();
			FVector PlayerLocation = TargetPawn->GetActorLocation();
			
			// 플레이어 방향으로의 벡터 계산
			FVector DirectionToPlayer = (PlayerLocation - CurrentLocation).GetSafeNormal();
			
			// 내려다보는 각도 적용
			FRotator TargetRotation = FRotationMatrix::MakeFromX(DirectionToPlayer).Rotator();
			TargetRotation.Pitch = LookDownAngle; // 내려다보는 각도
			
			// 부드러운 회전 적용
			FRotator CurrentRotation = OwnerCharacter->GetActorRotation();
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
			
			OwnerCharacter->SetActorRotation(NewRotation);
		}
	}
		
	// 디버그 시각화
	if (GetWorld())
	{
		FVector CurrentLocation = OwnerCharacter->GetActorLocation();
		
		if (bIsMovingToRandomPoint)
		{
			// 랜덤 이동 중: 목표 위치와 이동 방향 표시
			DrawDebugSphere(GetWorld(), RandomTargetLocation, 30.0f, 8, FColor::Red, false, 0.1f, 0, 3.0f);
			DrawDebugLine(GetWorld(), CurrentLocation, RandomTargetLocation, FColor::Red, false, 0.1f, 0, 2.0f);
		}
		else
		{
			// 호버링 중: 플레이어와 호버링 위치 표시
			APawn* TargetPawn = nullptr;
			for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
			{
				APlayerController* PC = Iterator->Get();
				if (PC && PC->GetPawn())
				{
					TargetPawn = PC->GetPawn();
					break;
				}
			}
			
			if (TargetPawn)
			{
				FVector PlayerLocation = TargetPawn->GetActorLocation();
				DrawDebugLine(GetWorld(), CurrentLocation, PlayerLocation, FColor::Yellow, false, 0.1f, 0, 2.0f);
			}
			
			DrawDebugSphere(GetWorld(), HoveringLocation, 50.0f, 12, FColor::Purple, false, 0.1f, 0, 3.0f);
			DrawDebugLine(GetWorld(), CurrentLocation, HoveringLocation, FColor::Purple, false, 0.1f, 0, 2.0f);
		}
	}
}

bool UFlyingComponent::IsPlayerTooClose(float MinDistance) const
{
	if (!OwnerCharacter || !GetWorld()) return false;
	
	// 플레이어 찾기
	APawn* TargetPawn = nullptr;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC && PC->GetPawn())
		{
			TargetPawn = PC->GetPawn();
			break;
		}
	}
	
	if (!TargetPawn) return false;
	
	// 간단한 거리 계산
	float Distance = OwnerCharacter->GetDistanceTo(TargetPawn);
	
	return Distance < MinDistance;
}

FVector UFlyingComponent::GetRandomSplinePoint() const
{
	if (!SpawnedSplineActor) return FVector::ZeroVector;
	
	// 가장 높은 스플라인 (인덱스 3) 가져오기
	TArray<USplineComponent*> Splines = SpawnedSplineActor->GetHorizontalSplines();
	if (Splines.Num() <= 3 || !Splines[3]) return FVector::ZeroVector;
	
	USplineComponent* HighestSpline = Splines[3];
	int32 NumPoints = HighestSpline->GetNumberOfSplinePoints();
	
	if (NumPoints <= 0) return FVector::ZeroVector;
	
	// 랜덤 포인트 인덱스 선택
	int32 RandomIndex = FMath::RandRange(0, NumPoints - 1);
	
	// 해당 포인트의 월드 위치 반환
	return HighestSpline->GetLocationAtSplinePoint(RandomIndex, ESplineCoordinateSpace::World);
}

void UFlyingComponent::StartRandomMovement()
{
	
	if (!bIsHovering) 
	{
		return;
	}
	
	// 랜덤 위치 선택 n
	FVector RandomPoint = GetRandomSplinePoint();
	if (RandomPoint == FVector::ZeroVector)
	{
		return;
	}
	
	// 로그 출력
	CLog::Log(">>> StartRandomMovement 시작!");
	CLog::Log(FString::Printf(TEXT("시작 시간: %f"), GetWorld()->GetTimeSeconds()));
	
	// 랜덤 이동 시작
	bIsMovingToRandomPoint = true;
	RandomTargetLocation = RandomPoint;
	bJustStartedMovement = true; // 첫 프레임 플래그 설정
	
}


bool UFlyingComponent::IsPlayerTooFar() const
{
	if (!OwnerCharacter || !GetWorld()) return false;
	
	// 플레이어 찾기
	APawn* TargetPawn = nullptr;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC && PC->GetPawn())
		{
			TargetPawn = PC->GetPawn();
			break;
		}
	}
	
	if (!TargetPawn) return false;
	
	// 거리 계산
	float Distance = OwnerCharacter->GetDistanceTo(TargetPawn);
	
	return Distance > MaxPlayerDistance;
}

void UFlyingComponent::StartRandomMovementFromStateTree()
{
	StartRandomMovement();
}

void UFlyingComponent::OnRandomMovementCompleted()
{
	// 로그 출력
	CLog::Log("=== OnRandomMovementCompleted! ===");
	CLog::Log(FString::Printf(TEXT("현재 시간: %f"), GetWorld()->GetTimeSeconds()));
	
	// StateTree에 랜덤 이동 완료 이벤트 발생
	if (OwnerCharacter && OwnerCharacter->GetController())
	{
		if (AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController()))
		{
			// StateTreeComponent 찾기
			UStateTreeComponent* StateTreeComp = AIController->FindComponentByClass<UStateTreeComponent>();
			if (!StateTreeComp) return;

			FGameplayTag RandomMovementCompletedTag = FGameplayTag::RequestGameplayTag("BOSS.Event.Interrupt");
			StateTreeComp->SendStateTreeEvent(RandomMovementCompletedTag);
			
			CLog::Log("랜덤 이동 완료 - BOSS.Event.Interrupt!");
		}
	}
}

void UFlyingComponent::OnSideMovementCompleted()
{
	// 로그 출력
	CLog::Log("=== OnSideMovementCompleted! ===");
	CLog::Log(FString::Printf(TEXT("현재 시간: %f"), GetWorld()->GetTimeSeconds()));
	
	// StateTree에 사이드 이동 완료 이벤트 발생
	if (OwnerCharacter && OwnerCharacter->GetController())
	{
		if (AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController()))
		{
			// StateTreeComponent 찾기
			UStateTreeComponent* StateTreeComp = AIController->FindComponentByClass<UStateTreeComponent>();
			if (!StateTreeComp) return;

			FGameplayTag SideMovementCompletedTag = FGameplayTag::RequestGameplayTag("BOSS.Event.Interrupt");
			StateTreeComp->SendStateTreeEvent(SideMovementCompletedTag);
			
			CLog::Log("사이드 이동 완료 - BOSS.Event.Interrupt!");
		}
	}
}

void UFlyingComponent::StartSplineSemicircleMovement()
{
	if (!OwnerCharacter || !SpawnedSplineActor)
	{
		return;
	}
	
	// 로그 출력
	CLog::Log(">>> StartSplineSemicircleMovement 시작!");
	CLog::Log(FString::Printf(TEXT("시작 시간: %f"), GetWorld()->GetTimeSeconds()));
	
	// 스플라인 컴포넌트 찾기 (아래쪽 스플라인 사용)
	TArray<USplineComponent*> Splines = SpawnedSplineActor->GetHorizontalSplines();
	if (Splines.Num() <= 2 || !Splines[2])
	{
		return;
	}
	
	USplineComponent* TargetSpline = Splines[2];
	int32 NumPoints = TargetSpline->GetNumberOfSplinePoints();
	
	// 현재 위치에서 가장 가까운 스플라인 포인트 찾기
	FVector CurrentPos = OwnerCharacter->GetActorLocation();
	int32 ClosestIndex = 0;
	float ClosestDistance = FLT_MAX;
	
	for (int32 i = 0; i < NumPoints; i++)
	{
		FVector SplinePoint = TargetSpline->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
		float Distance = FVector::Dist(CurrentPos, SplinePoint);
		
		if (Distance < ClosestDistance)
		{
			ClosestDistance = Distance;
			ClosestIndex = i;
		}
	}
	
	// 시작 인덱스 설정 (가장 가까운 포인트)
	CurrentSplineSemicircleIndex = ClosestIndex;
	StartSplineSemicircleIndex = ClosestIndex; // 시작 인덱스 저장
	
	// 15개 포인트 이동 (반원 그리기)
	TargetSplineSemicircleIndex = (ClosestIndex + 15) % NumPoints;
	
	// 카운터와 각도 초기화
	SplineMoveCount = 0;
	MovedAngle = 0.0f;
	
	// 첫 번째 타겟 설정
	UpdateSplineSemicircleTarget();
	
	// 스플라인 반원 이동 시작
	bIsHovering = true;
	bIsMovingSplineSemicircle = true;
	bJustStartedMovement = true; // 첫 프레임 플래그 설정
}

void UFlyingComponent::UpdateSplineSemicircleTarget()
{
	if (!SpawnedSplineActor)
	{
		return;
	}
	
	TArray<USplineComponent*> Splines = SpawnedSplineActor->GetHorizontalSplines();
	if (Splines.Num() <= 2 || !Splines[2])
	{
		return;
	}
	
	USplineComponent* TargetSpline = Splines[2];
	int32 NumPoints = TargetSpline->GetNumberOfSplinePoints();
	
	// 현재 인덱스의 스플라인 포인트를 타겟으로 설정
	int32 TargetIndex = CurrentSplineSemicircleIndex % NumPoints;
	CurrentSplineSemicircleTarget = TargetSpline->GetLocationAtSplinePoint(TargetIndex, ESplineCoordinateSpace::World);
}

void UFlyingComponent::OnSplineSemicircleMovementCompleted()
{
	// 로그 출력
	CLog::Log("=== OnSplineSemicircleMovementCompleted! ===");
	CLog::Log(FString::Printf(TEXT("현재 시간: %f"), GetWorld()->GetTimeSeconds()));
	CLog::Log(FString::Printf(TEXT("이동한 각도: %f도"), MovedAngle));
	
	// StateTree에 스플라인 반원 이동 완료 이벤트 발생
	if (OwnerCharacter && OwnerCharacter->GetController())
	{
		if (AAIController* AIController = Cast<AAIController>(OwnerCharacter->GetController()))
		{
			// StateTreeComponent 찾기
			UStateTreeComponent* StateTreeComp = AIController->FindComponentByClass<UStateTreeComponent>();
			if (!StateTreeComp) return;

			FGameplayTag SplineSemicircleCompletedTag = FGameplayTag::RequestGameplayTag("BOSS.Event.Interrupt");
			StateTreeComp->SendStateTreeEvent(SplineSemicircleCompletedTag);
			
			CLog::Log("스플라인 반원 이동 완료 - BOSS.Event.Interrupt!");
		}
	}
}

void UFlyingComponent::ResetSplineSemicircleMovement()
{
	// 스플라인 반원 이동 관련 모든 상태 초기화
	bIsMovingSplineSemicircle = false;
	CurrentSplineSemicircleIndex = 0;
	TargetSplineSemicircleIndex = 0;
	StartSplineSemicircleIndex = 0;
	CurrentSplineSemicircleTarget = FVector::ZeroVector;
	SplineMoveCount = 0; // 카운터 리셋 추가
	MovedAngle = 0.0f; // 각도 리셋 추가
	
	// 호버링 상태도 초기화
	bIsHovering = false;
	bIsMovingToRandomPoint = false;
	bIsMovingSideways = false;
	bJustStartedMovement = false; // 첫 프레임 플래그 리셋
	bWaitingForRandomCompletion = false; // 딜레이 플래그 리셋
	bWaitingForSideCompletion = false; // 딜레이 플래그 리셋
	bWaitingForSplineCompletion = false; // 딜레이 플래그 리셋
	CompletionDelayTimer = 0.0f; // 딜레이 타이머 리셋
	HoveringProgress = 0.0f;
	RandomTargetLocation = FVector::ZeroVector;
	SideTargetLocation = FVector::ZeroVector;
}

void UFlyingComponent::MoveSideways()
{
	if (!OwnerCharacter)
	{
		return;
	}
	
	// 로그 출력
	CLog::Log(">>> MoveSideways 시작!");
	CLog::Log(FString::Printf(TEXT("시작 시간: %f"), GetWorld()->GetTimeSeconds()));
	
	// 좌우 랜덤 선택 (0: 왼쪽, 1: 오른쪽)
	bool bGoLeft = FMath::RandBool();
	
	FVector CurrentPos = OwnerCharacter->GetActorLocation();
	FVector RightVector = OwnerCharacter->GetActorRightVector();
	
	FVector TargetPos;
	if (bGoLeft)
	{
		TargetPos = CurrentPos + (RightVector * -SideMovementDistance);
	}
	else
	{
		TargetPos = CurrentPos + (RightVector * SideMovementDistance);
	}
	
	// 플레이어를 바라보도록 회전 설정
	APawn* TargetPawn = nullptr;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC && PC->GetPawn())
		{
			TargetPawn = PC->GetPawn();
			break;
		}
	}
	
	if (TargetPawn)
	{
		FVector PlayerLocation = TargetPawn->GetActorLocation();
		FVector LookDirection = (PlayerLocation - CurrentPos).GetSafeNormal();
		FRotator TargetRotation = FRotationMatrix::MakeFromX(LookDirection).Rotator();
		
		// Yaw만 업데이트 (비행 중 고도 유지)
		FRotator CurrentRotation = OwnerCharacter->GetActorRotation();
		TargetRotation.Pitch = CurrentRotation.Pitch;
		TargetRotation.Roll = CurrentRotation.Roll;
		
		OwnerCharacter->SetActorRotation(TargetRotation);
	}
	
	// 사이드 이동 시작 (실제 이동은 UpdateHovering에서 처리)
	bIsHovering = true;
	bIsMovingSideways = true;
	SideTargetLocation = TargetPos;
	bJustStartedMovement = true; // 첫 프레임 플래그 설정
	
}


void UFlyingComponent::SetAutoHoveringLogic(bool bEnabled)
{
	bAutoHoveringLogicEnabled = bEnabled;
}

bool UFlyingComponent::CheckDistanceConditions() const
{ 
	return IsPlayerTooClose(MinPlayerDistance) || IsPlayerTooFar();
}

bool UFlyingComponent::CheckPeriodicMovementTime() const
{
	return PeriodicMovementTimer >= NextPeriodicMovementTime;
}

void UFlyingComponent::UpdateCooldowns(float DeltaTime)
{
	// 이륙 쿨타임 업데이트
	if (!bCanTakeoff)
	{
		TakeoffCooldownTimer -= DeltaTime;
		if (TakeoffCooldownTimer <= 0.0f)
		{
			bCanTakeoff = true;
			TakeoffCooldownTimer = 0.0f;
		}
	}
	
	// 착륙 쿨타임 업데이트
	if (!bCanLanding)
	{
		LandingCooldownTimer -= DeltaTime;
		if (LandingCooldownTimer <= 0.0f)
		{
			bCanLanding = true;
			LandingCooldownTimer = 0.0f;
		}
	}
}

/**
 * @brief 비행 시스템 완전 초기화 (매니저용)
 * 
 * 모든 비행 관련 상태를 초기화하고 착륙 상태로 리셋합니다.
 * 보스 매니저에서 보스 리셋 시 사용됩니다.
 */
void UFlyingComponent::ResetFlyingSystem()
{
	// 1. 모든 비행 상태 플래그 초기화
	bIsFlying = false;
	bIsTakingOff = false;
	bIsLanding = false;
	bIsOrbiting = false;
	bIsHovering = false;
	bIsOrbitingWithSpline = false;

	// 2. 이동 관련 상태 초기화
	bIsMovingToRandomPoint = false;
	bIsMovingSideways = false;
	bIsMovingSplineSemicircle = false;
	bJustStartedMovement = false;
	bWaitingForRandomCompletion = false;
	bWaitingForSideCompletion = false;
	bWaitingForSplineCompletion = false;

	// 3. 위치 및 타겟 초기화
	if (OwnerCharacter)
	{
		StartLocation = OwnerCharacter->GetActorLocation();
		CurrentAltitude = StartLocation.Z;
		TargetAltitude = StartLocation.Z; // 지상 고도로 설정
		
		// 캐릭터 이동 모드를 걷기로 변경
		if (CharacterMovement)
		{
			CharacterMovement->SetMovementMode(MOVE_Walking);
		}
	}

	// 4. 타겟 위치들 초기화
	LandingLocation = FVector::ZeroVector;
	HoveringLocation = FVector::ZeroVector;
	RandomTargetLocation = FVector::ZeroVector;
	SideTargetLocation = FVector::ZeroVector;
	SideStartLocation = FVector::ZeroVector;
	SideMidPoint = FVector::ZeroVector;
	SideMovementProgress = 0.0f;
	CurrentSplineSemicircleTarget = FVector::ZeroVector;
	CurrentRandomOffset = FVector::ZeroVector;

	// 5. 프로그레스 및 타이머 초기화
	TakeoffProgress = 0.0f;
	LandingProgress = 0.0f;
	HoveringProgress = 0.0f;
	HoveringFloatTimer = 0.0f;
	PeriodicMovementTimer = 0.0f;
	CompletionDelayTimer = 0.0f;
	AltitudeVariationTimer = 0.0f;
	AltitudeVariationOffset = 0.0f;

	// 6. 스플라인 관련 초기화
	CurrentSplineIndex = 0;
	SplineProgress = 0.0f;
	CurrentSplineSemicircleIndex = 0;
	TargetSplineSemicircleIndex = 0;
	SplineMoveCount = 0;
	StartSplineSemicircleIndex = 0;
	MovedAngle = 0.0f;

	// 7. 궤도 이동 관련 초기화
	OrbitCenter = FVector::ZeroVector;
	CurrentOrbitAngle = 0.0f;

	// 8. 쿨타임 초기화
	bCanTakeoff = true;
	bCanLanding = true;
	TakeoffCooldownTimer = 0.0f;
	LandingCooldownTimer = 0.0f;

	// 9. 자동 호버링 로직 비활성화
	bAutoHoveringLogicEnabled = false;

	// 10. 다음 주기적 이동 시간 재설정
	NextPeriodicMovementTime = FMath::RandRange(PeriodicMovementMinTime, PeriodicMovementMaxTime);

	// 11. 스폰된 스플라인 액터 정리 (필요시)
	if (SpawnedSplineActor)
	{
		SpawnedSplineActor->Destroy();
		SpawnedSplineActor = nullptr;
	}

	UE_LOG(LogTemp, Warning, TEXT("비행 시스템 완전 초기화 완료 - 착륙 상태로 리셋"));
}

/**
 * @brief 베지어 곡선 계산 함수 (3점 기준 2차 베지어)
 */
FVector UFlyingComponent::CalculateBezierPoint(const FVector& P0, const FVector& P1, const FVector& P2, float t) const
{
	// 2차 베지어 곡선 공식: (1-t)²P0 + 2(1-t)tP1 + t²P2
	float OneMinusT = 1.0f - t;
	float OneMinusTSquared = OneMinusT * OneMinusT;
	float TSquared = t * t;
	float TwoOneMinusTT = 2.0f * OneMinusT * t;
	
	return OneMinusTSquared * P0 + TwoOneMinusTT * P1 + TSquared * P2;
}

/**
 * @brief 이징 함수 - 천천히 → 정상 → 천천히 (SmoothStep 기반)
 */
float UFlyingComponent::CalculateEaseInOutMultiplier(float t) const
{
	// SmoothStep 함수: 3t² - 2t³
	float SmoothT = t * t * (3.0f - 2.0f * t);
	
	// 속도 배수 계산 (미분값 기반)
	// SmoothStep의 미분: 6t - 6t²
	float SpeedMultiplier = 6.0f * t * (1.0f - t);
	
	// 최소 속도 0.3배, 최대 속도 1.2배로 조정 (더 부드럽게)
	float MinSpeed = 0.3f;
	float MaxSpeed = 1.2f;
	
	return FMath::Lerp(MinSpeed, MaxSpeed, SpeedMultiplier);
}