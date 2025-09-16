// Fill out your copyright notice in the Description page of Project Settings.

#include "Notifies/CAnimNotify_SpawnPoisonProjectile.h"
#include "ODH/ODH_Enemy/Projectile/CPoisonProjectile.h"
#include "ODH/ODH_Enemy/Projectile/CPoisonProjectilePoolManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

void UCAnimNotify_SpawnPoisonProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp)
		return;

	// 플레이어 찾기
	APawn* Player = FindPlayer(MeshComp);
	if (!Player)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, 
				TEXT("CAnimNotify_SpawnPoisonProjectile: Player not found!"));
		}
		return;
	}

	// 발사 위치 계산
	FVector SpawnLocation = MeshComp->GetComponentLocation() + SpawnOffset;
	
	// 목표 위치 계산 (플레이어 위치에서 일정 거리 떨어진 지점)
	FVector PlayerLocation = Player->GetActorLocation();
	FVector ToPlayer = (PlayerLocation - MeshComp->GetComponentLocation()).GetSafeNormal();
	FVector TargetLocation = PlayerLocation + ToPlayer * LaunchDistance;

	// 독성 프로젝타일 스폰
	SpawnPoisonProjectile(MeshComp, SpawnLocation, TargetLocation);
}

APawn* UCAnimNotify_SpawnPoisonProjectile::FindPlayer(USkeletalMeshComponent* MeshComp)
{
	if (!MeshComp)
		return nullptr;

	// 메쉬 컴포넌트의 소유자에서 플레이어 찾기
	AActor* Owner = MeshComp->GetOwner();
	if (!Owner)
		return nullptr;

	// 월드에서 플레이어 찾기
	APawn* Player = UGameplayStatics::GetPlayerPawn(Owner->GetWorld(), 0);
	return Player;
}

void UCAnimNotify_SpawnPoisonProjectile::SpawnPoisonProjectile(USkeletalMeshComponent* MeshComp, FVector SpawnLocation, FVector TargetLocation)
{
	if (!MeshComp || !MeshComp->GetWorld())
		return;

	UWorld* World = MeshComp->GetWorld();

	// PoisonProjectile 풀 매니저 찾기
	ACPoisonProjectilePoolManager* Pool = Cast<ACPoisonProjectilePoolManager>(UGameplayStatics::GetActorOfClass(World, ACPoisonProjectilePoolManager::StaticClass()));
	
	if (Pool)
	{
		// 풀에서 독성 프로젝타일 대여
		ACPoisonProjectile* PooledProjectile = Pool->AcquireProjectile(World);
		if (PooledProjectile)
		{
			PooledProjectile->SetActorLocation(SpawnLocation);
			PooledProjectile->SetOwner(MeshComp->GetOwner());
			PooledProjectile->SetInstigator(Cast<APawn>(MeshComp->GetOwner()));
			
			// 독성 프로젝타일 활성화
			PooledProjectile->ActivateProjectile(SpawnLocation, TargetLocation);

			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, 
					FString::Printf(TEXT("Poison Projectile spawned from pool by AnimNotify at %s"), *SpawnLocation.ToString()));
			}
			return;
		}
	}

	// 풀을 사용하지 못하면 기존 스폰 방식으로 폴백
	TSubclassOf<ACPoisonProjectile> ProjectileClassToUse = PoisonProjectileClass;
	if (!ProjectileClassToUse)
	{
		ProjectileClassToUse = ACPoisonProjectile::StaticClass();
	}

	// 독성 프로젝타일 스폰
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = MeshComp->GetOwner();
	SpawnParams.Instigator = Cast<APawn>(MeshComp->GetOwner());

	ACPoisonProjectile* SpawnedProjectile = World->SpawnActor<ACPoisonProjectile>(
		ProjectileClassToUse,
		SpawnLocation,
		FRotator::ZeroRotator,
		SpawnParams
	);

	if (SpawnedProjectile)
	{
		// 독성 프로젝타일 활성화
		SpawnedProjectile->ActivateProjectile(SpawnLocation, TargetLocation);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, 
				FString::Printf(TEXT("Poison Projectile spawned (fallback) by AnimNotify at %s"), *SpawnLocation.ToString()));
		}
	}
	else
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, 
				TEXT("Failed to spawn Poison Projectile from AnimNotify!"));
		}
	}
}
