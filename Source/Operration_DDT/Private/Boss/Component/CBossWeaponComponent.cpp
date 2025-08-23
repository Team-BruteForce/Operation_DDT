// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/Component/CBossWeaponComponent.h"
#include "Boss/BossWeapon/CBossDoAction.h"
#include "Global.h"
#include "Boss/BossWeapon/CBossEquipment.h"
#include "Boss/BossWeapon/CBossWeaponAsset.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UCBossWeaponComponent::UCBossWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UCBossWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
	// 데이터 에셋들 초기화
	for (auto& DataAssetPair : DataAssets)
	{
		if (DataAssetPair.Value)
		{
			DataAssetPair.Value->BeginPlay(OwnerCharacter);
		}
	}
	
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	
	// 무기 모드 초기화 - BeginPlay에서 확실하게 설정
	UnarmedTag = BossTags.Unarmed;
	CurrentWeaponMode = UnarmedTag;
}

// Called every frame
void UCBossWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

// 태그 기반 무기 모드 관리 함수들 (CWeaponComponent와 동일한 패턴)
void UCBossWeaponComponent::SetUnarmedMode()
{
	// CWeaponComponent와 동일: Idle 모드 체크 제거
	GetBossEquipment()->Unequip(CurrentWeaponMode);
	ChangeType(BossTags.Unarmed);
}

void UCBossWeaponComponent::SetPaladinMode()
{
	SetMode(BossTags.Paladin);
}

void UCBossWeaponComponent::SetMode(const FGameplayTag& InTag)
{
	// if (BossTags.Unarmed == InTag)
	// {
	// 	SetUnarmedMode();
	// 	return;
	// }
	// else if (IsUnarmedMode() == false)
		// GetBossEquipment()->Unequip(CurrentWeaponMode);
	
	if (DataAssets.Contains(InTag))
	{
		CLog::Log("BossTags.Paladin");
		DataAssets[InTag]->GetBossEquipment()->Equip(InTag);
		ChangeType(InTag);
	}
}

void UCBossWeaponComponent::ChangeType(const FGameplayTag& InTag)
{
	FGameplayTag prevType = CurrentWeaponMode;
	CurrentWeaponMode = InTag;
	
	if (OnWeaponModeChanged.IsBound())
		OnWeaponModeChanged.Broadcast(prevType, InTag);
}

class UCBossEquipment* UCBossWeaponComponent::GetBossEquipment()
{
	CheckTrueResult ( IsUnarmedMode ( ) , nullptr );
	CheckFalseResult ( !!DataAssets[CurrentWeaponMode] , nullptr );
	return DataAssets[CurrentWeaponMode]->GetBossEquipment();
}

class ACBossWeapon* UCBossWeaponComponent::GetBossWeapon()
{
	CheckTrueResult ( IsUnarmedMode ( ) , nullptr );
	CheckFalseResult ( !!DataAssets[CurrentWeaponMode] , nullptr );
	return DataAssets[CurrentWeaponMode]->GetBossWeapon();
}

class UCBossDoAction* UCBossWeaponComponent::GetBossDoAction()
{
	CheckTrueResult ( IsUnarmedMode ( ) , nullptr );
	CheckFalseResult ( !!DataAssets[CurrentWeaponMode] , nullptr );
	return DataAssets[CurrentWeaponMode]->BossDoAction;
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

void UCBossWeaponComponent::BossDoAction(FGameplayTag Tag)
{
	if (UCBossDoAction* DoAction = GetBossDoAction())
	{
		DoAction->DoAction(Tag);
	}
}
