#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CFlowerComboAttackEndNotify.generated.h"

/**
 * 플라워 에너미의 콤보 공격 완료를 알리는 애니메이션 노티파이
 * 블랙보드의 IsComboAttackComplete 변수를 true로 설정
 */
UCLASS()
class OPERRATION_DDT_API UCFlowerComboAttackEndNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UCFlowerComboAttackEndNotify();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
