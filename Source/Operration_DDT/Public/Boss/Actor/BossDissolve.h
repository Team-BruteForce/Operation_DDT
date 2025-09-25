// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossDissolve.generated.h"

/**
 * @file BossDissolve.h
 * @brief 보스 디졸브 액터 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 보스의 디졸브 효과를 관리하는 액터를 정의합니다.
 * 보스가 죽을 때 디졸브 효과를 처리합니다.
 * 
 * @section architecture 아키텍처
 * - ABossDissolve: 보스 디졸브 관리 메인 액터
 * - 디졸브 효과 시스템
 * - 블루프린트 이벤트 시스템
 * 
 * @section features 주요 기능
 * - 디졸브 효과 트리거
 * - 블루프린트 이벤트 호출
 * - 디졸브 애니메이션 관리
 * 
 * @section dissolve_system 디졸브 시스템
 * - TriggerDissolveEffect: 디졸브 효과 트리거
 * - 블루프린트 이벤트: 실제 디졸브 구현
 * 
 * @author 이효원
 * @date 2024-12-19
 * @version 1.0
 */
/**
 * @brief 보스 디졸브 액터 클래스
 * 
 * @details
 * 보스의 디졸브 효과를 관리하는 액터입니다.
 * 보스가 죽을 때 디졸브 효과를 처리합니다.
 * 
 * @section design_patterns 설계 패턴
 * - 액터 패턴: 언리얼 엔진 액터 시스템 활용
 * - 이벤트 패턴: 블루프린트 이벤트 호출
 * - 브리지 패턴: C++와 블루프린트 연결
 * 
 * @section responsibilities 책임
 * - 디졸브 효과 트리거
 * - 블루프린트 이벤트 호출
 * - 디졸브 애니메이션 관리
 * 
 * @section integration 연동 시스템
 * - 보스 캐릭터: 디졸브 요청 수신
 * - 블루프린트: 실제 디졸브 구현
 * - 이펙트 시스템: 디졸브 이펙트
 * - 애니메이션 시스템: 디졸브 애니메이션
 * 
 * @section usage 사용법
 * 1. 보스 블루프린트에서 스폰
 * 2. 디졸브 효과 트리거 호출
 * 3. 블루프린트에서 디졸브 구현
 * 4. 디졸브 완료 후 정리
 */
	/**
	 * @brief C++에서 블루프린트의 디졸브 기능을 호출하는 함수
	 * 블루프린트에서 이 이벤트를 구현하면 C++에서 호출 가능
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "Dissolve")
	void TriggerDissolveEffect();


};
