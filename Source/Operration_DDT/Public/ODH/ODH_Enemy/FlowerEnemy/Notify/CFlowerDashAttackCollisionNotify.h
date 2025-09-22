#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CFlowerDashAttackCollisionNotify.generated.h"

/**
 * FlowerEnemy의 돌진 공격 콜리전 활성화/비활성화 애니메이션 노티파이
 */
UCLASS()
class OPERRATION_DDT_API UCFlowerDashAttackCollisionNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UCFlowerDashAttackCollisionNotify();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
