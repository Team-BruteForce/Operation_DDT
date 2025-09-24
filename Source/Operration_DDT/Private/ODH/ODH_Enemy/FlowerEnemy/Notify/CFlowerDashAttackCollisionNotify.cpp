#include "ODH/ODH_Enemy/FlowerEnemy/Notify/CFlowerDashAttackCollisionNotify.h"
#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemy.h"
#include "Engine/Engine.h"

UCFlowerDashAttackCollisionNotify::UCFlowerDashAttackCollisionNotify()
{
	// 기본 생성자
}

void UCFlowerDashAttackCollisionNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 메시 컴포넌트의 소유자가 FlowerEnemy인지 확인
	if (ACFlowerEnemy* FlowerEnemy = Cast<ACFlowerEnemy>(MeshComp->GetOwner()))
	{
		FlowerEnemy->EnableDashAttackCollision();
	}

	else
	{
		
	}
}
