/**
 * @file AnimNotify_DeadDissolve.cpp
 * @brief 사망 디졸브 애니메이션 노티파이 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 사망 디졸브 효과를 처리하는 노티파이입니다.
 * 보스 사망 시 디졸브 애니메이션을 실행합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotify_DeadDissolve.h"
#include "Boss/CBoss.h"
#include "Global.h"

FString UAnimNotify_DeadDissolve::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_DeadDissolve::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	ACBoss* Boss=Cast<ACBoss>(MeshComp->GetOwner());
	CheckNull(Boss);
	Boss->BossDissolve->TriggerDissolveEffectDead();
}
