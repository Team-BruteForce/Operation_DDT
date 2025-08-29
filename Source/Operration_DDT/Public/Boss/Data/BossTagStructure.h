// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameplayTagContainer.h"
#include "BossTagStructure.generated.h"

USTRUCT(BlueprintType)
struct FBossRangeTags {
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.Range.Bucket")) FGameplayTag TooClose;
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.Range.Bucket")) FGameplayTag Melee;
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.Range.Bucket")) FGameplayTag Dash;
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.Range.Bucket")) FGameplayTag Ranged;
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.Range.Bucket")) FGameplayTag OutOfRange;
};

USTRUCT(BlueprintType)
struct FBossTargetState {
	GENERATED_BODY()
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.TargetDirection.Right")) FGameplayTag Right;
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.TargetDirection.Center")) FGameplayTag Center;
	UPROPERTY(EditDefaultsOnly, meta=(Categories="BOSS.Flag.TargetDirection.Left")) FGameplayTag Left;
};

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API UBossTagStructure : public UObject
{
	GENERATED_BODY()
	
};
