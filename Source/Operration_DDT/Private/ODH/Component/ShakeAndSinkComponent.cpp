#include "ODH/Component/ShakeAndSinkComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

UShakeAndSinkComponent::UShakeAndSinkComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bIsActive = false;
	DelaySeconds = 0.0f;
	SinkDistance = 0.0f;
	SinkDuration = 1.0f;
	ShakeAmplitude = 0.0f;
	ShakeFrequency = 0.0f;
	ElapsedSinceStart = 0.0f;
	bInitializedLocation = false;
	bCollisionDisabled = false;
}

void UShakeAndSinkComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UShakeAndSinkComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UShakeAndSinkComponent::StartEffect(float InDelaySeconds, float InSinkDistance, float InSinkDuration, float InShakeAmplitude, float InShakeFrequency)
{
	DelaySeconds = FMath::Max(0.0f, InDelaySeconds);
	SinkDistance = FMath::Max(0.0f, InSinkDistance);
	SinkDuration = FMath::Max(KINDA_SMALL_NUMBER, InSinkDuration);
	ShakeAmplitude = FMath::Max(0.0f, InShakeAmplitude);
	ShakeFrequency = FMath::Max(0.0f, InShakeFrequency);

	ElapsedSinceStart = 0.0f;
	bInitializedLocation = false;
	bIsActive = true;
	bCollisionDisabled = false;
}

void UShakeAndSinkComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsActive)
		return;

	AActor* OwnerActor = GetOwner();
	if (!OwnerActor)
	{
		bIsActive = false;
		return;
	}

	ElapsedSinceStart += DeltaTime;

	// After 6 seconds, disable mesh collision once
	if (!bCollisionDisabled && ElapsedSinceStart >= 6.0f)
	{
		USkeletalMeshComponent* MeshComp = OwnerActor->FindComponentByClass<USkeletalMeshComponent>();
		if (MeshComp)
		{
			MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		bCollisionDisabled = true;
	}

	if (ElapsedSinceStart < DelaySeconds)
	{
		return; // waiting phase
	}

	if (!bInitializedLocation)
	{
		InitialLocation = OwnerActor->GetActorLocation();
		bInitializedLocation = true;
	}

	const float SinkElapsed = ElapsedSinceStart - DelaySeconds;
	const float Alpha = FMath::Clamp(SinkElapsed / SinkDuration, 0.0f, 1.0f);
	const float EasedAlpha = UKismetMathLibrary::Ease(0.0f, 1.0f, Alpha, EEasingFunc::EaseInOut);
	const float SinkOffset = -SinkDistance * EasedAlpha; // downward along -Z

	// Shake: simple sin-based vertical jitter, can add horizontal subtle noise
	float ShakeOffsetZ = 0.0f;
	if (ShakeAmplitude > 0.0f && ShakeFrequency > 0.0f)
	{
		const float Time = GetWorld() ? GetWorld()->GetTimeSeconds() : SinkElapsed;
		ShakeOffsetZ = FMath::Sin(Time * 2.0f * PI * ShakeFrequency) * ShakeAmplitude;
	}

	FVector TargetLocation = InitialLocation + FVector(0.0f, 0.0f, SinkOffset + ShakeOffsetZ);
	OwnerActor->SetActorLocation(TargetLocation, false, nullptr, ETeleportType::TeleportPhysics);

	if (Alpha >= 1.0f)
	{
		// Stop shaking after sink completes
		bIsActive = false;
		OwnerActor->SetActorLocation(InitialLocation + FVector(0.0f, 0.0f, -SinkDistance));
	}
}


