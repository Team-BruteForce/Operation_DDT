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
	USoundBase* IdleSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* RunSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* GetHitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* DieSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* MeleeAttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* ComboAttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* DashAttackSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound Collection")
	USoundBase* RangedAttackSound;

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
	void PlayDashAttackSound();

	UFUNCTION(BlueprintCallable, Category = "Sound Collection")
	void PlayRangedAttackSound();

private:
	// 사운드 재생 헬퍼 함수
	void PlaySoundInternal(USoundBase* SoundToPlay);
};
