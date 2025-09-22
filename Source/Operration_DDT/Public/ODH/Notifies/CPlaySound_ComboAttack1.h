#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CPlaySound_ComboAttack1.generated.h"

UCLASS()
class OPERRATION_DDT_API UCPlaySound_ComboAttack1 : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
