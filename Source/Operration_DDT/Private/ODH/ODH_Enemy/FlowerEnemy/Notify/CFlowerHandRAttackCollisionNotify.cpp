#include "ODH/ODH_Enemy/FlowerEnemy/Notify/CFlowerHandRAttackCollisionNotify.h"
#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemy.h"
#include "Engine/Engine.h"

UCFlowerHandRAttackCollisionNotify::UCFlowerHandRAttackCollisionNotify()
{
	// 기본 생성자
}

void UCFlowerHandRAttackCollisionNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 메시 컴포넌트의 소유자가 FlowerEnemy인지 확인
	if (ACFlowerEnemy* FlowerEnemy = Cast<ACFlowerEnemy>(MeshComp->GetOwner()))
	{
		FlowerEnemy->EnableHandRAttackCollision();
	}
	else
	{
	}
}
