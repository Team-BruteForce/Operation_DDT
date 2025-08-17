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
}

// Called every frame
void UCBossMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
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

