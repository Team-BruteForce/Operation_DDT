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
		// 상태 머신 업데이트
		UpdateStateMachine(DeltaTime);
		
		// 거리 유지 이동 업데이트
		if (bIsDistanceMaintaining)
		{
			UpdateDistanceMaintenance(DeltaTime);
		}
		// 스플라인 거리 유지 이동 업데이트
		else if (bIsSplineDistanceMaintaining)
		{
			UpdateSplineDistanceMaintenance(DeltaTime);
		}
		// 호버링 모드 업데이트 (호버링 중에는 고도 변화 중지)
		else if (bIsHovering)
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
	CLog::Log(FString::Printf(TEXT("StartTakeoff 호출 - bCanTakeoff: %s, bIsFlying: %s"), 
		bCanTakeoff ? TEXT("true") : TEXT("false"), bIsFlying ? TEXT("true") : TEXT("false")));
	
	if (!OwnerCharacter || bIsFlying || !bCanTakeoff) return;
	
	bIsTakingOff = true;
	bIsFlying = true;  // 이륙 시작 시 비행 상태로 설정
	bIsLanding = false;
	bIsOrbiting = false;
	bIsHovering = false;  // 호버링 상태 초기화
	bWaitingForTakeoffCompletion = false;  // 이륙 완료 대기 상태 초기화
	
	StartLocation = OwnerCharacter->GetActorLocation();
	TargetAltitude = StartLocation.Z + TargetHeight;
	TakeoffProgress = 0.0f;
	
	// 이륙 시작 로그
	CLog::Log(FString::Printf(TEXT("이륙 시작 - 시작위치: %s, 목표고도: %f"), 
		*StartLocation.ToString(), TargetAltitude));
	
	// 이륙 속도 설정
	TakeoffSpeed = Speed;
	
	// 비행 모드로 전환
	CharacterMovement->SetMovementMode(MOVE_Flying);
	
}

void UFlyingComponent::StartLanding(FVector LandLocation)
{
	CLog::Log(FString::Printf(TEXT("StartLanding 호출 - bCanLanding: %s, bIsFlying: %s"), 
		bCanLanding ? TEXT("true") : TEXT("false"), bIsFlying ? TEXT("true") : TEXT("false")));
	
	if (!OwnerCharacter || !bIsFlying) return;
	
	bIsLanding = true;
	bIsFlying = false;  // 착륙 시작 시 비행 상태 해제
	bIsTakingOff = false;
	bIsOrbiting = false;
	bIsHovering = false;  // 호버링 상태 초기화
	
	// 이륙 쿨타임 시작 (착륙 시작 시)
	bCanTakeoff = false;
	TakeoffCooldownTimer = TakeoffCooldownTime;
	CLog::Log(FString::Printf(TEXT("=== StartLanding - 이륙 쿨타임 시작: %.1f초 ==="), TakeoffCooldownTime));
	
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
	bIsHovering = false;  // 호버링 상태 초기화
	
	// 이륙 쿨타임 시작 (StopFlying 호출 시)
	bCanTakeoff = false;
	TakeoffCooldownTimer = TakeoffCooldownTime;
	CLog::Log(FString::Printf(TEXT("=== StopFlying - 이륙 쿨타임 시작: %.1f초 ==="), TakeoffCooldownTime));
	
	// 지상 이동 모드로 전환
	CharacterMovement->SetMovementMode(MOVE_NavWalking);
	
}

void UFlyingComponent::UpdateTakeoff(float DeltaTime)
{
	if (!OwnerCharacter) return;
	
	// 이륙 진행도 업데이트 (속도 기반)
	// TakeoffSpeed를 1000으로 나누어서 적절한 속도로 조정
	TakeoffProgress += DeltaTime * (TakeoffSpeed / 1000.0f);
	TakeoffProgress = FMath::Clamp(TakeoffProgress, 0.0f, 1.0f);
	
	// 이륙 진행도 디버그 로그
	CLog::Log(FString::Printf(TEXT("이륙 진행 - 진행도: %f, TakeoffSpeed: %f"), 
		TakeoffProgress, TakeoffSpeed));
	
	// 현재 위치에서 목표 고도까지 부드럽게 상승
	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	FVector TargetLocation = StartLocation;
	TargetLocation.Z = FMath::Lerp(StartLocation.Z, TargetAltitude, TakeoffProgress);
	
	// 부드러운 곡선으로 이륙 (EaseOut)
	float SmoothProgress = 1.0f - FMath::Pow(1.0f - TakeoffProgress, 3.0f);
	TargetLocation.Z = FMath::Lerp(StartLocation.Z, TargetAltitude, SmoothProgress);
	
	// 이륙 중에는 직접 위치 설정 (자연스러운 이동 시스템 우회)
	OwnerCharacter->SetActorLocation(TargetLocation);
	
	// 이륙 완료 체크
	if (TakeoffProgress >= 1.0f)
	{
		bIsTakingOff = false;
		bIsFlying = true;
		CurrentAltitude = TargetAltitude;
		
		// 이륙 완료 후 착륙 쿨타임 시작
		bCanLanding = false;
		LandingCooldownTimer = LandingCooldownTime;
		CLog::Log(FString::Printf(TEXT("=== 이륙 완료 - 착륙 쿨타임 시작: %.1f초 ==="), LandingCooldownTime));
		
		// 이륙 완료 대기 중인 호버링이 있으면 시작
		if (bWaitingForTakeoffCompletion)
		{
			bWaitingForTakeoffCompletion = false;
			
			// 이륙 완료 시점의 정확한 위치로 호버링 시작
			FVector TakeoffCompletedLocation = OwnerCharacter->GetActorLocation();
			
			// 이륙 완료 로그
			CLog::Log(FString::Printf(TEXT("이륙 완료 - 위치: %s, 목표고도: %f"), 
				*TakeoffCompletedLocation.ToString(), TargetAltitude));
			
			if (PendingHoverLocation != FVector::ZeroVector)
			{
				// 특정 위치가 지정되었으면 그 위치 사용
				StartHovering(PendingHoverLocation);
			}
			else
			{
				// 현재 위치(이륙 완료 위치)에서 호버링
				StartHovering(TakeoffCompletedLocation);
			}
			PendingHoverLocation = FVector::ZeroVector;
		}
		
		// StateTree에 이륙 완료 이벤트 전송
		if (OwnerCharacter && OwnerCharacter->GetController())
		{
			UStateTreeComponent* StateTreeComp = OwnerCharacter->GetController()->FindComponentByClass<UStateTreeComponent>();
			if (StateTreeComp)
			{
				StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag("BOSS.Event.Interrupt"));
			}
		}
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
	
	// 착륙 진행도 업데이트 (속도 기반)
	// LandingSpeed를 1000으로 나누어서 적절한 속도로 조정
	LandingProgress += DeltaTime * (LandingSpeed / 1000.0f);
	LandingProgress = FMath::Clamp(LandingProgress, 0.0f, 1.0f);
	
	// 현재 위치에서 착륙 위치까지 부드럽게 하강
	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	FVector TargetLocation = FMath::Lerp(CurrentLocation, LandingLocation, LandingProgress);
	
	// 부드러운 곡선으로 착륙 (EaseIn)
	float SmoothProgress = FMath::Pow(LandingProgress, 2.0f);
	TargetLocation = FMath::Lerp(CurrentLocation, LandingLocation, SmoothProgress);
	
	// 착륙 중에는 직접 위치 설정 (자연스러운 이동 시스템 우회)
	OwnerCharacter->SetActorLocation(TargetLocation);
	
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
		CLog::Log("착륙 완료 - 지상 이동 모드로 전환");
		bIsLanding = false;
		
		// 지상 이동 모드로 전환
		CharacterMovement->SetMovementMode(MOVE_NavWalking);
		
		// 최종 회전 보정 (완전히 수평으로)
		FRotator FinalRotation = OwnerCharacter->GetActorRotation();
		FinalRotation.Pitch = 0.0f;
		OwnerCharacter->SetActorRotation(FinalRotation);
		
		// 착륙 완료 후 StartLocation을 현재 위치로 업데이트 (다음 이륙을 위해)
		StartLocation = OwnerCharacter->GetActorLocation();
		
		// 착륙 완료 후 이륙 쿨타임 시작
		bCanTakeoff = false;
		TakeoffCooldownTimer = TakeoffCooldownTime;
		CLog::Log(FString::Printf(TEXT("=== 착륙 완료 - 이륙 쿨타임 시작: %.1f초 ==="), TakeoffCooldownTime));
		
		// StateTree에 착륙 완료 이벤트 전송
		if (OwnerCharacter && OwnerCharacter->GetController())
		{
			UStateTreeComponent* StateTreeComp = OwnerCharacter->GetController()->FindComponentByClass<UStateTreeComponent>();
			if (StateTreeComp)
			{
				StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag("BOSS.Event.Interrupt"));
			}
		}
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
	// 자연스러운 이동 시스템 사용
	return MoveToLocationNatural(TargetLocation, Speed, ReachDistance);
}

