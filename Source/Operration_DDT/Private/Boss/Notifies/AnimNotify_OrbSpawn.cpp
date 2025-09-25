/**
 * @file AnimNotify_OrbSpawn.cpp
 * @brief 오브 스폰 애니메이션 노티파이 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스가 오브를 스폰하는 노티파이입니다.
 * 프로젝타일 컴포넌트를 통해 오브를 생성합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotify_OrbSpawn.h"
#include "Global.h"
#include "Boss/Component/BossProjectileComponent.h"

FString UAnimNotify_OrbSpawn::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_OrbSpawn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	APawn* Owner=Cast<APawn>(MeshComp->GetOwner());
	CheckNull(Owner);
	UBossProjectileComponent* ProjectileComp=CHelpers::GetComponent<UBossProjectileComponent>(Owner);
	CheckNull(ProjectileComp);
	ProjectileComp->SpawnOrb();
}
