// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/ODH_Enemy/SkeletonEnemy/Notify/CSkeletonOnComboAttackLColli.h"
#include "ODH/ODH_Enemy/SkeletonEnemy/CSkeletonEnemy.h"

UCSkeletonOnComboAttackLColli::UCSkeletonOnComboAttackLColli()
{

}

void UCSkeletonOnComboAttackLColli::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// �޽� ������Ʈ�� �����ڰ� ���̷��� ���ʹ����� Ȯ��
	if (ACSkeletonEnemy* SkeletonEnemy = Cast<ACSkeletonEnemy>(MeshComp->GetOwner()))
	{
		// �޺� ���� �Ϸ� �˸�
		SkeletonEnemy->EnableComboLCollision();


	}
	else
	{
	}
}	
