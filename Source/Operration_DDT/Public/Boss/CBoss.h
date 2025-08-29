/**
 * @file CBoss.h
 * @brief 보스 캐릭터 메인 클래스 헤더 파일
 * 
 * @details
 * 게임의 보스 캐릭터를 정의하는 핵심 클래스입니다.
 * 보스의 기본 구조, 컴포넌트, 그리고 주요 기능들을 선언합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "CBoss.generated.h"

/**
 * @brief 보스 캐릭터 클래스
 * 
 * 보스 캐릭터의 메인 클래스입니다.
 * 컴포넌트 기반 설계로 모듈화된 기능을 제공합니다.
 */
UCLASS()
class OPERRATION_DDT_API ACBoss : public ACharacter
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief 보스 캐릭터 생성자
	 */
	ACBoss();
	/**
	 *@brief 데미지 함수
	 */
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	

	/**
	 * @brief 공격 애니메이션 몽타주 맵
	 */
	UPROPERTY(EditAnywhere, Category = "Montage")
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> AttackOptions;

	/**
	 * @brief 보스 상태 컴포넌트
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	class UBossStateComponent* BossStateComponent;

	/**
	 * @brief 보스 이동 컴포넌트
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	class UCBossMovementComponent* BossMovementComponent;

	/**
	 * @brief 보스 무기 컴포넌트
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	class UCBossWeaponComponent* BossWeaponComponent;

	/**
	 * @brief 보스 스탯 컴포넌트
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	class UCBossStatusComponent* BossStatusComponent;
	
	/**
	 * @brief 보스 모션 워핑 컴포넌트
	 */
	UPROPERTY(EditAnywhere)
	class UMotionWarpingComponent* BossMotionWarping;

	/**
	 *@brief 보스 프로젝타일(투사체) 컴포넌트
	 */
	UPROPERTY(EditAnywhere)
	class UBossProjectileComponent* ProjectileComp;

	/**
 *@brief 보스 타겟 연산 관련 컴포넌트
 */
	UPROPERTY(EditAnywhere)
	class UCBossTargetingComponent* TargetingComp;

	/**
 *@brief 보스 디버그 컴포넌트
 */
	UPROPERTY(VisibleAnywhere)
	class UBossDebugComponent* DebugComp;
	

protected:
	
	/**
	 * @brief 게임 시작 시 호출되는 함수
	 */
	virtual void BeginPlay() override;
};
