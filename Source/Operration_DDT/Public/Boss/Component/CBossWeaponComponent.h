// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file CBossWeaponComponent.h
 * @brief 보스 무기 시스템 컴포넌트 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 보스 캐릭터의 무기 시스템을 관리하는 컴포넌트를 정의합니다.
 * 게임플레이 태그 기반 무기 모드 관리와 무기 관련 액션을 처리합니다.
 * 
 * @section architecture 아키텍처
 * - UCBossWeaponComponent: 보스 무기 시스템 메인 컴포넌트
 * - FBossWeaponTags: 무기 태그 구조체
 * - 게임플레이 태그 기반 무기 모드 관리
 * - 데이터 에셋 연동 시스템
 * 
 * @section features 주요 기능
 * - 무기 모드 관리 (Unarmed, Paladin 등)
 * - 게임플레이 태그 기반 무기 시스템
 * - 무기 관련 액션 처리
 * - 무기 모드 변경 이벤트
 * - 데이터 에셋 기반 무기 정보 관리
 * 
 * @section weapon_modes 무기 모드
 * - Unarmed: 맨손 모드
 * - Paladin: 팔라딘 무기 모드
 * - 확장 가능한 태그 기반 시스템
 * 
 * @section integration 연동 시스템
 * - 게임플레이 태그 시스템
 * - 데이터 에셋 시스템
 * - 상태 컴포넌트
 * - 무기 액션 시스템
 * 
 * @author [개발자명]
 * @date [작성일]
 * @version 1.0
 */

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CBossWeaponComponent.generated.h"

/**
 * @brief 보스 무기 태그 구조체
 * 
 * @details
 * 보스의 무기 모드를 식별하는 게임플레이 태그들을 정의합니다.
 * 각 무기 타입별로 고유한 태그를 가지며, 무기 모드 전환에 사용됩니다.
 * 
 * @section weapon_tags 무기 태그
 * - Unarmed: 맨손 모드 태그
 * - Paladin: 팔라딘 무기 모드 태그
 * 
 * @section design_patterns 설계 패턴
 * - 태그 기반 패턴: 게임플레이 태그로 무기 식별
 * - 상수 패턴: 무기 태그를 상수로 관리
 * - 확장 패턴: 새로운 무기 타입 쉽게 추가 가능
 * 
 * @section usage 사용법
 * - 무기 모드 확인: IsUnarmedMode(), IsPaladinMode()
 * - 무기 모드 설정: SetUnarmedMode(), SetPaladinMode()
 * - 무기 모드 전환: SetMode(), ChangeType()
 */
USTRUCT(BlueprintType)
struct FBossWeaponTags {
	GENERATED_BODY()
	
	/**
	 * @brief 맨손 모드 태그
	 * 
	 * @details
	 * 보스가 무기를 사용하지 않는 맨손 모드를 식별하는 태그입니다.
	 * 기본값으로 "BOSS.Weapon.Unarmed" 태그를 사용합니다.
	 */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag Unarmed = FGameplayTag::RequestGameplayTag("BOSS.Weapon.Unarmed");
	
	/**
	 * @brief 팔라딘 무기 모드 태그
	 * 
	 * @details
	 * 보스가 팔라딘 무기를 사용하는 모드를 식별하는 태그입니다.
	 * 기본값으로 "BOSS.Weapon.Paladin" 태그를 사용합니다.
	 */
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag Paladin= FGameplayTag::RequestGameplayTag("BOSS.Weapon.Paladin");
};

