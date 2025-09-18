#include "ODH/ODH_Enemy/Notifies/CFlyingSkullHitStateEnd.h"
#include "ODH/ODH_Enemy/FlyingSkull/CFlyingSkull.h"

void UCFlyingSkullHitStateEnd::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp) return;
	ACFlyingSkull* Enemy = Cast<ACFlyingSkull>(MeshComp->GetOwner());
	if (!Enemy) return;

	Enemy->EndHitState();
}