bool UFlyingComponent::MoveToLocationNatural(const FVector& TargetLocation, float Speed, float ReachDistance)
{
	if (!OwnerCharacter || !CharacterMovement) return false;
	
	FVector CurrentLocation = OwnerCharacter->GetActorLocation();
	float Distance = FVector::Dist(CurrentLocation, TargetLocation);
	
	// 도달했는지 확인
	bool bReached = Distance <= ReachDistance;
	
	// 도달했으면 이동 중지
	if (bReached)
	{
		// 관성을 고려한 부드러운 정지
		CurrentVelocity = FVector::ZeroVector;
		TargetVelocity = FVector::ZeroVector;
		CharacterMovement->Velocity = FVector::ZeroVector;
		return true;
	}
	
	// 비행 중에는 자연스러운 이동 시스템 사용
	if (bIsFlying || bIsTakingOff || bIsLanding)
	{
		// 비행 모드로 전환
		CharacterMovement->SetMovementMode(MOVE_Flying);
		
		// 거리 기반 속도 계산
		float DistanceBasedSpeed = CalculateDistanceBasedSpeed(Distance, Speed);
		
		// 목표 방향 계산
		FVector NewTargetDirection = (TargetLocation - CurrentLocation).GetSafeNormal();
		TargetDirection = NewTargetDirection;
		
		// 목표 속도 벡터 설정
		TargetVelocity = TargetDirection * DistanceBasedSpeed;
		
		// 자연스러운 이동 업데이트
		UpdateNaturalMovement(GetWorld()->GetDeltaSeconds());
		
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
		// 거리 유지 이동 중에는 상태 머신이 회전을 처리하므로 여기서는 회전하지 않음
	}
	else
	{
		// 지상에서는 기존 방식 사용 (NavMesh 기반)
		FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();
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

void UFlyingComponent::StartHoveringAfterTakeoff(FVector HoverLocation)
{
	// 이륙 중이면 완료까지 대기
	if (bIsTakingOff)
	{
		bWaitingForTakeoffCompletion = true;
		PendingHoverLocation = HoverLocation;
		return;
	}
	
	// 이륙이 완료되었으면 바로 호버링 시작
	StartHovering(HoverLocation);
}

void UFlyingComponent::StartHovering(FVector HoverLocation)
{
	if (!OwnerCharacter) return;
	
	// 착륙 완료 후에는 이륙부터 해야 함
	if (!bIsFlying && !bIsTakingOff && !bIsLanding)
	{
		CLog::Log("호버링 요청 - 착륙 완료 상태이므로 이륙부터 필요");
		return;
	}
	
	// 이륙 중이면 완료까지 대기
	if (bIsTakingOff)
	{
		CLog::Log("호버링 요청 - 이륙 중이므로 완료까지 대기");
		bWaitingForTakeoffCompletion = true;
		PendingHoverLocation = HoverLocation;
		return;
	}
	
	// 다른 비행 모드들 중지
	bIsOrbiting = false;
	bIsOrbitingWithSpline = false;
	
	// 호버링 모드 시작
	bIsHovering = true;
	
	// 호버링 위치 설정
	if (HoverLocation == FVector::ZeroVector)
	{
		// 현재 위치를 호버링 위치로 설정
		HoverLocation = OwnerCharacter->GetActorLocation();
	}
	
	// 호버링 위치 설정 (현재 높이 그대로 호버링)
	this->HoveringLocation = HoverLocation;
	HoveringProgress = 0.0f;
	
	// 디버그 로그
	CLog::Log(FString::Printf(TEXT("호버링 시작 - 현재위치: %s, 호버링위치: %s"), 
		*OwnerCharacter->GetActorLocation().ToString(), *HoveringLocation.ToString()));
	
	
	// 주기적 이동 타이머 초기화
	PeriodicMovementTimer = 0.0f;
	NextPeriodicMovementTime = FMath::RandRange(PeriodicMovementMinTime, PeriodicMovementMaxTime);
	
}

void UFlyingComponent::StopHovering()
{
	if (!bIsHovering) return;
	
	bIsHovering = false;
	HoveringProgress = 0.0f;
}

void UFlyingComponent::UpdateHovering(float DeltaTime)
{
	if (!OwnerCharacter || !bIsHovering) return;
	
	// 기본 호버링: 지정된 위치에 정확히 멈추기
	bool bReached = MoveToLocation(HoveringLocation, HoveringSpeed, 50.0f);
		if (bReached && OwnerCharacter->GetController())
		{
			OwnerCharacter->GetController()->StopMovement();
	}
	
		// 호버링 중: 플레이어를 내려다보기 (상태 머신이 Turn 상태가 아닐 때만)
		if (CurrentFlyingState != EFlyingState::Turn && CurrentFlyingState != EFlyingState::Fly)
		{
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
				
				// 부드러운 회전 적용 (거리 유지와 같은 속도)
				FRotator CurrentRotation = OwnerCharacter->GetActorRotation();
				float SlowRotationSpeed = 2.5f; // 5도/초 (거리 유지와 같은 속도)
				FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, SlowRotationSpeed);
				
				OwnerCharacter->SetActorRotation(NewRotation);
			}
		}
		
	// 디버그 시각화
	if (GetWorld())
	{
		FVector CurrentLocation = OwnerCharacter->GetActorLocation();
		
			// 호버링 중: 플레이어와 호버링 위치 표시
		APawn* DebugTargetPawn = nullptr;
			for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
			{
				APlayerController* PC = Iterator->Get();
				if (PC && PC->GetPawn())
				{
				DebugTargetPawn = PC->GetPawn();
					break;
				}
			}
			
		if (DebugTargetPawn)
			{
			FVector PlayerLocation = DebugTargetPawn->GetActorLocation();
				DrawDebugLine(GetWorld(), CurrentLocation, PlayerLocation, FColor::Yellow, false, 0.1f, 0, 2.0f);
			}
			
			DrawDebugSphere(GetWorld(), HoveringLocation, 50.0f, 12, FColor::Purple, false, 0.1f, 0, 3.0f);
			DrawDebugLine(GetWorld(), CurrentLocation, HoveringLocation, FColor::Purple, false, 0.1f, 0, 2.0f);
	}
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
			CLog::Log("=== 이륙 쿨타임 완료 - 이륙 가능 ===");
		}
		else
		{
			// 1초마다 쿨타임 남은 시간 로그
			static float TakeoffLogTimer = 0.0f;
			TakeoffLogTimer += DeltaTime;
			if (TakeoffLogTimer >= 1.0f)
			{
				CLog::Log(FString::Printf(TEXT("이륙 쿨타임 진행 중: %.1f초 남음 (전체: %.1f초)"), 
					TakeoffCooldownTimer, TakeoffCooldownTime));
				TakeoffLogTimer = 0.0f;
			}
		}
	}
	else
	{
		// 이륙 가능 상태일 때 주기적으로 로그
		static float TakeoffAvailableLogTimer = 0.0f;
		TakeoffAvailableLogTimer += DeltaTime;
		if (TakeoffAvailableLogTimer >= 5.0f) // 5초마다
		{
			CLog::Log("이륙 가능 상태 - 쿨타임 없음");
			TakeoffAvailableLogTimer = 0.0f;
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
			CLog::Log("=== 착륙 쿨타임 완료 - 착륙 가능 ===");
		}
		else
		{
			// 1초마다 쿨타임 남은 시간 로그
			static float LandingLogTimer = 0.0f;
			LandingLogTimer += DeltaTime;
			if (LandingLogTimer >= 1.0f)
			{
				CLog::Log(FString::Printf(TEXT("착륙 쿨타임 진행 중: %.1f초 남음 (전체: %.1f초)"), 
					LandingCooldownTimer, LandingCooldownTime));
				LandingLogTimer = 0.0f;
			}
		}
	}
	else
	{
		// 착륙 가능 상태일 때 주기적으로 로그
		static float LandingAvailableLogTimer = 0.0f;
		LandingAvailableLogTimer += DeltaTime;
		if (LandingAvailableLogTimer >= 5.0f) // 5초마다
		{
			CLog::Log("착륙 가능 상태 - 쿨타임 없음");
			LandingAvailableLogTimer = 0.0f;
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
	bIsDistanceMaintaining = false;
	bIsSplineDistanceMaintaining = false;

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
	CurrentRandomOffset = FVector::ZeroVector;
	DistanceMaintenanceTarget = FVector::ZeroVector;
	SplineDistanceMaintenanceTarget = FVector::ZeroVector;

	// 5. 프로그레스 및 타이머 초기화
	TakeoffProgress = 0.0f;
	LandingProgress = 0.0f;
	HoveringProgress = 0.0f;
	HoveringFloatTimer = 0.0f;
	AltitudeVariationTimer = 0.0f;
	AltitudeVariationOffset = 0.0f;

	// 6. 스플라인 관련 초기화
	CurrentSplineIndex = 0;
	SplineProgress = 0.0f;

	// 7. 궤도 이동 관련 초기화
	OrbitCenter = FVector::ZeroVector;
	CurrentOrbitAngle = 0.0f;

	// 8. 쿨타임 초기화
	bCanTakeoff = true;
	bCanLanding = true;
	TakeoffCooldownTimer = 0.0f;
	LandingCooldownTimer = 0.0f;


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
	float MinSpeedMultiplier = 0.3f;
	float MaxSpeedMultiplier = 1.2f;
	
	return FMath::Lerp(MinSpeedMultiplier, MaxSpeedMultiplier, SpeedMultiplier);
}

/**
 * @brief 호버링 중 환경 분석 디버그 (플레이어 기준 1000 반지름 원 체크)
 */
void UFlyingComponent::DebugHoveringEnvironmentAnalysis()
{
	if (!bEnableHoveringDebug || !GetWorld() || !OwnerCharacter)
	{
		return;
	}
	
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
	
	if (!TargetPawn)
	{
		CLog::Log("호버링 환경 분석: 플레이어를 찾을 수 없습니다!");
		return;
	}
	
	FVector PlayerLocation = TargetPawn->GetActorLocation();
	FVector BossLocation = OwnerCharacter->GetActorLocation();
	
	CLog::Log("=== 호버링 환경 분석 시작 ===");
	CLog::Log(FString::Printf(TEXT("플레이어 위치: %s"), *PlayerLocation.ToString()));
	CLog::Log(FString::Printf(TEXT("보스 위치: %s"), *BossLocation.ToString()));
	CLog::Log(FString::Printf(TEXT("분석 반지름: %f"), EnvironmentAnalysisRadius));
	CLog::Log(FString::Printf(TEXT("체크 포인트 수: %d"), EnvironmentAnalysisPoints));
	
	// 플레이어를 중심으로 기본 원 그리기 (나중에 색상 변경됨)
	// DrawDebugCircle(GetWorld(), PlayerLocation, EnvironmentAnalysisRadius, EnvironmentAnalysisPoints, FColor::Blue, false, 0.1f, 0, 3.0f);
	
	// 원주상의 각 포인트에서 라인트레이스 수행
	int32 AccessiblePoints = 0;
	int32 BlockedPoints = 0;
	
	for (int32 i = 0; i < EnvironmentAnalysisPoints; i++)
	{
		// 원주상의 각도 계산 (0도부터 시작해서 시계방향)
		float Angle = (360.0f / EnvironmentAnalysisPoints) * i;
		float RadianAngle = FMath::DegreesToRadians(Angle);
		
		// 원주상의 포인트 위치 계산 (800 높이에서)
		FVector CirclePoint = PlayerLocation + FVector(
			FMath::Cos(RadianAngle) * EnvironmentAnalysisRadius,
			FMath::Sin(RadianAngle) * EnvironmentAnalysisRadius,
			800.0f // 800 높이에서 원 생성
		);
		
		// 플레이어 위치에서 해당 포인트로 라인트레이스
		FVector TraceStart = PlayerLocation;
		FVector TraceEnd = CirclePoint;
		
		// Z축을 800 높이로 설정 (보스 비행 고도 400~600보다 높게)
		TraceStart.Z = PlayerLocation.Z + 800.0f;
		TraceEnd.Z = PlayerLocation.Z + 800.0f; // 800 높이에서 라인트레이스
		
		FVector TraceDirection = (TraceEnd - TraceStart).GetSafeNormal();
		float TraceLength = FVector::Dist(TraceStart, TraceEnd);
		
		// 라인트레이스 수행 - 여러 채널로 시도
		FHitResult HitResult;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(TargetPawn); // 플레이어는 무시
		QueryParams.bTraceComplex = true; // 복잡한 충돌 체크
		
		// 여러 충돌 채널로 시도
		bool bHit = false;
		ECollisionChannel Channels[] = {
			ECC_WorldStatic,    // 정적 월드 지오메트리
			ECC_WorldDynamic,   // 동적 월드 지오메트리  
			ECC_Pawn,          // 폰 (다른 캐릭터들)
			ECC_Visibility     // 시각적 요소
		};
		
		for (ECollisionChannel Channel : Channels)
		{
			bHit = GetWorld()->LineTraceSingleByChannel(
				HitResult,
				TraceStart,
				TraceStart + TraceDirection * TraceLength,
				Channel,
				QueryParams
			);
			
			if (bHit)
			{
				CLog::Log(FString::Printf(TEXT("충돌 감지! 채널: %d, 거리: %f"), (int32)Channel, HitResult.Distance));
				break;
			}
		}
		
		// 디버그용 라인트레이스 시각화 (항상 표시)
		FColor DebugColor = bHit ? FColor::Red : FColor::Green;
		DrawDebugLine(GetWorld(), TraceStart, TraceStart + TraceDirection * TraceLength, DebugColor, false, 0.1f, 0, 2.0f);
		
		// 거리 정보 로그
		CLog::Log(FString::Printf(TEXT("포인트 %d: 플레이어에서 원주로 - 시작점 %s, 끝점 %s, 거리 %f"), 
			i, *TraceStart.ToString(), *TraceEnd.ToString(), TraceLength));
		
		// 결과에 따라 디버그 표시
		if (bHit)
		{
			// 장애물에 막힘 - 해당 방향의 원을 빨간색으로 표시
			DrawDebugSphere(GetWorld(), CirclePoint, 30.0f, 8, FColor::Red, false, 0.1f, 0, 3.0f);
			
			// 충돌 지점도 빨간색으로 표시
			DrawDebugSphere(GetWorld(), HitResult.Location, 20.0f, 6, FColor::Red, false, 0.1f, 0, 2.0f);
			
			BlockedPoints++;
			
			CLog::Log(FString::Printf(TEXT("포인트 %d (각도 %f도): 막힘 - %s (거리: %f)"), 
				i, Angle, *HitResult.GetActor()->GetName(), HitResult.Distance));
		}
		else
		{
			// 갈 수 있음 - 해당 방향의 원을 초록색으로 표시
			DrawDebugSphere(GetWorld(), CirclePoint, 25.0f, 6, FColor::Green, false, 0.1f, 0, 3.0f);
			AccessiblePoints++;
			
			CLog::Log(FString::Printf(TEXT("포인트 %d (각도 %f도): 갈 수 있음 (거리: %f)"), i, Angle, TraceLength));
		}
	}
	
	// 전체 원을 다시 그려서 결과 요약 표시
	FColor CircleColor = FColor::Blue;
	if (AccessiblePoints > BlockedPoints)
	{
		CircleColor = FColor::Green; // 대부분 갈 수 있음
	}
	else if (BlockedPoints > AccessiblePoints)
	{
		CircleColor = FColor::Red; // 대부분 막힘
	}
	else
	{
		CircleColor = FColor::Yellow; // 반반
	}
	
	// 800 높이에서 원 그리기
	FVector CircleCenter = PlayerLocation;
	CircleCenter.Z += 800.0f;
	DrawDebugCircle(GetWorld(), CircleCenter, EnvironmentAnalysisRadius, EnvironmentAnalysisPoints, CircleColor, false, 0.1f, 0, 5.0f);
	
	// 결과 요약
	float AccessibilityRatio = (float)AccessiblePoints / (float)EnvironmentAnalysisPoints * 100.0f;
	
	CLog::Log("=== 호버링 환경 분석 결과 ===");
	CLog::Log(FString::Printf(TEXT("갈 수 있는 포인트: %d"), AccessiblePoints));
	CLog::Log(FString::Printf(TEXT("막힌 포인트: %d"), BlockedPoints));
	CLog::Log(FString::Printf(TEXT("접근 가능성: %.1f%%"), AccessibilityRatio));
	
	// 접근 가능성에 따른 색상 변경
	if (AccessibilityRatio >= 70.0f)
	{
		CLog::Log("환경 상태: 양호 (70% 이상 접근 가능)");
	}
	else if (AccessibilityRatio >= 40.0f)
	{
		CLog::Log("환경 상태: 보통 (40-70% 접근 가능)");
	}
	else
	{
		CLog::Log("환경 상태: 제한적 (40% 미만 접근 가능)");
	}
}

/**
 * @brief 간단한 라인트레이스 테스트 (디버그용)
 */
void UFlyingComponent::DebugSimpleLineTraceTest()
{
	if (!GetWorld() || !OwnerCharacter)
	{
		CLog::Log("라인트레이스 테스트: 월드나 보스가 없습니다!");
		return;
	}
	
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
	
	if (!TargetPawn)
	{
		CLog::Log("라인트레이스 테스트: 플레이어를 찾을 수 없습니다!");
		return;
	}
	
	FVector BossLocation = OwnerCharacter->GetActorLocation();
	FVector PlayerLocation = TargetPawn->GetActorLocation();
	
	CLog::Log("=== 간단한 라인트레이스 테스트 ===");
	CLog::Log(FString::Printf(TEXT("보스 위치: %s"), *BossLocation.ToString()));
	CLog::Log(FString::Printf(TEXT("플레이어 위치: %s"), *PlayerLocation.ToString()));
	
	float Distance = FVector::Dist(BossLocation, PlayerLocation);
	CLog::Log(FString::Printf(TEXT("직선 거리: %f"), Distance));
	
	// 보스에서 플레이어로 라인트레이스 (테스트용)
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerCharacter);
	QueryParams.AddIgnoredActor(TargetPawn);
	QueryParams.bTraceComplex = true;
	
	// 여러 채널로 테스트
	ECollisionChannel Channels[] = {
		ECC_WorldStatic,
		ECC_WorldDynamic,
		ECC_Pawn,
		ECC_Visibility
	};
	
	const char* ChannelNames[] = {
		"WorldStatic",
		"WorldDynamic", 
		"Pawn",
		"Visibility"
	};
	
	for (int32 i = 0; i < 4; i++)
	{
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			BossLocation,
			PlayerLocation,
			Channels[i],
			QueryParams
		);
		
		if (bHit)
		{
			
			// 충돌 지점 표시
			DrawDebugSphere(GetWorld(), HitResult.Location, 50.0f, 12, FColor::Red, false, 2.0f, 0, 5.0f);
			DrawDebugLine(GetWorld(), BossLocation, HitResult.Location, FColor::Red, false, 2.0f, 0, 3.0f);
		}
		else
		{
		}
	}
	
	// 전체 라인 표시
	FColor LineColor = FColor::Yellow;
	DrawDebugLine(GetWorld(), BossLocation, PlayerLocation, LineColor, false, 2.0f, 0, 2.0f);
	
	CLog::Log("=== 라인트레이스 테스트 완료 ===");
}

