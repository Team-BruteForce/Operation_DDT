#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CPlaySound_ComboAttack2.generated.h"

UCLASS()
class OPERRATION_DDT_API UCPlaySound_ComboAttack2 : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
