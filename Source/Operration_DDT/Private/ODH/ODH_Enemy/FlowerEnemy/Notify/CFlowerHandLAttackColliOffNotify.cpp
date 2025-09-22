// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/ODH_Enemy/FlowerEnemy/Notify/CFlowerHandLAttackColliOffNotify.h"
#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemy.h"

UCFlowerHandLAttackColliOffNotify::UCFlowerHandLAttackColliOffNotify()
{

}

void UCFlowerHandLAttackColliOffNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 메시 컴포넌트의 소유자가 플라워 에너미인지 확인
	if (ACFlowerEnemy* FlowerEnemy = Cast<ACFlowerEnemy>(MeshComp->GetOwner()))
	{
		FlowerEnemy->DisableHandLAttackCollision();

		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Hand L Attack Collision Off"));
		}
	else
	{
		// 디버그 메시지 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("CSkeletonDashAttackEndNotify: FlowerEnemy not found!"));
		}
	}
	}
}
