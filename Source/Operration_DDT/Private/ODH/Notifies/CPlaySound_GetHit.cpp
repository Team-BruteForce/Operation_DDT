#include "ODH/Notifies/CPlaySound_GetHit.h"
#include "ODH/Component/CSoundCollectionComponent.h"

void UCPlaySound_GetHit::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp || !MeshComp->GetOwner())
		return;

	UCSoundCollectionComponent* SoundComponent = MeshComp->GetOwner()->FindComponentByClass<UCSoundCollectionComponent>();
	
	if (SoundComponent)
	{
		SoundComponent->PlayGetHitSound();
	}
}
