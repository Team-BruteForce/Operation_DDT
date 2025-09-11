// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Components/CMontageComponent.h"
#include "Global.h"
#include "SkeletalDebugRendering.h"
#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Runtime/Json/Public/Dom/JsonObject.h"
#include "Runtime/Json/Public/Serialization/JsonReader.h"
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
UCMontageComponent::UCMontageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UCMontageComponent::BeginPlay()
{
	Super::BeginPlay();

	// OwnerCharacter 초기화
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	
}

void UCMontageComponent::FetchGoogleSheetData()
{
	// http module ��������
	FHttpModule* Http = &FHttpModule::Get();

	// http request ���� 
	//TSharedRef<IHttpRequest> Request = Http->CreateRequest ();
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	// Google Sheet API URL ����

	FString SheetID = TEXT("1sQaPJZixoOduqmINaVAUQymC_2Ca3PkdJ8h_2VYqZU8");
	/*
		Sheet ID
		https://docs.google.com/spreadsheets/d/<SPREADSHEET_ID>/edit#gid=0
		���� Sheet ����Ʈ���� <> ���� �ִ� �κ��� ID
	*/
	FString SheetRange = TEXT("Sheet1!A1:A4"); // <��Ʈ�̸�>!<���ۼ�>:<����>
	FString APIKey = TEXT("AIzaSyA1I36UVo9LXNpMos65iOV8IMTcFDzxWN4");
	FString Url = FString::Printf(TEXT("https://sheets.googleapis.com/v4/spreadsheets/%s/values/%s?key=%s"), *SheetID, *SheetRange, *APIKey);

	Request->SetURL(Url);
	Request->SetVerb ("Get");
	Request->SetHeader (TEXT("Content-Type"), TEXT("application/json"));

	Request->OnProcessRequestComplete().BindUObject(this, &UCMontageComponent::OnResponseReceived);

	Request->ProcessRequest ();

}


// Called every frame
void UCMontageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCMontageComponent::PlayRollingMode()
{
	PlayAnimMontage(EStateType::Rolling);
}

void UCMontageComponent::PlayRollingMode(FVector InDirection)
{
	CheckNull(OwnerCharacter);
	
	// 입력 방향을 정규화
	FVector NormalizedDirection = InDirection.GetSafeNormal();
	

	FVector Forward = OwnerCharacter->GetActorForwardVector();
	FVector Right = OwnerCharacter->GetActorRightVector();

	float ForwardComponent = FVector::DotProduct(Forward, NormalizedDirection);
	float RightComponent = FVector::DotProduct(Right, NormalizedDirection);
	
	// 주 방향에 따라 적절한 몽타주 선택
	UAnimMontage* SelectedMontage = nullptr;
	
	// 절댓값이 더 큰 방향을 우선으로 선택
	if (FMath::Abs(ForwardComponent) > FMath::Abs(RightComponent))
	{
		// 전후 방향이 우선
		if (ForwardComponent > 0.3f)
		{
			SelectedMontage = ForwardRollMontage;
		}
		else if (ForwardComponent < -0.3f)
		{
			SelectedMontage = BackwardRollMontage;
		}
	}
	else
	{
		// 좌우 방향이 우선
		if (RightComponent > 0.3f)
		{
			SelectedMontage = RightRollMontage;

		}
		else if (RightComponent < -0.3f)
		{
			SelectedMontage = LeftRollMontage;
		}
	}
	
	// 기본 몽타주가 없으면 기존 방식 사용
	if (SelectedMontage == nullptr)
	{
		PlayAnimMontage(EStateType::Rolling);
		return;
	}
	
	// 선택된 몽타주 재생
	OwnerCharacter->PlayAnimMontage(SelectedMontage, 1.0f);

	
}

void UCMontageComponent::PlayHittedMode()
{
	PlayAnimMontage(EStateType::Hitted);
}

void UCMontageComponent::PlayDeadMode()
{
	PlayAnimMontage(EStateType::Dead);
	DieDelegate.Broadcast();
}

void UCMontageComponent::PlayReloadMode()
{
	PlayAnimMontage(EStateType::RifleReload);
}

void UCMontageComponent::PlayHealingMode()
{
	PlayAnimMontage(EStateType::Healing);
}

void UCMontageComponent::PlayAnimMontage(EStateType InType)
{
	CheckNull(OwnerCharacter);

	FMontageData& target = Data[(int8)InType];
	
	if (target.Montage == nullptr)
	{
		GLog->Log(ELogVerbosity::Error , "No Montage Data");
		return;
	}

	OwnerCharacter->PlayAnimMontage(target.Montage, target.PlayRate);
}

void UCMontageComponent::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || Response.IsValid ()) return;

	FString ResponseString = Response->GetContentAsString();

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		// JSON���� values �迭 ��������
		const TArray<TSharedPtr<FJsonValue>>* Values;
		if (JsonObject->TryGetArrayField(TEXT("values"), Values))
		{
			for (const TSharedPtr<FJsonValue>& Row : *Values)
			{
				const TArray<TSharedPtr<FJsonValue>>* RowData;
				if (Row->TryGetArray(RowData))
				{
					FString IndexStr;
					if ((*RowData)[0]->TryGetString(IndexStr))
					{	
						// Ÿ�� ����
						FString TypeStr;
						EStateType newType = EStateType::Max;
						if ((*RowData)[1]->TryGetString(TypeStr))
						{
							if (TypeStr == TEXT("Idle")) newType = EStateType::Idle;
							else if(TypeStr == TEXT("Rolling")) newType = EStateType::Rolling;
							//else if (TypeStr == TEXT("BackStep")) newType = EStateType::BackStep;
							else if (TypeStr == TEXT("Hitted")) newType = EStateType::Hitted;
							else if (TypeStr == TEXT("Dead")) newType = EStateType::Dead;
							else if (TypeStr == TEXT("Action")) newType = EStateType::Action;

							Data[(int8)newType].Type = newType;

						}

						// ��Ÿ�� ��� ����
						FString MontagePath;
						if ((*RowData)[2]->TryGetString(MontagePath))
						{
							Data[(int8)newType].Montage = LoadObject<UAnimMontage>(nullptr, *MontagePath);
						}

						// PlayRate
						FString PlayRateStr;
						if ((*RowData)[3]->TryGetString(PlayRateStr))
						{
							Data[(int8)newType].PlayRate = FCString::Atof(*PlayRateStr);
						}



					}
					

				}
			}
		}

	}
}

