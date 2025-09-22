#include "ODH/ODH_Enemy/FlowerEnemy/Notify/CFlowerComboAttackEndNotify.h"
#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemy.h"
#include "Engine/Engine.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UCFlowerComboAttackEndNotify::UCFlowerComboAttackEndNotify()
{
	// 기본 생성자
}

void UCFlowerComboAttackEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 메시 컴포넌트의 소유자가 플라워 에너미인지 확인
	if (ACFlowerEnemy* FlowerEnemy = Cast<ACFlowerEnemy>(MeshComp->GetOwner()))
	{
		// 콤보 공격 완료 알림
		FlowerEnemy->NotifyComboAttackCompleted();
		
		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Flower Combo Attack Completed - Blackboard Updated"));
		}
	}
	else
	{
		// 디버그 메시지 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("CFlowerComboAttackEndNotify: FlowerEnemy not found!"));
		}
	}
}
