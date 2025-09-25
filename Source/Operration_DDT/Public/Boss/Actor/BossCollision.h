/**
 * @file BossCollision.h
 * @brief 보스 콜리전 액터 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 보스의 공격 콜리전을 관리하는 액터를 정의합니다.
 * 박스 컴포넌트를 통해 보스의 공격 범위를 정의합니다.
 * 
 * @section architecture 아키텍처
 * - ABossCollision: 보스 콜리전 관리 메인 액터
 * - 박스 콜리전 시스템
 * - 데미지 처리 시스템
 * - 콜리전 활성화/비활성화 시스템
 * 
 * @section features 주요 기능
 * - 박스 콜리전 기반 공격 범위
 * - 데미지 처리 및 전달
 * - 콜리전 활성화 제어
 * - 디버그 시각화
 * 
 * @section collision_system 콜리전 시스템
 * - 박스 컴포넌트: 공격 범위 정의
 * - 데미지 처리: 플레이어에게 데미지 전달
 * - 활성화 제어: 콜리전 켜기/끄기
 * 
 * @author 이효원
 * @date 2024-12-19
 * @version 1.0
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossCollision.generated.h"

/**
 * @brief 보스 콜리전 액터 클래스
 * 
 * @details
 * 보스의 공격 콜리전을 관리하는 액터입니다.
 * 박스 컴포넌트를 통해 보스의 공격 범위를 정의합니다.
 * 
 * @section design_patterns 설계 패턴
 * - 액터 패턴: 언리얼 엔진 액터 시스템 활용
 * - 컴포넌트 패턴: 박스 콜리전 컴포넌트 사용
 * - 옵저버 패턴: 콜리전 이벤트 처리
 * 
 * @section responsibilities 책임
 * - 공격 범위 콜리전 관리
 * - 데미지 처리 및 전달
 * - 콜리전 활성화 제어
 * - 디버그 정보 제공
 * 
 * @section integration 연동 시스템
 * - 보스 캐릭터: 공격 명령 수신
 * - 플레이어: 데미지 전달
 * - 데미지 시스템: 데미지 처리
 * - 디버그 시스템: 시각화 정보
 * 
 * @section usage 사용법
 * 1. 보스 블루프린트에서 스폰
 * 2. 공격 범위 설정
 * 3. 콜리전 활성화/비활성화
 * 4. 데미지 처리 확인
 */
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossCollision();
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBoxComponent* BoxComp;

	UPROPERTY()
	class ACBossWeapon* Weapon;

private:
	UFUNCTION()
	void OnBossComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBossComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
