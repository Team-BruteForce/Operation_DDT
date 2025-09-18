#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CSkeletonEnemyHitStateEnd.generated.h"

UCLASS()
class OPERRATION_DDT_API UCSkeletonEnemyHitStateEnd : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override { return TEXT("CSkeletonEnemyHitStateEnd"); }

protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};


