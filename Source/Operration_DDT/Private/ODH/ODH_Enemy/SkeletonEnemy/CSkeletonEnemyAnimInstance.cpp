#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemyAnimInstance.h"
#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCSkeletonEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* OwnerPawn = TryGetPawnOwner();
	CachedOwner = OwnerPawn ? Cast<ACSkeletonEnemy>(OwnerPawn) : nullptr;
}

void UCSkeletonEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	APawn* OwnerPawn = TryGetPawnOwner();
	if (!OwnerPawn)
		return;

	if (!CachedOwner)
	{
		CachedOwner = Cast<ACSkeletonEnemy>(OwnerPawn);
	}

	// 이동 상태 업데이트
	const FVector Velocity = OwnerPawn->GetVelocity();
	Speed = Velocity.Size2D();
	bIsMoving = Speed > 3.0f;

	// 전투/사망 플래그는 캐릭터에서 가져옴
	if (CachedOwner)
	{
		bIsMeleeAttacking = CachedOwner->GetIsMeleeAttacking();
		bIsDead = CachedOwner->IsDead_Implementation();
	}

	// bIsComboAttacking / bIsDashAttacking 은 AnimBP나 캐릭터 로직에서 세터로 제어
}
