#include "ODH/ODH_Enemy/SkeletonEnemy/Notify/CSkeletonComboAttackMovementState.h"
#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"
#include "Engine/Engine.h"

UCSkeletonComboAttackMovementState::UCSkeletonComboAttackMovementState()
{
	// 기본 생성자
}

void UCSkeletonComboAttackMovementState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// 메시 컴포넌트의 소유자가 스켈레톤 에너미인지 확인
	if (ACSkeletonEnemy* SkeletonEnemy = Cast<ACSkeletonEnemy>(MeshComp->GetOwner()))
	{
		// 콤보 공격 이동 시작
		SkeletonEnemy->StartComboAttackMovement();
		bIsMoving = true;
		

	}
	else
	{
	}
}

void UCSkeletonComboAttackMovementState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	// 이동 중이 아니면 리턴
	if (!bIsMoving)
		return;

	// 메시 컴포넌트의 소유자가 스켈레톤 에너미인지 확인
	if (ACSkeletonEnemy* SkeletonEnemy = Cast<ACSkeletonEnemy>(MeshComp->GetOwner()))
	{
		// 콤보 공격 이동 업데이트
		SkeletonEnemy->UpdateComboAttackMovement(FrameDeltaTime, MovementSpeed, MovementDistance);
	}
}

void UCSkeletonComboAttackMovementState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	// 메시 컴포넌트의 소유자가 스켈레톤 에너미인지 확인
	if (ACSkeletonEnemy* SkeletonEnemy = Cast<ACSkeletonEnemy>(MeshComp->GetOwner()))
	{
		// 콤보 공격 이동 종료
		SkeletonEnemy->EndComboAttackMovement();
		bIsMoving = false;
		

	}
}

