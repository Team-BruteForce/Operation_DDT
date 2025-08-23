// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file CBossMovementComponent.h
 * @brief 보스 이동 관리 컴포넌트 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 보스 캐릭터의 이동 로직을 관리하는 컴포넌트를 정의합니다.
 * 플레이어 추적, 회전, 이동 패턴 등을 처리합니다.
 * 
 * @section architecture 아키텍처
 * - UCBossMovementComponent: 보스 이동 관리 메인 컴포넌트
 * - 플레이어 추적 시스템
 * - 부드러운 회전 시스템
 * - AI 컨트롤러 연동
 * 
 * @section features 주요 기능
 * - 플레이어 방향으로 부드러운 회전
 * - 플레이어 자동 탐지
 * - 이동 속도 및 가속도 관리
 * - AI 컨트롤러와의 연동
 * - 블루프린트 호출 가능한 함수들
 * 
 * @section movement_system 이동 시스템
 * - 플레이어 추적: 자동으로 플레이어 위치 감지
 * - 부드러운 회전: 자연스러운 회전 애니메이션
 * - 속도 조절: 다양한 이동 속도 지원
 * - AI 연동: AI 컨트롤러와 협력하여 이동
 * 
 * @author [개발자명]
 * @date [작성일]
 * @version 1.0
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBossMovementComponent.generated.h"

/**
 * @brief 보스 이동 관리 컴포넌트 클래스
 * 
 * @details
 * 보스 캐릭터의 이동 로직을 관리하는 컴포넌트입니다.
 * 플레이어 추적, 부드러운 회전, 이동 패턴 등을 처리합니다.
 * 
 * @section design_patterns 설계 패턴
 * - 컴포넌트 패턴: 언리얼 엔진 컴포넌트 시스템 활용
 * - 옵저버 패턴: 플레이어 위치 변화 감지
 * - 전략 패턴: 다양한 이동 전략 지원
 * - 연동 패턴: AI 컨트롤러와 협력
 * 
 * @section responsibilities 책임
 * - 플레이어 위치 추적
 * - 부드러운 회전 처리
 * - 이동 속도 관리
 * - AI 컨트롤러 연동
 * - 이동 상태 추적
 * 
 * @section integration 연동 시스템
 * - AI 컨트롤러: 이동 명령 처리
 * - 플레이어 시스템: 타겟 위치 제공
 * - 애니메이션 시스템: 이동 애니메이션 연동
 * - 네비게이션 시스템: 경로 찾기
 * 
 * @section usage 사용법
 * 1. 보스 블루프린트에 컴포넌트 추가
 * 2. AI 컨트롤러 설정
 * 3. 회전 함수 호출 (RotateTowardsPlayer)
 * 4. 플레이어 추적 시작
 * 5. 이동 패턴 적용
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCBossMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/**
	 * @brief 생성자
	 * 
	 * @details
	 * 컴포넌트의 기본 설정을 초기화합니다.
	 * 기본 이동 설정과 컴포넌트를 활성화합니다.
	 * 
	 * @section initialization 초기화 과정
	 * 1. 부모 클래스 초기화
	 * 2. 기본 이동 설정
	 * 3. 컴포넌트 활성화
	 * 4. 틱 함수 활성화
	 */
	UCBossMovementComponent();

protected:
	/**
	 * @brief 게임 시작 시 호출되는 함수
	 * 
	 * @details
	 * 컴포넌트가 게임에 참여할 때 초기화 작업을 수행합니다.
	 * 소유자와 AI 컨트롤러를 설정합니다.
	 * 
	 * @section initialization 초기화 과정
	 * 1. 부모 클래스 BeginPlay 호출
	 * 2. 소유자 설정
	 * 3. AI 컨트롤러 참조 설정
	 * 4. 플레이어 탐지 시스템 초기화
	 * 5. 이동 시스템 준비 완료
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
	 * 매 프레임마다 이동 관련 업데이트를 수행합니다.
	 * 플레이어 추적, 회전 처리 등을 담당합니다.
	 * 
	 * @section update_process 업데이트 과정
	 * 1. 부모 클래스 틱 함수 호출
	 * 2. 플레이어 위치 업데이트
	 * 3. 회전 처리
	 * 4. 이동 상태 업데이트
	 * 5. AI 컨트롤러와 동기화
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief 플레이어 방향으로 부드럽게 회전하는 함수
	 * 
	 * @param DeltaTime 델타 타임
	 * @param RotationSpeed 회전 속도 (높을수록 빠름)
	 * 
	 * @details
	 * 보스가 플레이어를 향해 부드럽게 회전하도록 처리합니다.
	 * 자연스러운 회전 애니메이션을 위해 보간을 사용합니다.
	 * 
	 * @section rotation_process 회전 과정
	 * 1. 플레이어 위치 가져오기
	 * 2. 현재 방향과 목표 방향 계산
	 * 3. 보간을 통한 부드러운 회전
	 * 4. Yaw 회전만 적용 (수직 회전 제외)
	 * 5. 회전 완료 확인
	 * 
	 * @section parameters 매개변수
	 * - DeltaTime: 프레임 간 시간 간격
	 * - RotationSpeed: 회전 속도 (기본값: 5.0f)
	 * 
	 * @section usage 사용법
	 * - 블루프린트에서 호출 가능
	 * - 틱 함수에서 지속적으로 호출
	 * - 회전 속도 조절 가능
	 */
	UFUNCTION(BlueprintCallable, Category="Player Tracking")
	void RotateTowardsPlayer(float DeltaTime, float RotationSpeed = 5.0f);

private:
	/**
	 * @brief 플레이어를 찾는 함수
	 * 
	 * @return 찾은 플레이어 액터, 없으면 nullptr
	 * 
	 * @details
	 * 월드에서 플레이어를 찾아서 반환합니다.
	 * 여러 플레이어가 있을 경우 첫 번째 플레이어를 반환합니다.
	 * 
	 * @section search_process 탐색 과정
	 * 1. 모든 Pawn 액터 검색
	 * 2. 플레이어 컨트롤러 소유 여부 확인
	 * 3. 첫 번째 플레이어 반환
	 * 4. 플레이어가 없으면 nullptr 반환
	 * 
	 * @section usage 사용법
	 * - 내부적으로 자동 호출
	 * - 플레이어 추적 시 사용
	 * - 회전 함수에서 타겟으로 사용
	 */
	class APawn* FindPlayer();

	/**
	 * @brief 소유자 액터
	 * 
	 * @details
	 * 이 컴포넌트를 소유하는 액터입니다.
	 * 보통 보스 캐릭터가 됩니다.
	 * 
	 * @section usage 사용법
	 * - 위치 및 방향 정보 접근
	 * - 컴포넌트 참조
	 * - 소유자 기반 로직 실행
	 */
	UPROPERTY()
	class APawn* Owner;
	
	/**
	 * @brief AI 컨트롤러
	 * 
	 * @details
	 * 보스를 제어하는 AI 컨트롤러입니다.
	 * 이동 명령과 협력하여 보스의 움직임을 제어합니다.
	 * 
	 * @section integration 연동
	 * - 이동 명령 처리
	 * - 네비게이션 시스템 연동
	 * - AI 행동과 이동 동기화
	 * 
	 * @section usage 사용법
	 * - AI 명령 전달
	 * - 이동 상태 확인
	 * - 네비게이션 정보 접근
	 */
	UPROPERTY()
	class AAIController* AIC;
};
