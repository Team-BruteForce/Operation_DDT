/**
 * @file BossTagStructure.h
 * @brief 보스 태그 구조체 헤더 파일
 * 
 * @details
 * 보스의 태그 관련 구조체들을 정의하는 파일입니다.
 * 보스의 상태와 행동을 정의하는 태그들을 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

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
