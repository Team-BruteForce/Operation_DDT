// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file BossStateComponent.h
 * @brief 보스 상태 관리 컴포넌트 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 보스 캐릭터의 상태를 관리하는 컴포넌트를 정의합니다.
 * 게임플레이 태그 기반 상태 관리와 StateTree 연동을 처리합니다.
 * 
 * @section architecture 아키텍처
 * - UBossStateComponent: 보스 상태 관리 메인 컴포넌트
 * - 게임플레이 태그 기반 상태 시스템
 * - StateTree 연동 시스템
 * - 상태 변경 이벤트 처리
 * 
 * @section features 주요 기능
 * - 게임플레이 태그 기반 상태 관리
 * - 상태 변경 이벤트 발생
 * - StateTree와의 연동
 * - 블루프린트 호출 가능한 함수들
 * - 편의용 상태 태그 제공
 * 
 * @section state_system 상태 시스템
 * - Idle: 대기 상태
 * - Action: 행동 상태
 * - 확장 가능한 태그 기반 시스템
 * - StateTree 이벤트 전달
 * 
 * @author [개발자명]
 * @date [작성일]
 * @version 1.0
 */

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "Components/ActorComponent.h"
#include "BossStateComponent.generated.h"

/**
 * @brief 상태 태그 변경 이벤트
 * 
 * @details
 * 상태 태그가 변경될 때 발생하는 이벤트입니다.
 * 이전 상태와 새로운 상태 정보를 포함합니다.
 * 
 * @section parameters 매개변수
 * - Prev: 이전 상태 태그
 * - NewTag: 새로운 상태 태그
 * 
 * @section usage 사용법
 * - 블루프린트에서 이벤트 바인딩
 * - 상태 변경 감지
 * - UI 업데이트
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTagChanged,FGameplayTag,Prev,FGameplayTag,NewTag);

/**
 * @brief 보스 상태 관리 컴포넌트 클래스
 * 
 * @details
 * 보스 캐릭터의 상태를 관리하는 컴포넌트입니다.
 * 게임플레이 태그 기반으로 상태를 관리하고 StateTree와 연동합니다.
 * 
 * @section design_patterns 설계 패턴
 * - 컴포넌트 패턴: 언리얼 엔진 컴포넌트 시스템 활용
 * - 태그 기반 패턴: 게임플레이 태그로 상태 관리
 * - 옵저버 패턴: 상태 변경 시 이벤트 발생
 * - 인터페이스 패턴: IGameplayTagAssetInterface 구현
 * 
 * @section responsibilities 책임
 * - 상태 태그 관리 및 변경
 * - 상태 변경 이벤트 처리
 * - StateTree 연동
 * - 태그 컨테이너 관리
 * - 편의용 상태 태그 제공
 * 
 * @section integration 연동 시스템
 * - 게임플레이 태그: 상태 식별
 * - StateTree: 상태 기반 AI 행동
 * - 이벤트 시스템: 상태 변경 알림
 * - 블루프린트: 상태 관리 UI
 * 
 * @section usage 사용법
 * 1. 보스 블루프린트에 컴포넌트 추가
 * 2. 상태 태그 설정
 * 3. 상태 변경 함수 호출
 * 4. 상태 변경 이벤트 처리
 * 5. StateTree와 연동
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UBossStateComponent : public UActorComponent, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	/**
	 * @brief 생성자
	 * 
	 * @details
	 * 컴포넌트의 기본 설정을 초기화합니다.
	 * 기본 상태와 태그를 설정합니다.
	 */
	UBossStateComponent();

	/**
	 * @brief 소유한 게임플레이 태그 가져오기 함수
	 * 
	 * @param TagContainer 태그를 저장할 컨테이너
	 * 
	 * @details
	 * IGameplayTagAssetInterface의 구현 함수입니다.
	 * 이 컴포넌트가 소유한 모든 게임플레이 태그를 반환합니다.
	 * 
	 * @section implementation 구현
	 * - OwnedTags의 모든 태그를 TagContainer에 추가
	 * - StateTree와 다른 시스템에서 사용
	 */
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer.AppendTags(OwnedTags); }

	/**
	 * @brief 상태 태그 설정 함수
	 * 
	 * @param NewState 새로운 상태 태그
	 * @param bSendEvent 이벤트 발생 여부
	 * 
	 * @details
	 * 보스의 현재 상태를 새로운 태그로 변경합니다.
	 * 상태 변경 시 이벤트를 발생시킬 수 있습니다.
	 * 
	 * @section process 처리 과정
	 * 1. 이전 상태 저장
	 * 2. 새로운 상태로 변경
	 * 3. StateTree에 이벤트 전달
	 * 4. 상태 변경 이벤트 발생 (bSendEvent가 true인 경우)
	 * 
	 * @section usage 사용법
	 * - 블루프린트에서 호출 가능
	 * - 상태 전환 시 사용
	 * - 이벤트 발생 여부 제어 가능
	 */
	UFUNCTION(BlueprintCallable) void SetStateTag(FGameplayTag NewState, bool bSendEvent=true);
	
	/**
	 * @brief 태그 추가 함수
	 * 
	 * @param Tag 추가할 태그
	 * @param bSendEvent 이벤트 발생 여부
	 * 
	 * @details
	 * 보스에게 새로운 태그를 추가합니다.
	 * 
	 * @section usage 사용법
	 * - 블루프린트에서 호출 가능
	 * - 버프나 디버프 태그 추가 시 사용
	 */
	UFUNCTION(BlueprintCallable) void AddTag(FGameplayTag Tag,bool bSendEvent=false);
	
	/**
	 * @brief 태그 제거 함수
	 * 
	 * @param Tag 제거할 태그
	 * @param bSendEvent 이벤트 발생 여부
	 * 
	 * @details
	 * 보스에서 지정된 태그를 제거합니다.
	 * 
	 * @section usage 사용법
	 * - 블루프린트에서 호출 가능
	 * - 버프나 디버프 태그 제거 시 사용
	 */
	UFUNCTION(BlueprintCallable) void RemoveTag(FGameplayTag Tag,bool bSendEvent=false);
	
	/**
	 * @brief 태그 보유 여부 확인 함수
	 * 
	 * @param Tag 확인할 태그
	 * @return bool 태그 보유 여부
	 * 
	 * @details
	 * 보스가 지정된 태그를 가지고 있는지 확인합니다.
	 * 
	 * @section usage 사용법
	 * - 블루프린트에서 호출 가능
	 * - 조건부 로직 실행 시 사용
	 */
	UFUNCTION(BlueprintPure)  bool HasTag(FGameplayTag Tag) const { return OwnedTags.HasTag(Tag); }
	
	/**
	 * @brief 현재 상태 태그 가져오기 함수
	 * 
	 * @return FGameplayTag 현재 상태 태그
	 * 
	 * @details
	 * 보스의 현재 상태 태그를 반환합니다.
	 * 
	 * @section usage 사용법
	 * - 블루프린트에서 호출 가능
	 * - 현재 상태 확인 시 사용
	 */
	UFUNCTION(BlueprintPure)  FGameplayTag GetStateTag() const { return CurrentState; }

	/**
	 * @brief 보스 상태 완전 초기화 (매니저용)
	 * 
	 * @details
	 * 보스의 모든 상태와 태그를 초기 상태로 리셋합니다.
	 * 보스 매니저에서 보스 리셋 시 사용됩니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "Boss Reset")
	void ResetBossState();

	/**
	 * @brief Tick 함수 추가 (AI 컴포넌트 안전 초기화용)
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/**
	 * @brief 대기 상태 태그
	 * 
	 * @details
	 * 보스가 대기 상태일 때 사용하는 태그입니다.
	 * 편의를 위해 인스펙터에서 설정 가능합니다.
	 */
	UPROPERTY(EditDefaultsOnly,Category="StateTags")
	FGameplayTag Tag_State_Idle;
	
	/**
	 * @brief 행동 상태 태그
	 * 
	 * @details
	 * 보스가 행동 중일 때 사용하는 태그입니다.
	 * 편의를 위해 인스펙터에서 설정 가능합니다.
	 */
	UPROPERTY(EditDefaultsOnly,Category="StateTags")
	FGameplayTag Tag_State_Action;
	
	/**
	 * @brief 상태 태그 변경 이벤트
	 * 
	 * @details
	 * 상태 태그가 변경될 때 발생하는 이벤트입니다.
	 * 블루프린트에서 바인딩하여 사용할 수 있습니다.
	 * 
	 * @section usage 사용법
	 * - 블루프린트에서 이벤트 바인딩
	 * - 상태 변경 감지
	 * - UI 업데이트
	 */
	UPROPERTY(BlueprintAssignable)
	FStateTagChanged OnStateTagChanged;
	
