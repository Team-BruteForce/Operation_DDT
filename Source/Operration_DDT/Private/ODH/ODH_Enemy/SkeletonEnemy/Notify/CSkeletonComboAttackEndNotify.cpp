#include "ODH/ODH_Enemy/SkeletonEnemy/Notify/CSkeletonComboAttackEndNotify.h"
#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"
#include "Engine/Engine.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UCSkeletonComboAttackEndNotify::UCSkeletonComboAttackEndNotify()
{
	// 기본 생성자
}

void UCSkeletonComboAttackEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 메시 컴포넌트의 소유자가 스켈레톤 에너미인지 확인
	if (ACSkeletonEnemy* SkeletonEnemy = Cast<ACSkeletonEnemy>(MeshComp->GetOwner()))
	{
		// 콤보 공격 완료 알림
		SkeletonEnemy->NotifyComboAttackCompleted();
		

	}
	else
	{
	}
}
