/**
 * @file CBossWeaponComponent.h
 * @brief 보스 무기 시스템 컴포넌트 헤더 파일
 * 
 * @details
 * 보스 캐릭터의 무기 시스템을 관리하는 컴포넌트를 정의합니다.
 * 게임플레이 태그 기반 무기 모드 관리와 무기 관련 액션을 처리합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "CBossWeaponComponent.generated.h"

/**
 * @brief 보스 무기 태그 구조체
 * 
 * 보스의 무기 모드를 정의하는 게임플레이 태그들을 관리합니다.
 */
USTRUCT(BlueprintType)
struct FBossWeaponTags {
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PaseZero = FGameplayTag::RequestGameplayTag("BOSS.Pase.Zero");
	
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag PaseOne= FGameplayTag::RequestGameplayTag("BOSS.Pase.One");
};

/**
 * @brief 보스 무기 시스템 컴포넌트
 * 
 * 보스 캐릭터의 무기 시스템을 관리하는 컴포넌트입니다.
 * 게임플레이 태그 기반 무기 모드 관리와 무기 관련 액션을 처리합니다.
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OPERRATION_DDT_API UCBossWeaponComponent : public UActorComponent
{
	GENERATED_BODY()
	
private:
	/**
	 * @brief 무기 데이터 에셋 맵
	 */
	UPROPERTY(EditAnywhere, Category="DataAsset")
	TMap<FGameplayTag, class UCBossWeaponAsset*> DataAssets;
	
	/**
	 * @brief 보스 무기 태그들
	 */
	UPROPERTY(EditAnywhere, Category="Boss Tags")
	FBossWeaponTags BossTags;

public:
	/**
	 * @brief 보스 무기 컴포넌트 생성자
	 */
	UCBossWeaponComponent();

protected:
	/**
	 * @brief 게임 시작 시 호출되는 함수
	 */
	virtual void BeginPlay() override;

public:
	/**
	 * @brief 매 프레임 호출되는 틱 함수
	 */
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:

	/**
	 * @brief 맨손 모드인지 확인
	 */
	FORCEINLINE bool IsUnarmedMode() { return CurrentWeaponMode == BossTags.PaseZero; }
	
	/**
	 * @brief 팔라딘 모드인지 확인
	 */
	FORCEINLINE bool IsPaladinMode() { return CurrentWeaponMode == BossTags.PaseOne; }

	/**
	 * @brief 현재 무기 모드 반환
	 */
	FORCEINLINE FGameplayTag GetCurrentWeaponMode() { return CurrentWeaponMode; }
	
	/**
	 * @brief 보스 무기 객체 반환
	 */
	class ACBossWeapon* GetBossWeapon();
	

	/**
	 * @brief 보스 장비 객체 반환
	 */
	class UCBossEquipment* GetBossEquipment();
	

	/**
	 * @brief 보스 액션 객체 반환
	 */
	class UCBossDoAction* GetBossDoAction();

	
	/**
	 * @brief 보스 액션 실행
	 * 
	 * @param Tag 실행할 액션 태그
	 */
	void BossDoAction(FGameplayTag Tag);
	void BossHitAction(FGameplayTag Tag);
	
	/**
	 * @brief 태그로 데이터 에셋 반환
	 * 
	 * @param InTag 찾을 태그
	 */
	class UCBossWeaponAsset* GetDataAssetByTag(const FGameplayTag& InTag);

	/**
	 * @brief 맨손 모드로 설정
	 */
	void SetUnarmedMode();
	
	/**
	 * @brief 페이즈 원 모드로 설정
	 */
	void SetPaseOneMode();
	
public:
	/**
	 * @brief 무기 모드 설정
	 * 
	 * @param InTag 설정할 모드 태그
	 */
	void SetMode(const FGameplayTag& InTag);
	
	/**
	 * @brief 무기 타입 변경
	 * 
	 * @param InTag 변경할 타입 태그
	 */
	void ChangeType(const FGameplayTag& InTag);

private:
	/**
	 * @brief 소유자 캐릭터 참조
	 */
	class ACharacter* OwnerCharacter;
	
	/**
	 * @brief 상태 컴포넌트 참조
	 */
	class UCStateComponent* State;
	

	/**
	 * @brief 현재 무기 모드
	 */
	UPROPERTY(EditAnywhere)
	FGameplayTag CurrentWeaponMode;

	/**
	 * @brief 맨손 태그
	 */
	UPROPERTY()
	FGameplayTag UnarmedTag;
	
	/**
	 * @brief 무기 모드 변경 델리게이트
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeaponModeChanged, FGameplayTag, InPrevMode, FGameplayTag, InNewMode);
	UPROPERTY(BlueprintAssignable, Category = "Weapon Mode")
	FWeaponModeChanged OnWeaponModeChanged;
};
