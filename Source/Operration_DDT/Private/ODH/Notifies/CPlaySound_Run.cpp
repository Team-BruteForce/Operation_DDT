#include "ODH/Notifies/CPlaySound_Run.h"
#include "ODH/Component/CSoundCollectionComponent.h"

void UCPlaySound_Run::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp || !MeshComp->GetOwner())
		return;

	UCSoundCollectionComponent* SoundComponent = MeshComp->GetOwner()->FindComponentByClass<UCSoundCollectionComponent>();
	
	if (SoundComponent)
	{
		SoundComponent->PlayRunSound();
	}
}