void UFlyingComponent::StartRandomMovementFromStateTree()
{
	// 간단한 구현 - 실제로는 호버링만 시작
	StartHovering();
}

void UFlyingComponent::StartSplineSemicircleMovement()
{
	// 간단한 구현 - 실제로는 호버링만 시작
	StartHovering();
}

void UFlyingComponent::ResetSplineSemicircleMovement()
{
	// 간단한 구현 - 호버링 중지
	StopHovering();
}

void UFlyingComponent::StartDistanceMaintenance(float TargetDistance)
{
	if (!OwnerCharacter || !bIsFlying) return;
	
	// 다른 비행 모드들 중지
	bIsHovering = false;
	bIsOrbiting = false;
	bIsOrbitingWithSpline = false;
	
	// 거리 유지 시작
	bIsDistanceMaintaining = true;
	
	// 상태 머신을 Turn 상태로 변경
	ChangeState(EFlyingState::Turn);
	TargetDistanceFromPlayer = TargetDistance;
	
	// 1순위: 플레이어 정면 60도 내 위치 찾기
	FVector TargetPosition = FindPositionInPlayerView(TargetDistance);
	
	// 2순위: 60도 내에 없으면 360도 랜덤 위치
	if (TargetPosition == FVector::ZeroVector)
	{
		TargetPosition = FindRandomPositionAroundPlayer(TargetDistance);
		CLog::Log("거리 유지 - 플레이어 정면 60도 내 위치 없음, 360도 랜덤 위치 사용");
	}
	else
	{
		CLog::Log("거리 유지 - 플레이어 정면 60도 내 위치 사용");
	}
	
	DistanceMaintenanceTarget = TargetPosition;
	
	// 자연스러운 이동 초기화
	CurrentSpeed = 0.0f; // 속도 초기화
	
	CLog::Log(FString::Printf(TEXT("거리 유지 시작 - 목표거리: %f, 목표위치: %s"), 
		TargetDistance, *TargetPosition.ToString()));
}

