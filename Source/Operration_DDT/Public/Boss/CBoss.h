// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "CBoss.generated.h"

/**
 * @brief 보스 캐릭터 클래스
 * 
 * 게임의 보스 캐릭터를 정의하는 클래스입니다.
 * 스켈레탈 메시, 애니메이션, AI 컨트롤러를 설정합니다.
 */
UCLASS()
class OPERRATION_DDT_API ACBoss : public ACharacter
{
	GENERATED_BODY()

public:
	/**
	 * @brief 생성자
	 * 
	 * 보스 캐릭터의 기본 설정을 초기화합니다.
	 * - 스켈레탈 메시 설정
	 * - 애니메이션 인스턴스 클래스 설정
	 * - AI 컨트롤러 클래스 설정
	 */
	ACBoss();

	/**
	 * @brief 공격 애니메이션을 재생하는 테스트 함수
	 * 
	 * @param StateTag 재생할 공격 애니메이션을 식별하는 게임플레이 태그
	 * 
	 * 지정된 StateTag에 해당하는 공격 애니메이션 몽타주를 재생합니다.
	 * 현재는 테스트 목적으로 사용되며, 향후 실제 공격 시스템으로 확장될 예정입니다.
	 */
	UFUNCTION()
	void AttackTest(FGameplayTag StateTag);

	
	
	/**
	 * @brief 공격 애니메이션 몽타주 맵
	 * 
	 * 게임플레이 태그와 공격 애니메이션 몽타주를 매핑하는 맵입니다.
	 * 에디터에서 수정 가능하며, 다양한 공격 패턴에 대한 애니메이션을 관리합니다.
	 */
	UPROPERTY(EditAnywhere, Category = "Montage")
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> AttackOptions;
	UPROPERTY(EditAnywhere, Category = "Parameter")
	TArray<FGameplayTag> AttackTags;
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "State")
	class UBossStateComponent* BossStateComponent;

	UPROPERTY(EditAnywhere, Category = "State")
	class UCBossMovementComponent* BossMovementComponent;
	
protected:
	/**
	 * @brief 게임 시작 또는 스폰 시 호출되는 함수
	 * 
	 * 캐릭터가 게임에 스폰될 때 초기화 작업을 수행합니다.
	 */
	virtual void BeginPlay() override;



};
