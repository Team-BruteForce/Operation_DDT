#include "ODH/ODH_Enemy/SkeletonEnemy/Notify/CSkeletonOnDashAttackCollision.h"
#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"
#include "Engine/Engine.h"

UCSkeletonOnDashAttackCollision::UCSkeletonOnDashAttackCollision()
{
	// 기본 생성자
}

void UCSkeletonOnDashAttackCollision::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 메시 컴포넌트의 소유자가 스켈레톤 에너미인지 확인
	if (ACSkeletonEnemy* SkeletonEnemy = Cast<ACSkeletonEnemy>(MeshComp->GetOwner()))
	{
		// 돌진 공격 콜리전 활성화
		SkeletonEnemy->EnableDashCollision();
	}
	else
	{
		// 디버그 메시지 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("CSkeletonOnDashAttackCollision: SkeletonEnemy not found!"));
		}
	}
}



