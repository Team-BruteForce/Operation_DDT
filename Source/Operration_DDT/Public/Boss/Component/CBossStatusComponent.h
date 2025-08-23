// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file CBossStatusComponent.h
 * @brief 보스 스탯 관리 컴포넌트 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 보스 캐릭터의 스탯(체력, 공격력, 방어력 등)을 관리하는 컴포넌트를 정의합니다.
 * 데이터 테이블에서 기본 스탯을 로드하고, 런타임에서 현재 스탯을 추적합니다.
 * 
 * @section architecture 아키텍처
 * - UCBossStatusComponent: 보스 스탯 관리 메인 컴포넌트
 * - FBossCurrentStats: 현재 스탯 구조체
 * - 데이터 테이블 연동 시스템
 * 
 * @section features 주요 기능
 * - 보스 스탯 초기화 및 관리
 * - 데이터 테이블에서 기본 스탯 로드
 * - 현재 스탯과 최대 스탯 분리 관리
 * - 스탯 변경 이벤트 처리
 * - 버프/디버프 시스템 지원
 * 
 * @section stat_system 스탯 시스템
 * - HP (체력): 최대/현재 체력 관리
 * - AP (액션 포인트): 최대/현재 액션 포인트 관리
 * - ATK (공격력): 기본/현재 공격력 관리
 * - DEF (방어력): 기본/현재 방어력 관리
 * - Speed (속도): 최대/현재 속도 관리
 * 
 * @author [개발자명]
 * @date [작성일]
 * @version 1.0
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBossStatusComponent.generated.h"

/**
 * @brief 보스 현재 스탯 구조체
 * 
 * @details
 * 보스의 현재 스탯 정보를 저장하는 구조체입니다.
 * 최대값과 현재값을 분리하여 관리하여 버프/디버프 시스템을 지원합니다.
 * 
 * @section stat_types 스탯 타입
 * - HP (체력): 생명력 관리
 * - AP (액션 포인트): 행동 가능한 포인트
 * - ATK (공격력): 데미지 계산에 사용
 * - DEF (방어력): 받는 데미지 감소
 * - Speed (속도): 이동 및 행동 속도
 * 
 * @section design_patterns 설계 패턴
 * - 분리 패턴: 최대값과 현재값 분리
 * - 불변성 패턴: 기본값은 변경되지 않음
 * - 추적 패턴: 현재값만 런타임에 변경
 * 
 * @section usage 사용법
 * - 데이터 테이블에서 기본값 로드
 * - 런타임에서 현재값만 수정
 * - 버프/디버프 적용 시 현재값만 변경
 * - 최대값은 참조용으로 사용
 */
USTRUCT()
struct FBossCurrentStats
{
	GENERATED_BODY()
public:
	UPROPERTY()
	int32 MaxHP; ///< 최대 체력 (기본값, 변경되지 않음)

	UPROPERTY()
	int32 CurrentHP; ///< 현재 체력 (런타임에 변경됨)

	UPROPERTY()
	int32 MaxAP; ///< 최대 액션 포인트 (기본값, 변경되지 않음)

	UPROPERTY()
	int32 CurrentAP; ///< 현재 액션 포인트 (런타임에 변경됨)

	UPROPERTY()
	int32 ConstATK; ///< 기본 공격력 (기본값, 변경되지 않음)

	UPROPERTY()
	int32 CurrentATK; ///< 현재 공격력 (런타임에 변경됨)

	UPROPERTY()
	int32 ConstDEF; ///< 기본 방어력 (기본값, 변경되지 않음)

	UPROPERTY()
	int32 CurrentDEF; ///< 현재 방어력 (런타임에 변경됨)

	UPROPERTY()
	int32 MaxSpeed; ///< 최대 속도 (기본값, 변경되지 않음)

	UPROPERTY()
	int32 CurrentSpeed; ///< 현재 속도 (런타임에 변경됨)
};

