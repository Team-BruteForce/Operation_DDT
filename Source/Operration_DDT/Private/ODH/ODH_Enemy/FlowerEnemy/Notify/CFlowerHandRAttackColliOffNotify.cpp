// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/ODH_Enemy/FlowerEnemy/Notify/CFlowerHandRAttackColliOffNotify.h"
#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemy.h"

UCFlowerHandRAttackColliOffNotify::UCFlowerHandRAttackColliOffNotify()
{

}

void UCFlowerHandRAttackColliOffNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// �޽� ������Ʈ�� �����ڰ� �ö�� ���ʹ����� Ȯ��
	if (ACFlowerEnemy* FlowerEnemy = Cast<ACFlowerEnemy>(MeshComp->GetOwner()))
	{
		FlowerEnemy->DisableHandRAttackCollision();

	}
}
