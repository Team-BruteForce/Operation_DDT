#include "ODH/Notifies/CAnimNotify_SinkAfterDelay.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "ODH/Component/ShakeAndSinkComponent.h"

FString UCAnimNotify_SinkAfterDelay::GetNotifyName_Implementation() const
{
	return TEXT("SinkAfterDelay");
}

void UCAnimNotify_SinkAfterDelay::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)
		return;

	AActor* OwnerActor = MeshComp->GetOwner();
	if (!OwnerActor)
		return;

	UShakeAndSinkComponent* EffectComp = OwnerActor->FindComponentByClass<UShakeAndSinkComponent>();
	if (!EffectComp)
	{
		EffectComp = NewObject<UShakeAndSinkComponent>(OwnerActor);
		if (EffectComp)
		{
			EffectComp->RegisterComponent();
		}
	}

	if (EffectComp)
	{
		EffectComp->StartEffect(DelaySeconds, SinkDistance, SinkDuration, ShakeAmplitude, ShakeFrequency);
	}
}



