#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CFlowerEnemyHitStateEnd.generated.h"

UCLASS()
class OPERRATION_DDT_API UCFlowerEnemyHitStateEnd : public UAnimNotify
{
	GENERATED_BODY()

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};


