#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemyAnimInstance.h"
#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

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
		bIsComboAttacking = CachedOwner->GetIsComboAttacking();
		bIsDashAttacking = CachedOwner->GetIsDashAttacking();
		bIsDead = CachedOwner->IsDead_Implementation();

		// Blackboard 의 IsInCombat 값을 읽어서 bIsRun 동기화
		if (AAIController* AICon = Cast<AAIController>(CachedOwner->GetController()))
		{
			if (UBlackboardComponent* BB = AICon->GetBlackboardComponent())
			{
				static const FName IsInCombatKey = TEXT("IsInCombat");
				const bool bInCombat = BB->GetValueAsBool(IsInCombatKey);
				bIsRun = bInCombat;
			}
			else
			{
				bIsRun = false;
			}
		}
		else
		{
			bIsRun = false;
		}
	}

	// bIsComboAttacking / bIsDashAttacking 은 AnimBP나 캐릭터 로직에서 세터로 제어
}
