/**
 * @file CBossStatusComponent.cpp
 * @brief 보스 스탯 관리 컴포넌트 구현 파일
 * 
 * @details
 * 보스 캐릭터의 스탯을 관리하는 컴포넌트의 구현 파일입니다.
 * 데이터 테이블에서 기본 스탯을 로드하고 런타임에서 현재 스탯을 추적합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#include "Boss/Component/CBossStatusComponent.h"

#include "AIController.h"
#include "Boss/Data/TBossStats.h"
#include "Global.h"
#include "Components/StateTreeAIComponent.h"

/**
 * @brief 보스 스탯 컴포넌트 생성자
 * 
 * 컴포넌트의 기본 설정을 초기화합니다.
 */
UCBossStatusComponent::UCBossStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CHelpers::GetAsset<UDataTable>(&BossStatsTable,AssetPaths::Boss_StatsDataTable);
}

/**
 * @brief 게임 시작 시 호출되는 함수
 * 
 * 데이터 테이블에서 보스 스탯을 로드하고 초기화합니다.
 */
void UCBossStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	if (BossStatsTable)
	{
		FTBossStats* BossData = BossStatsTable->FindRow<FTBossStats>("Boss","",true);
		if (BossData)
		{
			BossCurrentStats.MaxHP = BossData->MaxHP;
			BossCurrentStats.CurrentHP = BossData->MaxHP;
			BossCurrentStats.MaxAP = BossData->MaxAP;
			BossCurrentStats.IncreaseAP = BossData->IncreaseAP;
			BossCurrentStats.CurrentAP = 0;
			BossCurrentStats.ConstATK = BossData->ATK;
			BossCurrentStats.CurrentATK = BossData->ATK;
			BossCurrentStats.ConstDEF = BossData->DEF;
			BossCurrentStats.CurrentDEF = BossData->DEF;
			BossCurrentStats.MaxSpeed = BossData->Speed;
			BossCurrentStats.MaxGroggyGauge = BossData->MaxGroggyGauge;
			BossCurrentStats.CurrentGroggyGauge = 0.0f;
		}
	}
	Owner = Cast<APawn>(GetOwner());
	// AI 컨트롤러는 지연 초기화 (필요할 때 InitializeAIComponents에서 처리)
}

/**
 * @brief 매 프레임 호출되는 틱 함수
 * 
 * 액션 포인트를 증가시키고 최대치에 도달하면 이벤트를 발생시킵니다.
 */
void UCBossStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// AI 컴포넌트가 초기화되지 않았다면 시도
	if (!StateTreeComp && Owner)
	{
		InitializeAIComponents();
	}
	
	// StateTreeComp가 유효할 때만 실행
	if (StateTreeComp)
	{
		if (BossCurrentStats.CurrentAP >= BossCurrentStats.MaxAP)
		{
			StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag("BOSS.Flag.IsActing"));
		}
		else
		{
			IncreaseAP(BossCurrentStats.IncreaseAP);
		}
	}
	else
	{
		// StateTreeComp가 없어도 AP는 증가시킴
		IncreaseAP(BossCurrentStats.IncreaseAP);
	}
}

/**
 * @brief 액션 포인트 증가
 * 
 * @param AP 증가할 액션 포인트량
 * 
 * 현재 액션 포인트에 지정된 양을 더합니다.
 */
void UCBossStatusComponent::IncreaseAP(float AP)
{
	if (BossCurrentStats.CurrentAP>=BossCurrentStats.MaxAP)
		BossCurrentStats.CurrentAP = BossCurrentStats.MaxAP;
	else
		BossCurrentStats.CurrentAP += AP;
}

void UCBossStatusComponent::SetDamage(float Damage)
{
	if (!GetIsGroggy())
		BossCurrentStats.CurrentHP-=Damage-BossCurrentStats.CurrentDEF;
		
		// BossCurrentStats.CurrentHP-=Damage-BossCurrentStats.CurrentDEF;
	else
		BossCurrentStats.CurrentHP-=Damage;
}

