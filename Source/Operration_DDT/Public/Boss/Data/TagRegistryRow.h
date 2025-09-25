/**
 * @file TagRegistryRow.h
 * @brief 태그 레지스트리 행 헤더 파일
 * 
 * @details
 * 게임플레이 태그의 레지스트리 행을 정의하는 파일입니다.
 * 태그의 카테고리와 메타데이터를 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "TagRegistryRow.generated.h"

// CSV의 Category 열과 1:1
UENUM(BlueprintType)
enum class ETagCategory : uint8
{
	State,
	Action,
	Event,
	Flag,
	Sense,
	Other
};

// CSV 컬럼과 이름/타입이 정확히 일치해야 함
USTRUCT(BlueprintType)
struct FTagRegistryRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayTag Tag;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString      Category; // 혹은 ETagCategory
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayTagContainer ParentTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FText        DisplayName;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(MultiLine="true"))
	FText        Description;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32        SortOrder = 0;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool         bDeprecated = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayTag RedirectTo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayTagContainer Grants;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FGameplayTagContainer Requires;

	UPROPERTY(EditAnywhere, BlueprintReadOnly) FLinearColor DebugColor = FLinearColor::White;
};
