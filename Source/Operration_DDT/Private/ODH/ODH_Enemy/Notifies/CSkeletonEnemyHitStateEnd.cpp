#include "ODH/ODH_Enemy/Notifies/CSkeletonEnemyHitStateEnd.h"
#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"

void UCSkeletonEnemyHitStateEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp) return;
	ACSkeletonEnemy* Enemy = Cast<ACSkeletonEnemy>(MeshComp->GetOwner());
	if (!Enemy) return;

	Enemy->EndHitState();
}


