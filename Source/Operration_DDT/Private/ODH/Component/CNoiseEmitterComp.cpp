// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/Component/CNoiseEmitterComp.h"
#include "Perception/AIPerceptionSystem.h"

// Sets default values for this component's properties
UCNoiseEmitterComp::UCNoiseEmitterComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Default values
	DefaultLoudness = 0.5f;
	DefaultNoiseTag = FName("GenericNoise");
	Owner = nullptr;
	
	// Test values
	bAutoTest = false;
	TestInterval = 3.0f;
	TestTimer = 0.0f;
	TestLoudness = 0.5f;
	TestNoiseTag = FName("TestNoise");
}


// Called when the game starts
void UCNoiseEmitterComp::BeginPlay()
{
	Super::BeginPlay();

	if (!Owner)
	{
		Owner = GetOwner();
	}
}


// Called every frame
void UCNoiseEmitterComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 자동 테스트를 위한 타이머
	TestTimer += DeltaTime;
	if (TestTimer >= TestInterval)
	{
		TestTimer = 0.0f;
		
		// 자동으로 소리 생성 (테스트용)
		if (bAutoTest)
		{
			EmitNoise(TestLoudness, TestNoiseTag);
			UE_LOG(LogTemp, Log, TEXT("Auto Test: Emitted noise with Loudness: %.2f, Tag: %s"), TestLoudness, *TestNoiseTag.ToString());
		}
	}
}

void UCNoiseEmitterComp::EmitNoise(float Loudness /*= 0.5f*/, const FName& NoiseTag /*= "GenericNoise"*/)
{
    if (!GetOwner())
    {
        UE_LOG(LogTemp, Warning, TEXT("NoiseEmitterComponent: No owner found!"));
        return;
    }

    // Use provided values or defaults
    float FinalLoudness = (Loudness > 0.0f) ? Loudness : DefaultLoudness;
    FName FinalNoiseTag = (NoiseTag != NAME_None) ? NoiseTag : DefaultNoiseTag;

    // Get the noise location from the owner
    FVector NoiseLocation = GetOwner()->GetActorLocation();

    // Emit noise using the AISystem
    UAIPerceptionSystem* PerceptionSystem = UAIPerceptionSystem::GetCurrent(GetWorld());
    if (PerceptionSystem)
    {
        // Create noise event using the correct method
        FAINoiseEvent NoiseEvent;
        NoiseEvent.NoiseLocation = NoiseLocation;
        NoiseEvent.Loudness = FinalLoudness;
        NoiseEvent.Instigator = Owner;
        NoiseEvent.Tag = FinalNoiseTag;
        
        PerceptionSystem->OnEvent(NoiseEvent);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("NoiseEmitterComponent: No Perception System found!"));
    }
}

// 테스트용 함수들
void UCNoiseEmitterComp::StartAutoTest()
{
	bAutoTest = true;
	UE_LOG(LogTemp, Log, TEXT("Auto Test Started - Noise will be emitted every %.1f seconds"), TestInterval);
}

void UCNoiseEmitterComp::StopAutoTest()
{
	bAutoTest = false;
	UE_LOG(LogTemp, Log, TEXT("Auto Test Stopped"));
}

void UCNoiseEmitterComp::SetTestInterval(float NewInterval)
{
	if (NewInterval > 0.0f)
	{
		TestInterval = NewInterval;
		UE_LOG(LogTemp, Log, TEXT("Test Interval set to %.1f seconds"), TestInterval);
	}
}

void UCNoiseEmitterComp::SetTestLoudness(float NewLoudness)
{
	if (NewLoudness >= 0.0f)
	{
		TestLoudness = NewLoudness;
		UE_LOG(LogTemp, Log, TEXT("Test Loudness set to %.2f"), TestLoudness);
	}
}

void UCNoiseEmitterComp::SetTestNoiseTag(const FName& NewTag)
{
	if (NewTag != NAME_None)
	{
		TestNoiseTag = NewTag;
		UE_LOG(LogTemp, Log, TEXT("Test Noise Tag set to %s"), *NewTag.ToString());
	}
}

void UCNoiseEmitterComp::TestEmitNoise()
{
	UE_LOG(LogTemp, Log, TEXT("Manual Test: Emitting noise with Loudness: %.2f, Tag: %s"), TestLoudness, *TestNoiseTag.ToString());
	EmitNoise(TestLoudness, TestNoiseTag);
}

