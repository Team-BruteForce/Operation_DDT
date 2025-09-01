// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Perception/AISense_Hearing.h"
#include "CNoiseEmitterComp.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCNoiseEmitterComp : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCNoiseEmitterComp();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Function to emit noise
    UFUNCTION(BlueprintCallable, Category = "Noise")
    void EmitNoise(float Loudness = 0.5f, const FName& NoiseTag = "GenericNoise");

    // Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
    float DefaultLoudness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
    FName DefaultNoiseTag;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Noise")
    AActor* Owner;

    // 테스트용 속성들
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    bool bAutoTest;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    float TestInterval;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    float TestLoudness;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Test")
    FName TestNoiseTag;

    // 테스트용 함수들
    UFUNCTION(BlueprintCallable, Category = "Test")
    void StartAutoTest();

    UFUNCTION(BlueprintCallable, Category = "Test")
    void StopAutoTest();

    UFUNCTION(BlueprintCallable, Category = "Test")
    void SetTestInterval(float NewInterval);

    UFUNCTION(BlueprintCallable, Category = "Test")
    void SetTestLoudness(float NewLoudness);

    UFUNCTION(BlueprintCallable, Category = "Test")
    void SetTestNoiseTag(const FName& NewTag);

    UFUNCTION(BlueprintCallable, Category = "Test")
    void TestEmitNoise();

private:
    // 테스트용 private 변수
    float TestTimer;
};
