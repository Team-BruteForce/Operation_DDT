// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/Projectile/Projectile_LightSpear.h"
#include "Global.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/Component/CBossTargetingComponent.h"
#include "Boss/Component/BossProjectileComponent.h"

// Sets default values
AProjectile_LightSpear::AProjectile_LightSpear()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	SetRootComponent(Root);
	
	CHelpers::CreateComponent<UCapsuleComponent>(this, &Shape, "Shape", Root);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &NiagaraDestroyEffect, "NiagaraDestroyEffect", Root);
	CHelpers::CreateComponent<UNiagaraComponent>(this, &NiagaraProjectileEffect, "NiagaraProjectileEffect", Root);
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &ProjectileComp, "ProjectileComp");
	
	// 기본값 설정
	ProjectileSpeed = 7000.0f;  // 매우 빠른 속도
	LifeTime = 3.0f;            // 3초 후 파괴
	CurrentTime = 0.0f;
	
	// 충돌 설정
	if (Shape)
	{
		Shape->OnComponentBeginOverlap.AddDynamic(this, &AProjectile_LightSpear::OnProjectileHit);
	}
}

// Called when the game starts or when spawned
void AProjectile_LightSpear::BeginPlay()
{
	Super::BeginPlay();
}

// 발사 함수
void AProjectile_LightSpear::FireProjectile()
{
	
	// 타겟팅 컴포넌트 가져오기
	TargetingComp = CHelpers::GetComponent<UCBossTargetingComponent>(GetOwner());
	Shape->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	// 타겟 방향 계산
	if (TargetingComp && TargetingComp->FindPlayer())
	{
		FVector PlayerLocation = TargetingComp->FindPlayer()->GetActorLocation();
		FVector ProjectileLocation = GetActorLocation();
		TargetDirection = (PlayerLocation - ProjectileLocation).GetSafeNormal();
	}
	else
	{
		// 타겟을 찾지 못하면 현재 방향 사용
		TargetDirection = GetActorForwardVector();
	}
	
	// 타이머 리셋
	CurrentTime = 0.0f;
	
	// 발사 사운드 재생
	if (LightSpearSpawnSound)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			LightSpearSpawnSound,
			GetActorLocation(),
			GetActorRotation(),
			1.0f,
			1.0f,
			0.0f,
			SoundAttenuation,
			nullptr
		);
	}
	
	// 3초 후 풀로 반환은 Tick에서 처리 (중복 방지)
	/*
	if (bUseObjectPool)
	{
		// WeakPtr을 사용하여 안전한 참조
		TWeakObjectPtr<AProjectile_LightSpear> WeakThis = this;
		TWeakObjectPtr<AActor> WeakOwner = GetOwner();
		
		GetWorld()->GetTimerManager().SetTimer(LifeTimeTimerHandle, [WeakThis, WeakOwner]()
		{
			// WeakPtr로 유효성 확인
			if (!WeakThis.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Projectile_LightSpear - 3초 타이머: WeakThis가 유효하지 않음 (이미 파괴됨)"));
				return;
			}
			
			if (!WeakOwner.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Projectile_LightSpear - 3초 타이머: WeakOwner가 유효하지 않음"));
				return;
			}
			
			AProjectile_LightSpear* Projectile = WeakThis.Get();
			AActor* Owner = WeakOwner.Get();
			
			UE_LOG(LogTemp, Warning, TEXT("Projectile_LightSpear - 3초 타이머 실행: 풀로 반환 시도"));
			
			// 오브젝트 풀에 반환
			class UBossProjectileComponent* ProjectileComp = CHelpers::GetComponent<class UBossProjectileComponent>(Owner);
			if (ProjectileComp)
			{
				UE_LOG(LogTemp, Warning, TEXT("Projectile_LightSpear - 3초 타이머: 풀로 반환 성공"));
				ProjectileComp->ReturnProjectileToPool(Projectile);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Projectile_LightSpear - 3초 타이머: ProjectileComp를 찾을 수 없음"));
			}
		}, 3.0f, false);
	}
	*/
}

// Called every frame
void AProjectile_LightSpear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentTime += DeltaTime;
	
	// 타겟 방향으로 직선 이동 (중력 영향 없음)
	FVector NewLocation = GetActorLocation() + TargetDirection * ProjectileSpeed * DeltaTime;
	SetActorLocation(NewLocation);
	
	// 3초 후 풀로 반환 (오브젝트 풀 사용 시) 또는 파괴
	if (CurrentTime >= LifeTime)
	{
		if (bUseObjectPool)
		{
			// 타이머 클리어 (중복 실행 방지)
			if (LifeTimeTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(LifeTimeTimerHandle);
				LifeTimeTimerHandle.Invalidate();
			}
			
			
			// 오브젝트 풀에 반환
			if (IsValid(GetOwner()))
			{
				UBossProjectileComponent* BossProjectileComp = CHelpers::GetComponent<UBossProjectileComponent>(GetOwner());
				if (BossProjectileComp)
				{
					BossProjectileComp->ReturnProjectileToPool(this);
				}
			}
		}
		else
		{
			Destroy();
		}
	}
}


// 파괴 시 이펙트 재생
void AProjectile_LightSpear::PlayDestroyEffect()
{
	if (NiagaraDestroyEffect)
	{
		NiagaraDestroyEffect->SetWorldLocation(GetActorLocation());
		NiagaraDestroyEffect->Activate(true);
	}
}

// 충돌 판정 함수
void AProjectile_LightSpear::OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull( Cast<ACharacter>(OtherActor));
	ACharacter* Player = Cast<ACharacter>(OtherActor);
		if (Player->GetName()!=GetOwner()->GetName())
		{
			// 플레이어에게 데미지
			UGameplayStatics::ApplyDamage(Player, 10.0f, GetOwner()->GetInstigatorController(), GetOwner(), nullptr);
			
			// 충돌 후 콜리전 비활성화
			if (Shape)
			{
				Shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
			
			// 3초 타이머 클리어 (충돌로 인한 조기 반환)
			if (LifeTimeTimerHandle.IsValid())
			{
				GetWorld()->GetTimerManager().ClearTimer(LifeTimeTimerHandle);
				LifeTimeTimerHandle.Invalidate();
			}
			
			PlayDestroyEffect();
			
			// 착탄 사운드 재생
			if (LightSpearDestroySound)
			{
				UGameplayStatics::PlaySoundAtLocation(
					GetWorld(),
					LightSpearDestroySound,
					GetActorLocation(),
					GetActorRotation(),
					1.0f,
					1.0f,
					0.0f,
					SoundAttenuation,
					nullptr
				);
			}
			
			// 0.25초 후 파괴 또는 풀 반환
			FTimerHandle DestroyTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, [this]()
			{
				// 오브젝트가 여전히 유효한지 확인
				if (!IsValid(this))
				{
					return;
				}
				
				// 오브젝트 풀 사용 시 Destroy 대신 비활성화만
				if (bUseObjectPool)
				{
					// Owner 유효성 확인
					if (!IsValid(GetOwner()))
					{
						return;
					}
					
					// 오브젝트 풀에 반환
					UBossProjectileComponent* BossProjectileComp = CHelpers::GetComponent<UBossProjectileComponent>(GetOwner());
					if (BossProjectileComp)
					{
						BossProjectileComp->ReturnProjectileToPool(this);
					}
				}
				else
				{
					Destroy();
				}
			}, 0.25f, false);
			
			// 이동 정지
			SetActorTickEnabled(false);
		}
}