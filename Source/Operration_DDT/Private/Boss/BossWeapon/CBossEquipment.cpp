// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossWeapon/CBossEquipment.h"

#include "GameplayTagContainer.h"
#include "Global.h"
#include "GameFramework/Character.h"

void UCBossEquipment::BeginPlay ( class ACharacter* InOwner , TMap<FGameplayTag,FBossEquipmentData>& InData )
{
	OwnerCharacter = InOwner;
	Data = InData;

	Movement = CHelpers::GetComponent<UCBossMovementComponent> ( InOwner );
	State = CHelpers::GetComponent<UBossStateComponent> ( InOwner );
}

void UCBossEquipment::Equip_Implementation(FGameplayTag PlayTag)
{
	// OwnerCharacter가 유효한지 확인
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("CBossEquipment::Equip - OwnerCharacter is null!"));
		return;
	}
	
	// State 컴포넌트 안전하게 가져오기
	// State = CHelpers::GetComponent<UBossStateComponent>(OwnerCharacter);
	
	// Data가 유효한지 확인
	if (!Data.IsEmpty() && Data.Contains(PlayTag))
	{
		// 애니메이션 몽타주 재생
		if (Data[PlayTag].Montage)
		{
			OwnerCharacter->PlayAnimMontage(Data[PlayTag].Montage);
		}
		
		// State 컴포넌트가 있으면 SetEquipMode 호출
		if (State)
		{
			// State->SetEquipMode();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("CBossEquipment::Equip - Invalid PlayTag or empty Data!"));
	}
}


void UCBossEquipment::End_Equip_Implementation ( )
{
	bBeginEquip = true;
	// State->SetIdleMode();
	
}

void UCBossEquipment::Begin_Equip_Implementation ( )
{
	bBeginEquip = false;
	bEquipped = true;
	// State->SetEquipMode();
	
	if ( OnBossEquipmentBeginEquip.IsBound ( ) )
		OnBossEquipmentBeginEquip.Broadcast ( );
	// Movement->Move ( );

}

void UCBossEquipment::Unequip_Implementation(FGameplayTag PlayTag)
{
	bEquipped = false;
	
	// OwnerCharacter가 유효한지 확인
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("CBossEquipment::Unequip - OwnerCharacter is null!"));
		return;
	}
	
	// Data가 유효하고 애니메이션이 있으면 재생
	if (!Data.IsEmpty() && Data.Contains(PlayTag) && Data[PlayTag].Montage)
	{
		OwnerCharacter->PlayAnimMontage(Data[PlayTag].Montage, Data[PlayTag].PlayRate);
	}
	
	// 이벤트 브로드캐스트
	if (OnBossEquipmentUnequip.IsBound())
	{
		OnBossEquipmentUnequip.Broadcast();
	}
}