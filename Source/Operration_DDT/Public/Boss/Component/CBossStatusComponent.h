/**
 * @file CBossStatusComponent.h
 * @brief 보스 스탯 관리 컴포넌트 헤더 파일
 * 
 * @details
 * 보스 캐릭터의 스탯(체력, 공격력, 방어력 등)을 관리하는 컴포넌트를 정의합니다.
 * 데이터 테이블에서 기본 스탯을 로드하고, 런타임에서 현재 스탯을 추적합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CBossStatusComponent.generated.h"

/**
 * @brief 보스 현재 스탯 구조체
 * 
 * 보스의 현재 스탯 정보를 저장하는 구조체입니다.
 */
USTRUCT()
struct FBossCurrentStats
{
	GENERATED_BODY()
public:
	/**
	 * @brief 최대 체력
	 */
	UPROPERTY()
	int32 MaxHP;

	/**
	 * @brief 현재 체력
	 */
	UPROPERTY()
	int32 CurrentHP;

	/**
	 * @brief 최대 액션 포인트
	 */
	UPROPERTY()
	float MaxAP;

	/**
	 * @brief 액션 포인트 증가량
	 */
	UPROPERTY()
	float IncreaseAP;

	/**
	 * @brief 현재 액션 포인트
	 */
	UPROPERTY()
	float CurrentAP;

	/**
	 * @brief 기본 공격력
	 */
	UPROPERTY()
	int32 ConstATK;

	/**
	 * @brief 현재 공격력
	 */
	UPROPERTY()
	int32 CurrentATK;

	/**
	 * @brief 기본 방어력
	 */
	UPROPERTY()
	int32 ConstDEF;

	/**
	 * @brief 현재 방어력
	 */
	UPROPERTY()
	int32 CurrentDEF;

	/**
	 * @brief 최대 속도
	 */
	UPROPERTY()
	int32 MaxSpeed;

	/**
	 * @brief 현재 속도
	 */
	UPROPERTY()
	int32 CurrentSpeed;

	/**
	 * @brief 최대 그로기 게이지
	 */
	UPROPERTY()
	float MaxGroggyGauge;

	/**
	 * @brief 현재 그로기 게이지
	 */
	UPROPERTY()
	float CurrentGroggyGauge; 
};

/**
 * @brief 보스 스탯 관리 컴포넌트
 * 
 * 보스 캐릭터의 스탯을 관리하는 컴포넌트입니다.
 * 데이터 테이블에서 기본 스탯을 로드하고 런타임에서 현재 스탯을 추적합니다.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCBossStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	/**
	 * @brief 보스 스탯 컴포넌트 생성자
	 */
	UCBossStatusComponent();

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

	/**
	 * @brief 액션 포인트 증가
	 * 
	 * @param AP 증가할 액션 포인트량
	 */
	void IncreaseAP(float AP);

	void SetDamage(float Damage, bool isCritical);

	void ResetAp();

	FORCEINLINE void OnGroggy(){IsGroggy=true;};
	FORCEINLINE void OffGroggy(){IsGroggy=false;};
	bool const GetIsGroggy  (){return IsGroggy; };

	UPROPERTY()
	bool IsGroggy=false;

	FORCEINLINE void OnPaseChange(){IsPaseChange=true;};
	FORCEINLINE void OffPaseChange(){IsPaseChange=false;};
	bool const GetIsPaseChange  (){return IsPaseChange; };

	UPROPERTY()
	bool IsPaseChange=false;
	/**
	 * @brief 그로기 게이지 증가
	 * 
	 * @param GroggyAmount 증가할 그로기 게이지량
	 */
	void IncreaseGroggyGauge(float GroggyAmount);

	/**
	 * @brief 그로기 게이지 감소
	 * 
	 * @param GroggyAmount 감소할 그로기 게이지량
	 */
	void DecreaseGroggyGauge(float GroggyAmount);

	/**
	 * @brief 그로기 게이지 초기화
	 */
	void ResetGroggyGauge();

	/**
	 * @brief 보스 스탯 완전 초기화 (매니저용)
	 * 
	 * @details
	 * 모든 스탯을 데이터 테이블 기본값으로 초기화합니다.
	 * 보스 매니저에서 보스 리셋 시 사용됩니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "Boss Reset")
	void ResetAllStats();

private:
	/**
	 * @brief AI 컴포넌트들 안전 초기화
	 * 
	 * AI 컨트롤러가 준비되었을 때 안전하게 초기화합니다.
	 */
	void InitializeAIComponents();
public:
	/**
	 * @brief 보스 스탯 데이터 테이블
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss Stats")
	UDataTable* BossStatsTable;

	/**
	 * @brief 보스 현재 스탯
	 */
	UPROPERTY(EditAnywhere)
	FBossCurrentStats BossCurrentStats;

	/**
	 * @brief AI 컨트롤러 참조
	 */
	UPROPERTY()
	class AAIController* AIC;
	
	/**
	 * @brief 소유자 Pawn 참조
	 */
	UPROPERTY()
	class APawn* Owner;
	
	/**
	 * @brief StateTree AI 컴포넌트 참조
	 */
	UPROPERTY()
	class UStateTreeAIComponent* StateTreeComp;
	
};
