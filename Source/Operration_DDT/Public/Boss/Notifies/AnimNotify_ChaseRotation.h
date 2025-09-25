/**
 * @file AnimNotify_ChaseRotation.h
 * @brief 추적 회전 애니메이션 노티파이 헤더 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스가 플레이어를 추적하며 회전하는 노티파이입니다.
 * 보스의 회전 기능을 활성화/비활성화합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotify_ChaseRotation.generated.h"

/**
 * @brief 추적 회전 애니메이션 노티파이
 * 
 * 애니메이션 시퀀스에서 보스가 플레이어를 추적하며 회전하는 노티파이입니다.
 * 보스의 회전 기능을 활성화/비활성화합니다.
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotify_ChaseRotation : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation ( ) const override;
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	UPROPERTY(EditAnywhere, Category = "Chase Settings")
	float RotationSpeed = 5.0f;

private:
	class UCBossMovementComponent* MovementComponent;
	bool bIsChasing = false;
};
