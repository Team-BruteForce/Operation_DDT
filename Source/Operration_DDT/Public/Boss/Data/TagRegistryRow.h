// TagRegistryRow.h
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
