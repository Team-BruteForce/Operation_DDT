// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossEffectStructures.h"
#include "BossEffect.generated.h"

/**
 * @brief 보스 이펙트 액터
 * 
 * @details
 * 오브젝트 풀링을 지원하는 보스 이펙트 액터입니다.
 * Niagara System을 사용하여 이펙트를 재생하고 관리합니다.
 */
UCLASS()
class OPERRATION_DDT_API ABossEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossEffect();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UNiagaraComponent* NiagaraComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Effect")
	bool bIsActive = false;

	UPROPERTY(BlueprintReadOnly, Category = "Effect")
	FGameplayTag CurrentEffectTag;

	UPROPERTY(BlueprintReadOnly, Category = "Effect")
	float CurrentDuration = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Effect")
	float ElapsedTime = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "Effect")
	float LastLogTime = 0.0f; // 로그 스팸 방지용

public:
	// 이펙트 완료 델리게이트
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectFinished, ABossEffect*, Effect);

	UPROPERTY(BlueprintAssignable, Category = "Effect")
	FOnEffectFinished OnEffectFinishedDelegate;
	/**
	 * @brief 이펙트 활성화
	 * 
	 * @param InEffectData 이펙트 데이터
	 * @param InLocation 위치
	 * @param InRotation 회전
	 * @param InScale 스케일
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ActivateEffect(const FBossEffectData& InEffectData, const FVector& InLocation, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector::ZeroVector);

	/**
	 * @brief 소켓에 붙여서 이펙트 활성화
	 * 
	 * @param InEffectData 이펙트 데이터
	 * @param SocketName 소켓 이름
	 * @param InRotation 회전
	 * @param InScale 스케일
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ActivateEffectAttachedToSocket(const FBossEffectData& InEffectData, const FName& SocketName, const FVector& InLocation = FVector::ZeroVector, const FRotator& InRotation = FRotator::ZeroRotator, const FVector& InScale = FVector(1,1,1));

	/**
	 * @brief 이펙트 비활성화
	 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void DeactivateEffect();

	/**
	 * @brief 이펙트가 활성화되어 있는지 확인
	 * 
	 * @return true면 활성화, false면 비활성화
	 */
	UFUNCTION(BlueprintPure, Category = "Effect")
	bool IsActive() const { return bIsActive; }

	/**
	 * @brief 현재 이펙트 태그 반환
	 * 
	 * @return 현재 이펙트 태그
	 */
	UFUNCTION(BlueprintPure, Category = "Effect")
	FGameplayTag GetCurrentEffectTag() const { return CurrentEffectTag; }

protected:
	/**
	 * @brief 이펙트를 보스에 어태치
	 * 
	 * @param InBoss 보스 캐릭터
	 * @param InSocketName 소켓 이름
	 */
	void AttachToBoss(class ACharacter* InBoss, const FName& InSocketName);

	/**
	 * @brief 소켓에 이펙트 붙이기
	 * 
	 * @param SocketName 소켓 이름
	 * @param InLocation 커스텀 위치 오프셋
	 * @param InRotation 회전
	 */
	void AttachToSocket(const FName& SocketName, const FVector& InLocation, const FRotator& InRotation);

	/**
	 * @brief 이펙트를 월드에 배치
	 * 
	 * @param InLocation 위치
	 * @param InRotation 회전
	 */
	void PlaceInWorld(const FVector& InLocation, const FRotator& InRotation);
};