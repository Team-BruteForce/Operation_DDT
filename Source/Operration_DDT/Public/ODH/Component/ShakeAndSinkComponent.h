#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShakeAndSinkComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OPERRATION_DDT_API UShakeAndSinkComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UShakeAndSinkComponent();

public:
	UFUNCTION(BlueprintCallable, Category="ShakeAndSink")
	void StartEffect(float InDelaySeconds, float InSinkDistance, float InSinkDuration, float InShakeAmplitude, float InShakeFrequency);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	bool bIsActive;
	float DelaySeconds;
	float SinkDistance;
	float SinkDuration;
	float ShakeAmplitude;
	float ShakeFrequency;

	float ElapsedSinceStart;
	FVector InitialLocation;
	bool bInitializedLocation;

	// Once elapsed time passes 6 seconds, disable mesh collision once
	bool bCollisionDisabled;
};


