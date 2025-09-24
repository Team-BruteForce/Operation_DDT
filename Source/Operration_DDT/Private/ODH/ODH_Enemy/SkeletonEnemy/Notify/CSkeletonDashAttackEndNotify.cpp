#include "ODH/ODH_Enemy/SkeletonEnemy/Notify/CSkeletonDashAttackEndNotify.h"
#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"
#include "Engine/Engine.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UCSkeletonDashAttackEndNotify::UCSkeletonDashAttackEndNotify()
{
	// 기본 생성자
}

void UCSkeletonDashAttackEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 메시 컴포넌트의 소유자가 스켈레톤 에너미인지 확인
	if (ACSkeletonEnemy* SkeletonEnemy = Cast<ACSkeletonEnemy>(MeshComp->GetOwner()))
	{
		// 대시 공격 완료 알림
		SkeletonEnemy->NotifyDashAttackCompleted();
		

	}
	else
	{
	}
}
