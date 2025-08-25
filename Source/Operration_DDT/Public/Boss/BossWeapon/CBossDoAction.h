/**
 * @file CBossDoAction.h
 * @brief 보스 액션 실행 클래스 헤더 파일
 * 
 * @details
 * 보스의 액션을 실행하는 클래스를 정의합니다.
 * 무기 액션, 충돌 처리, 오버랩 처리 등을 담당합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "CBossWeaponStuctures.h"
#include "GameplayTagContainer.h"
#include "CBossDoAction.generated.h"

/**
 * @brief 보스 액션 실행 클래스
 * 
 * 보스의 액션을 실행하는 클래스입니다.
 * 무기 액션, 충돌 처리, 오버랩 처리 등을 담당합니다.
 */
UCLASS()
class OPERRATION_DDT_API UCBossDoAction : public UObject
{
	GENERATED_BODY()
public:
	// UCBossDoAction();

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
	virtual void BeginPlay (
		class ACBossWeapon* InAttachment ,
		class UCBossEquipment* InEquipment ,
		class ACharacter* InOwner ,
		class TMap<FGameplayTag, FBossMeleeLightAttack>& InDoActionData,
		class TMap<FGameplayTag,FBossHitData>& InHitData
	);

public:
	/**
	 * @brief 액션 실행
	 * 
	 * @param Tag 실행할 액션 태그
	 * 
	 * 지정된 태그에 해당하는 액션을 실행합니다.
	 */
	virtual void DoAction (FGameplayTag Tag);
	
	/**
	 * @brief 액션 시작
	 * 
	 * 액션 실행을 시작합니다.
	 */
	virtual void Begin_DoAction ( );
	
	/**
	 * @brief 액션 종료
	 * 
	 * 액션 실행을 종료합니다.
	 */
	virtual void End_DoAction ( );

public:
	/**
	 * @brief 첨부물 충돌 시작 이벤트
	 */
	UFUNCTION()
		virtual void OnAttachmentBeginCollision() { }

	/**
	 * @brief 첨부물 충돌 종료 이벤트
	 */
	UFUNCTION()
		virtual void OnAttachmentEndCollision() { }

	/**
	 * @brief 첨부물 오버랩 시작 이벤트
	 * 
	 * @param InAttacker 공격자 캐릭터
	 * @param InAttackCuaser 공격 원인 액터
	 * @param InOther 대상 캐릭터
	 */
	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCuaser, class ACharacter* InOther) { }

	/**
	 * @brief 첨부물 오버랩 종료 이벤트
	 * 
	 * @param InAttacker 공격자 캐릭터
	 * @param InOther 대상 캐릭터
	 */
	UFUNCTION()
		virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) { }

protected:
	/**
	 * @brief 액션 시작 여부
	 */
	bool bBeginAction;
	
	/**
	 * @brief 소유자 캐릭터
	 */
	UPROPERTY()
	class ACharacter* OwnerCharacter;
	
	/**
	 * @brief 월드 참조
	 */
	UPROPERTY()
	class UWorld* World;

	/**
	 * @brief 이동 컴포넌트
	 */
	UPROPERTY()
	class UCBossMovementComponent* Movement;
	
	/**
	 * @brief 상태 컴포넌트
	 */
	UPROPERTY()
	class UBossStateComponent* State;

	/**
	 * @brief 보스 액션 데이터 맵
	 */
	TMap<FGameplayTag,FBossMeleeLightAttack> BossDoActionDatas;
	
	/**
	 * @brief 보스 히트 데이터 맵
	 */
	TMap<FGameplayTag,FBossHitData> BossHitDatas;
};
