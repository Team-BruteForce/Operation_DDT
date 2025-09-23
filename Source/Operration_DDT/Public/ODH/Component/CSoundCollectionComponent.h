#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundBase.h"
#include "CSoundCollectionComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCSoundCollectionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCSoundCollectionComponent();

protected:
	virtual void BeginPlay() override;

public:
	// 사운드 에셋들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	TArray<USoundBase*> IdleSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	TArray<USoundBase*> RunSounds;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* GetHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* DieSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* MeleeAttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* ComboAttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* ComboAttack2Sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* DashAttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* RangedAttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* HeadOpenSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* HeadCloseSound;

	// 사운드 재생 함수들
	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void PlayIdleSound();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void PlayRunSound();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void PlayGetHitSound();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void PlayDieSound();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void PlayMeleeAttackSound();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void PlayComboAttackSound();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void PlayComboAttack2Sound();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void PlayDashAttackSound();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void PlayRangedAttackSound();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void PlayHeadOpenSound();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void PlayHeadCloseSound();

	// FlyingSkull 전용 함수들 (노티파이로 호출)
	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void PlayFlyingSkullIdleSound();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void PlayFlyingSkullRunSound();

	// 사운드 루프 관리 함수들
	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void StartIdleSoundLoop();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void StopIdleSoundLoop();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void StartRunSoundLoop();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void StopRunSoundLoop();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void StopAllSoundLoops();

	// 사운드 간격 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	float IdleSoundMinInterval = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	float IdleSoundMaxInterval = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	float RunSoundMinInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	float RunSoundMaxInterval = 3.0f;

private:
	// 사운드 재생 헬퍼 함수
	void PlaySoundInternal(USoundBase* SoundToPlay);

	// 타이머 콜백 함수들
	void PlayIdleSoundWithCheck();
	void PlayRunSoundWithCheck();

	// 타이머 핸들들
	FTimerHandle IdleSoundTimer;
	FTimerHandle RunSoundTimer;
};
