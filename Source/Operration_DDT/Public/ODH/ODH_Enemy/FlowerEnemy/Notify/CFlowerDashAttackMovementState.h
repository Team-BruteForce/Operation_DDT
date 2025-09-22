#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CFlowerDashAttackMovementState.generated.h"

/**
 * FlowerEnemy의 돌진 공격 시 플레이어를 향해 이동하는 애니메이션 노티파이 스테이트
 * Begin: 플레이어 위치 저장
 * Tick: 저장된 플레이어 위치를 향해 이동
 * End: 이동 종료
 */
UCLASS()
class OPERRATION_DDT_API UCFlowerDashAttackMovementState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UCFlowerDashAttackMovementState();

	// 노티파이 시작 시 호출
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	// 노티파이 진행 중 매 프레임 호출
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	// 노티파이 종료 시 호출
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	// 이동 속도 (설정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 1000.0f;

	// 이동 중인지 확인하는 플래그
	bool bIsMoving = false;
};


