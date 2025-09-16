// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Projectile/BossProjectileOrb.h"
#include "Global.h"
#include "Boss/Component/CBossTargetingComponent.h"
#include "Boss/Projectile/BossProjectileActor.h"
#include "Boss/Component/BossProjectileComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
ABossProjectileOrb::ABossProjectileOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::GetClass<ABossProjectileActor>(&ProjectileClass,AssetPaths::Boss_Projectile);
	CHelpers::CreateComponent<USphereComponent>(this,&SphereComponent,"SphereComponent",RootComponent);

}

// Called when the game starts or when spawned
void ABossProjectileOrb::BeginPlay()
{
	Super::BeginPlay();
	CurrentTime = 0.0f;
	
	// 콜리전 설정
	if (SphereComponent)
	{
		// 콜리전 활성화
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		
		// 오버랩 이벤트 바인딩 (함수명 수정)
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossProjectileOrb::OnOverlap);
		
		CLog::Log("BossProjectileOrb - Collision Setup Complete");
	}
	else
	{
		CLog::Log("BossProjectileOrb - SphereComponent is null!");
	}
}

// Called every frame
void ABossProjectileOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CurrentTime = CurrentTime + DeltaTime;
	
	if (CurrentTime >= MaxTime)
	{
		SpawnProjectile();
		CurrentTime = 0.0f;
	}
}

void ABossProjectileOrb::SpawnProjectile()
{
	UCBossTargetingComponent* TargetingComp = CHelpers::GetComponent<UCBossTargetingComponent>(GetOwner());
	if (TargetingComp && TargetingComp->FindPlayer())
	{
		// 오브젝트 풀에서 보스 투사체 가져오기
		UBossProjectileComponent* BossProjectileComp = CHelpers::GetComponent<UBossProjectileComponent>(GetOwner());
		if (BossProjectileComp)
		{
			ABossProjectileActor* Projectile = BossProjectileComp->GetBossProjectileFromPool();
			if (Projectile)
			{
				// 상태 리셋
				Projectile->CurrentTime = 0.0f;
				Projectile->bHasPassedTarget = false;
				Projectile->bCanTrack = true;
				Projectile->bWaitingToMove = false;
				Projectile->bMovingToLocation = false;
				Projectile->WaitTimer = 0.0f;
				
				// 위치와 회전 설정 후 활성화
				Projectile->SetActorLocation(GetActorLocation());
				Projectile->SetActorRotation(FRotator(0));
				Projectile->SetActorHiddenInGame(false);
				Projectile->SetActorEnableCollision(true);
				Projectile->SetActorTickEnabled(true);
				
				// Shape 콜리전 직접 활성화 (오브젝트 풀 재사용 시 필요)
				if (Projectile->Shape)
				{
					Projectile->Shape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				}
				
				// 오너 설정
				Projectile->SetOwner(GetOwner());
				
				CLog::Log("ABossProjectileOrb - Get Projectile from Pool Success");
				Projectile->FireProjectile(TargetingComp->FindPlayer());
			}
			else
			{
				CLog::Log("ABossProjectileOrb - No Available Projectile in Pool");
			}
		}
		else
		{
			CLog::Log("ABossProjectileOrb - Cannot Find BossProjectileComponent");
		}
	}
	else
	{
		CLog::Log("ABossProjectileOrb - Cannot Find TargetingComp or Player");
	}
	
	// TODO: 오버랩 이벤트에서 오브를 풀로 반환하도록 나중에 구현
	/*
	if (bUseObjectPool)
	{
		// Owner 유효성 확인
		if (IsValid(GetOwner()))
		{
			class UBossProjectileComponent* ProjectileComp = CHelpers::GetComponent<class UBossProjectileComponent>(GetOwner());
			if (ProjectileComp)
			{
				ProjectileComp->ReturnOrbToPool(this);
			}
		}
	}
	else
	{
		Destroy();
	}
	*/
}


void ABossProjectileOrb::DestroyOrb()
{
	CLog::Log("BossProjectileOrb - DestroyOrb Called");
	
	if (bUseObjectPool)
	{
		// 오브젝트 풀 사용 시 풀로 반환
		if (IsValid(GetOwner()))
		{
			UBossProjectileComponent* ProjectileComp = CHelpers::GetComponent<UBossProjectileComponent>(GetOwner());
			if (ProjectileComp)
			{
				CLog::Log("BossProjectileOrb - Return to Object Pool");
				ProjectileComp->ReturnOrbToPool(this);
				
				// 오브 카운트 감소 및 ExitOrb 상태 업데이트
				ProjectileComp->DestroyOrb();
			}
			else
			{
				CLog::Log("BossProjectileOrb - No BossProjectileComponent, Destroy Actor");
				Destroy();
			}
		}
		else
		{
			CLog::Log("BossProjectileOrb - No Owner, Destroy Actor");
			Destroy();
		}
	}
	else
	{
		// 오브젝트 풀 미사용 시 액터 파괴
		CLog::Log("BossProjectileOrb - Object Pool Not Used, Destroy Actor");
		Destroy();
	}
}

void ABossProjectileOrb::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetClass()==ProjectileClass)return;
	if (OtherActor)
	{
		CLog::Log(FString::Printf(TEXT("BossProjectileOrb - OnOverlap! Collision Actor: %s"), *OtherActor->GetName()));
		DestroyOrb();
	}
	else
	{
		CLog::Log("BossProjectileOrb - OnOverlap Occurred but OtherActor is null");
	}
}

