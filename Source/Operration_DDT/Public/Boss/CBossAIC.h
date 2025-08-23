// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file CBossAIC.h
 * @brief 보스 AI 컨트롤러 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 보스 캐릭터의 AI 동작을 제어하는 컨트롤러 클래스를 정의합니다.
 * StateTree 기반 AI 시스템을 사용하여 보스의 행동 패턴과 의사결정을 관리합니다.
 * 
 * @section architecture 아키텍처
 * - ACBossAIC: 보스 AI 컨트롤러 메인 클래스
 * - StateTreeAIComponent: StateTree 기반 AI 시스템
 * - Pawn 소유 및 초기화 관리
 * 
 * @section features 주요 기능
 * - StateTree 기반 AI 행동 관리
 * - Pawn 소유 및 초기화
 * - AI 컴포넌트 관리
 * - 보스 행동 패턴 제어
 * 
 * @section integration 연동 시스템
 * - StateTree: AI 행동 정의
 * - Pawn: 제어할 보스 캐릭터
 * - StateTreeAIComponent: AI 실행 엔진
 * 
 * @author [개발자명]
 * @date [작성일]
 * @version 1.0
 */

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "StateTree.h"
#include "CBossAIC.generated.h"

/**
 * @brief 보스 AI 컨트롤러 클래스
 * 
 * @details
 * 보스 캐릭터의 AI 동작을 제어하는 컨트롤러입니다.
 * 보스의 행동 패턴과 의사결정을 관리합니다.
 * 
 * @section design_patterns 설계 패턴
 * - StateTree 패턴: AI 행동을 상태 기반으로 관리
 * - 컴포넌트 패턴: AI 기능을 컴포넌트로 분리
 * - 컨트롤러 패턴: Pawn과 AI 로직을 분리
 * 
 * @section inheritance 상속 구조
 * - AAIController: 언리얼 엔진 기본 AI 컨트롤러 상속
 * - StateTree 시스템: 고급 AI 행동 관리 시스템
 * 
 * @section responsibilities 책임
 * - Pawn 소유 및 초기화
 * - StateTree 컴포넌트 관리
 * - AI 행동 실행
 * - 보스 행동 패턴 제어
 * 
 * @section usage 사용법
 * 1. 보스 블루프린트에서 AI 컨트롤러 클래스로 설정
 * 2. StateTree 에셋을 StateTreeAIComponent에 할당
 * 3. StateTree에서 보스 행동 패턴 정의
 * 4. 런타임에서 자동으로 AI 행동 실행
 */
UCLASS()
class OPERRATION_DDT_API ACBossAIC : public AAIController
{
	GENERATED_BODY()

	/**
	 * @brief 생성자
	 * 
	 * @details
	 * AI 컨트롤러의 기본 설정을 초기화합니다.
	 * StateTree 컴포넌트를 생성하고 기본 설정을 적용합니다.
	 * 
	 * @section initialization 초기화 과정
	 * 1. 부모 클래스 초기화
	 * 2. StateTreeAIComponent 생성
	 * 3. 기본 AI 설정 적용
	 */
	ACBossAIC();

protected:
	/**
	 * @brief Pawn 소유 시 호출되는 함수
	 * 
	 * @param InPawn 소유할 Pawn 객체
	 * 
	 * @details
	 * AI 컨트롤러가 Pawn을 소유할 때 초기화 작업을 수행합니다.
	 * StateTree 컴포넌트를 활성화하고 AI 시스템을 시작합니다.
	 * 
	 * @section initialization 초기화 과정
	 * 1. 부모 클래스 OnPossess 호출
	 * 2. StateTree 컴포넌트 활성화
	 * 3. AI 행동 시스템 시작
	 * 4. 보스 특화 초기화 수행
	 * 
	 * @section integration 연동 시스템
	 * - StateTreeAIComponent: AI 행동 실행
	 * - Pawn: 제어할 보스 캐릭터
	 * - StateTree: 행동 패턴 정의
	 */
	virtual void OnPossess(APawn* InPawn) override;

	/**
	 * @brief StateTree AI 컴포넌트
	 * 
	 * @details
	 * StateTree 기반 AI 시스템을 실행하는 컴포넌트입니다.
	 * 보스의 행동 패턴을 정의하고 실행하는 핵심 컴포넌트입니다.
	 * 
	 * @section responsibilities 책임
	 * - StateTree 에셋 실행
	 * - AI 행동 패턴 관리
	 * - 상태 전환 처리
	 * - 행동 우선순위 관리
	 * 
	 * @section configuration 설정
	 * - 에디터에서 StateTree 에셋 할당
	 * - 런타임에서 동적 상태 변경 가능
	 * - 디버그 정보 출력 지원
	 */
	UPROPERTY(EditAnywhere)
	class UStateTreeAIComponent* StateTreeComp;
};
