/**
 * @file BossProjectileActor.cpp
 * @brief 보스 프로젝타일 액터 구현 파일
 * 
 * @details
 * 보스의 프로젝타일을 구현하는 액터입니다.
 * 나이아가라 이펙트와 프로젝타일 이동을 처리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Projectile/BossProjectileActor.h"
#include "Global.h"
#include "NiagaraComponent.h"
#include "Components/ShapeComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Boss/Component/BossProjectileComponent.h"
#include "Kismet/GameplayStatics.h"

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
	
	// 오디오 컴포넌트 생성
	CHelpers::CreateComponent<UAudioComponent>(this, &AudioComponent, "AudioComponent", Root);
	
	// 기본값 설정
	ProjectileSpeed = 1000.0f;
	LifeTime = 5.0f;
	bHasPassedTarget = false;
	
	// 추적 각도 제한 설정
	MaxTrackingAngle = 0.75f;     // 프레임당 최대 2도만 회전
	bCanTrack = true;
	
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
	
	// 기존 추적 로직 (각도 제한 추가)
	if (!bHasPassedTarget)
	{
		// Phase 1: 타겟 추적 (관통) - 방향 전환 각도 제한
		if (TargetActor && TargetActor->IsValidLowLevel() && bCanTrack)
		{
			// 타겟 방향 계산
			FVector TargetLocation = TargetActor->GetActorLocation();
			FVector DesiredDirection = (TargetLocation - GetActorLocation()).GetSafeNormal();
			
			// 현재 방향에서 원하는 방향으로의 각도 차이 계산
			float DotProduct = FVector::DotProduct(CurrentDirection, DesiredDirection);
			DotProduct = FMath::Clamp(DotProduct, -1.0f, 1.0f); // 안전한 범위로 제한
			float AngleDifference = FMath::RadiansToDegrees(FMath::Acos(DotProduct));
			
			FVector NewDirection;
			if (AngleDifference <= MaxTrackingAngle)
			{
				// 각도 내에서 완전 추적
				NewDirection = DesiredDirection;
			}
			else
			{
				// 각도 제한 내에서만 방향 전환
				FVector RotationAxis = FVector::CrossProduct(CurrentDirection, DesiredDirection);
				
				// CrossProduct가 0벡터인 경우 처리 (같은 방향)
				if (RotationAxis.IsNearlyZero())
				{
					NewDirection = CurrentDirection; // 방향 유지
				}
				else
				{
					RotationAxis = RotationAxis.GetSafeNormal();
					FQuat RotationQuat = FQuat(RotationAxis, FMath::DegreesToRadians(MaxTrackingAngle));
					NewDirection = RotationQuat.RotateVector(CurrentDirection);
				}
			}
			
			// 새로운 위치로 이동
			FVector NewLocation = GetActorLocation() + NewDirection * ProjectileSpeed * DeltaTime;
			SetActorLocation(NewLocation);
			
			// 방향 업데이트
			CurrentDirection = NewDirection;
			SetActorRotation(NewDirection.Rotation());
			
			// 타겟을 완전히 통과했는지 확인
			FVector StartToTarget = TargetLocation - StartLocation;
			FVector CurrentToStart = GetActorLocation() - StartLocation;
			
			if (FVector::DotProduct(StartToTarget, CurrentToStart) > FVector::DotProduct(StartToTarget, StartToTarget))
			{
				bHasPassedTarget = true;
				LastDirection = CurrentDirection;
			}
		}
		else if (!bCanTrack)
		{
			// 추적 불가능 시 직진
			FVector NewLocation = GetActorLocation() + LastDirection * ProjectileSpeed * DeltaTime;
			SetActorLocation(NewLocation);
		}
	}
	else
	{
		// Phase 2: 직진 (타겟 통과 후)
		FVector NewLocation = GetActorLocation() + LastDirection * ProjectileSpeed * DeltaTime;
		SetActorLocation(NewLocation);
	}
	
	// 수명이 다하면 풀로 반환 또는 파괴
	if (CurrentTime >= LifeTime)
	{
		// 발사 사운드 중지
		if (AudioComponent && AudioComponent->IsPlaying())
		{
			AudioComponent->Stop();
			CLog::Log("BossProjectileActor - Projectile Sound Stopped (LifeTime)");
		}
		
		PlayDestroyEffect();
		
		// 착탄 사운드 재생
		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				ImpactSound,
				GetActorLocation(),
				GetActorRotation(),
				1.0f,
				1.0f,
				0.0f,
				nullptr,
				nullptr
			);
		}
		
		if (bUseObjectPool)
		{
			// 오브젝트 풀에 반환
			if (IsValid(GetOwner()))
			{
				UBossProjectileComponent* BossProjectileComp = CHelpers::GetComponent<UBossProjectileComponent>(GetOwner());
				if (BossProjectileComp)
				{
					BossProjectileComp->ReturnBossProjectileToPool(this);
				}
			}
		}
		else
		{
			Destroy();
		}
	}
}

void ABossProjectileActor::FireProjectile(AActor* Target)
{
	TargetActor = Target;
	CurrentTime = 0.0f;
	bHasPassedTarget = false;
	bCanTrack = true;
	Shape->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	
	// 시작 위치 저장
	StartLocation = GetActorLocation();
	
	// 초기 방향 계산 및 저장
	if (TargetActor && TargetActor->IsValidLowLevel())
	{
		FVector TargetLocation = TargetActor->GetActorLocation();
		InitialDirection = (TargetLocation - StartLocation).GetSafeNormal();
		CurrentDirection = InitialDirection;  // 현재 방향도 초기화
	}
	
	// 스폰 이펙트 재생
	PlaySpawnEffect();
	
	// 발사 사운드 재생 (오디오 컴포넌트 사용)
	if (ProjectileSound && AudioComponent)
	{
		AudioComponent->SetSound(ProjectileSound);
		AudioComponent->Play();
		CLog::Log("BossProjectileActor - Projectile Sound Started");
	}
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
	
	// 발사 사운드 재생 (오디오 컴포넌트 사용)
	if (ProjectileSound && AudioComponent)
	{
		AudioComponent->SetSound(ProjectileSound);
		AudioComponent->Play();
		CLog::Log("BossProjectileActor - Projectile Sound Started (ToLocation)");
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
		
		// 발사 사운드 중지
		if (AudioComponent && AudioComponent->IsPlaying())
		{
			AudioComponent->Stop();
			CLog::Log("BossProjectileActor - Projectile Sound Stopped");
		}
		
		UGameplayStatics::ApplyDamage(Boss,10,Boss->GetController(),Boss,nullptr);
		
		// 파괴 이펙트 재생
		PlayDestroyEffect();
		
		// 착탄 사운드 재생
		if (ImpactSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				ImpactSound,
				GetActorLocation(),
				GetActorRotation(),
				1.0f,
				1.0f,
				0.0f,
				nullptr,
				nullptr
			);
		}
		
		// 0.25초 후에 풀로 반환 또는 파괴 (이펙트 재생 시간 확보)
		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, [this]()
		{
			if (bUseObjectPool)
			{
				// 오브젝트 풀에 반환
				if (IsValid(this) && IsValid(GetOwner()))
				{
					UBossProjectileComponent* BossProjectileComp = CHelpers::GetComponent<UBossProjectileComponent>(GetOwner());
					if (BossProjectileComp)
					{
						BossProjectileComp->ReturnBossProjectileToPool(this);
					}
				}
			}
			else
			{
				Destroy();
			}
		}, 0.25f, false);
		
		// 충돌 후 더 이상 이동하지 않도록 설정
		SetActorTickEnabled(false);
	}
}

