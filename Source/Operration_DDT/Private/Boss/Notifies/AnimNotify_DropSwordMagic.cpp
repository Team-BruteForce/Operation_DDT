/**
 * @file AnimNotify_DropSwordMagic.cpp
 * @brief 검 마법 드롭 애니메이션 노티파이 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스가 검 마법을 드롭하는 노티파이입니다.
 * 프로젝타일 컴포넌트를 통해 검 마법을 생성합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotify_DropSwordMagic.h"
#include "Global.h"
#include "Boss/Component/BossProjectileComponent.h"
#include "Boss/Component/CBossTargetingComponent.h"
#include "GameFramework/Character.h"

FString UAnimNotify_DropSwordMagic::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_DropSwordMagic::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	UBossProjectileComponent* ProjectileComp = CHelpers::GetComponent<UBossProjectileComponent>(MeshComp->GetOwner());
	CheckNull(ProjectileComp);
	ProjectileComp->SpawnHolySwordMagicRepeatedly(1.5f, 3);
}
