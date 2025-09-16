#include "ODH/ODH_Enemy/FlowerEnemy/Notify/CFlowerHandLAttackCollisionNotify.h"
#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemy.h"
#include "Engine/Engine.h"

UCFlowerHandLAttackCollisionNotify::UCFlowerHandLAttackCollisionNotify()
{
	// 기본 생성자
}

void UCFlowerHandLAttackCollisionNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 메시 컴포넌트의 소유자가 FlowerEnemy인지 확인
	if (ACFlowerEnemy* FlowerEnemy = Cast<ACFlowerEnemy>(MeshComp->GetOwner()))
	{
		if (bEnableCollision)
		{
			FlowerEnemy->EnableHandLAttackCollision();
		}
		else
		{
			FlowerEnemy->DisableHandLAttackCollision();
		}
	}
	else
	{
		// 디버그 메시지 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("CFlowerHandLAttackCollisionNotify: FlowerEnemy not found!"));
		}
	}
}
