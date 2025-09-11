// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Projectile/BossProjectileOrb.h"
#include "Global.h"
#include "Boss/Component/CBossTargetingComponent.h"
#include "Boss/Projectile/BossProjectileActor.h"
#include "Boss/Component/BossProjectileComponent.h"

// Sets default values
ABossProjectileOrb::ABossProjectileOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::GetClass<ABossProjectileActor>(&ProjectileClass,AssetPaths::Boss_Projectile);

}

// Called when the game starts or when spawned
void ABossProjectileOrb::BeginPlay()
{
	Super::BeginPlay();
	CurrentTime = 0.0f;
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
				
				// 오너 설정
				Projectile->SetOwner(GetOwner());
				
				CLog::Log("ABossProjectileOrb - 프로젝타일 풀에서 가져오기 성공");
				Projectile->FireProjectile(TargetingComp->FindPlayer());
			}
			else
			{
				CLog::Log("ABossProjectileOrb - 프로젝타일 풀에서 사용 가능한 투사체가 없음");
			}
		}
		else
		{
			CLog::Log("ABossProjectileOrb - BossProjectileComponent를 찾을 수 없음");
		}
	}
	else
	{
		CLog::Log("ABossProjectileOrb - TargetingComp 또는 플레이어를 찾을 수 없음");
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

