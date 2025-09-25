/**
 * @file AnimNotify_GateOfBabylonSpawn.cpp
 * @brief 바빌론의 문 스폰 애니메이션 노티파이 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 바빌론의 문을 스폰하는 노티파이입니다.
 * 프로젝타일 컴포넌트를 통해 바빌론의 문을 생성합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotify_GateOfBabylonSpawn.h"
#include "Global.h"
#include "Boss/Component/BossProjectileComponent.h"

FString UAnimNotify_GateOfBabylonSpawn::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_GateOfBabylonSpawn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	APawn* Owner=Cast<APawn>(MeshComp->GetOwner());
	CheckNull(Owner);
	UBossProjectileComponent* ProjectileComp=CHelpers::GetComponent<UBossProjectileComponent>(Owner);
	CheckNull(ProjectileComp);
	ProjectileComp->SpawnMagicCirclesAtCirclePositions();
	CLog::Log("SpawnMagicCirclesAtCirclePositions1");
	
	Super::Notify(MeshComp, Animation);
}