/**
 * @brief 보스 무기 시스템 컴포넌트 클래스
 * 
 * @details
 * 보스 캐릭터의 무기 시스템을 관리하는 컴포넌트입니다.
 * 게임플레이 태그 기반으로 무기 모드를 관리하고, 무기 관련 액션을 처리합니다.
 * 
 * @section design_patterns 설계 패턴
 * - 컴포넌트 패턴: 언리얼 엔진 컴포넌트 시스템 활용
 * - 태그 기반 패턴: 게임플레이 태그로 무기 모드 관리
 * - 옵저버 패턴: 무기 모드 변경 시 이벤트 발생
 * - 팩토리 패턴: 데이터 에셋에서 무기 정보 생성
 * 
 * @section responsibilities 책임
 * - 무기 모드 관리 및 전환
 * - 무기 관련 액션 처리
 * - 무기 데이터 에셋 관리
 * - 무기 모드 변경 이벤트 처리
 * - 무기 상태 추적
 * 
 * @section integration 연동 시스템
 * - 게임플레이 태그: 무기 모드 식별
 * - 데이터 에셋: 무기 정보 저장
 * - 상태 컴포넌트: 무기 상태 관리
 * - 액션 시스템: 무기 액션 실행
 * 
 * @section usage 사용법
 * 1. 보스 블루프린트에 컴포넌트 추가
 * 2. 무기 태그 및 데이터 에셋 설정
 * 3. 무기 모드 전환 함수 호출
 * 4. 무기 액션 실행
 * 5. 무기 모드 변경 이벤트 처리
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OPERRATION_DDT_API UCBossWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:
	/**
	 * @brief 무기 데이터 에셋 맵
	 * 
	 * @details
	 * 게임플레이 태그와 무기 데이터 에셋을 매핑하는 맵입니다.
	 * 각 무기 모드별로 해당하는 데이터 에셋을 저장합니다.
	 * 
	 * @section data_structure 데이터 구조
	 * - 키: FGameplayTag (무기 모드 태그)
	 * - 값: UCBossWeaponAsset* (무기 데이터 에셋)
	 * 
	 * @section usage 사용법
	 * - 에디터에서 무기 태그와 데이터 에셋 매핑
	 * - 런타임에서 GetDataAssetByTag()로 데이터 에셋 검색
	 * - 무기 모드별 정보 관리
	 */
	UPROPERTY(EditAnywhere, Category="DataAsset")
	TMap<FGameplayTag, class UCBossWeaponAsset*> DataAssets;
	
	/**
	 * @brief 보스 무기 태그
	 * 
	 * @details
	 * 보스의 무기 모드를 식별하는 태그들을 저장합니다.
	 * Unarmed, Paladin 등의 무기 모드 태그를 포함합니다.
	 * 
	 * @section tags 포함 태그
	 * - Unarmed: 맨손 모드
	 * - Paladin: 팔라딘 무기 모드
	 */
	UPROPERTY(EditAnywhere, Category="Boss Tags")
	FBossWeaponTags BossTags;

public:
	/**
	 * @brief 생성자
	 * 
	 * @details
	 * 컴포넌트의 기본 설정을 초기화합니다.
	 * 기본 무기 모드를 설정하고 컴포넌트를 활성화합니다.
	 * 
	 * @section initialization 초기화 과정
	 * 1. 부모 클래스 초기화
	 * 2. 기본 무기 모드 설정 (Unarmed)
	 * 3. 컴포넌트 활성화
	 * 4. 틱 함수 활성화
	 */
	UCBossWeaponComponent();

