#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CSkeletonEnemyAnimInstance.generated.h"

class ACSkeletonEnemy;

UCLASS()
class OPERRATION_DDT_API UCSkeletonEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	// 이동 상태
	UPROPERTY(BlueprintReadOnly, Category = "State")
	float Speed = 0.0f;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsMoving = false;

	// 공격/사망 상태
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsMeleeAttacking = false;

	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsDead = false;

	// 근접 공격 세부 상태
	// 콤보 공격 중 여부 (연속 애니메이션 트리거용)
	UPROPERTY(BlueprintReadOnly, Category = "Melee")
	bool bIsComboAttacking = false;

	// 돌진 공격 중 여부 (플레이어에게 돌진하는 애니메이션 분기용)
	UPROPERTY(BlueprintReadOnly, Category = "Melee")
	bool bIsDashAttacking = false;

	// 외부에서 애님 상태를 제어하기 위한 세터 (AnimBP 이벤트용)
	UFUNCTION(BlueprintCallable, Category = "Melee")
	void SetComboAttacking(bool bNewValue) { bIsComboAttacking = bNewValue; }

	UFUNCTION(BlueprintCallable, Category = "Melee")
	void SetDashAttacking(bool bNewValue) { bIsDashAttacking = bNewValue; }

private:
	UPROPERTY(Transient)
	ACSkeletonEnemy* CachedOwner = nullptr;
};
