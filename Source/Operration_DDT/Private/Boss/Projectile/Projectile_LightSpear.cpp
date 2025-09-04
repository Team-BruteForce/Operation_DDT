// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/Projectile/Projectile_LightSpear.h"
#include "Global.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Boss/Component/CBossTargetingComponent.h"

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
}

// Called every frame
void AProjectile_LightSpear::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CurrentTime += DeltaTime;
	
	// 타겟 방향으로 직선 이동 (중력 영향 없음)
	FVector NewLocation = GetActorLocation() + TargetDirection * ProjectileSpeed * DeltaTime;
	SetActorLocation(NewLocation);
	
	// 3초 후 파괴
	if (CurrentTime >= LifeTime)
	{
		Destroy();
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
			PlayDestroyEffect();
			// 0.25초 후 파괴
			FTimerHandle DestroyTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, [this]()
			{
				Destroy();
			}, 0.25f, false);
			
			// 이동 정지
			SetActorTickEnabled(false);
		}
}