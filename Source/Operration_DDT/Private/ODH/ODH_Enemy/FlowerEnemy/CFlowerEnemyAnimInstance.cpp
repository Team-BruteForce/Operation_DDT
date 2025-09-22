#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemyAnimInstance.h"
#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCFlowerEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	CachedOwner = Cast<ACFlowerEnemy>(TryGetPawnOwner());
}

void UCFlowerEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedOwner)
	{
		CachedOwner = Cast<ACFlowerEnemy>(TryGetPawnOwner());
	}

	if (!CachedOwner)
	{
		Speed = 0.0f;
		bIsMoving = false;
		bIsComboAttack = false;
		bIsDashAttack = false;
		bIsRangedAttack = false;
		bIsHeadOpen = false;
		bIsDead = false;
		bIsHitState = false;
		return;
	}

	// 이동 상태 계산
	const FVector Velocity = CachedOwner->GetVelocity();
	Speed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
	bIsMoving = Speed > 3.0f; // 임계값 약간 부여

	// 캐릭터 상태 플래그 참조
	bIsDead = CachedOwner->IsDead_Implementation();
	bIsHitState = CachedOwner->bIsHitState;

	// 공격 플래그는 캐릭터 상태를 참조
	bIsComboAttack = CachedOwner->GetIsComboAttacking();
	bIsDashAttack = CachedOwner->GetIsDashAttacking();

    // 대쉬 공격 전환 트리거
	bIsRangedAttack = CachedOwner->GetIsRangedAttacking();

	// 머리 상태 플래그는 캐릭터 상태를 참조
	bIsHeadOpen = CachedOwner->GetIsHeadOpen();
}
