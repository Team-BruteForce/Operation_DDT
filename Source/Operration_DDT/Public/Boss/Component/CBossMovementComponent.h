// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @brief 보스 이동 관리 컴포넌트 클래스
 * 
 * 보스 캐릭터의 이동 로직을 관리하는 컴포넌트입니다.
 * 플레이어 추적, 회전, 이동 패턴 등을 처리합니다.
 */

#pragma once

#include "CoreMinimal.h"
#include "Boss/Data/BossTagStructure.h"
#include "Components/ActorComponent.h"
#include "CBossMovementComponent.generated.h"

/**
 * @brief 보스 이동 관리 컴포넌트 클래스
 * 
 * 보스 캐릭터의 이동 로직을 관리하는 컴포넌트입니다.
 * 플레이어 추적, 부드러운 회전, 이동 패턴 등을 처리합니다.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCBossMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/**
	 * @brief 생성자
	 */
	UCBossMovementComponent();

protected:
	/**
	 * @brief 게임 시작 시 호출되는 함수
	 */
	virtual void BeginPlay() override;

public:	
	/**
	 * @brief 매 프레임 호출되는 틱 함수
	 * 
	 * @param DeltaTime 프레임 간 시간 간격
	 * @param TickType 틱 타입
	 * @param ThisTickFunction 틱 함수 정보
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief 플레이어 방향으로 부드럽게 회전하는 함수
	 * 
	 * @param DeltaTime 델타 타임
	 * @param RotationSpeed 회전 속도 (높을수록 빠름)
	 */
	UFUNCTION(BlueprintCallable, Category="Player Tracking")
	void RotateTowardsPlayer(float DeltaTime, float RotationSpeed = 5.0f);

	// ===== 백스탭 위치 찾기 함수들 =====
	
	/**
	 * @brief 플레이어 기준 360도 영역에서 백스탭 가능한 위치를 찾습니다.
	 * 
	 * @param BackstepDistance 백스탭 거리
	 * @return 백스탭 위치
	 */
	UFUNCTION(BlueprintCallable, Category="Backstep Movement")
	FVector FindBackstepPosition();

	/**
	 * @brief Nav Mesh를 사용하여 특정 방향에서 안전한 위치를 찾습니다.
	 * 
	 * @param Direction 검색 방향
	 * @param Distance 검색 거리
	 * @return 안전한 위치
	 */
	FVector FindSafePositionOnNavMesh(const FVector& Direction, float Distance);

	/**
	 * @brief 위치가 플레이어로부터 충분히 떨어져 있는지 확인합니다.
	 * 
	 * @param Position 확인할 위치
	 * @param MinDistanceFromPlayer 최소 거리
	 * @return 안전 여부
	 */
	bool IsPositionFarFromPlayer(const FVector& Position, float MinDistanceFromPlayer);

	// ===== 궤도 이동 함수들 =====
	
	/**
	 * @brief 플레이어 주변에서 궤도 이동 위치를 계산합니다.
	 * 
	 * @param DeltaTime 델타 타임
	 * @param MinDistance 최소 거리
	 * @param MaxDistance 최대 거리
	 * @param CurrentTargetStateTag 현재 타겟 상태 태그
	 * @param OutClosestPosition 계산된 궤도 위치
	 */
	UFUNCTION(BlueprintCallable, Category="Orbit Movement")
	void CalculateOrbitPosition(float DeltaTime, float MinDistance, float MaxDistance, 
		const FGameplayTag& CurrentTargetStateTag, FVector& OutClosestPosition);

	/**
	 * @brief 플레이어와의 거리를 계산합니다.
	 * 
	 * @param OutTargetLocation 타겟 위치
	 * @param OutOwnerLocation 소유자 위치
	 * @param OutDirectionToTarget 타겟으로의 방향
	 * @param OutCurrentDistance 현재 거리
	 */
	UFUNCTION(BlueprintCallable, Category="Distance Calculation")
	void CalculatePlayerDistance(FVector& OutTargetLocation, FVector& OutOwnerLocation, 
		FVector& OutDirectionToTarget, float& OutCurrentDistance);
	
	UFUNCTION(BlueprintCallable, Category="Orbit Movement")
	void MoveInOrbit(float DeltaTime, float MinDistance, float MaxDistance, 
		const FGameplayTag& CurrentTargetStateTag,
		FVector& OutClosestPosition, FVector& OutArcStart, FVector& OutArcEnd, 
		float& OutArcRadius, FVector& OutBossForward, FVector& OutBossBackward);
	
	UFUNCTION(BlueprintCallable, Category="Target Look")
	void LookAtTarget(const FVector& DirectionToTarget);
	
	// 궤도 이동 전체 처리 함수
	UFUNCTION(BlueprintCallable, Category="Orbit Movement")
	void ExecuteOrbitMovement(float DeltaTime, float MinDistance, float MaxDistance);
	
	/**
	 * @brief 플레이어 움직임 상태 태그를 반환하는 함수
	 * 
	 * @return 플레이어 움직임에 따른 상태 태그
	 */
	FGameplayTag GetPlayerMovementStateTag();
	
	// ===== 디버그 데이터 Getter 함수들 =====
	
	/** @brief 타겟 위치 반환 */
	FVector GetDebugTargetLocation() const;
	
	/** @brief 소유자 위치 반환 */
	FVector GetDebugOwnerLocation() const;
	
	/** @brief 가장 가까운 위치 반환 */
	FVector GetDebugClosestPosition() const;
	
	/** @brief 호 시작점 반환 */
	FVector GetDebugArcStart() const;
	
	/** @brief 호 끝점 반환 */
	FVector GetDebugArcEnd() const;
	
	/** @brief 호 반지름 반환 */
	float GetDebugArcRadius() const;
	
	/** @brief 보스 전방 방향 반환 */
	FVector GetDebugBossForward() const;
	
	/** @brief 보스 후방 방향 반환 */
	FVector GetDebugBossBackward() const;
	
	/** @brief 현재 거리 반환 */
	float GetDebugCurrentDistance() const;

	/**
	 * @brief 플레이어를 찾는 함수
	 * 
	 * @return 찾은 플레이어 액터, 없으면 nullptr
	 */
