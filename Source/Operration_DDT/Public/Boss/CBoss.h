// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file CBoss.h
 * @brief 보스 캐릭터 메인 클래스 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 게임의 보스 캐릭터를 정의하는 핵심 클래스입니다.
 * 보스의 기본 구조, 컴포넌트, 그리고 주요 기능들을 선언합니다.
 * 
 * @section architecture 아키텍처
 * - ACBoss: 보스 캐릭터의 메인 클래스
 * - 컴포넌트 기반 설계로 모듈화된 기능 분리
 * - StateTree 기반 AI 시스템과 연동
 * 
 * @section components 주요 컴포넌트
 * - BossStateComponent: 보스의 상태 관리
 * - BossMovementComponent: 보스의 이동 로직
 * - BossWeaponComponent: 보스의 무기 시스템
 * - BossStatusComponent: 보스의 스탯 관리
 * 
 * @section features 주요 기능
 * - 애니메이션 몽타주 시스템
 * - 게임플레이 태그 기반 공격 시스템
 * - 컴포넌트 기반 모듈화
 * - AI 컨트롤러 연동
 * 
 * @author [개발자명]
 * @date [작성일]
 * @version 1.0
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameplayTagContainer.h"
#include "CBoss.generated.h"

/**
 * @brief 보스 캐릭터 클래스
 * 
 * @details
 * 게임의 보스 캐릭터를 정의하는 클래스입니다.
 * 스켈레탈 메시, 애니메이션, AI 컨트롤러를 설정합니다.
 * 
 * @section design_patterns 설계 패턴
 * - 컴포넌트 기반 설계: 각 기능을 독립적인 컴포넌트로 분리
 * - StateTree 패턴: AI 행동을 StateTree로 관리
 * - 태그 기반 시스템: GameplayTag를 사용한 유연한 시스템
 * 
 * @section inheritance 상속 구조
 * - ACharacter: 언리얼 엔진 기본 캐릭터 클래스 상속
 * - 컴포넌트 시스템: 다양한 기능을 컴포넌트로 분리
 * 
 * @section usage 사용법
 * 1. 보스 블루프린트에서 이 클래스를 상속
 * 2. 필요한 컴포넌트들을 에디터에서 설정
 * 3. StateTree를 통해 AI 행동 정의
 * 4. GameplayTag를 사용해 공격 패턴 관리
 */
UCLASS()
class OPERRATION_DDT_API ACBoss : public ACharacter
{
	GENERATED_BODY()

public:
	/**
	 * @brief 생성자
	 * 
	 * @details
	 * 보스 캐릭터의 기본 설정을 초기화합니다.
	 * - 스켈레탈 메시 설정
	 * - 애니메이션 인스턴스 클래스 설정
	 * - AI 컨트롤러 클래스 설정
	 * 
	 * @section initialization 초기화 과정
	 * 1. 기본 캐릭터 설정
	 * 2. 스켈레탈 메시 컴포넌트 설정
	 * 3. 애니메이션 블루프린트 설정
	 * 4. AI 컨트롤러 클래스 설정
	 */
	ACBoss();

	/**
	 * @brief 공격 애니메이션을 재생하는 테스트 함수
	 * 
	 * @param StateTag 재생할 공격 애니메이션을 식별하는 게임플레이 태그
	 * 
	 * @details
	 * 지정된 StateTag에 해당하는 공격 애니메이션 몽타주를 재생합니다.
	 * 현재는 테스트 목적으로 사용되며, 향후 실제 공격 시스템으로 확장될 예정입니다.
	 * 
	 * @section implementation 구현 세부사항
	 * - AttackOptions 맵에서 StateTag에 해당하는 몽타주 검색
	 * - 몽타주가 존재하면 재생
	 * - 디버그 로그 출력
	 * 
	 * @section future_plans 향후 계획
	 * - 실제 공격 시스템과 연동
	 * - 데미지 계산 시스템 추가
	 * - 히트박스 시스템 구현
	 */
	UFUNCTION()
	void AttackTest(FGameplayTag StateTag);

	/**
	 * @brief 공격 애니메이션 몽타주 맵
	 * 
	 * @details
	 * 게임플레이 태그와 공격 애니메이션 몽타주를 매핑하는 맵입니다.
	 * 에디터에서 수정 가능하며, 다양한 공격 패턴에 대한 애니메이션을 관리합니다.
	 * 
	 * @section usage 사용법
	 * - 에디터에서 태그와 몽타주를 매핑
	 * - 런타임에서 StateTag로 몽타주 검색
	 * - 동적으로 공격 패턴 추가/수정 가능
	 * 
	 * @section benefits 장점
	 * - 유연한 공격 시스템
	 * - 런타임 수정 가능
	 * - 태그 기반 관리로 확장성 향상
	 */
	UPROPERTY(EditAnywhere, Category = "Montage")
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> AttackOptions;

	/**
	 * @brief 보스 상태 관리 컴포넌트
	 * 
	 * @details
	 * 보스의 현재 상태와 상태 전환을 관리하는 컴포넌트입니다.
	 * StateTree와 연동하여 AI 행동을 제어합니다.
	 * 
	 * @section responsibilities 책임
	 * - 현재 상태 추적
	 * - 상태 전환 조건 관리
	 * - StateTree 연동
	 * - 상태별 행동 정의
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	class UBossStateComponent* BossStateComponent;

	/**
	 * @brief 보스 이동 관리 컴포넌트
	 * 
	 * @details
	 * 보스의 이동 로직을 담당하는 컴포넌트입니다.
	 * 다양한 이동 패턴과 네비게이션을 관리합니다.
	 * 
	 * @section responsibilities 책임
	 * - 이동 패턴 정의
	 * - 네비게이션 시스템 연동
	 * - 이동 속도 및 가속도 관리
	 * - 장애물 회피 로직
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	class UCBossMovementComponent* BossMovementComponent;

	/**
	 * @brief 보스 무기 시스템 컴포넌트
	 * 
	 * @details
	 * 보스의 무기와 공격 시스템을 관리하는 컴포넌트입니다.
	 * 다양한 무기와 공격 패턴을 지원합니다.
	 * 
	 * @section responsibilities 책임
	 * - 무기 장착/해제
	 * - 공격 패턴 관리
	 * - 데미지 계산
	 * - 히트박스 관리
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	class UCBossWeaponComponent* BossWeaponComponent;

	/**
	 * @brief 보스 스탯 관리 컴포넌트
	 * 
	 * @details
	 * 보스의 체력, 공격력, 방어력 등의 스탯을 관리하는 컴포넌트입니다.
	 * 데이터 테이블과 연동하여 동적 스탯 관리가 가능합니다.
	 * 
	 * @section responsibilities 책임
	 * - 체력 관리
	 * - 공격력/방어력 관리
	 * - 스탯 버프/디버프
	 * - 데이터 테이블 연동
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	class UCBossStatusComponent* BossStatusComponent;

protected:
	/**
	 * @brief 게임 시작 또는 스폰 시 호출되는 함수
	 * 
	 * @details
	 * 캐릭터가 게임에 스폰될 때 초기화 작업을 수행합니다.
	 * 컴포넌트들의 초기화와 기본 설정을 담당합니다.
	 * 
	 * @section initialization 초기화 과정
	 * 1. 부모 클래스 초기화
	 * 2. 컴포넌트 초기화
	 * 3. 기본 스탯 설정
	 * 4. AI 시스템 초기화
	 */
	virtual void BeginPlay() override;
};
