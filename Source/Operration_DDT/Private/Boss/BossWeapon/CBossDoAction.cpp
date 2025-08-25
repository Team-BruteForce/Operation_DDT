/**
 * @file CBossDoAction.cpp
 * @brief 보스 액션 실행 클래스 구현 파일
 * 
 * @details
 * 보스의 액션을 실행하는 클래스의 구현 파일입니다.
 * 무기 액션, 충돌 처리, 오버랩 처리 등을 담당합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#include "Boss/BossWeapon/CBossDoAction.h"
#include "Global.h"
#include "Boss/Component/BossStateComponent.h"
#include "GameFramework/Character.h"

/**
 * @brief 게임 시작 시 호출되는 함수
 * 
 * @param InAttachment 보스 무기 참조
 * @param InEquipment 보스 장비 참조
 * @param InOwner 소유자 캐릭터
 * @param InDoActionData 액션 데이터 맵
 * @param InHitData 히트 데이터 맵
 * 
 * 액션 실행에 필요한 초기화를 수행합니다.
 */
void UCBossDoAction::BeginPlay(class ACBossWeapon* InAttachment, class UCBossEquipment* InEquipment,
	class ACharacter* InOwner, TMap<FGameplayTag, FBossMeleeLightAttack>& InDoActionData,
	TMap<FGameplayTag, FBossHitData>& InHitData)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld ( );

	// State = CHelpers::GetComponent<UC> ( InOwner );
	Movement = CHelpers::GetComponent<UCBossMovementComponent> ( InOwner );

	BossDoActionDatas = InDoActionData;
	BossHitDatas = InHitData;
}

/**
 * @brief 액션 실행
 * 
 * @param Tag 실행할 액션 태그
 * 
 * 지정된 태그에 해당하는 액션을 실행합니다.
 */
void UCBossDoAction::DoAction (FGameplayTag Tag)
{
	BossDoActionDatas[Tag].PlayPaseOneMontage(OwnerCharacter);
}

/**
 * @brief 액션 시작
 * 
 * 액션 실행을 시작합니다.
 */
void UCBossDoAction::Begin_DoAction ( )
{
	// bBeginAction = true;
}

/**
 * @brief 액션 종료
 * 
 * 액션 실행을 종료합니다.
 */
void UCBossDoAction::End_DoAction ( )
{
	// bBeginAction = false;

	// State->SetIdleMode ( );

}