public:
	class APawn* FindPlayer();
private:
	

	/** 소유자 액터 */
	UPROPERTY()
	class APawn* Owner;
	
	/** AI 컨트롤러 */
	UPROPERTY()
	class AAIController* AIC;

	UPROPERTY()
	class UCBossTargetingComponent* TargetingComp;

	UPROPERTY(EditAnywhere)
	FBossTargetState TargetStateTag;
public:
	UPROPERTY()
	FVector SafePosition;
	
	// ===== 디버그 데이터 멤버 변수들 =====
	
	/** 디버그용 타겟 위치 */
	FVector DebugTargetLocation;
	
	/** 디버그용 소유자 위치 */
	FVector DebugOwnerLocation;
	
	/** 디버그용 가장 가까운 위치 */
	FVector DebugClosestPosition;
	
	/** 디버그용 호 시작점 */
	FVector DebugArcStart;
	
	/** 디버그용 호 끝점 */
	FVector DebugArcEnd;
	
	/** 디버그용 호 반지름 */
	float DebugArcRadius;
	
	/** 디버그용 보스 전방 방향 */
	FVector DebugBossForward;
	
	/** 디버그용 보스 후방 방향 */
	FVector DebugBossBackward;
	
	/** 디버그용 현재 거리 */
	float DebugCurrentDistance;
};
// ===== 백스탭 상수들 =====
namespace BackstepConstants
{
	const int32 MAX_ANGLE_STEPS = 10;
	const float ANGLE_STEPS[MAX_ANGLE_STEPS] = { 0.0f, 15.0f, 30.0f, 45.0f, 60.0f, 90.0f, 120.0f, 150.0f, 180.0f, 210.0f };
	const FColor DEBUG_COLORS[MAX_ANGLE_STEPS] = { 
		FColor::Green, FColor::Yellow, FColor::Orange, FColor::Red, FColor::Purple, 
		FColor::Blue, FColor::Cyan, FColor::Magenta, FColor::White, FColor::Black 
	};
}

