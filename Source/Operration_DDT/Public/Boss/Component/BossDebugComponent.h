// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @brief 보스 디버그 컴포넌트 클래스
 * 
 * 보스 캐릭터의 디버그 시각화를 관리하는 컴포넌트입니다.
 * 거리, 궤도, 백스탭 위치 등의 디버그 정보를 시각화합니다.
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BossDebugComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UBossDebugComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBossDebugComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// ===== 거리 디버그 함수들 =====
	
	/**
	 * @brief 거리 디버그 정보를 그립니다.
	 * 
	 * @param TargetLocation 타겟 위치
	 * @param OwnerLocation 소유자 위치
	 * @param MinDistance 최소 거리
	 * @param MaxDistance 최대 거리
	 * @param CurrentDistance 현재 거리
	 * @param ClosestPosition 목표 위치
	 */
	UFUNCTION(BlueprintCallable, Category="Distance Debug")
	void DrawDistanceDebug(const FVector& TargetLocation, const FVector& OwnerLocation, 
		float MinDistance, float MaxDistance, float CurrentDistance, const FVector& ClosestPosition);

	// ===== 궤도 디버그 함수들 =====
	
	/**
	 * @brief 궤도 디버그 정보를 그립니다.
	 * 
	 * @param TargetLocation 타겟 위치
	 * @param OwnerLocation 소유자 위치
	 * @param ArcStart 호 시작점
	 * @param ArcEnd 호 끝점
	 * @param ArcRadius 호 반지름
	 * @param BossForward 보스 전방 방향
	 * @param BossBackward 보스 후방 방향
	 * @param ClosestPosition 목표 위치
	 */
	UFUNCTION(BlueprintCallable, Category="Orbit Debug")
	void DrawOrbitDebug(const FVector& TargetLocation, const FVector& OwnerLocation,
		const FVector& ArcStart, const FVector& ArcEnd, float ArcRadius,
		const FVector& BossForward, const FVector& BossBackward, const FVector& ClosestPosition);

	// ===== 백스탭 디버그 함수들 =====
	
	/**
	 * @brief 백스탭 디버그 정보를 그립니다.
	 * 
	 * @param BackstepPosition 백스탭 위치
	 * @param PlayerLocation 플레이어 위치
	 * @param StepIndex 단계 인덱스
	 */
	UFUNCTION(BlueprintCallable, Category="Backstep Debug")
	void DrawBackstepDebug(const FVector& BackstepPosition, const FVector& PlayerLocation, int32 StepIndex);

	/**
	 * @brief 백스탭 실패 시 디버그 정보를 그립니다.
	 * 
	 * @param BossLocation 보스 위치
	 */
	UFUNCTION(BlueprintCallable, Category="Backstep Debug")
	void DrawBackstepFailureDebug(const FVector& BossLocation);
	
	UFUNCTION(BlueprintCallable, Category="Orbit Debug")
	void DrawOrbitDistanceDebug(const FVector& TargetLocation, const FVector& OwnerLocation, 
		float MinDistance, float MaxDistance, const FVector& ClosestPosition);
	
	UFUNCTION(BlueprintCallable, Category="Arc Debug")
	void DrawArcDebug(const FVector& TargetLocation, const FVector& OwnerLocation,
		const FVector& ArcStart, const FVector& ArcEnd, float ArcRadius,
		const FVector& BossForward, const FVector& BossBackward, const FVector& ClosestPosition);
	
	UFUNCTION(BlueprintCallable, Category="Debug Visualization")
	void ExecuteDebugVisualization();
	
	// MovementComponent 참조 설정
	UFUNCTION(BlueprintCallable, Category="Debug Setup")
	void SetMovementComponent(class UCBossMovementComponent* InMovementComponent);
	
private:
	/** MovementComponent 참조 */
	UPROPERTY()
	class UCBossMovementComponent* MovementComponent;
		
};
