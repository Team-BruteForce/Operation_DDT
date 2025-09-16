#include "ODH/ODH_Enemy/SkeletonEnemy/Notify/CSkeletonDashAttackMovementState.h"
#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UCSkeletonDashAttackMovementState::UCSkeletonDashAttackMovementState()
{
	// 기본 생성자
}

void UCSkeletonDashAttackMovementState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// 메시 컴포넌트의 소유자가 스켈레톤 에너미인지 확인
	if (ACSkeletonEnemy* SkeletonEnemy = Cast<ACSkeletonEnemy>(MeshComp->GetOwner()))
	{
		// 플레이어 위치를 저장하고 이동 시작
		SkeletonEnemy->StartDashMovementToPlayer();
		bIsMoving = true;
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Dash Movement Started - Player Position Captured"));
		}
	}
	else
	{
		// 디버그 메시지 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("CSkeletonDashAttackMovementState: SkeletonEnemy not found!"));
		}
	}
}

void UCSkeletonDashAttackMovementState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	// 이동 중이 아니면 리턴
	if (!bIsMoving)
		return;

	// 메시 컴포넌트의 소유자가 스켈레톤 에너미인지 확인
	if (ACSkeletonEnemy* SkeletonEnemy = Cast<ACSkeletonEnemy>(MeshComp->GetOwner()))
	{
		// 저장된 플레이어 위치를 향해 이동
		SkeletonEnemy->UpdateDashMovementToPlayer(FrameDeltaTime, MovementSpeed);
	}
}

void UCSkeletonDashAttackMovementState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	// 메시 컴포넌트의 소유자가 스켈레톤 에너미인지 확인
	if (ACSkeletonEnemy* SkeletonEnemy = Cast<ACSkeletonEnemy>(MeshComp->GetOwner()))
	{
		// 이동 종료
		SkeletonEnemy->EndDashMovementToPlayer();
		bIsMoving = false;
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Dash Movement Ended"));
		}
	}
}