protected:
	/**
	 * @brief 게임 시작 시 호출되는 함수
	 * 
	 * @details
	 * 컴포넌트가 게임에 참여할 때 초기화 작업을 수행합니다.
	 * 소유자 캐릭터와 상태 컴포넌트를 설정합니다.
	 * 
	 * @section initialization 초기화 과정
	 * 1. 부모 클래스 BeginPlay 호출
	 * 2. 소유자 캐릭터 설정
	 * 3. 상태 컴포넌트 참조 설정
	 * 4. 기본 무기 모드 설정
	 * 5. 무기 시스템 초기화 완료
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
	 * 매 프레임마다 무기 시스템 관련 업데이트를 수행합니다.
	 * 무기 상태 체크, 액션 처리 등을 담당합니다.
	 * 
	 * @section update_process 업데이트 과정
	 * 1. 부모 클래스 틱 함수 호출
	 * 2. 무기 상태 업데이트
	 * 3. 무기 액션 처리
	 * 4. 무기 모드 변경 확인
	 * 5. 이벤트 발생
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/**
	 * @brief 맨손 모드 확인 함수
	 * 
	 * @return bool 맨손 모드 여부
	 * 
	 * @details
	 * 현재 무기 모드가 맨손 모드인지 확인합니다.
	 * 
	 * @section usage 사용법
	 * - 무기 모드 확인: if (IsUnarmedMode()) { ... }
	 * - 조건부 로직 실행
	 */
	FORCEINLINE bool IsUnarmedMode() { return CurrentWeaponMode == BossTags.Unarmed; }
	
	/**
	 * @brief 팔라딘 모드 확인 함수
	 * 
	 * @return bool 팔라딘 모드 여부
	 * 
	 * @details
	 * 현재 무기 모드가 팔라딘 모드인지 확인합니다.
	 * 
	 * @section usage 사용법
	 * - 무기 모드 확인: if (IsPaladinMode()) { ... }
	 * - 조건부 로직 실행
	 */
	FORCEINLINE bool IsPaladinMode() { return CurrentWeaponMode == BossTags.Paladin; }
	
	/**
	 * @brief 현재 무기 모드 가져오기 함수
	 * 
	 * @return FGameplayTag 현재 무기 모드 태그
	 * 
	 * @details
	 * 현재 설정된 무기 모드의 게임플레이 태그를 반환합니다.
	 * 
	 * @section usage 사용법
	 * - 현재 모드 확인: FGameplayTag CurrentMode = GetCurrentWeaponMode()
	 * - 모드 비교: if (CurrentMode == BossTags.Paladin) { ... }
	 */
	FORCEINLINE FGameplayTag GetCurrentWeaponMode() { return CurrentWeaponMode; }
	
	/**
	 * @brief 보스 무기 객체 가져오기 함수
	 * 
	 * @return ACBossWeapon* 보스 무기 객체
	 * 
	 * @details
	 * 현재 무기 모드에 해당하는 보스 무기 객체를 반환합니다.
	 * 
	 * @section usage 사용법
	 * - 무기 객체 접근: ACBossWeapon* Weapon = GetBossWeapon()
	 * - 무기 기능 사용: Weapon->Attack()
	 */
	class ACBossWeapon* GetBossWeapon();
	
	/**
	 * @brief 보스 장비 객체 가져오기 함수
	 * 
	 * @return UCBossEquipment* 보스 장비 객체
	 * 
	 * @details
	 * 현재 무기 모드에 해당하는 보스 장비 객체를 반환합니다.
	 * 
	 * @section usage 사용법
	 * - 장비 객체 접근: UCBossEquipment* Equipment = GetBossEquipment()
	 * - 장비 기능 사용: Equipment->Equip()
	 */
	class UCBossEquipment* GetBossEquipment();
	
	/**
	 * @brief 보스 액션 객체 가져오기 함수
	 * 
	 * @return UCBossDoAction* 보스 액션 객체
	 * 
	 * @details
	 * 현재 무기 모드에 해당하는 보스 액션 객체를 반환합니다.
	 * 
	 * @section usage 사용법
	 * - 액션 객체 접근: UCBossDoAction* Action = GetBossDoAction()
	 * - 액션 실행: Action->Execute()
	 */
	class UCBossDoAction* GetBossDoAction();
	
	/**
	 * @brief 보스 액션 실행 함수
	 * 
	 * @param Tag 실행할 액션의 게임플레이 태그
	 * 
	 * @details
	 * 지정된 태그에 해당하는 보스 액션을 실행합니다.
	 * 
	 * @section usage 사용법
	 * - 액션 실행: BossDoAction(AttackTag)
	 * - 태그 기반 액션 처리
	 */
	void BossDoAction(FGameplayTag Tag);
	
	/**
	 * @brief 태그로 데이터 에셋 가져오기 함수
	 * 
	 * @param InTag 검색할 무기 모드 태그
	 * @return UCBossWeaponAsset* 해당하는 데이터 에셋
	 * 
	 * @details
	 * 지정된 태그에 해당하는 무기 데이터 에셋을 반환합니다.
	 * 
	 * @section usage 사용법
	 * - 데이터 에셋 검색: UCBossWeaponAsset* Asset = GetDataAssetByTag(PaladinTag)
	 * - 무기 정보 접근: Asset->GetWeaponInfo()
	 */
	class UCBossWeaponAsset* GetDataAssetByTag(const FGameplayTag& InTag);
	
	/**
	 * @brief 맨손 모드 설정 함수
	 * 
	 * @details
	 * 무기 모드를 맨손 모드로 변경합니다.
	 * 
	 * @section process 처리 과정
	 * 1. 현재 모드 저장
	 * 2. 맨손 모드로 변경
	 * 3. 모드 변경 이벤트 발생
	 * 4. 관련 시스템 업데이트
	 */
	void SetUnarmedMode();
	
	/**
	 * @brief 팔라딘 모드 설정 함수
	 * 
	 * @details
	 * 무기 모드를 팔라딘 모드로 변경합니다.
	 * 
	 * @section process 처리 과정
	 * 1. 현재 모드 저장
	 * 2. 팔라딘 모드로 변경
	 * 3. 모드 변경 이벤트 발생
	 * 4. 관련 시스템 업데이트
	 */
	void SetPaladinMode();
	
