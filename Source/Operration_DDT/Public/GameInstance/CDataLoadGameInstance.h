// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/GameInstance.h"
#include "Interfaces/IHttpRequest.h"
#include "CDataLoadGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTagsLoaded, const TArray<FGameplayTag>&, Tags);
UCLASS()
class OPERRATION_DDT_API UCDataLoadGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UCDataLoadGameInstance();
protected:
	virtual void Init() override;

public:
	UPROPERTY(BlueprintReadOnly, Category="Tags")
	TArray<FGameplayTag> LoadedTags;

	UPROPERTY(BlueprintAssignable, Category="Tags")
	FOnTagsLoaded OnTagsLoaded;

	UFUNCTION(BlueprintPure, Category="Tags")
	TArray<FGameplayTag> GetTags() const { return LoadedTags; }

	UFUNCTION(BlueprintPure, Category="Tags")
	TArray<FGameplayTag> GetTagsByCategory(const FString& Category) const;

	UFUNCTION(BlueprintPure, Category="Tags")
	bool IsTagsLoaded() const { return LoadedTags.Num() > 0; }

private:
	TSharedPtr<IHttpRequest> TagRequest;

	void OnTagsReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess);

	void ProcessTagsData(const FString& JsonString);

	UPROPERTY(EditAnywhere, Category="API")
	FString ApiBaseUrl;

	void LoadTagsFromAPI();
};
