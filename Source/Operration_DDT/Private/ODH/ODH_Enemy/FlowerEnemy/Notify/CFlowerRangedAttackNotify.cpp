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

	// 메시 컴포넌트의 소유자가 플라워 에너미인지 확인
	if (ACFlowerEnemy* FlowerEnemy = Cast<ACFlowerEnemy>(MeshComp->GetOwner()))
	{
		ACFlowerEnemyAIController* FlowerAIController = Cast<ACFlowerEnemyAIController>(FlowerEnemy->GetController());
		if (FlowerAIController)
		{
			UBlackboardComponent* BlackboardComp = FlowerAIController->GetBlackboardComponent();
			if (BlackboardComp)
			{
				// 현재 게이지 값 가져오기
				UObject* BlackboardObject = BlackboardComp->GetValueAsObject("TargetPlayer");
				if (BlackboardObject)
				{
					FlowerEnemy->SpawnRangedProjectile(Cast<AActor>(BlackboardObject));
				}
			}
		}

		FlowerEnemy->SpawnRangedProjectile(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

		// 디버그 출력
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Cyan, TEXT("Dash Attack Collision Off"));
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
