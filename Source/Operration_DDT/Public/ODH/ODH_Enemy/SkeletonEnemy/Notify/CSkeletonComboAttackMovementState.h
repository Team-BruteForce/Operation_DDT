#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CSkeletonComboAttackMovementState.generated.h"

/**
 * 스켈레톤 에너미의 콤보 공격 시 살짝 앞으로 이동하는 애니메이션 노티파이 스테이트
 * Begin: 이동 방향 설정 및 시작
 * Tick: 매 프레임마다 부드럽게 이동
 * End: 이동 종료
 */
UCLASS()
class OPERRATION_DDT_API UCSkeletonComboAttackMovementState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UCSkeletonComboAttackMovementState();

	// 노티파이 시작 시 호출
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;

	// 노티파이 진행 중 매 프레임 호출
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

	// 노티파이 종료 시 호출
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	// 이동 속도 (설정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MovementSpeed = 200.0f;

	// 이동 거리 (설정 가능)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MovementDistance = 10.0f;

	// 이동 중인지 확인하는 플래그
	bool bIsMoving = false;
};

