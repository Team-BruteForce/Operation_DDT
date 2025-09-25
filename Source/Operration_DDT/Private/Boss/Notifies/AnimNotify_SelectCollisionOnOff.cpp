/**
 * @file AnimNotify_SelectCollisionOnOff.cpp
 * @brief 선택 콜리전 온/오프 애니메이션 노티파이 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 선택된 콜리전을 활성화/비활성화하는 노티파이입니다.
 * 웨폰 컴포넌트를 통해 특정 콜리전을 제어합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotify_SelectCollisionOnOff.h"
#include "GLobal.h"
#include "Boss/BossWeapon/CBossWeapon.h"
#include "Boss/Component/CBossWeaponComponent.h"
FString UAnimNotify_SelectCollisionOnOff::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_SelectCollisionOnOff::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCBossWeaponComponent* weapon = CHelpers::GetComponent<UCBossWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetBossWeapon());
	weapon->GetBossWeapon()->OnSelectCollision(CollisionName);
}

void UAnimNotify_SelectCollisionOnOff::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCBossWeaponComponent* weapon = CHelpers::GetComponent<UCBossWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetBossWeapon());

	weapon->GetBossWeapon()->OffBossCollisions();
}
