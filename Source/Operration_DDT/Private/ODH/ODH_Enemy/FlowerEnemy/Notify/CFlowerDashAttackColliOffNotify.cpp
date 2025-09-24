// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/ODH_Enemy/FlowerEnemy/Notify/CFlowerDashAttackColliOffNotify.h"
#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemy.h"

UCFlowerDashAttackColliOffNotify::UCFlowerDashAttackColliOffNotify()
{

}

void UCFlowerDashAttackColliOffNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// �޽� ������Ʈ�� �����ڰ� �ö�� ���ʹ����� Ȯ��
	if (ACFlowerEnemy* FlowerEnemy = Cast<ACFlowerEnemy>(MeshComp->GetOwner()))
	{
		FlowerEnemy->DisableDashAttackCollision();


	}
}
