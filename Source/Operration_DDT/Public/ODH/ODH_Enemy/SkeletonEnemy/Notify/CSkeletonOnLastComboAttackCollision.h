#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CSkeletonOnLastComboAttackCollision.generated.h"

/**
 * 스켈레톤 에너미의 마지막 콤보 공격 콜리전을 활성화하는 애니메이션 노티파이
 */
UCLASS()
class OPERRATION_DDT_API UCSkeletonOnLastComboAttackCollision : public UAnimNotify
{
	GENERATED_BODY()

public:
	UCSkeletonOnLastComboAttackCollision();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};








