// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file CBossEndAttack.cpp
 * @brief 보스 공격 종료 애니메이션 노티파이 구현 파일
 * 
 * 이 파일은 보스 캐릭터의 공격 애니메이션이 끝날 때 호출되는 애니메이션 노티파이를 구현합니다.
 * 몽타주 재생이 완료되면 StateTree에 이벤트를 보내어 다음 상태로 전환할 수 있도록 합니다.
 * 
 * 주요 기능:
 * - 공격 애니메이션 완료 감지
 * - StateTree 이벤트 전송 (Action.NextAttack 태그)
 * - AI 컨트롤러와 StateTree 컴포넌트 연동
 */

#include "Enemy/Notifies/CBossEndAttack.h"
#include "AIController.h"
#include "Components/StateTreeComponent.h"

FString UCBossEndAttack::GetNotifyName_Implementation() const
{
	return "BossEndAttack";
}

void UCBossEndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	FGameplayTag EventTag = FGameplayTag::RequestGameplayTag(FName("ACTION.NextAttack"));

	// 1. 이 애니메이션을 재생한 '몸'(Pawn)을 찾습니다.
	APawn* OwnerPawn = Cast<APawn>(MeshComp->GetOwner());
	if (!OwnerPawn) return;

	// 2. 그 '몸'을 조종하는 '뇌'(AIController)를 찾습니다.
	AAIController* AIController = Cast<AAIController>(OwnerPawn->GetController());
	if (!AIController) return;

	// 3. '뇌'에 붙어있는 'State Tree 신경계'(StateTreeComponent)를 찾습니다.
	UStateTreeComponent* StateTreeComp = AIController->FindComponentByClass<UStateTreeComponent>();
	if (!StateTreeComp) return;
	
	StateTreeComp->SendStateTreeEvent(EventTag);

	// (디버깅용) 어떤 태그가 보내졌는지 로그로 확인
	UE_LOG(LogTemp, Log, TEXT("AnimNotify sent StateTreeEvent: %s"), *EventTag.ToString());

}
