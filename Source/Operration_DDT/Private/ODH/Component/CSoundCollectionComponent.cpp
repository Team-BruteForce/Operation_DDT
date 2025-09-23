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
	if (IdleSounds.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, IdleSounds.Num() - 1);
		PlaySoundInternal(IdleSounds[RandomIndex]);
	}
}

void UCSoundCollectionComponent::PlayRunSound()
{
	if (RunSounds.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, RunSounds.Num() - 1);
		PlaySoundInternal(RunSounds[RandomIndex]);
	}
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

void UCSoundCollectionComponent::PlayComboAttack2Sound()
{
	PlaySoundInternal(ComboAttack2Sound);
}

void UCSoundCollectionComponent::PlayDashAttackSound()
{
	PlaySoundInternal(DashAttackSound);
}

void UCSoundCollectionComponent::PlayRangedAttackSound()
{
	PlaySoundInternal(RangedAttackSound);
}

void UCSoundCollectionComponent::PlayHeadOpenSound()
{
	PlaySoundInternal(HeadOpenSound);
}

void UCSoundCollectionComponent::PlayHeadCloseSound()
{
	PlaySoundInternal(HeadCloseSound);
}

void UCSoundCollectionComponent::PlayFlyingSkullIdleSound()
{
	if (IdleSounds.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, IdleSounds.Num() - 1);
		PlaySoundInternal(IdleSounds[RandomIndex]);
	}
}

void UCSoundCollectionComponent::PlayFlyingSkullRunSound()
{
	if (RunSounds.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, RunSounds.Num() - 1);
		PlaySoundInternal(RunSounds[RandomIndex]);
	}
}

void UCSoundCollectionComponent::StartIdleSoundLoop()
{
	StopIdleSoundLoop(); // 기존 타이머 정지
	PlayIdleSoundWithCheck(); // 즉시 첫 번째 사운드 재생 후 루프 시작
}

void UCSoundCollectionComponent::StopIdleSoundLoop()
{
	GetWorld()->GetTimerManager().ClearTimer(IdleSoundTimer);
}

void UCSoundCollectionComponent::StartRunSoundLoop()
{
	StopRunSoundLoop(); // 기존 타이머 정지
	PlayRunSoundWithCheck(); // 즉시 첫 번째 사운드 재생 후 루프 시작
}

void UCSoundCollectionComponent::StopRunSoundLoop()
{
	GetWorld()->GetTimerManager().ClearTimer(RunSoundTimer);
}

void UCSoundCollectionComponent::StopAllSoundLoops()
{
	StopIdleSoundLoop();
	StopRunSoundLoop();
}

void UCSoundCollectionComponent::PlayIdleSoundWithCheck()
{
	// 조건 체크 후 재생 (CFlowerEnemy에서 제어)
	PlayIdleSound();
	
	// 다음 재생 스케줄링
	float RandomDelay = FMath::RandRange(IdleSoundMinInterval, IdleSoundMaxInterval);
	GetWorld()->GetTimerManager().SetTimer(IdleSoundTimer, this, 
		&UCSoundCollectionComponent::PlayIdleSoundWithCheck, RandomDelay, false);
}

void UCSoundCollectionComponent::PlayRunSoundWithCheck()
{
	// 조건 체크 후 재생 (CFlowerEnemy에서 제어)
	PlayRunSound();
	
	// 다음 재생 스케줄링
	float RandomDelay = FMath::RandRange(RunSoundMinInterval, RunSoundMaxInterval);
	GetWorld()->GetTimerManager().SetTimer(RunSoundTimer, this, 
		&UCSoundCollectionComponent::PlayRunSoundWithCheck, RandomDelay, false);
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
