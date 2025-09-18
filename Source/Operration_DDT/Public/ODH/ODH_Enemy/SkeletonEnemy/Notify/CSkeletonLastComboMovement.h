#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CSkeletonLastComboMovement.generated.h"

/**
 * 스켈레톤 에너미의 마지막 콤보 공격 시 전진 움직임을 처리하는 애니메이션 노티파이
 */
UCLASS()
class OPERRATION_DDT_API UCSkeletonLastComboMovement : public UAnimNotify
{
	GENERATED_BODY()

public:
	UCSkeletonLastComboMovement();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	// 애니메이션 에디터에서 설정할 수 있는 전진 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float ForwardDistance = 100.0f;
};