public:
	/**
	 * @brief 무기 모드 설정 함수
	 * 
	 * @param InTag 설정할 무기 모드 태그
	 * 
	 * @details
	 * 지정된 태그에 해당하는 무기 모드로 변경합니다.
	 * 
	 * @section process 처리 과정
	 * 1. 태그 유효성 검사
	 * 2. 현재 모드와 비교
	 * 3. 모드 변경 (다른 경우만)
	 * 4. 이벤트 발생
	 */
	void SetMode(const FGameplayTag& InTag);
	
	/**
	 * @brief 무기 타입 변경 함수
	 * 
	 * @param InTag 변경할 무기 타입 태그
	 * 
	 * @details
	 * 무기 타입을 변경하고 관련 시스템을 업데이트합니다.
	 * 
	 * @section process 처리 과정
	 * 1. 무기 타입 변경
	 * 2. 관련 컴포넌트 업데이트
	 * 3. 애니메이션 변경
	 * 4. 이벤트 발생
	 */
	void ChangeType(const FGameplayTag& InTag);

private:
	class ACharacter* OwnerCharacter; ///< 소유자 캐릭터 참조
	class UCStateComponent* State; ///< 상태 컴포넌트 참조
	
	/**
	 * @brief 현재 무기 모드
	 * 
	 * @details
	 * 현재 설정된 무기 모드의 게임플레이 태그입니다.
	 * 
	 * @section default_value 기본값
	 * - 기본적으로 맨손 모드로 설정
	 * - 런타임에 동적으로 변경 가능
	 */
	UPROPERTY(EditAnywhere)
	FGameplayTag CurrentWeaponMode;
	
	/**
	 * @brief 맨손 태그
	 * 
	 * @details
	 * 맨손 모드를 식별하는 게임플레이 태그입니다.
	 * 
	 * @section usage 사용법
	 * - 맨손 모드 확인: if (CurrentWeaponMode == UnarmedTag)
	 * - 기본 모드 설정: CurrentWeaponMode = UnarmedTag
	 */
	UPROPERTY()
	FGameplayTag UnarmedTag;
	
	/**
	 * @brief 무기 모드 변경 이벤트
	 * 
	 * @details
	 * 무기 모드가 변경될 때 발생하는 이벤트입니다.
	 * 이전 모드와 새로운 모드 정보를 포함합니다.
	 * 
	 * @section parameters 매개변수
	 * - InPrevMode: 이전 무기 모드
	 * - InNewMode: 새로운 무기 모드
	 * 
	 * @section usage 사용법
	 * - 블루프린트에서 이벤트 바인딩
	 * - UI 업데이트
	 * - 애니메이션 변경
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponModeChanged, FGameplayTag, InPrevMode, FGameplayTag, InNewMode);
	UPROPERTY(BlueprintAssignable, Category = "Weapon Mode")
	FWeaponModeChanged OnWeaponModeChanged;
};
