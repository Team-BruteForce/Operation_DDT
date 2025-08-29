// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Component/BossProjectileComponent.h"
#include "Global.h"
#include "Boss/Component/CBossTargetingComponent.h"
#include "Boss/Projectile/BossProjectileActor.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values for this component's properties
UBossProjectileComponent::UBossProjectileComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CHelpers::GetClass<ABossProjectileActor>(&ProjectileClass,AssetPaths::Boss_Projectile);
}

// 기존 단일 발사 함수
void UBossProjectileComponent::ShotProjectile()
{
	UE_LOG(LogTemp, Warning, TEXT("ShotProjectile() 호출됨"));
	
	UCBossTargetingComponent* TargetingComp = CHelpers::GetComponent<UCBossTargetingComponent>(GetOwner());
	UE_LOG(LogTemp, Warning, TEXT("ShotProjectile - TargetingComp: %s"), TargetingComp ? TEXT("찾음") : TEXT("못찾음"));
	
	if (TargetingComp && TargetingComp->FindPlayer())
	{
		UE_LOG(LogTemp, Warning, TEXT("ShotProjectile - 플레이어 찾음"));
		ABossProjectileActor* Projectile = GetWorld()->SpawnActor<ABossProjectileActor>(ProjectileClass, GetOwner()->GetActorLocation(), FRotator(0));
		if (Projectile)
		{
			UE_LOG(LogTemp, Warning, TEXT("ShotProjectile - 투사체 스폰 성공"));
			Projectile->FireProjectile(TargetingComp->FindPlayer());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ShotProjectile - 투사체 스폰 실패"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ShotProjectile - TargetingComp 또는 플레이어 못찾음"));
	}
}

// 새로운 기능: 타겟 액터를 받아서 발사 직전에 타겟의 위치로 이동
void UBossProjectileComponent::ShotProjectileToLocation(AActor* Target, float WaitTime)
{
	// UE_LOG(LogTemp, Warning, TEXT("ShotProjectileToLocation() 호출됨 - 타겟: %s, 대기시간: %.1f초"), Target ? TEXT("찾음") : TEXT("못찾음"), WaitTime);
	
	if (!Target)
	{
		UE_LOG(LogTemp, Error, TEXT("ShotProjectileToLocation - 타겟이 null입니다"));
		return;
	}
	ACharacter* Player=Cast<ACharacter>(Target);
	
	// 발사 직전에 타겟의 현재 위치 가져오기
	FVector TargetLocation = Player->GetMesh()->GetBoneLocation(FName("root"));
	
	// 투사체 스폰
	ABossProjectileActor* Projectile = GetWorld()->SpawnActor<ABossProjectileActor>(ProjectileClass, GetOwner()->GetActorLocation(), FRotator(0));
	if (Projectile)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShotProjectileToLocation - 투사체 스폰 성공"));
		// 새로운 기능 호출 (타겟의 현재 위치로 이동)
		Projectile->FireProjectileToLocation(TargetLocation, WaitTime);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ShotProjectileToLocation - 투사체 스폰 실패"));
	}
}