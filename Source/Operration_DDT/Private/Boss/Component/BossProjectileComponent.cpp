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
	CHelpers::GetClass<ABossProjectileOrb>(&ProjectileOrbClass,AssetPaths::Boss_Orb);
}

// 기존 단일 발사 함수
void UBossProjectileComponent::ShotProjectile()
{
	UCBossTargetingComponent* TargetingComp = CHelpers::GetComponent<UCBossTargetingComponent>(GetOwner());
	if (TargetingComp && TargetingComp->FindPlayer())
	{
		ACharacter* Boss = Cast<ACharacter>(GetOwner());
		if (Boss)
		{
			// 플레이어 방향으로 회전 계산 (창이 세워서 날아가도록)
			FVector PlayerLocation = TargetingComp->FindPlayer()->GetActorLocation();
			FVector BossLocation = Boss->GetActorLocation();
			FVector Direction = (PlayerLocation - BossLocation).GetSafeNormal();
			FRotator TargetRotation = Direction.Rotation();
			
			
			// 보스 소켓 위치에서 스폰 (플레이어 방향으로 회전)
			FVector SpawnLocation = Boss->GetMesh()->GetSocketLocation(FName("LightningSpear"));
			
			AProjectile_LightSpear* Projectile = GetWorld()->SpawnActor<AProjectile_LightSpear>(ProjectileSpearClass, SpawnLocation, TargetRotation);
			if (Projectile)
			{
				// 오너 설정
				Projectile->SetOwner(GetOwner());
				
				// 발사 함수 호출
				Projectile->FireProjectile();
			}
		}
	}
}

void UBossProjectileComponent::SpawnOrb()
{
		ExitOrb=true;
	// 0.5초 간격으로 오브 3개 소환
	GetWorld()->GetTimerManager().SetTimer(OrbTimerHandle, [this]()
	{
		// 오브 소환 카운터 증가
		OrbSpawnCount++;
		
		// 소켓 이름 동적 생성
		FString SocketNameString = FString::Printf(TEXT("OrbSocket_%d"), OrbSpawnCount);
		FName SocketName = FName(*SocketNameString);
		ACharacter* Boss=Cast<ACharacter>(GetOwner());
		
		// 소켓 위치 가져오기
		FVector SocketLocation = Boss->GetMesh()->GetSocketLocation(SocketName);
		FRotator SocketRotation = Boss->GetMesh()->GetSocketRotation(SocketName);
		
		// 오브 스폰 시 소유자 설정
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		
		ABossProjectileOrb* Orb = GetWorld()->SpawnActor<ABossProjectileOrb>(ProjectileOrbClass, SocketLocation, SocketRotation, SpawnParams);
		if (Orb)
		{
			UE_LOG(LogTemp, Warning, TEXT("SpawnOrb - 오브 스폰 성공 (소켓: %s, 카운트: %d)"), *SocketNameString, OrbSpawnCount);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("SpawnOrb - 오브 스폰 실패 (소켓: %s, 카운트: %d)"), *SocketNameString, OrbSpawnCount);
		}
	}, 0.5f, true, 0.0f);
	
	// 3번 소환 후 타이머 정리
	FTimerHandle CleanupTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(CleanupTimerHandle, [this]()
	{
		GetWorld()->GetTimerManager().ClearTimer(OrbTimerHandle);
		// 카운터 리셋
		OrbSpawnCount = 0;
	}, 1.5f, false);
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

void UBossProjectileComponent::DestroyOrb()
{
	OrbSpawnCount--;
	if (OrbSpawnCount>0)
	{
		ExitOrb=false;
	}
	else
	{
		ExitOrb=true;
	}
}

