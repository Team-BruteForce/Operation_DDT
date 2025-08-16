// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "Components/ActorComponent.h"
#include "BossStateComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTagChanged,FGameplayTag,Prev,FGameplayTag,NewTag);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UBossStateComponent : public UActorComponent, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	UBossStateComponent();

	// IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override { TagContainer.AppendTags(OwnedTags); }

	UFUNCTION(BlueprintCallable) void SetStateTag(FGameplayTag NewState, bool bSendEvent=true);
	UFUNCTION(BlueprintCallable) void AddTag(FGameplayTag Tag,bool bSendEvent=false);
	UFUNCTION(BlueprintCallable) void RemoveTag(FGameplayTag Tag,bool bSendEvent=false);
	UFUNCTION(BlueprintPure)  bool HasTag(FGameplayTag Tag) const { return OwnedTags.HasTag(Tag); }
	UFUNCTION(BlueprintPure)  FGameplayTag GetStateTag() const { return CurrentState; }

	/** 편의용: 프로젝트 세팅>GameplayTags에 등록해둔 태그를 노출해두고 인스펙터에서 지정 */
	UPROPERTY(EditDefaultsOnly,Category="StateTags")
	FGameplayTag Tag_State_Idle;
	UPROPERTY(EditDefaultsOnly,Category="StateTags")
	FGameplayTag Tag_State_Action;
	UPROPERTY(BlueprintAssignable)
	FStateTagChanged OnStateTagChanged;
	
protected:
	virtual void BeginPlay() override;

private:
	void SendEventToStateTree(const FGameplayTag& Tag);

private:
	UPROPERTY(VisibleAnywhere,Category="State")
	FGameplayTag CurrentState;

	UPROPERTY(VisibleAnywhere,Category="State")
	FGameplayTagContainer OwnedTags;

	UPROPERTY()
	class UStateTreeComponent* CachedStateTree = nullptr;
	UPROPERTY()
	class APawn* Owner;
	UPROPERTY()
	class AAIController* AIC;
};
