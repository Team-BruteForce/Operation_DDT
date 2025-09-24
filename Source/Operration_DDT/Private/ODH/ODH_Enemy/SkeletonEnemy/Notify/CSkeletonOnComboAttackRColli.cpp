// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/ODH_Enemy/SkeletonEnemy/Notify/CSkeletonOnComboAttackRColli.h"
#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"

UCSkeletonOnComboAttackRColli::UCSkeletonOnComboAttackRColli()
{

}

void UCSkeletonOnComboAttackRColli::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// �޽� ������Ʈ�� �����ڰ� ���̷��� ���ʹ����� Ȯ��
	if (ACSkeletonEnemy* SkeletonEnemy = Cast<ACSkeletonEnemy>(MeshComp->GetOwner()))
	{
		// �޺� ���� �Ϸ� �˸�
		SkeletonEnemy->EnableComboRCollision();


	}
	else
	{
	}
}
