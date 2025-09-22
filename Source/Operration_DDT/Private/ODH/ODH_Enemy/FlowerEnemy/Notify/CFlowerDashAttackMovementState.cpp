#include "ODH/ODH_Enemy/FlowerEnemy/Notify/CFlowerDashAttackMovementState.h"
#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemy.h"
#include "Engine/Engine.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

UCFlowerDashAttackMovementState::UCFlowerDashAttackMovementState()
{
	// 기본 생성자
}

void UCFlowerDashAttackMovementState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// 메시 컴포넌트의 소유자가 FlowerEnemy인지 확인
	if (ACFlowerEnemy* FlowerEnemy = Cast<ACFlowerEnemy>(MeshComp->GetOwner()))
	{
		// 플레이어 위치를 저장하고 이동 시작
		FlowerEnemy->StartDashMovementToPlayer();
		bIsMoving = true;
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Flower Enemy Dash Movement Started - Player Position Captured"));
		}
	}
	else
	{
		// 디버그 메시지 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("CFlowerDashAttackMovementState: FlowerEnemy not found!"));
		}
	}
}

void UCFlowerDashAttackMovementState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	// 이동 중이 아니면 리턴
	if (!bIsMoving)
		return;

	// 메시 컴포넌트의 소유자가 FlowerEnemy인지 확인
	if (ACFlowerEnemy* FlowerEnemy = Cast<ACFlowerEnemy>(MeshComp->GetOwner()))
	{
		// 저장된 플레이어 위치를 향해 이동
		FlowerEnemy->UpdateDashMovementToPlayer(FrameDeltaTime, MovementSpeed);
	}
}

void UCFlowerDashAttackMovementState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	// 메시 컴포넌트의 소유자가 FlowerEnemy인지 확인
	if (ACFlowerEnemy* FlowerEnemy = Cast<ACFlowerEnemy>(MeshComp->GetOwner()))
	{
		// 이동 종료
		FlowerEnemy->EndDashMovementToPlayer();
		bIsMoving = false;
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Orange, TEXT("Flower Enemy Dash Movement Ended"));
		}
	}
}


