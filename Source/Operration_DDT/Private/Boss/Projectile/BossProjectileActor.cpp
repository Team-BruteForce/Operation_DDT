// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/Projectile/BossProjectileActor.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABossProjectileActor::ABossProjectileActor()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	SetRootComponent(Root);
	
	CHelpers::CreateComponent<UNiagaraComponent>(this, &NiagaraProjectile, "NiagaraProjectileComp", Root);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &NiagaraSpawnEffect, "NiagaraSpawnEffect", Root);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &NiagaraDestroyEffect, "NiagaraDestroyEffect", Root);
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
	
	// 충돌 설정
	if (Shape)
	{
		Shape->OnComponentBeginOverlap.AddDynamic(this, &ABossProjectileActor::OnProjectileHit);
	}
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
			bMovingToLocation = false;
		}
		
		return;
	}
	
	// 기존 추적 로직
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
			
			// 타겟을 완전히 통과했는지 확인
			FVector StartToTarget = TargetLocation - StartLocation;
			FVector CurrentToStart = GetActorLocation() - StartLocation;
			
			if (FVector::DotProduct(StartToTarget, CurrentToStart) > FVector::DotProduct(StartToTarget, StartToTarget))
			{
				bHasPassedTarget = true;
				LastDirection = Direction;
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
		PlayDestroyEffect();
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
	
	// 스폰 이펙트 재생
	PlaySpawnEffect();
}

// 스폰 시 나이아가라 이펙트 재생
void ABossProjectileActor::PlaySpawnEffect()
{
	if (NiagaraSpawnEffect)
	{
		NiagaraSpawnEffect->Activate();
	}
}

// 파괴 시 나이아가라 이펙트 재생
void ABossProjectileActor::PlayDestroyEffect()
{
	if (NiagaraDestroyEffect)
	{
		// 현재 위치로 설정
		NiagaraDestroyEffect->SetWorldLocation(GetActorLocation());
		
		// 이펙트가 이미 활성화되어 있다면 재시작
		if (NiagaraDestroyEffect->IsActive())
		{
			NiagaraDestroyEffect->Deactivate();
		}
		
		// 이펙트 활성화
		NiagaraDestroyEffect->Activate(true);
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

// 충돌 판정 함수
void ABossProjectileActor::OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ACharacter* Boss = Cast<ACharacter>(OtherActor))
	{
		
		// 충돌 후 콜리전 비활성화 (다단히트 방지)
		if (Shape)
		{
			Shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		UGameplayStatics::ApplyDamage(Boss,1,Boss->GetController(),Boss,nullptr);
		
		// 파괴 이펙트 재생
		PlayDestroyEffect();
		
		// 0.5초 후에 프로젝타일 파괴 (이펙트 재생 시간 확보)
		FTimerHandle DestroyTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, [this]()
		{
			Destroy();
		}, 0.25f, false);
		
		// 충돌 후 더 이상 이동하지 않도록 설정
		SetActorTickEnabled(false);
	}
}

