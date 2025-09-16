// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StateTreeAIComponent.h"
#include "StateTree.h"
#include "BossManager.generated.h"

/**
 * @brief 보스 매니저 클래스
 * 
 * 보스를 스폰하고 생명주기를 관리하는 매니저 액터입니다.
 * 매니저 위치에서 보스를 스폰하고 완전 초기화 기능을 제공합니다.
 */
UCLASS()
class OPERRATION_DDT_API ABossManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ABossManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief 보스 클래스 (스폰할 보스 타입)
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Spawn")
	TSubclassOf<class ACBoss> BossClass;

	/**
	 * @brief 관리 중인 보스 참조
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Management")
	class ACBoss* SpawnedBoss;

	/**
	 * @brief 보스 초기 위치 저장
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Management")
	FVector BossInitialLocation;

	/**
	 * @brief 박스 콜리전 (플레이어 감지용)
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* TriggerBox;

	/**
	 * @brief StateTree 이벤트 태그
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Event")
	FGameplayTag BossStartEventTag;

	/**
	 * @brief 보스 완전 초기화 (메인 함수)
	 * 
	 * @details
	 * 모든 보스 컴포넌트를 초기화하고 매니저 위치로 이동시킵니다.
	 * 플레이어 죽음 시 호출되는 메인 초기화 함수입니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "Boss Reset")
	void ResetBossCompletely();

	/**
	 * @brief 월드에서 보스 찾기
	 */
	UFUNCTION(BlueprintCallable, Category = "Boss Management")
	void FindBossInWorld();


protected:
	/**
	 * @brief 콜리전 오버랩 시작 이벤트
	 */
	UFUNCTION()
	void OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	/**
	 * @brief 각 컴포넌트별 초기화 실행
	 */
	UFUNCTION(BlueprintCallable)
	void ResetAllBossComponents();

	/**
	 * @brief StateTree 완전 리스타트
	 */
	UFUNCTION(BlueprintCallable)
	void ResetBossStateTree();


	UPROPERTY()
	FTimerHandle RestartTimer;
	
};
