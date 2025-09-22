#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CFlowerEnemyAnimInstance.generated.h"

class ACFlowerEnemy;

UCLASS()
class OPERRATION_DDT_API UCFlowerEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	// 이동 속도 및 이동 여부
	UPROPERTY(BlueprintReadOnly, Category = "State")
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsMoving = false;

	// 공격/사망 상태 (캐릭터 플래그를 참조)
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsComboAttack = false;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsDashAttack = false;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsRangedAttack = false;

	// 머리 상태 (캐릭터 플래그를 참조)
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsHeadOpen = false;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsDead = false;

private:
	UPROPERTY(Transient)
	ACFlowerEnemy* CachedOwner = nullptr;
};
