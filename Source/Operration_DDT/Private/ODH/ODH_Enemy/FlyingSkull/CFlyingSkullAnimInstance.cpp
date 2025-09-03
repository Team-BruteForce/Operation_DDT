#include "ODH/ODH_Enemy/FlyingSkull/CFlyingSkullAnimInstance.h"
#include "ODH/ODH_Enemy/FlyingSkull/CFlyingSkull.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCFlyingSkullAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	CachedOwner = Cast<ACFlyingSkull>(TryGetPawnOwner());
}

void UCFlyingSkullAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedOwner)
	{
		CachedOwner = Cast<ACFlyingSkull>(TryGetPawnOwner());
	}

	if (!CachedOwner)
	{
		Speed = 0.0f;
		bIsMoving = false;
		bIsMeleeAttacking = false;
		bIsRangedAttacking = false;
		bIsDead = false;
		return;
	}

	// 이동 상태 계산
	const FVector Velocity = CachedOwner->GetVelocity();
	Speed = FVector(Velocity.X, Velocity.Y, 0.0f).Size();
	bIsMoving = Speed > 3.0f; // 임계값 약간 부여

	// 캐릭터 상태 플래그 참조
	bIsDead = CachedOwner->IsDead_Implementation();

	// 공격 플래그는 캐릭터에 변수 추가 후 참조(추가 예정). 안전 장치로 false 유지
	bIsMeleeAttacking = false;
	bIsRangedAttacking = false;
}

