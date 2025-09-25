/**
 * @file BossProjectileOrb.cpp
 * @brief 보스 프로젝타일 오브 구현 파일
 * 
 * @details
 * 보스의 오브 프로젝타일을 구현하는 파일입니다.
 * 타겟팅 컴포넌트와 연동하여 플레이어를 추적합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Projectile/BossProjectileOrb.h"
#include "Global.h"
#include "Boss/Component/CBossTargetingComponent.h"
#include "Boss/Projectile/BossProjectileActor.h"
#include "Boss/Component/BossProjectileComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
ABossProjectileOrb::ABossProjectileOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::GetClass<ABossProjectileActor>(&ProjectileClass,AssetPaths::Boss_Projectile);
	CHelpers::CreateComponent<USphereComponent>(this,&SphereComponent,"SphereComponent",RootComponent);
	
	// 오디오 컴포넌트 생성
	CHelpers::CreateComponent<UAudioComponent>(this,&AudioComponent,"AudioComponent",RootComponent);

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
	
	// BeginPlay에서는 소리 재생하지 않음 (오브젝트 풀 재사용 시 문제)
	// 실제 활성화 시점에 소리 재생하도록 수정 필요
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
				
				// 오디오 컴포넌트 리셋 (오브젝트 풀 재사용 시 필요)
				if (Projectile->AudioComponent)
				{
					Projectile->AudioComponent->Stop();
					Projectile->AudioComponent->SetSound(nullptr);
					CLog::Log("BossProjectileOrb - AudioComponent Reset for Pool Reuse");
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
	
	// 풀 반환 사운드 및 이펙트 재생
	PlayReturnToPoolSound();
	PlayReturnToPoolEffect();
	
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
		
		// 충돌 사운드 및 이펙트 재생
		PlayCollisionSound();
		PlayCollisionEffect();
		
		// 딜레이 후 파괴
		DestroyOrbWithDelay();
	}
	else
	{
		CLog::Log("BossProjectileOrb - OnOverlap Occurred but OtherActor is null");
	}
}

// ===== 사운드 재생 함수들 =====

void ABossProjectileOrb::PlaySpawnSound()
{
	if (SpawnSound && AudioComponent)
	{
		AudioComponent->SetSound(SpawnSound);
		AudioComponent->AttenuationSettings = SoundAttenuation; // 어테뉴에이션 적용
		AudioComponent->Play();
		CLog::Log("BossProjectileOrb - Spawn Sound Played with Attenuation");
	}
	else
	{
		CLog::Log("BossProjectileOrb - Spawn Sound or AudioComponent is null");
	}
}

void ABossProjectileOrb::PlayReturnToPoolSound()
{
	if (ReturnToPoolSound)
	{
		// 어테뉴에이션과 함께 위치 기반 사운드 재생
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ReturnToPoolSound, GetActorLocation(), 1.0f, 1.0f, 0.0f, SoundAttenuation);
		CLog::Log("BossProjectileOrb - Return to Pool Sound Played with Attenuation");
	}
	else
	{
		CLog::Log("BossProjectileOrb - Return to Pool Sound is null");
	}
}

void ABossProjectileOrb::PlayCollisionSound()
{
	if (CollisionSound)
	{
		// 어테뉴에이션과 함께 위치 기반 사운드 재생
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), CollisionSound, GetActorLocation(), 1.0f, 1.0f, 0.0f, SoundAttenuation);
		CLog::Log("BossProjectileOrb - Collision Sound Played with Attenuation");
	}
	else
	{
		CLog::Log("BossProjectileOrb - Collision Sound is null");
	}
}

// ===== 이펙트 재생 함수들 =====

void ABossProjectileOrb::PlaySpawnEffect()
{
	if (SpawnEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), SpawnEffect, GetActorLocation(), GetActorRotation());
		CLog::Log("BossProjectileOrb - Spawn Effect Played");
	}
	else
	{
		CLog::Log("BossProjectileOrb - Spawn Effect is null");
	}
}

void ABossProjectileOrb::PlayReturnToPoolEffect()
{
	if (ReturnToPoolEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ReturnToPoolEffect, GetActorLocation(), GetActorRotation());
		CLog::Log("BossProjectileOrb - Return to Pool Effect Played");
	}
	else
	{
		CLog::Log("BossProjectileOrb - Return to Pool Effect is null");
	}
}

void ABossProjectileOrb::PlayCollisionEffect()
{
	if (CollisionEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), CollisionEffect, GetActorLocation(), GetActorRotation());
		CLog::Log("BossProjectileOrb - Collision Effect Played");
	}
	else
	{
		CLog::Log("BossProjectileOrb - Collision Effect is null");
	}
}

// ===== 딜레이 파괴 함수 =====

void ABossProjectileOrb::DestroyOrbWithDelay()
{
	CLog::Log(FString::Printf(TEXT("BossProjectileOrb - DestroyOrbWithDelay Called, Delay: %.2f초"), DestroyDelay));
	
	// 딜레이 후 파괴 실행 (멤버 변수 사용)
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ABossProjectileOrb::DestroyOrb, DestroyDelay, false);
}

// ===== 오브젝트 풀 활성화 함수 =====

void ABossProjectileOrb::ActivateOrb()
{
	CLog::Log("BossProjectileOrb - ActivateOrb Called");
	
	// 오브 활성화 시 소리 및 이펙트 재생 (오브젝트 풀 재사용 시)
	PlaySpawnSound();
	PlaySpawnEffect();
}

