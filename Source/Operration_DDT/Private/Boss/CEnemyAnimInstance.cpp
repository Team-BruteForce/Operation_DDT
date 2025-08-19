// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file CEnemyAnimInstance.cpp
 * @brief 적 캐릭터의 애니메이션 인스턴스 구현 파일
 * 
 * @date 2024-12-19
 * @author [이효원]
 * @version 1.0
 * 
 * @section overview 개요
 * 이 파일은 적 캐릭터들의 애니메이션 상태와 로직을 관리합니다.
 * 현재는 기본 구현만 포함되어 있으며, 향후 적 캐릭터의 
 * 다양한 애니메이션 상태와 전환 로직이 추가될 예정입니다.
 * 
 * @section purpose 개발 목적
 * - 기획자가 설계한 애니메이션 패턴을 실시간으로 반영
 * - 보스 캐릭터의 다양한 공격 패턴과 애니메이션 연동
 * - 데이터 기반 애니메이션 시스템으로 유지보수성 향상
 * 
 * @section features 주요 기능
 * - 적 캐릭터 애니메이션 상태 관리
 * - 애니메이션 전환 로직 (향후 구현 예정)
 * - 애니메이션 이벤트 처리 (향후 구현 예정)
 * - 실시간 데이터 동기화를 통한 애니메이션 패턴 업데이트
 * 
 * @section technical_details 기술적 세부사항
 * - Unreal Engine의 애니메이션 시스템 활용
 * - 애니메이션 블루프린트와 C++ 코드의 연동
 * - 데이터 테이블을 통한 애니메이션 패턴 관리
 * 
 * @section portfolio_highlight 포트폴리오 하이라이트
 * 이 애니메이션 시스템은 게임 개발에서 중요한 시각적 표현과
 * 사용자 경험을 담당하는 핵심 시스템입니다. 특히 실시간 데이터 동기화를
 * 통해 기획자가 즉시 애니메이션 패턴을 수정할 수 있도록 한 점은
 * 현대 게임 개발에서 요구되는 빠른 반복 개발과 유연성을 잘 보여줍니다.
 */

#include "Boss/CEnemyAnimInstance.h"

