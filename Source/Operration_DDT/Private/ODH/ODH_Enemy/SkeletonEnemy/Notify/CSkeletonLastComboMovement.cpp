#include "ODH/ODH_Enemy/SkeletonEnemy/Notify/CSkeletonLastComboMovement.h"
#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"
#include "Engine/Engine.h"

UCSkeletonLastComboMovement::UCSkeletonLastComboMovement()
{
	// 기본 생성자
}

void UCSkeletonLastComboMovement::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 메시 컴포넌트의 소유자가 스켈레톤 에너미인지 확인
	if (ACSkeletonEnemy* SkeletonEnemy = Cast<ACSkeletonEnemy>(MeshComp->GetOwner()))
	{
		// 마지막 콤보 공격 전진 움직임 실행 (매개변수로 전진 거리 전달)
		SkeletonEnemy->LastComboMovement(ForwardDistance);
	}
	else
	{
		
	}
}
