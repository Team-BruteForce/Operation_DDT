/**
 * @file AnimNotify_ArmorDissolve.cpp
 * @brief 아머 디졸브 애니메이션 노티파이 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 아머 디졸브 효과를 처리하는 노티파이입니다.
 * 보스의 아머가 사라지는 효과를 실행합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotify_ArmorDissolve.h"

#include "Boss/CBoss.h"
#include "Global.h"

FString UAnimNotify_ArmorDissolve::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_ArmorDissolve::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	ACBoss* Boss=Cast<ACBoss>(MeshComp->GetOwner());
	CheckNull(Boss);
	Boss->BossDissolve->TriggerDissolveEffect();
}