/**
 * @brief 보스 스탯 관리 컴포넌트 클래스
 * 
 * @details
 * 보스 캐릭터의 스탯을 관리하는 컴포넌트입니다.
 * 데이터 테이블에서 기본 스탯을 로드하고, 런타임에서 현재 스탯을 추적합니다.
 * 
 * @section design_patterns 설계 패턴
 * - 컴포넌트 패턴: 언리얼 엔진 컴포넌트 시스템 활용
 * - 옵저버 패턴: 스탯 변경 시 이벤트 발생
 * - 팩토리 패턴: 데이터 테이블에서 스탯 생성
 * - 싱글톤 패턴: 보스당 하나의 스탯 컴포넌트
 * 
 * @section responsibilities 책임
 * - 스탯 초기화 및 로드
 * - 현재 스탯 추적 및 업데이트
 * - 스탯 변경 이벤트 처리
 * - 버프/디버프 시스템 관리
 * - 스탯 UI 연동
 * 
 * @section integration 연동 시스템
 * - 데이터 테이블: 기본 스탯 로드
 * - UI 시스템: 스탯 표시
 * - 게임플레이 시스템: 스탯 활용
 * - 이벤트 시스템: 스탯 변경 알림
 * 
 * @section usage 사용법
 * 1. 보스 블루프린트에 컴포넌트 추가
 * 2. 데이터 테이블 설정
 * 3. BeginPlay에서 스탯 초기화
 * 4. 런타임에서 스탯 수정
 * 5. UI에서 스탯 표시
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCBossStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/**
	 * @brief 생성자
	 * 
	 * @details
	 * 컴포넌트의 기본 설정을 초기화합니다.
	 * 기본 스탯 값들을 설정하고 컴포넌트를 활성화합니다.
	 * 
	 * @section initialization 초기화 과정
	 * 1. 부모 클래스 초기화
	 * 2. 기본 스탯 값 설정
	 * 3. 컴포넌트 활성화
	 * 4. 틱 함수 활성화
	 */
	UCBossStatusComponent();

protected:
	/**
	 * @brief 게임 시작 시 호출되는 함수
	 * 
	 * @details
	 * 컴포넌트가 게임에 참여할 때 초기화 작업을 수행합니다.
	 * 데이터 테이블에서 기본 스탯을 로드하고 현재 스탯을 설정합니다.
	 * 
	 * @section initialization 초기화 과정
	 * 1. 부모 클래스 BeginPlay 호출
	 * 2. 데이터 테이블 유효성 검사
	 * 3. 기본 스탯 로드
	 * 4. 현재 스탯을 최대값으로 초기화
	 * 5. 스탯 초기화 완료 이벤트 발생
	 */
	virtual void BeginPlay() override;

public:	
	/**
	 * @brief 매 프레임 호출되는 틱 함수
	 * 
	 * @param DeltaTime 프레임 간 시간 간격
	 * @param TickType 틱 타입
	 * @param ThisTickFunction 틱 함수 정보
	 * 
	 * @details
	 * 매 프레임마다 스탯 관련 업데이트를 수행합니다.
	 * 스탯 재생, 버프/디버프 지속시간 관리 등을 담당합니다.
	 * 
	 * @section update_process 업데이트 과정
	 * 1. 부모 클래스 틱 함수 호출
	 * 2. 스탯 재생 처리 (HP, AP 등)
	 * 3. 버프/디버프 지속시간 감소
	 * 4. 만료된 버프/디버프 제거
	 * 5. 스탯 변경 이벤트 발생
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief 보스 스탯 데이터 테이블
	 * 
	 * @details
	 * 보스의 기본 스탯 정보가 저장된 데이터 테이블입니다.
	 * 에디터에서 설정하며, 게임 시작 시 이 테이블에서 기본 스탯을 로드합니다.
	 * 
	 * @section data_structure 데이터 구조
	 * - 보스 이름을 키로 사용
	 * - 각 보스별 기본 스탯 정보 저장
	 * - 런타임에 동적으로 로드 가능
	 * 
	 * @section usage 사용법
	 * - 에디터에서 데이터 테이블 할당
	 * - BeginPlay에서 기본 스탯 로드
	 * - 런타임에서 스탯 참조
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
	UDataTable* BossStatsTable;

	/**
	 * @brief 보스 현재 스탯
	 * 
	 * @details
	 * 보스의 현재 스탯 정보를 저장하는 구조체입니다.
	 * 최대값과 현재값을 모두 포함하며, 런타임에 변경됩니다.
	 * 
	 * @section stat_management 스탯 관리
	 * - MaxHP/CurrentHP: 체력 관리
	 * - MaxAP/CurrentAP: 액션 포인트 관리
	 * - ConstATK/CurrentATK: 공격력 관리
	 * - ConstDEF/CurrentDEF: 방어력 관리
	 * - MaxSpeed/CurrentSpeed: 속도 관리
	 * 
	 * @section runtime_usage 런타임 사용법
	 * - 현재값만 수정 (CurrentHP, CurrentAP 등)
	 * - 최대값은 참조용 (MaxHP, MaxAP 등)
	 * - 버프/디버프 적용 시 현재값만 변경
	 */
	UPROPERTY(EditAnywhere)
	FBossCurrentStats BossCurrentStats;
};