void UFlyingComponent::StopDistanceMaintenance()
{
	if (!bIsDistanceMaintaining) return;
	
	bIsDistanceMaintaining = false;
	DistanceMaintenanceTarget = FVector::ZeroVector;
	
	// 자연스러운 이동 초기화
	CurrentSpeed = 0.0f;
	
	CLog::Log("거리 유지 이동 중지");
}

void UFlyingComponent::UpdateNaturalMovement(float DeltaTime)
{
	if (!OwnerCharacter || !CharacterMovement) return;
	
	// 1. 부드러운 방향 전환
	FVector DirectionDifference = TargetDirection - CurrentDirection;
	if (DirectionDifference.Size() > 0.01f)
	{
		// 회전 속도 제한
		float MaxRotationThisFrame = RotationSpeed * DeltaTime;
		float RotationAmount = FMath::Min(DirectionDifference.Size(), MaxRotationThisFrame);
		
		FVector RotationStep = DirectionDifference.GetSafeNormal() * RotationAmount;
		CurrentDirection = (CurrentDirection + RotationStep).GetSafeNormal();
	}
	
	// 2. 가속/감속 시스템
	FVector VelocityDifference = TargetVelocity - CurrentVelocity;
	float VelocityDifferenceSize = VelocityDifference.Size();
	
	if (VelocityDifferenceSize > 0.1f)
	{
		// 가속 또는 감속 결정
		bool bAccelerating = CurrentVelocity.Size() < TargetVelocity.Size();
		float AccelerationRateToUse = bAccelerating ? AccelerationRate : DecelerationRate;
		
		// 속도 변화량 계산
		float MaxVelocityChange = AccelerationRateToUse * DeltaTime;
		float VelocityChange = FMath::Min(VelocityDifferenceSize, MaxVelocityChange);
		
		// 방향 유지하면서 속도만 조절
		FVector VelocityDirection = VelocityDifference.GetSafeNormal();
		FVector VelocityChangeVector = VelocityDirection * VelocityChange;
		
		CurrentVelocity += VelocityChangeVector;
	}
	
	// 3. 관성 시스템 적용
	if (InertiaFactor > 0.0f)
	{
		// 현재 속도에 관성 적용
		CurrentVelocity = FMath::Lerp(CurrentVelocity, TargetVelocity, (1.0f - InertiaFactor) * DeltaTime * 10.0f);
	}
	
	// 4. 속도 제한
	float CurrentVelocitySize = CurrentVelocity.Size();
	if (CurrentVelocitySize > MaxSpeed)
	{
		CurrentVelocity = CurrentVelocity.GetSafeNormal() * MaxSpeed;
	}
	else if (CurrentVelocitySize < MinSpeed && TargetVelocity.Size() > MinSpeed)
	{
		CurrentVelocity = CurrentVelocity.GetSafeNormal() * MinSpeed;
	}
	
	// 5. 자연스러운 가감속 적용
	if (TargetVelocity.Size() > 0.0f)
	{
		float TargetSpeed = TargetVelocity.Size();
		
		// 가속/감속 로직
		if (CurrentSpeed < TargetSpeed)
		{
			// 가속
			CurrentSpeed += AccelerationRate * DeltaTime;
			CurrentSpeed = FMath::Min(CurrentSpeed, TargetSpeed);
		}
		else if (CurrentSpeed > TargetSpeed)
		{
			// 감속
			CurrentSpeed -= DecelerationRate * DeltaTime;
			CurrentSpeed = FMath::Max(CurrentSpeed, TargetSpeed);
		}
		
		// 현재 속도로 방향 벡터 업데이트
		CurrentVelocity = TargetVelocity.GetSafeNormal() * CurrentSpeed;
	}
	else
	{
		// 목표 속도가 0이면 감속
		CurrentSpeed -= DecelerationRate * DeltaTime;
		CurrentSpeed = FMath::Max(CurrentSpeed, 0.0f);
		
		if (CurrentSpeed > 0.0f)
		{
			CurrentVelocity = CurrentVelocity.GetSafeNormal() * CurrentSpeed;
		}
		else
		{
			CurrentVelocity = FVector::ZeroVector;
		}
	}
	
	// 6. 타겟 바라보기 (이동 중) - Turn 상태가 아니고 거리 유지 중이 아닐 때만
	if (CurrentVelocity.Size() > 0.0f && CurrentFlyingState != EFlyingState::Turn && !bIsDistanceMaintaining && !bIsSplineDistanceMaintaining)
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
		
		// 플레이어를 바라보도록 회전
		if (TargetPawn)
		{
			FVector PlayerLocation = TargetPawn->GetActorLocation();
			FVector BossLocation = OwnerCharacter->GetActorLocation();
			FVector LookDirection = (PlayerLocation - BossLocation).GetSafeNormal();
			
			// Yaw 회전만 적용 (수평 회전)
			FRotator TargetRotation = FRotationMatrix::MakeFromX(LookDirection).Rotator();
			TargetRotation.Pitch = 0.0f; // Pitch는 0으로 고정
			TargetRotation.Roll = 0.0f;  // Roll은 0으로 고정
			
			// 부드러운 회전
			FRotator CurrentRotation = OwnerCharacter->GetActorRotation();
			FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
			OwnerCharacter->SetActorRotation(NewRotation);
		}
	}
	
	// 7. 최종 속도 적용
	CharacterMovement->Velocity = CurrentVelocity;
	
	// 디버그 로그
	CLog::Log(FString::Printf(TEXT("자연스러운 이동 - 현재속도: %f, 목표속도: %f, 방향: %s"), 
		CurrentVelocity.Size(), TargetVelocity.Size(), *CurrentDirection.ToString()));
}

