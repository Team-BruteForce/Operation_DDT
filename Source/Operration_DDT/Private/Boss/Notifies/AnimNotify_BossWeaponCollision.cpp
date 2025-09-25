/**
 * @file AnimNotify_BossWeaponCollision.cpp
 * @brief 보스 웨폰 콜리전 애니메이션 노티파이 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스 웨폰의 콜리전을 활성화/비활성화하는 노티파이입니다.
 * 보스의 공격 범위를 제어합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotify_BossWeaponCollision.h"
#include "GLobal.h"
#include "Boss/BossWeapon/CBossWeapon.h"
#include "Boss/Component/CBossWeaponComponent.h"

FString UAnimNotify_BossWeaponCollision::GetNotifyName_Implementation() const
{
	return "BossCollision";
}

void UAnimNotify_BossWeaponCollision::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCBossWeaponComponent* weapon = CHelpers::GetComponent<UCBossWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetBossWeapon());

	weapon->GetBossWeapon()->OnBossCollisions();
}

void UAnimNotify_BossWeaponCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	UCBossWeaponComponent* weapon = CHelpers::GetComponent<UCBossWeaponComponent>(MeshComp->GetOwner());
	CheckNull(weapon);
	CheckNull(weapon->GetBossWeapon());

	weapon->GetBossWeapon()->OffBossCollisions();

}