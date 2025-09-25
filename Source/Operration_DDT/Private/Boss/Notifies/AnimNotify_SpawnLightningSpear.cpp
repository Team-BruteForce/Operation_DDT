/**
 * @file AnimNotify_SpawnLightningSpear.cpp
 * @brief 번개 창 스폰 애니메이션 노티파이 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스가 번개 창을 스폰하는 노티파이입니다.
 * 프로젝타일 컴포넌트를 통해 번개 창을 생성합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotify_SpawnLightningSpear.h"
#include "Global.h"
#include "Boss/Component/BossProjectileComponent.h"

FString UAnimNotify_SpawnLightningSpear::GetNotifyName_Implementation() const
{
	return "SpawnLightningSpear";
}

void UAnimNotify_SpawnLightningSpear::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	APawn* Owner=Cast<APawn>(MeshComp->GetOwner());
	CheckNull(Owner);
	UBossProjectileComponent* ProjectileComp=CHelpers::GetComponent<UBossProjectileComponent>(Owner);
	CheckNull(ProjectileComp);
	ProjectileComp->ShotProjectile();
}