float UFlyingComponent::CalculateDistanceBasedSpeed(float Distance, float TargetMaxSpeed) const
{
	// 거리에 따른 속도 조절 - 스무스한 전환
	// 시작: 천천히 가속 -> 중간: 정상 속도 -> 도착: 천천히 감속
	
	if (Distance <= 150.0f)
	{
		// 도착 직전: 스무스한 감속
		float T = FMath::Clamp(Distance / 150.0f, 0.0f, 1.0f);
		float SmoothT = FMath::SmoothStep(0.0f, 1.0f, T);
		return FMath::Lerp(MinSpeed, TargetMaxSpeed * 0.4f, SmoothT);
	}
	else if (Distance <= 400.0f)
	{
		// 도착 근처: 스무스한 중간 속도
		float T = FMath::Clamp((Distance - 150.0f) / 250.0f, 0.0f, 1.0f);
		float SmoothT = FMath::SmoothStep(0.0f, 1.0f, T);
		return FMath::Lerp(TargetMaxSpeed * 0.4f, TargetMaxSpeed * 0.9f, SmoothT);
	}
	else if (Distance <= 1000.0f)
	{
		// 중간 거리: 정상 속도
		return TargetMaxSpeed;
	}
	else
	{
		// 시작: 스무스한 가속
		float T = FMath::Clamp((Distance - 1000.0f) / 500.0f, 0.0f, 1.0f);
		float SmoothT = FMath::SmoothStep(0.0f, 1.0f, T);
		return FMath::Lerp(MinSpeed, TargetMaxSpeed, SmoothT);
	}
}


FVector UFlyingComponent::FindPositionInPlayerView(float TargetDistance) const
{
	// 플레이어 찾기
	APawn* PlayerPawn = nullptr;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC && PC->GetPawn())
		{
			PlayerPawn = PC->GetPawn();
			break;
		}
	}
	
	if (!PlayerPawn) return FVector::ZeroVector;
	
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	FRotator PlayerRotation = PlayerPawn->GetActorRotation();
	
	// 플레이어가 바라보는 방향
	FVector PlayerForward = PlayerRotation.Vector();
	
	// 60도 내에서 랜덤 각도 선택
	float RandomAngle = FMath::RandRange(-30.0f, 30.0f); // ±30도 = 60도 범위
	FVector ViewDirection = PlayerForward.RotateAngleAxis(RandomAngle, FVector::UpVector);
	
	// 목표 위치 계산 (최소 거리 800 적용)
	float MinDistance = 800.0f;
	float ActualDistance = FMath::Max(TargetDistance, MinDistance);
	float Height = FMath::RandRange(400.0f, 800.0f);
	FVector TargetPosition = PlayerLocation + ViewDirection * ActualDistance + FVector(0, 0, Height);
	
	// 라인트레이스로 장애물 확인
	FHitResult HitResult;
	FVector Start = PlayerLocation + FVector(0, 0, 100); // 플레이어 눈 높이
	FVector End = TargetPosition;
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Start,
		End,
		ECC_Visibility
	);
	
	// 장애물이 없으면 해당 위치 반환
	if (!bHit)
	{
		return TargetPosition;
	}
	
	// 장애물이 있으면 FVector::ZeroVector 반환
	return FVector::ZeroVector;
}

