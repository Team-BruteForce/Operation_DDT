#include "ODH/ODH_Enemy/Notifies/CFlowerEnemyHitStateEnd.h"
#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemy.h"

void UCFlowerEnemyHitStateEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp) return;
	ACFlowerEnemy* Enemy = Cast<ACFlowerEnemy>(MeshComp->GetOwner());
	if (!Enemy) return;

	Enemy->EndHitState();
}


