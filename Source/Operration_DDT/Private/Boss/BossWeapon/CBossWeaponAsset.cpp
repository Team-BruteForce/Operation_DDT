

#include "Boss/BossWeapon/CBossWeaponAsset.h"

#include "Boss/BossWeapon/CBossDoAction.h"
#include "Boss/BossWeapon/CBossWeapon.h"
#include "Boss/BossWeapon/CBossEquipment.h"
#include "GameFramework/Character.h"

UCBossWeaponAsset::UCBossWeaponAsset ( )
{
	BossWeaponClass = ACBossWeapon::StaticClass();
	BossEquipmentClass = UCBossEquipment::StaticClass();
	BossDoActionClass = UCBossDoAction::StaticClass();
}

void UCBossWeaponAsset::BeginPlay ( class ACharacter* InOwner )
{
	if ( !!BossWeaponClass ) {
		FActorSpawnParameters parmas;
		parmas.Owner = InOwner;

		BossWeapon = InOwner->GetWorld ( )->SpawnActor<ACBossWeapon> ( BossWeaponClass , parmas );
	}
	if ( !!BossEquipmentClass ) {
		BossEquipment = NewObject<UCBossEquipment> ( this , BossEquipmentClass );
		BossEquipment->BeginPlay ( InOwner , BossEquipmentData );

		if ( !!BossWeapon ) {
			BossEquipment->OnBossEquipmentBeginEquip.AddDynamic ( BossWeapon , &ACBossWeapon::OnBossBeginEquip );
			BossEquipment->OnBossEquipmentUnequip.AddDynamic ( BossWeapon , &ACBossWeapon::OnBossUnequip );
		}
	}
	if ( !!BossDoActionClass ) {
		BossDoAction = NewObject<UCBossDoAction> ( this , BossDoActionClass );
		BossDoAction->BeginPlay ( BossWeapon , BossEquipment , InOwner , BossDoActionDatas ,HitDatas );

		if ( !!BossWeapon ) {
			BossWeapon->OnBossAttachmentBeginCollision.AddDynamic ( BossDoAction , &UCBossDoAction::OnBossWeaponBeginCollision );
			BossWeapon->OnBossAttachmentEndCollision.AddDynamic ( BossDoAction , &UCBossDoAction::OnBossWeaponEndCollision );
			BossWeapon->OnBossAttachmentBeginOverlap.AddDynamic ( BossDoAction , &UCBossDoAction::OnBossWeaponBeginOverlap );
			BossWeapon->OnBossAttachmentEndOverlap.AddDynamic ( BossDoAction , &UCBossDoAction::OnBossWeaponEndOverlap );
		}
	}
}
