// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DDTGameMode.generated.h"

/**
 * 
 */
UCLASS()
class OPERRATION_DDT_API ADDTGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	ADDTGameMode();
	
	virtual void BeginPlay() override;
	
	class ADDTPlayer* OwnerCharater;


	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> UCPlayerUIWidget;

	UPROPERTY()
	class UCPlayerUI* playerUI;

	// 드랍 아이템 이펙트 풀 매니저
	UPROPERTY()
	class ACDropItemEffectPoolManager* DropItemEffectPoolManager;

	void test();
	
};
