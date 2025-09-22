#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CFlowerHandRAttackCollisionNotify.generated.h"

/**
 * FlowerEnemy의 오른손 공격 콜리전 활성화/비활성화 애니메이션 노티파이
 */
UCLASS()
class OPERRATION_DDT_API UCFlowerHandRAttackCollisionNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	UCFlowerHandRAttackCollisionNotify();

	// 콜리전 활성화 여부
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Attack")
	bool bEnableCollision = true;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