FVector UFlyingComponent::FindRandomPositionAroundPlayer(float TargetDistance) const
{
	// 플레이어 찾기
	APawn* PlayerPawn = nullptr;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC && PC->GetPawn())
		{
			PlayerPawn = PC->GetPawn();
			break;
		}
	}
	
	if (!PlayerPawn) return FVector::ZeroVector;
	
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	
	// 최대 10번 시도해서 갈 수 있는 위치 찾기
	for (int32 i = 0; i < 10; i++)
	{
		// 360도 랜덤 각도
		float RandomAngle = FMath::RandRange(0.0f, 360.0f);
		FVector RandomDirection = FVector(
			FMath::Cos(FMath::DegreesToRadians(RandomAngle)),
			FMath::Sin(FMath::DegreesToRadians(RandomAngle)),
			0
		);
		
		// 목표 위치 계산 (최소 거리 800 적용)
		float MinDistance = 1500.0f;
		float ActualDistance = FMath::Max(TargetDistance, MinDistance);
		float Height = FMath::RandRange(400.0f, 800.0f);
		FVector TestPosition = PlayerLocation + RandomDirection * ActualDistance + FVector(0, 0, Height);
		
		// 라인트레이스로 장애물 확인
		FHitResult HitResult;
		FVector Start = PlayerLocation + FVector(0, 0, 100); // 플레이어 눈 높이
		FVector End = TestPosition;
		
		bool bHit = GetWorld()->LineTraceSingleByChannel(
			HitResult,
			Start,
			End,
			ECC_Visibility
		);
		
		// 장애물이 없으면 해당 위치 반환
		if (!bHit)
		{
			return TestPosition;
		}
	}
	
	// 10번 시도해도 갈 수 있는 위치가 없으면 기본 위치 반환
	CLog::Log("거리 유지 - 360도 랜덤에서도 갈 수 있는 위치 없음, 기본 위치 사용");
	return PlayerLocation + FVector(0, 0, 300); // 플레이어 위 300cm
}

void UFlyingComponent::UpdateDistanceMaintenance(float DeltaTime)
{
	if (!OwnerCharacter || !bIsDistanceMaintaining) return;
	
	// 상태 머신이 모든 로직을 처리하므로 여기서는 디버그 시각화만 수행
	// 디버그 시각화
	if (GetWorld())
	{
		FVector CurrentLocation = OwnerCharacter->GetActorLocation();
		DrawDebugLine(GetWorld(), CurrentLocation, DistanceMaintenanceTarget, FColor::Orange, false, 0.1f, 0, 2.0f);
		DrawDebugSphere(GetWorld(), DistanceMaintenanceTarget, 50.0f, 12, FColor::Orange, false, 0.1f, 0, 3.0f);
	}
}

void UFlyingComponent::StartSplineDistanceMaintenance(float TargetDistance)
{
	if (!OwnerCharacter || !bIsFlying) return;
	
	// 다른 비행 모드들 중지
	bIsHovering = false;
	bIsOrbiting = false;
	bIsOrbitingWithSpline = false;
	bIsDistanceMaintaining = false; // 기존 거리 유지도 중지
	
	// 스플라인 거리 유지 시작
	bIsSplineDistanceMaintaining = true;
	
	// 상태 머신을 Turn 상태로 변경
	ChangeState(EFlyingState::Turn);
	TargetDistanceFromPlayer = TargetDistance;
	
	// 가장 먼 스플라인 위치 찾기
	FVector TargetPosition = FindFarthestSplinePosition(TargetDistance);
	
	if (TargetPosition == FVector::ZeroVector)
	{
		CLog::Log("스플라인 거리 유지 - 가장 먼 스플라인 위치를 찾을 수 없음");
		StopSplineDistanceMaintenance();
		return;
	}
	
	SplineDistanceMaintenanceTarget = TargetPosition;
	
	// 자연스러운 이동 초기화
	CurrentSpeed = 0.0f; // 속도 초기화
	
	CLog::Log(FString::Printf(TEXT("스플라인 거리 유지 시작 - 목표거리: %f, 목표위치: %s"), 
		TargetDistance, *TargetPosition.ToString()));
}

void UFlyingComponent::StopSplineDistanceMaintenance()
{
	if (!bIsSplineDistanceMaintaining) return;
	
	bIsSplineDistanceMaintaining = false;
	SplineDistanceMaintenanceTarget = FVector::ZeroVector;
	
	// 자연스러운 이동 초기화
	CurrentSpeed = 0.0f;
	
	CLog::Log("스플라인 거리 유지 이동 중지");
}

FVector UFlyingComponent::FindFarthestSplinePosition(float TargetDistance) const
{
	if (!SpawnedSplineActor)
	{
		CLog::Log("스플라인 거리 유지 - 스폰된 스플라인 액터가 없습니다");
		return FVector::ZeroVector;
	}
	
	// 플레이어 찾기
	APawn* PlayerPawn = nullptr;
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC && PC->GetPawn())
		{
			PlayerPawn = PC->GetPawn();
			break;
		}
	}
	
	if (!PlayerPawn)
	{
		CLog::Log("스플라인 거리 유지 - 플레이어를 찾을 수 없습니다");
		return FVector::ZeroVector;
	}
	
	FVector PlayerLocation = PlayerPawn->GetActorLocation();
	TArray<USplineComponent*> Splines = SpawnedSplineActor->GetHorizontalSplines();
	
	if (Splines.Num() == 0)
	{
		CLog::Log("스플라인 거리 유지 - 스플라인이 없습니다");
		return FVector::ZeroVector;
	}
	
	// 모든 스플라인 포인트 중에서 플레이어로부터 가장 먼 위치 찾기
	FVector FarthestPosition = FVector::ZeroVector;
	float MaxDistance = 0.0f;
	
	for (int32 SplineIndex = 0; SplineIndex < Splines.Num(); SplineIndex++)
	{
		USplineComponent* CurrentSpline = Splines[SplineIndex];
		if (!CurrentSpline) continue;
		
		int32 NumPoints = CurrentSpline->GetNumberOfSplinePoints();
		
		// 각 스플라인의 모든 포인트를 체크
		for (int32 PointIndex = 0; PointIndex < NumPoints; PointIndex++)
		{
			FVector SplinePoint = CurrentSpline->GetLocationAtSplinePoint(PointIndex, ESplineCoordinateSpace::World);
			
			// 플레이어로부터의 거리 계산
			float DistanceToPlayer = FVector::Dist(PlayerLocation, SplinePoint);
			
			// 목표 거리 이상이면서 가장 먼 위치 찾기
			if (DistanceToPlayer >= TargetDistance && DistanceToPlayer > MaxDistance)
			{
				// 라인트레이스로 장애물 확인
				FHitResult HitResult;
				FVector Start = PlayerLocation + FVector(0, 0, 100); // 플레이어 눈 높이
				FVector End = SplinePoint;
				
				bool bHit = GetWorld()->LineTraceSingleByChannel(
					HitResult,
					Start,
					End,
					ECC_Visibility
				);
				
				// 장애물이 없으면 해당 위치를 후보로 선택
				if (!bHit)
				{
					FarthestPosition = SplinePoint;
					MaxDistance = DistanceToPlayer;
				}
			}
		}
	}
	
	if (FarthestPosition != FVector::ZeroVector)
	{
		CLog::Log(FString::Printf(TEXT("스플라인 거리 유지 - 가장 먼 위치 발견: %s (거리: %f)"), 
			*FarthestPosition.ToString(), MaxDistance));
	}
	else
	{
		CLog::Log("스플라인 거리 유지 - 목표 거리 이상의 접근 가능한 스플라인 위치를 찾을 수 없음");
	}
	
	return FarthestPosition;
}

