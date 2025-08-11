// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
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

	UFUNCTION()
	void AttackTest();
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="TestMontage")
	 class UAnimMontage* AttackMontage;

protected:
	/**
	 * @brief 게임 시작 또는 스폰 시 호출되는 함수
	 * 
	 * 캐릭터가 게임에 스폰될 때 초기화 작업을 수행합니다.
	 */
	virtual void BeginPlay() override;



};