void UCBossStatusComponent::ResetAp()
{
	BossCurrentStats.CurrentAP =0;
}

/**
 * @brief 그로기 게이지 증가
 * 
 * @param GroggyAmount 증가할 그로기 게이지량
 * 
 * 현재 그로기 게이지에 지정된 양을 더합니다.
 * 최대치를 초과하지 않도록 제한합니다.
 */
void UCBossStatusComponent::IncreaseGroggyGauge(float GroggyAmount)
{
	if (BossCurrentStats.CurrentGroggyGauge >= BossCurrentStats.MaxGroggyGauge)
		BossCurrentStats.CurrentGroggyGauge = BossCurrentStats.MaxGroggyGauge;
	else
		BossCurrentStats.CurrentGroggyGauge += GroggyAmount*0.1;
}

/**
 * @brief 그로기 게이지 감소
 * 
 * @param GroggyAmount 감소할 그로기 게이지량
 * 
 * 현재 그로기 게이지에서 지정된 양을 뺍니다.
 * 0 이하로 내려가지 않도록 제한합니다.
 */
void UCBossStatusComponent::DecreaseGroggyGauge(float GroggyAmount)
{
	if (BossCurrentStats.CurrentGroggyGauge <= 0.0f)
		BossCurrentStats.CurrentGroggyGauge = 0.0f;
	else
		BossCurrentStats.CurrentGroggyGauge -= GroggyAmount;
}

/**
 * @brief 그로기 게이지 초기화
 * 
 * 현재 그로기 게이지를 0으로 설정합니다.
 */
void UCBossStatusComponent::ResetGroggyGauge()
{
	BossCurrentStats.CurrentGroggyGauge = 0.0f;
}

/**
 * @brief 보스 스탯 완전 초기화 (매니저용)
 * 
 * 모든 스탯을 데이터 테이블 기본값으로 초기화합니다.
 * 보스 매니저에서 보스 리셋 시 사용됩니다.
 */
void UCBossStatusComponent::ResetAllStats()
{
	if (BossStatsTable)
	{
		FTBossStats* BossData = BossStatsTable->FindRow<FTBossStats>("Boss","",true);
		if (BossData)
		{
			// 모든 스탯을 데이터 테이블 값으로 초기화
			BossCurrentStats.MaxHP = BossData->MaxHP;
			BossCurrentStats.CurrentHP = BossData->MaxHP;
			BossCurrentStats.MaxAP = BossData->MaxAP;
			BossCurrentStats.IncreaseAP = BossData->IncreaseAP;
			BossCurrentStats.CurrentAP = 0.0f;
			BossCurrentStats.ConstATK = BossData->ATK;
			BossCurrentStats.CurrentATK = BossData->ATK;
			BossCurrentStats.ConstDEF = BossData->DEF;
			BossCurrentStats.CurrentDEF = BossData->DEF;
			BossCurrentStats.MaxSpeed = BossData->Speed;
			BossCurrentStats.CurrentSpeed = BossData->Speed;
			BossCurrentStats.MaxGroggyGauge = BossData->MaxGroggyGauge;
			BossCurrentStats.CurrentGroggyGauge = 0.0f;
			
			// 상태 플래그 초기화
			IsGroggy = false;
			IsPaseChange = false;
			
			UE_LOG(LogTemp, Warning, TEXT("보스 스탯 완전 초기화 완료"));
		}
	}
}

/**
 * @brief AI 컴포넌트들 안전 초기화
 * 
 * AI 컨트롤러가 준비되었을 때 안전하게 초기화합니다.
 */
void UCBossStatusComponent::InitializeAIComponents()
{
	if (!Owner) return;
	
	// AI 컨트롤러가 준비될 때까지 기다림
	AIC = Cast<AAIController>(Owner->GetController());
	if (AIC)
	{
		StateTreeComp = CHelpers::GetComponent<UStateTreeAIComponent>(AIC);
		if (StateTreeComp)
		{
			UE_LOG(LogTemp, Warning, TEXT("BossStatusComponent: AI 컴포넌트 초기화 완료"));
		}
	}
}

