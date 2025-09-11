// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.h"
#include "Engine/DataTable.h"
#include "Runtime/Online/HTTP/Public/Http.h"
/*
#include "HttpFwd.h"
#include "HttpModule.h"
#include "interfaces/IHttpRequest.h"
#include "interfaces/IHttpResponse.h"
#include "Json.h"
#include "JsonUtilities.h"
*/
#include "CMontageComponent.generated.h"

USTRUCT(BlueprintType)
struct FMontageData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	EStateType Type;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;


};

//DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDataFetched);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDieDelegate);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class OPERRATION_DDT_API UCMontageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCMontageComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	
	UFUNCTION(BlueprintCallable, Category = "HTTP")
	void FetchGoogleSheetData();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void PlayRollingMode();
	void PlayRollingMode(FVector InDirection);

	void PlayHittedMode();
	void PlayDeadMode();

	void PlayReloadMode();
	void PlayHealingMode();


private:
	void PlayAnimMontage(EStateType InType);
	class ACharacter* OwnerCharacter;

	// 방향별 구르기 몽타주
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* ForwardRollMontage;
	
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* BackwardRollMontage;
	
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* LeftRollMontage;
	
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* RightRollMontage;


public:
	FOnDieDelegate DieDelegate;

public:
	UPROPERTY(EditDefaultsOnly)
	FMontageData Data[(int32)EStateType::Max];
	//FOnDataFetched OnDataFetched;

protected:
	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

		
};
