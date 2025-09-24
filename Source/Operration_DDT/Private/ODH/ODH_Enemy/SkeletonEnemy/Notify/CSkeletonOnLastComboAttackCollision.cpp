#include "ODH/ODH_Enemy/SkeletonEnemy/Notify/CSkeletonOnLastComboAttackCollision.h"
#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"
#include "Engine/Engine.h"

UCSkeletonOnLastComboAttackCollision::UCSkeletonOnLastComboAttackCollision()
{
	// 기본 생성자
}

void UCSkeletonOnLastComboAttackCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 메시 컴포넌트의 소유자가 스켈레톤 에너미인지 확인
	if (ACSkeletonEnemy* SkeletonEnemy = Cast<ACSkeletonEnemy>(MeshComp->GetOwner()))
	{
		// 마지막 콤보 공격 콜리전 활성화
		SkeletonEnemy->EnableLastComboCollision();
	}
	else
	{
		
	}
}
