#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CFlowerRangedAttackEndNotify.generated.h"

/**
 * 플라워 에너미의 원거리 공격 완료를 알리는 애니메이션 노티파이
 * 블랙보드의 IsRangedAttackComplete 변수를 true로 설정
 */
UCLASS()
class OPERRATION_DDT_API UCFlowerRangedAttackEndNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UCFlowerRangedAttackEndNotify();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
