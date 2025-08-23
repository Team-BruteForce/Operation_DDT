// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Component/CBossStatusComponent.h"

#include "Boss/Data/TBossStats.h"
#include "Global.h"

UCBossStatusComponent::UCBossStatusComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CHelpers::GetAsset<UDataTable>(&BossStatsTable,AssetPaths::Boss_StatsDataTable);
}


// Called when the game starts
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
			BossCurrentStats.CurrentAP = BossData->MaxAP;
			BossCurrentStats.ConstATK = BossData->ATK;
			BossCurrentStats.CurrentATK = BossData->ATK;
			BossCurrentStats.ConstDEF = BossData->DEF;
			BossCurrentStats.CurrentDEF = BossData->DEF;
			BossCurrentStats.MaxSpeed = BossData->Speed;
		}
	}
}


// Called every frame
void UCBossStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

