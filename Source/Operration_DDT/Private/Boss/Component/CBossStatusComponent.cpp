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
		}
	}
	Owner=Cast<APawn>(GetOwner());
	AIC=Cast<AAIController>(Owner->GetController());
	StateTreeComp=CHelpers::GetComponent<UStateTreeAIComponent>(AIC);
}

/**
 * @brief 매 프레임 호출되는 틱 함수
 * 
 * 액션 포인트를 증가시키고 최대치에 도달하면 이벤트를 발생시킵니다.
 */
void UCBossStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (BossCurrentStats.CurrentAP>=BossCurrentStats.MaxAP){
		BossCurrentStats.CurrentAP=0;
		StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag("BOSS.Flag.IsActing"));
	}
	IncreaseAP(BossCurrentStats.IncreaseAP);
	// CLog::Log(BossCurrentStats.CurrentHP);
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
	BossCurrentStats.CurrentAP = BossCurrentStats.CurrentAP + AP;
}

