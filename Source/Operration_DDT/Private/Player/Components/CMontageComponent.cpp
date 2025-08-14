// Fill out your copyright notice in the Description page of Project Settings.

#include "Player/Components/CMontageComponent.h"
#include "Global.h"
#include "Runtime/Online/HTTP/Public/HttpModule.h"
#include "Runtime/Json/Public/Dom/JsonObject.h"
#include "Runtime/Json/Public/Serialization/JsonReader.h"
#include "Runtime/Json/Public/Serialization/JsonSerializer.h"
#include "GameFramework/Character.h"

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

	// ...
	
}

void UCMontageComponent::FetchGoogleSheetData()
{
	// http module 가져오기
	FHttpModule* Http = &FHttpModule::Get();

	// http request 생성 
	//TSharedRef<IHttpRequest> Request = Http->CreateRequest ();
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	// Google Sheet API URL 설정

	FString SheetID = TEXT("1sQaPJZixoOduqmINaVAUQymC_2Ca3PkdJ8h_2VYqZU8");
	/*
		Sheet ID
		https://docs.google.com/spreadsheets/d/<SPREADSHEET_ID>/edit#gid=0
		구글 Sheet 사이트에서 <> 내의 있는 부분이 ID
	*/
	FString SheetRange = TEXT("Sheet1!A1:A4"); // <시트이름>!<시작셀>:<끝셀>
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

void UCMontageComponent::PlaySlidingMode()
{

}

void UCMontageComponent::PlayAnimMontage(EStateType InType)
{
	CheckNull(OwnerCharacter);

	FMontageData* target = Data[(int8)InType];
	
	if (target == nullptr || target->Montage == nullptr)
	{
		GLog->Log(ELogVerbosity::Error , "No Montage Data");
		return;
	}

	OwnerCharacter->PlayAnimMontage(target->Montage, target->PlayRate);
}

void UCMontageComponent::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || Response.IsValid ()) return;

	FString ResponseString = Response->GetContentAsString();

	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseString);

	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		// JSON에서 values 배열 가져오기
		const TArray<TSharedPtr<FJsonValue>>* Values;
		if (JsonObject->TryGetArrayField(TEXT("values"), Values))
		{
			for (const TSharedPtr<FJsonValue>& Row : *Values)
			{
				const TArray<TSharedPtr<FJsonValue>>* RowData;
				if (Row->TryGetArray(RowData))
				{
					FMontageData* NewData = new FMontageData();

					FString IndexStr;
					if ((*RowData)[0]->TryGetString(IndexStr))
					{	
						// 타입 설정
						FString TypeStr;
						EStateType newType = EStateType::Max;
						if ((*RowData)[1]->TryGetString(TypeStr))
						{
							if (TypeStr == TEXT("Idle")) newType = EStateType::Idle;
							else if(TypeStr == TEXT("Sliding")) newType = EStateType::Sliding;
							else if (TypeStr == TEXT("BackStep")) newType = EStateType::BackStep;
							else if (TypeStr == TEXT("Hitted")) newType = EStateType::Hitted;
							else if (TypeStr == TEXT("Dead")) newType = EStateType::Dead;
							else if (TypeStr == TEXT("Action")) newType = EStateType::Action;

							NewData->Type = newType;

						}

						// 몽타주 경로 설정
						FString MontagePath;
						if ((*RowData)[2]->TryGetString(MontagePath))
						{
							NewData->Montage = LoadObject<UAnimMontage>(nullptr, *MontagePath);
						}

						// PlayRate
						FString PlayRateStr;
						if ((*RowData)[3]->TryGetString(PlayRateStr))
						{
							NewData->PlayRate = FCString::Atof(*PlayRateStr);
						}

						Data[(int8)NewData->Type] = NewData;

					}
					

				}
			}
		}

	}
}