void UFlyingComponent::UpdateSplineDistanceMaintenance(float DeltaTime)
{
	if (!OwnerCharacter || !bIsSplineDistanceMaintaining) return;
	
	// 상태 머신이 모든 로직을 처리하므로 여기서는 디버그 시각화만 수행
	// 디버그 시각화
	if (GetWorld())
	{
		FVector CurrentLocation = OwnerCharacter->GetActorLocation();
		DrawDebugLine(GetWorld(), CurrentLocation, SplineDistanceMaintenanceTarget, FColor::Magenta, false, 0.1f, 0, 2.0f);
		DrawDebugSphere(GetWorld(), SplineDistanceMaintenanceTarget, 50.0f, 12, FColor::Magenta, false, 0.1f, 0, 3.0f);
	}
}

// ===== 상태 머신 관련 함수들 =====


void UFlyingComponent::ChangeState(EFlyingState NewState)
{
	EFlyingState OldState = CurrentFlyingState;
	CurrentFlyingState = NewState;
	
	UE_LOG(LogTemp, Warning, TEXT("ChangeState: %d -> %d"), (int32)OldState, (int32)NewState);
}

void UFlyingComponent::UpdateStateMachine(float DeltaTime)
{
	if (!OwnerCharacter) return;
	
	static float LogTimer = 0.0f;
	LogTimer += DeltaTime;
	
	// 1초마다 현재 상태 로그 출력
	if (LogTimer >= 1.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdateStateMachine: CurrentState=%d, bIsHovering=%s, bIsDistanceMaintaining=%s"), 
			(int32)CurrentFlyingState, bIsHovering ? TEXT("true") : TEXT("false"), 
			bIsDistanceMaintaining ? TEXT("true") : TEXT("false"));
		LogTimer = 0.0f;
	}
	
	// 기존 시스템과 연동하여 상태 결정
	if (bIsHovering)
	{
		if (CurrentFlyingState != EFlyingState::Idle)
		{
			ChangeState(EFlyingState::Idle);
		}
	}
	else if (bIsDistanceMaintaining)
	{
		// 거리 유지 이동 중일 때 회전 처리
		if (CurrentFlyingState == EFlyingState::Idle)
		{
			ChangeState(EFlyingState::Turn);
		}
		else if (CurrentFlyingState == EFlyingState::Turn)
		{
			UpdateTurnState(DeltaTime);
		}
		else if (CurrentFlyingState == EFlyingState::Fly)
		{
			UpdateFlyState(DeltaTime);
		}
		else if (CurrentFlyingState == EFlyingState::FlyStop)
		{
			UpdateFlyStopState(DeltaTime);
		}
	}
	else if (bIsSplineDistanceMaintaining)
	{
		// 스플라인 거리 유지 이동 중일 때 회전 처리
		if (CurrentFlyingState == EFlyingState::Idle)
		{
			ChangeState(EFlyingState::Turn);
		}
		else if (CurrentFlyingState == EFlyingState::Turn)
		{
			UpdateSplineTurnState(DeltaTime);
		}
		else if (CurrentFlyingState == EFlyingState::Fly)
		{
			UpdateSplineFlyState(DeltaTime);
		}
		else if (CurrentFlyingState == EFlyingState::FlyStop)
		{
			UpdateSplineFlyStopState(DeltaTime);
		}
	}
	else
	{
		// 거리 유지가 중지되었지만 FlyStop 상태일 때는 계속 실행
		if (CurrentFlyingState == EFlyingState::FlyStop)
		{
			UpdateFlyStopState(DeltaTime);
		}
		else if (CurrentFlyingState != EFlyingState::Idle)
		{
			ChangeState(EFlyingState::Idle);
		}
	}
}

void UFlyingComponent::UpdateTurnState(float DeltaTime)
{
	if (!OwnerCharacter) return;
	
	// Turn 상태에서는 이동하지 않음 (속도 0으로 설정)
	CurrentVelocity = FVector::ZeroVector;
	TargetVelocity = FVector::ZeroVector;
	if (CharacterMovement)
	{
		CharacterMovement->Velocity = FVector::ZeroVector;
	}
	
	// 거리 유지 이동의 목표 방향으로 회전
	FVector CurrentForward = OwnerCharacter->GetActorForwardVector();
	FVector TargetForward = (DistanceMaintenanceTarget - OwnerCharacter->GetActorLocation()).GetSafeNormal();
	
	// 회전 각도 계산
	float DotProduct = FVector::DotProduct(CurrentForward, TargetForward);
	float Angle = FMath::Acos(FMath::Clamp(DotProduct, -1.0f, 1.0f)) * 180.0f / PI;
	
	// 회전 완료 체크 (15도 이내면 완료로 간주)
	if (Angle < 15.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Rotation Complete: Angle=%.2f"), Angle);
		ChangeState(EFlyingState::Fly);
		return;
	}
	
	// 매우 부드러운 회전 (180도를 12초로 기준)
	float SlowRotationSpeed = 5.0f; // 15도/초 (180도 회전 시 12초 소요, 매우 부드럽게)
	FRotator CurrentRotation = OwnerCharacter->GetActorRotation();
	FRotator TargetRotation = FRotationMatrix::MakeFromX(TargetForward).Rotator();
	
	// 매우 부드러운 회전을 위해 낮은 속도로 보간
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, SlowRotationSpeed);
	
	// 회전을 더 부드럽게 하기 위해 추가 보간
	FRotator FinalRotation = FMath::Lerp(CurrentRotation, NewRotation, 0.5f);
	OwnerCharacter->SetActorRotation(FinalRotation);
	
	UE_LOG(LogTemp, Warning, TEXT("Turn State: Angle=%.2f, Rotating to target"), Angle);
}

void UFlyingComponent::UpdateFlyState(float DeltaTime)
{
	if (!OwnerCharacter || !bIsDistanceMaintaining) return;
	
	// Fly 상태에서는 목표 위치로 이동
	bool bReached = MoveToLocation(DistanceMaintenanceTarget, FlyingSpeed * 2.0f, 100.0f);
	
	if (bReached)
	{
		UE_LOG(LogTemp, Warning, TEXT("Movement Complete: Reached target location"));
		
		// 거리 유지 중지
		StopDistanceMaintenance();
		
		// 도착하면 바로 이벤트 전송
		if (OwnerCharacter && OwnerCharacter->GetController())
		{ 
			UStateTreeComponent* StateTreeComp = OwnerCharacter->GetController()->FindComponentByClass<UStateTreeComponent>();
			if (StateTreeComp)
			{
				StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag("BOSS.Event.Interrupt"));
				UE_LOG(LogTemp, Warning, TEXT("Fly: Event sent immediately on arrival"));
			}
		}
		
		// FlyStop 상태로 전환 (1초 대기 후 이벤트 전송)
		ChangeState(EFlyingState::FlyStop);
	}
}

