#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CSkeletonOnDashAttackCollision.generated.h"

/**
 * 스켈레톤 에너미의 돌진 공격 콜리전을 활성화하는 애니메이션 노티파이
 */
UCLASS()
class OPERRATION_DDT_API UCSkeletonOnDashAttackCollision : public UAnimNotify
{
	GENERATED_BODY()

public:
	UCSkeletonOnDashAttackCollision();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};



