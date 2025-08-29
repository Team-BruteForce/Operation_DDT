// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/Projectile/BossProjectileActor.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABossProjectileActor::ABossProjectileActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	SetRootComponent(Root);
	
	CHelpers::CreateComponent<UNiagaraComponent>(this, &NiagaraProjectile, "NiagaraProjectileComp", Root);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &ProjectileComp, "ProjectileComp");
	CHelpers::CreateComponent<USphereComponent>(this, &Shape, "Shape", Root);
	
	// 기본값 설정
	ProjectileSpeed = 1000.0f;
	LifeTime = 10.0f;
	bHasPassedTarget = false;
	
	// 새로운 기능 초기화
	bWaitingToMove = false;
	bMovingToLocation = false;
	WaitTimer = 0.0f;
	WaitDuration = 2.0f;
}

// Called when the game starts or when spawned
void ABossProjectileActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABossProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentTime += DeltaTime;
	
	// 새로운 기능: 대기 후 지점 이동
	if (bWaitingToMove)
	{
		WaitTimer += DeltaTime;
		
		// 대기 시간이 끝나면 지점으로 이동 시작
		if (WaitTimer >= WaitDuration)
		{
			bWaitingToMove = false;
			bMovingToLocation = true;
			UE_LOG(LogTemp, Warning, TEXT("대기 완료! 지점으로 이동 시작"));
		}
		else
		{
			// 대기 중에는 제자리에 고정
			return;
		}
	}
	
	// 지점으로 이동 중
	if (bMovingToLocation)
	{
		FVector Direction = (DelayedTargetLocation - GetActorLocation()).GetSafeNormal();
		FVector NewLocation = GetActorLocation() + Direction * ProjectileSpeed * DeltaTime;
		SetActorLocation(NewLocation);
		
		// 목표 지점에 도달했는지 확인
		float DistanceToTarget = FVector::Dist(GetActorLocation(), DelayedTargetLocation);
		if (DistanceToTarget < 50.0f)
		{
			UE_LOG(LogTemp, Warning, TEXT("목표 지점 도달!"));
			bMovingToLocation = false;
		}
		
		return;
	}
	
	// 기존 추적 로직 (변경 없음)
	if (!bHasPassedTarget)
	{
		// Phase 1: 타겟 추적 (관통)
		if (TargetActor && TargetActor->IsValidLowLevel())
		{
			// 타겟 방향으로 이동
			FVector TargetLocation = TargetActor->GetActorLocation();
			FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
			
			FVector NewLocation = GetActorLocation() + Direction * ProjectileSpeed * DeltaTime;
			SetActorLocation(NewLocation);
			
			// 타겟 방향으로 회전
			SetActorRotation(Direction.Rotation());
			
			// 타겟을 완전히 통과했는지 확인 (시작점에서 타겟까지의 선을 넘어섰을 때)
			FVector StartToTarget = TargetLocation - StartLocation;
			FVector CurrentToStart = GetActorLocation() - StartLocation;
			
			// 현재 위치가 시작점에서 타겟까지의 선을 넘어섰는지 확인
			if (FVector::DotProduct(StartToTarget, CurrentToStart) > FVector::DotProduct(StartToTarget, StartToTarget))
			{
				// 타겟을 완전히 통과함
				bHasPassedTarget = true;
				LastDirection = Direction; // 마지막 이동 방향 저장
			}
		}
	}
	else
	{
		// Phase 2: 직진 (타겟 통과 후)
		FVector NewLocation = GetActorLocation() + LastDirection * ProjectileSpeed * DeltaTime;
		SetActorLocation(NewLocation);
	}
	
	// 수명이 다하면 파괴
	if (CurrentTime >= LifeTime)
	{
		Destroy();
	}
}

void ABossProjectileActor::FireProjectile(AActor* Target)
{
	TargetActor = Target;
	CurrentTime = 0.0f;
	bHasPassedTarget = false;
	
	// 시작 위치 저장
	StartLocation = GetActorLocation();
	
	// VFX 활성화
	if (NiagaraProjectile)
	{
		NiagaraProjectile->Activate();
	}
}

// 새로운 기능: 스폰 후 n초 대기 후 원하는 지점으로 이동
void ABossProjectileActor::FireProjectileToLocation(const FVector& TargetLocation, float WaitTime)
{
	
	// 기존 추적 기능 비활성화
	TargetActor = nullptr;
	bHasPassedTarget = true;
	
	// 새로운 기능 활성화
	bWaitingToMove = true;
	bMovingToLocation = false;
	WaitTimer = 0.0f;
	WaitDuration = WaitTime;
	DelayedTargetLocation = TargetLocation;
	
	// VFX 활성화
	if (NiagaraProjectile)
	{
		NiagaraProjectile->Activate();
	}
	
}