void UFlyingComponent::UpdateFlyStopState(float DeltaTime)
{
	if (!OwnerCharacter) return;
	
	// FlyStop 상태 진입 시 타이머 초기화
	if (!bFlyStopTimerStarted)
	{
		FlyStopEventTimer = 0.0f;
		bFlyStopEventSent = false;
		bFlyStopTimerStarted = true;
		UE_LOG(LogTemp, Warning, TEXT("FlyStop: Starting 1 second timer"));
	}
	
	// 1초 후 이벤트 전송
	FlyStopEventTimer += DeltaTime;
	UE_LOG(LogTemp, Warning, TEXT("FlyStop: Timer=%.3f, EventSent=%s"), FlyStopEventTimer, bFlyStopEventSent ? TEXT("true") : TEXT("false"));
	
	if (FlyStopEventTimer >= 1.0f && !bFlyStopEventSent)
	{
		UE_LOG(LogTemp, Warning, TEXT("FlyStop: Sending event after 1 second"));
		
		// StateTree에 이동 완료 이벤트 전송
		if (OwnerCharacter && OwnerCharacter->GetController())
		{ 
			UStateTreeComponent* StateTreeComp = OwnerCharacter->GetController()->FindComponentByClass<UStateTreeComponent>();
			if (StateTreeComp)
			{
				StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag("BOSS.Event.Interrupt"));
				UE_LOG(LogTemp, Warning, TEXT("FlyStop: Event sent successfully"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("FlyStop: StateTreeComponent not found"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("FlyStop: OwnerCharacter or Controller not found"));
		}
		
		bFlyStopEventSent = true;
		
		// 착륙 타이머 시작 (end 상태에서도 착륙 가능하도록)
		bCanLanding = false;
		LandingCooldownTimer = LandingCooldownTime;
		CLog::Log(FString::Printf(TEXT("=== FlyStop 완료 - 착륙 쿨타임 시작: %.1f초 ==="), LandingCooldownTime));
		
		// 이벤트 전송 후 Idle로 전환
		ChangeState(EFlyingState::Idle);
	}
	
}

void UFlyingComponent::UpdateSplineTurnState(float DeltaTime)
{
	if (!OwnerCharacter) return;
	
	// Turn 상태에서는 이동하지 않음 (속도 0으로 설정)
	CurrentVelocity = FVector::ZeroVector;
	TargetVelocity = FVector::ZeroVector;
	if (CharacterMovement)
	{
		CharacterMovement->Velocity = FVector::ZeroVector;
	}
	
	// 스플라인 거리 유지 이동의 목표 방향으로 회전
	FVector CurrentForward = OwnerCharacter->GetActorForwardVector();
	FVector TargetForward = (SplineDistanceMaintenanceTarget - OwnerCharacter->GetActorLocation()).GetSafeNormal();
	
	// 회전 각도 계산
	float DotProduct = FVector::DotProduct(CurrentForward, TargetForward);
	float Angle = FMath::Acos(FMath::Clamp(DotProduct, -1.0f, 1.0f)) * 180.0f / PI;
	
	// 회전 완료 체크 (15도 이내면 완료로 간주)
	if (Angle < 15.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spline Turn Complete: Angle=%.2f"), Angle);
		ChangeState(EFlyingState::Fly);
		return;
	}
	
	// 매우 부드러운 회전 (180도를 12초로 기준)
	float SlowRotationSpeed = 5.0f; // 15도/초 (180도 회전 시 12초 소요, 매우 부드럽게)
	FRotator CurrentRotation = OwnerCharacter->GetActorRotation();
	FRotator TargetRotation = FRotationMatrix::MakeFromX(TargetForward).Rotator();
	
	// 매우 부드러운 회전을 위해 낮은 속도로 보간
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, SlowRotationSpeed);
	
	// 회전을 더 부드럽게 하기 위해 추가 보간
	FRotator FinalRotation = FMath::Lerp(CurrentRotation, NewRotation, 0.5f);
	OwnerCharacter->SetActorRotation(FinalRotation);
	
	UE_LOG(LogTemp, Warning, TEXT("Spline Turn State: Angle=%.2f, Rotating to target"), Angle);
}

void UFlyingComponent::UpdateSplineFlyState(float DeltaTime)
{
	if (!OwnerCharacter || !bIsSplineDistanceMaintaining) return;
	
	// Fly 상태에서는 목표 위치로 이동하면서 목표를 바라보기
	bool bReached = MoveToLocation(SplineDistanceMaintenanceTarget, FlyingSpeed * 2.0f, 100.0f);
	
	// 이동 중에는 목표 위치를 바라보도록 회전
	if (!bReached)
	{
		FVector CurrentLocation = OwnerCharacter->GetActorLocation();
		FVector LookDirection = (SplineDistanceMaintenanceTarget - CurrentLocation).GetSafeNormal();
		
		// 목표 방향으로 회전
		FRotator TargetRotation = FRotationMatrix::MakeFromX(LookDirection).Rotator();
		TargetRotation.Pitch = 0.0f; // Pitch는 0으로 고정
		TargetRotation.Roll = 0.0f;  // Roll은 0으로 고정
		
		// 부드러운 회전
		FRotator CurrentRotation = OwnerCharacter->GetActorRotation();
		FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, RotationSpeed);
		OwnerCharacter->SetActorRotation(NewRotation);
	}
	
	if (bReached)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spline Movement Complete: Reached target location"));
		
		// 스플라인 거리 유지 중지
		StopSplineDistanceMaintenance();
		
		// 도착하면 바로 이벤트 전송
		if (OwnerCharacter && OwnerCharacter->GetController())
		{ 
			UStateTreeComponent* StateTreeComp = OwnerCharacter->GetController()->FindComponentByClass<UStateTreeComponent>();
			if (StateTreeComp)
			{
				StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag("BOSS.Event.Interrupt"));
				UE_LOG(LogTemp, Warning, TEXT("Spline Fly: Event sent immediately on arrival"));
			}
		}
		
		// FlyStop 상태로 전환 (1초 대기 후 이벤트 전송)
		ChangeState(EFlyingState::FlyStop);
	}
}

void UFlyingComponent::UpdateSplineFlyStopState(float DeltaTime)
{
	if (!OwnerCharacter) return;
	
	// FlyStop 상태 진입 시 타이머 초기화
	if (!bFlyStopTimerStarted)
	{
		FlyStopEventTimer = 0.0f;
		bFlyStopEventSent = false;
		bFlyStopTimerStarted = true;
		UE_LOG(LogTemp, Warning, TEXT("Spline FlyStop: Starting 1 second timer"));
	}
	
	// 1초 후 이벤트 전송
	FlyStopEventTimer += DeltaTime;
	UE_LOG(LogTemp, Warning, TEXT("Spline FlyStop: Timer=%.3f, EventSent=%s"), FlyStopEventTimer, bFlyStopEventSent ? TEXT("true") : TEXT("false"));
	
	if (FlyStopEventTimer >= 1.0f && !bFlyStopEventSent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Spline FlyStop: Sending event after 1 second"));
		
		// StateTree에 이동 완료 이벤트 전송
		if (OwnerCharacter && OwnerCharacter->GetController())
		{ 
			UStateTreeComponent* StateTreeComp = OwnerCharacter->GetController()->FindComponentByClass<UStateTreeComponent>();
			if (StateTreeComp)
			{
				StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag("BOSS.Event.Interrupt"));
				UE_LOG(LogTemp, Warning, TEXT("Spline FlyStop: Event sent successfully"));
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Spline FlyStop: StateTreeComponent not found"));
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Spline FlyStop: OwnerCharacter or Controller not found"));
		}
		
		bFlyStopEventSent = true;
		
		// 착륙 타이머 시작 (end 상태에서도 착륙 가능하도록)
		bCanLanding = false;
		LandingCooldownTimer = LandingCooldownTime;
		CLog::Log(FString::Printf(TEXT("=== Spline FlyStop 완료 - 착륙 쿨타임 시작: %.1f초 ==="), LandingCooldownTime));
		
		// 이벤트 전송 후 Idle로 전환
		ChangeState(EFlyingState::Idle);
	}
}