protected:
	/**
	 * @brief 게임 시작 시 호출되는 함수
	 * 
	 * @details
	 * 컴포넌트가 게임에 참여할 때 초기화 작업을 수행합니다.
	 * StateTree 컴포넌트와 AI 컨트롤러를 설정합니다.
	 */
	virtual void BeginPlay() override;

private:
	/**
	 * @brief StateTree에 이벤트 전달 함수
	 * 
	 * @param Tag 전달할 태그
	 * 
	 * @details
	 * 상태 변경을 StateTree에 알려서 AI 행동을 업데이트합니다.
	 * 
	 * @section process 처리 과정
	 * 1. StateTree 컴포넌트 확인
	 * 2. 태그를 StateTree에 전달
	 * 3. AI 행동 업데이트
	 */
	void SendEventToStateTree(const FGameplayTag& Tag);

private:
	/**
	 * @brief 현재 상태 태그
	 * 
	 * @details
	 * 보스의 현재 상태를 나타내는 게임플레이 태그입니다.
	 * 
	 * @section usage 사용법
	 * - 현재 상태 확인
	 * - 상태 기반 로직 실행
	 * - UI 표시
	 */
	UPROPERTY(VisibleAnywhere,Category="State")
	FGameplayTag CurrentState;

	/**
	 * @brief 소유한 태그 컨테이너
	 * 
	 * @details
	 * 보스가 현재 소유하고 있는 모든 게임플레이 태그를 저장합니다.
	 * 
	 * @section usage 사용법
	 * - 태그 추가/제거
	 * - 태그 보유 여부 확인
	 * - StateTree 연동
	 */
	UPROPERTY(VisibleAnywhere,Category="State")
	FGameplayTagContainer OwnedTags;

	/**
	 * @brief 캐시된 StateTree 컴포넌트
	 * 
	 * @details
	 * StateTree와의 연동을 위해 캐시된 컴포넌트 참조입니다.
	 * 
	 * @section usage 사용법
	 * - StateTree 이벤트 전달
	 * - AI 행동 업데이트
	 */
	UPROPERTY()
	class UStateTreeComponent* CachedStateTree = nullptr;
	
	/**
	 * @brief 소유자 액터
	 * 
	 * @details
	 * 이 컴포넌트를 소유하는 액터입니다.
	 * 보통 보스 캐릭터가 됩니다.
	 */
	UPROPERTY()
	class APawn* Owner;
	
	/**
	 * @brief AI 컨트롤러
	 * 
	 * @details
	 * 보스를 제어하는 AI 컨트롤러입니다.
	 * StateTree와 협력하여 AI 행동을 제어합니다.
	 */
	UPROPERTY()
	class AAIController* AIC;
public:
	/** 중복 실행 방지 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Tag", meta=(ToolTip="보스의 중복 스킬 사용 방지 변수입니다."))
	FGameplayTag LastActiveTag;
};
