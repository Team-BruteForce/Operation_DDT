#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_SinkAfterDelay.generated.h"

/**
 * Triggers a delayed sink-and-shake effect on the owning actor.
 * Parameters are editable per notify placement in animations.
 */
UCLASS(meta=(DisplayName="Sink After Delay"))
class OPERRATION_DDT_API UCAnimNotify_SinkAfterDelay : public UAnimNotify
{
	GENERATED_BODY()

public:
	// Seconds to wait before starting the effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SinkAndShake")
	float DelaySeconds = 3.0f;

	// How far to sink downward (in cm, along -Z)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SinkAndShake")
	float SinkDistance = 100.0f;

	// How long the sinking animation should take (in seconds)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SinkAndShake")
	float SinkDuration = 1.5f;

	// Shake amplitude (in cm)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SinkAndShake")
	float ShakeAmplitude = 3.0f;

	// Shake frequency (Hz)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="SinkAndShake")
	float ShakeFrequency = 25.0f;

public:
	virtual FString GetNotifyName_Implementation() const override;
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};



