// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/ODH_Enemy/SkeletonEnemy/Notify/CSkeletonOnComboAttackLColli.h"
#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"

UCSkeletonOnComboAttackLColli::UCSkeletonOnComboAttackLColli()
{

}

void UCSkeletonOnComboAttackLColli::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// 메시 컴포넌트의 소유자가 스켈레톤 에너미인지 확인
	if (ACSkeletonEnemy* SkeletonEnemy = Cast<ACSkeletonEnemy>(MeshComp->GetOwner()))
	{
		// 콤보 공격 완료 알림
		SkeletonEnemy->EnableComboLCollision();

		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Enable Combo L Collision"));
		}
	}
	else
	{
		// 디버그 메시지 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("SkeletonOnComboAttackLColliNotify: SkeletonEnemy not found!"));
		}
	}
}	
