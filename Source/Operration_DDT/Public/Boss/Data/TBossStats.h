// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file TBossStats.h
 * @brief 보스 스탯 데이터 구조체 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 보스의 스탯 정보를 정의하는 데이터 구조체를 포함합니다.
 * 데이터 테이블에서 사용되며, 보스의 기본 능력치를 관리합니다.
 * 
 * @section features 주요 기능
 * - 보스 스탯 데이터 구조 정의
 * - 데이터 테이블 연동
 * - 기본값 설정
 * - 블루프린트 지원
 * 
 * @section stat_types 스탯 타입
 * - MaxHP: 최대 체력
 * - MaxAP: 최대 액션 포인트
 * - ATK: 공격력
 * - DEF: 방어력
 * - Speed: 속도
 * 
 * @author [개발자명]
 * @date [작성일]
 * @version 1.0
 */

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "TBossStats.generated.h"

/**
 * @brief 보스 스탯 데이터 구조체
 * 
 * @details
 * 보스의 스탯 정보를 저장하는 구조체입니다.
 * 데이터 테이블에서 사용되며, FTableRowBase를 상속받아 테이블 행으로 사용됩니다.
 * 
 * @section design_patterns 설계 패턴
 * - 데이터 구조체 패턴: 순수 데이터 저장
 * - 기본값 패턴: 생성자에서 기본값 설정
 * - 테이블 행 패턴: FTableRowBase 상속
 * 
 * @section usage 사용법
 * - 데이터 테이블에서 보스별 스탯 정의
 * - 런타임에서 보스 스탯 참조
 * - 블루프린트에서 스탯 수정
 * 
 * @section default_values 기본값
 * - MaxHP: 100
 * - MaxAP: 100
 * - ATK: 100.0f
 * - DEF: 50.0f
 * - Speed: 300
 */
USTRUCT(BlueprintType)
struct OPERRATION_DDT_API FTBossStats : public FTableRowBase
{
	GENERATED_BODY()
public:
	/**
	 * @brief 최대 체력
	 * 
	 * @details
	 * 보스의 최대 체력입니다.
	 * 
	 * @section default_value 기본값
	 * - 기본값: 100
	 * - 에디터에서 수정 가능
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
	int32 MaxHP;
	
	/**
	 * @brief 최대 액션 포인트
	 * 
	 * @details
	 * 보스의 최대 액션 포인트입니다.
	 * 
	 * @section default_value 기본값
	 * - 기본값: 100
	 * - 에디터에서 수정 가능
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
	float MaxAP;
	
	/**
	 * @brief 공격력
	 * 
	 * @details
	 * 보스의 공격력입니다.
	 * 
	 * @section default_value 기본값
	 * - 기본값: 100.0f
	 * - 에디터에서 수정 가능
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
	float IncreaseAP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
	float ATK;
	
	/**
	 * @brief 방어력
	 * 
	 * @details
	 * 보스의 방어력입니다.
	 * 
	 * @section default_value 기본값
	 * - 기본값: 50.0f
	 * - 에디터에서 수정 가능
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
	float DEF;
	
	/**
	 * @brief 속도
	 * 
	 * @details
	 * 보스의 이동 및 행동 속도입니다.
	 * 
	 * @section default_value 기본값
	 * - 기본값: 300
	 * - 에디터에서 수정 가능
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
	int32 Speed;

	/**
	 * @brief 최대 그로기 게이지
	 * 
	 * @details
	 * 보스의 최대 그로기 게이지입니다.
	 * 
	 * @section default_value 기본값
	 * - 기본값: 100.0f
	 * - 에디터에서 수정 가능
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
	float MaxGroggyGauge;
	
	/**
	 * @brief 생성자
	 * 
	 * @details
	 * 구조체의 기본값을 설정합니다.
	 * 모든 스탯에 적절한 기본값을 할당합니다.
	 * 
	 * @section initialization 초기화 값
	 * - MaxHP: 100
	 * - MaxAP: 100
	 * - ATK: 100.0f
	 * - DEF: 50.0f
	 * - Speed: 300
	 */
	FTBossStats()
		: MaxHP(100)
		, MaxAP(100)
		, ATK(100.0f)
		, DEF(50.0f)
		, Speed(300)
		, MaxGroggyGauge(100.0f)
	{}
};
