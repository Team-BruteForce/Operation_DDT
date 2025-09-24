// Fill out your copyright notice in the Description page of Project Settings.


#include "ODH/ODH_Enemy/FlowerEnemy/Notify/CFlowerRangedAttackNotify.h"
#include "ODH/ODH_Enemy/FlowerEnemy/CFlowerEnemy.h"
#include "../../Engine/Classes/Kismet/GameplayStatics.h"
#include "ODH/ODH_AIController/CFlowerEnemyAIController.h"
#include "../../AIModule/Classes/BehaviorTree/BlackboardComponent.h"

UCFlowerRangedAttackNotify::UCFlowerRangedAttackNotify()
{

}

void UCFlowerRangedAttackNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	// �޽� ������Ʈ�� �����ڰ� �ö�� ���ʹ����� Ȯ��
	if (ACFlowerEnemy* FlowerEnemy = Cast<ACFlowerEnemy>(MeshComp->GetOwner()))
	{
		ACFlowerEnemyAIController* FlowerAIController = Cast<ACFlowerEnemyAIController>(FlowerEnemy->GetController());
		if (FlowerAIController)
		{
			UBlackboardComponent* BlackboardComp = FlowerAIController->GetBlackboardComponent();
			if (BlackboardComp)
			{
				// ���� ������ �� ��������
				UObject* BlackboardObject = BlackboardComp->GetValueAsObject("TargetPlayer");
				if (BlackboardObject)
				{
					FlowerEnemy->SpawnRangedProjectile(Cast<AActor>(BlackboardObject));
				}
			}
		}

		FlowerEnemy->SpawnRangedProjectile(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));


	}
}
