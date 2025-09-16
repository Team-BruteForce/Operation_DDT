#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CSkeletonDashAttackEndNotify.generated.h"

/**
 * 스켈레톤 에너미의 대시 공격 완료를 알리는 애니메이션 노티파이
 * 블랙보드의 IsDashAttackComplete 변수를 true로 설정
 */
UCLASS()
class OPERRATION_DDT_API UCSkeletonDashAttackEndNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UCSkeletonDashAttackEndNotify();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
