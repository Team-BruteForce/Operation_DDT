// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CBulletObjectPoolComponent.h"
#include "Global.h"
#include "Player/CPlayerBullet.h"
#include "Player/DDTPlayer.h"

// Sets default values for this component's properties
UCBulletObjectPoolComponent::UCBulletObjectPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//테스트용 주석

	// ...
}


// Called when the game starts
void UCBulletObjectPoolComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCharacter = Cast<ADDTPlayer>(GetOwner());

	// 오브젝트 풀 초기화
	for (int32 i = 0; i < MaxMagazinePool; i++)
	{
		ACPlayerBullet* bullet = CreateBulletForPool();
		MagazinePool.Add(bullet);
		
		ACPlayerBullet* vfx = CreateBulletVFXForPool();
		VFXPool.Add(vfx);
	}
	CurrentPoolIndex = 0;
	
}


// Called every frame
void UCBulletObjectPoolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

ACPlayerBullet* UCBulletObjectPoolComponent::CreateBulletForPool()
{
	// 총알 생성 (기존과 동일한 방식)
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	params.bNoFail = true;
	params.Owner = OwnerCharacter;
	
	ACPlayerBullet* bullet = GetWorld()->SpawnActor<ACPlayerBullet>(PlayerBulletClass, params);
	
	// 생성 즉시 상태 완전 초기화
	bullet->ResetBulletState();
	bullet->SetActive(false);
		
	return bullet;
}

ACPlayerBullet* UCBulletObjectPoolComponent::CreateBulletVFXForPool()
{
	// 총알 생성 (기존과 동일한 방식)
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	params.bNoFail = true;
	params.Owner = OwnerCharacter;
	
	ACPlayerBullet* bullet = GetWorld()->SpawnActor<ACPlayerBullet>(PlayerBulletVFXClass, params);
	
	// 생성 즉시 상태 완전 초기화
	bullet->ResetBulletState();
	bullet->SetActive(false);
	
	return bullet;
}

ACPlayerBullet* UCBulletObjectPoolComponent::GetInactiveBullet()
{
	// 순환 방식으로 비활성화된 총알 찾기
	for (int32 i = 0; i < MagazinePool.Num(); i++)
	{
		int32 index = (CurrentPoolIndex + i) % MagazinePool.Num();
		ACPlayerBullet* bullet = MagazinePool[index];
		
		// 더 엄격한 체크 - 사용 중이 아닌 총알만 반환
		if (!bullet->IsActive() && !bullet->GetIsInUse())
		{
			CurrentPoolIndex = (index + 1) % MagazinePool.Num();
			CLog::Log("Pool) Return Bullet : " + FString::FromInt(CurrentPoolIndex));
			return bullet;
		}
	}
	
	ACPlayerBullet* newbullet =  CreateBulletForPool();
	//MaxMagazinePool++;
	MagazinePool.Add(newbullet);
	
	return newbullet;// 모든 총알이 활성화된 상태
}

ACPlayerBullet* UCBulletObjectPoolComponent::GetInactiveVFX()
{
	// 순환 방식으로 비활성화된 총알 찾기
	for (int32 i = 0; i < VFXPool.Num(); i++)
	{
		int32 index = (CurrentPoolIndex + i) % VFXPool.Num();
		ACPlayerBullet* bullet = VFXPool[i];
		
		// 더 엄격한 체크 - 사용 중이 아닌 총알만 반환
		if (!bullet->IsActive() && !bullet->GetIsInUse())
		{
			CurrentPoolIndex = (index + 1) % VFXPool.Num();
			CLog::Log("Pool) Return VFX Bullet : " + FString::FromInt(CurrentPoolIndex));
			return bullet;
		}
	}
	
	ACPlayerBullet* newbullet =  CreateBulletVFXForPool();
	//MaxMagazinePool++;
	VFXPool.Add(newbullet);
	
	return newbullet;// 모든 총알이 활성화된 상태
}

