// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Component/CBossWeaponComponent.h"

#include "Boss/BossWeapon/CBossDoAction.h"
#include "Boss/BossWeapon/CBossWeaponAsset.h"

// Sets default values for this component's properties
UCBossWeaponComponent::UCBossWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCBossWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCBossWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

class UCBossEquipment* UCBossWeaponComponent::GetBossEquipment()
{
	// 기본 태그나 첫 번째 데이터 에셋을 사용
	if (DataAssets.Num() > 0)
	{
		auto FirstElement = DataAssets.CreateConstIterator();
		return FirstElement.Value()->GetBossEquipment();
	}
	return nullptr;
}

class ACBossWeapon* UCBossWeaponComponent::GetBossWeapon()
{
	// 기본 태그나 첫 번째 데이터 에셋을 사용
	if (DataAssets.Num() > 0)
	{
		auto FirstElement = DataAssets.CreateConstIterator();
		return FirstElement.Value()->GetBossWeapon();
	}
	return nullptr;
}

class UCBossDoAction* UCBossWeaponComponent::GetBossDoAction()
{
	// 기본 태그나 첫 번째 데이터 에셋을 사용
	if (DataAssets.Num() > 0)
	{
		auto FirstElement = DataAssets.CreateConstIterator();
		return FirstElement.Value()->GetBossDoAction();
	}
	return nullptr;
}

// 태그로 데이터 에셋을 찾는 함수 구현
class UCBossWeaponAsset* UCBossWeaponComponent::GetDataAssetByTag(const FGameplayTag& InTag)
{
	if (UCBossWeaponAsset** FoundAsset = DataAssets.Find(InTag))
	{
		return *FoundAsset;
	}
	return nullptr;
}

void UCBossWeaponComponent::BossDoAction()
{
	if ( !!GetBossDoAction ( ) ){
		GetBossDoAction ()->DoAction ( );
	}
}

