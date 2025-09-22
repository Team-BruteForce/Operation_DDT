#include "ODH/Component/CSoundCollectionComponent.h"
#include "Kismet/GameplayStatics.h"

UCSoundCollectionComponent::UCSoundCollectionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCSoundCollectionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCSoundCollectionComponent::PlayIdleSound()
{
	PlaySoundInternal(IdleSound);
}

void UCSoundCollectionComponent::PlayRunSound()
{
	PlaySoundInternal(RunSound);
}

void UCSoundCollectionComponent::PlayGetHitSound()
{
	PlaySoundInternal(GetHitSound);
}

void UCSoundCollectionComponent::PlayDieSound()
{
	PlaySoundInternal(DieSound);
}

void UCSoundCollectionComponent::PlayMeleeAttackSound()
{
	PlaySoundInternal(MeleeAttackSound);
}

void UCSoundCollectionComponent::PlayComboAttackSound()
{
	PlaySoundInternal(ComboAttackSound);
}

void UCSoundCollectionComponent::PlayDashAttackSound()
{
	PlaySoundInternal(DashAttackSound);
}

void UCSoundCollectionComponent::PlayRangedAttackSound()
{
	PlaySoundInternal(RangedAttackSound);
}

void UCSoundCollectionComponent::PlaySoundInternal(USoundBase* SoundToPlay)
{
	if (SoundToPlay && GetOwner())
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			SoundToPlay,
			GetOwner()->GetActorLocation()
		);
	}
}
