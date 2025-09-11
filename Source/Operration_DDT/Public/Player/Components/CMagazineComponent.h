// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMagazineComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCMagazineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCMagazineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool CheckCanReload();

	int32 RifleMagazines = 5;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magazine")
	int32 TotalRifleBullets;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Magazine")
	int32 CurrentRifleBullets;

	int32 RevolverMagazines = 6;
	int32 CurrentRevolverBullets = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* Reload_Start;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* Reload_Loop;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* Reload_End;

	bool bReloadStart = false;
	bool bReloadLoop = false;
	bool bReloadEnd = false;

	FORCEINLINE void InitializeReload() { bReloadStart = false; bReloadLoop = false; bReloadEnd = false; }
	FORCEINLINE void FireBullet() { --CurrentRifleBullets; }
	
	// 장전 시퀀스 관련 변수들
	int32 ReloadLoopCount = 0;           // 현재 루프 반복 횟수
	int32 MaxReloadLoopCount = 0;        // 최대 루프 반복 횟수
	bool bIsReloading = false;           // 장전 중인지 확인
	UAnimInstance* AnimInstance;         // 애니메이션 인스턴스 참조
	class ADDTPlayer* OwnerCharacter;


	FOnMontageEnded MontageEndedDelegate;
	
public:
	void ReloadRifleMagazine();
	void LootRifleBullets(int32 InValue);
	
	// 메인 장전 함수
	void StartReloadSequence();
	
	// 몽타주 완료 콜백 함수들
	UFUNCTION()
	void OnReloadStartFinished();
	
	UFUNCTION()
	void OnReloadLoopFinished();
	
	UFUNCTION()
	void OnReloadEndFinished();
	
	// 내부 함수들
	void StartReloadLoop();
	void StartReloadEnd();
	void PlayMontage(UAnimMontage* Montage);
	void CancelReload();
	
	// 델리게이트 바인딩 함수
	void BindMontageEndDelegate();
	void UnbindMontageEndDelegate();

		
};
