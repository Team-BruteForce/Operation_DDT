// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Projectile/BossProjectileOrb.h"
#include "Global.h"
#include "Boss/Component/CBossTargetingComponent.h"
#include "Boss/Projectile/BossProjectileActor.h"

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
		ABossProjectileActor* Projectile = GetWorld()->SpawnActor<ABossProjectileActor>(ProjectileClass, GetActorLocation(), FRotator(0));
		if (Projectile)
		{
			CLog::Log("ABossProjectileOrb - 프로젝타일 스폰 성공");
			Projectile->FireProjectile(TargetingComp->FindPlayer());
		}
		else
		{
			CLog::Log("ABossProjectileOrb - 프로젝타일 스폰 실패");
		}
	}
	else
	{
		CLog::Log("ABossProjectileOrb - TargetingComp 또는 플레이어를 찾을 수 없음");
	}
}

