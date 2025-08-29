/**
 * @file CBossWeaponComponent.cpp
 * @brief 보스 무기 시스템 컴포넌트 구현 파일
 * 
 * @details
 * 보스 캐릭터의 무기 시스템을 관리하는 컴포넌트의 구현 파일입니다.
 * 게임플레이 태그 기반 무기 모드 관리와 무기 관련 액션을 처리합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#include "Boss/Component/CBossWeaponComponent.h"
#include "Boss/BossWeapon/CBossDoAction.h"
#include "Global.h"
#include "Boss/BossWeapon/CBossEquipment.h"
#include "Boss/BossWeapon/CBossWeaponAsset.h"
#include "GameFramework/Character.h"

/**
 * @brief 보스 무기 컴포넌트 생성자
 * 
 * 컴포넌트의 기본 설정을 초기화합니다.
 */
UCBossWeaponComponent::UCBossWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

/**
 * @brief 게임 시작 시 호출되는 함수
 * 
 * 컴포넌트 초기화 및 데이터 에셋 설정을 수행합니다.
 */
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
	UnarmedTag = BossTags.PaseZero;
	CurrentWeaponMode = UnarmedTag;
}

/**
 * @brief 매 프레임 호출되는 틱 함수
 */
void UCBossWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

/**
 * @brief 맨손 모드로 설정
 * 
 * 현재 장비를 해제하고 맨손 모드로 변경합니다.
 */
void UCBossWeaponComponent::SetUnarmedMode()
{
	// CWeaponComponent와 동일: Idle 모드 체크 제거
	GetBossEquipment()->Unequip(CurrentWeaponMode);
	ChangeType(BossTags.PaseZero);
}

/**
 * @brief 페이즈 원 모드로 설정
 * 
 * 페이즈 원 모드로 변경합니다.
 */
void UCBossWeaponComponent::SetPaseOneMode()
{
	SetMode(BossTags.PaseOne);
}

/**
 * @brief 무기 모드 설정
 * 
 * @param InTag 설정할 모드 태그
 * 
 * 지정된 태그에 해당하는 무기 모드로 설정합니다.
 */
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
		DataAssets[InTag]->GetBossEquipment()->Equip(InTag);
		ChangeType(InTag);
	}
}

/**
 * @brief 무기 타입 변경
 * 
 * @param InTag 변경할 타입 태그
 * 
 * 무기 타입을 변경하고 이벤트를 발생시킵니다.
 */
void UCBossWeaponComponent::ChangeType(const FGameplayTag& InTag)
{
	FGameplayTag prevType = CurrentWeaponMode;
	CurrentWeaponMode = InTag;
	
	if (OnWeaponModeChanged.IsBound())
		OnWeaponModeChanged.Broadcast(prevType, InTag);
}

/**
 * @brief 보스 장비 객체 반환
 * 
 * @return 보스 장비 객체 포인터
 * 
 * 현재 무기 모드에 해당하는 장비 객체를 반환합니다.
 */
class UCBossEquipment* UCBossWeaponComponent::GetBossEquipment()
{
	CheckTrueResult ( IsUnarmedMode ( ) , nullptr );
	CheckFalseResult ( !!DataAssets[CurrentWeaponMode] , nullptr );
	return DataAssets[CurrentWeaponMode]->GetBossEquipment();
}

/**
 * @brief 보스 무기 객체 반환
 * 
 * @return 보스 무기 객체 포인터
 * 
 * 현재 무기 모드에 해당하는 무기 객체를 반환합니다.
 */
class ACBossWeapon* UCBossWeaponComponent::GetBossWeapon()
{
	CheckTrueResult ( IsUnarmedMode ( ) , nullptr );
	CheckFalseResult ( !!DataAssets[CurrentWeaponMode] , nullptr );
	return DataAssets[CurrentWeaponMode]->GetBossWeapon();
}

/**
 * @brief 보스 액션 객체 반환
 * 
 * @return 보스 액션 객체 포인터
 * 
 * 현재 무기 모드에 해당하는 액션 객체를 반환합니다.
 */
class UCBossDoAction* UCBossWeaponComponent::GetBossDoAction()
{
	CheckTrueResult ( IsUnarmedMode ( ) , nullptr );
	CheckFalseResult ( !!DataAssets[CurrentWeaponMode] , nullptr );
	return DataAssets[CurrentWeaponMode]->BossDoAction;
}

/**
 * @brief 태그로 데이터 에셋 반환
 * 
 * @param InTag 찾을 태그
 * @return 데이터 에셋 포인터
 * 
 * 지정된 태그에 해당하는 데이터 에셋을 반환합니다.
 */
class UCBossWeaponAsset* UCBossWeaponComponent::GetDataAssetByTag(const FGameplayTag& InTag)
{
	if (UCBossWeaponAsset** FoundAsset = DataAssets.Find(InTag))
	{
		return *FoundAsset;
	}
	return nullptr;
}

/**
 * @brief 보스 액션 실행
 * 
 * @param Tag 실행할 액션 태그
 * 
 * 지정된 태그에 해당하는 보스 액션을 실행합니다.
 */
void UCBossWeaponComponent::BossDoAction(FGameplayTag Tag)
{
	if (UCBossDoAction* DoAction = GetBossDoAction())
	{
		DoAction->DoAction(Tag);
	}
}

void UCBossWeaponComponent::BossHitAction(FGameplayTag Tag)
{
	if (UCBossDoAction* DoAction = GetBossDoAction())
	{
		DoAction->HitAction(Tag);
	}
}
