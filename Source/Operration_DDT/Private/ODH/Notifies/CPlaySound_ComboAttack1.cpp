#include "ODH/Notifies/CPlaySound_ComboAttack1.h"
#include "ODH/Component/CSoundCollectionComponent.h"

void UCPlaySound_ComboAttack1::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	if (!MeshComp || !MeshComp->GetOwner())
		return;

	UCSoundCollectionComponent* SoundComponent = MeshComp->GetOwner()->FindComponentByClass<UCSoundCollectionComponent>();
	
	if (SoundComponent)
	{
		SoundComponent->PlayComboAttackSound();
	}
}
