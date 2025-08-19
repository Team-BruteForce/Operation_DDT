// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossWeapon/CBossEquipment.h"
#include "Global.h"
#include "GameFramework/Character.h"

void UCBossEquipment::BeginPlay ( class ACharacter* InOwner , TArray<FBossEquipmentData>& InData )
{
	OwnerCharacter = InOwner;
	Data = InData;

	Movement = CHelpers::GetComponent<UCBossMovementComponent> ( InOwner );
	State = CHelpers::GetComponent<UBossStateComponent> ( InOwner );
}

void UCBossEquipment::Equip_Implementation ( )
{
	State = CHelpers::GetComponent<UBossStateComponent> ( OwnerCharacter );
	

	if ( Data[0].bCanMove == false )
		// Movement->Stop ( );


			if ( !!Data[0].Montage ){
				OwnerCharacter->PlayAnimMontage ( Data[0].Montage , Data[0].PlayRate );

				// State->SetEquipMode ( );
				// }
				// else {
				// 	Begin_Equip ( );
				// 	End_Equip ( );
				// }
				// if ( Data[0].bUseControlRotation )
				// Movement->EnableControlRotation ( );
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

void UCBossEquipment::Unequip_Implementation ( )
{
	bEquipped = false;
	// Movement->DisableControlRotation ( );

	if ( !!Data[1].Montage )
		OwnerCharacter->PlayAnimMontage ( Data[1].Montage , Data[1].PlayRate );

	if ( OnBossEquipmentUnequip.IsBound() )
	{
		OnBossEquipmentUnequip.Broadcast ( );
	}
}