/**
 * @file CBossEndAttack.cpp
 * @brief 보스 공격 종료 노티파이 구현 파일
 * 
 * @details
 * 보스 캐릭터의 공격 애니메이션이 끝날 때 호출되는 애니메이션 노티파이의 구현 파일입니다.
 * 몽타주 재생이 완료되면 StateTree에 이벤트를 보내어 다음 상태로 전환할 수 있도록 합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#include "Boss/Notifies/CBossEndAttack.h"
#include "AIController.h"
#include "Components/StateTreeComponent.h"

/**
 * @brief 노티파이 이름 반환
 * 
 * @return 노티파이의 이름 문자열
 * 
 * 이 노티파이의 이름을 반환합니다.
 */
FString UCBossEndAttack::GetNotifyName_Implementation() const
{
	return "BossEndAttack";
}

/**
 * @brief 노티파이 실행 함수
 * 
 * @param MeshComp 스켈레탈 메시 컴포넌트
 * @param Animation 애니메이션 시퀀스
 * 
 * 공격 애니메이션이 완료되면 StateTree에 이벤트를 전송합니다.
 */
void UCBossEndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

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
