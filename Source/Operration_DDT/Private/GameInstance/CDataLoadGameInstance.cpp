// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/CDataLoadGameInstance.h"

// Fill out your copyright notice in the Description page of Project Settings.

#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "Global.h"

UCDataLoadGameInstance::UCDataLoadGameInstance()
{
    // API 기본 URL 설정
    ApiBaseUrl = TEXT("http://localhost:3000");
}

void UCDataLoadGameInstance::Init()
{
    Super::Init();
    
    // 게임 시작 시 태그 로드
    LoadTagsFromAPI();
}

void UCDataLoadGameInstance::LoadTagsFromAPI()
{
    // HTTP 요청 생성 (IHttpRequest 사용)
    TagRequest = FHttpModule::Get().CreateRequest();
    TagRequest->SetVerb("GET");
    TagRequest->SetURL(ApiBaseUrl + TEXT("/api/tags"));
    TagRequest->OnProcessRequestComplete().BindUObject(this, &UCDataLoadGameInstance::OnTagsReceived);
    TagRequest->ProcessRequest();

    CLog::Print("태그 API 요청 전송: " + ApiBaseUrl + TEXT("/api/tags"));
}

void UCDataLoadGameInstance::OnTagsReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccess)
{
    if (bSuccess && Response.IsValid())
    {
        FString JsonString = Response->GetContentAsString();
        ProcessTagsData(JsonString);
    }
    else
    {
        CLog::Log("태그 API 요청 실패");
    }
}

void UCDataLoadGameInstance::ProcessTagsData(const FString& JsonString)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
    
    if (FJsonSerializer::Deserialize(Reader, JsonObject))
    {
        bool bSuccess = JsonObject->GetBoolField("success");
        if (bSuccess)
        {
            // 태그 배열 처리
            const TArray<TSharedPtr<FJsonValue>>* TagsArray;
            if (JsonObject->TryGetArrayField("tags", TagsArray))
            {
                LoadedTags.Empty();
                
                for (const TSharedPtr<FJsonValue>& TagValue : *TagsArray)
                {
                    const TSharedPtr<FJsonObject>& TagObject = TagValue->AsObject();
                    
                    FString TagName = TagObject->GetStringField("tag");
                    FString Category = TagObject->GetStringField("category");
                    FString Description = TagObject->GetStringField("description");
                    int32 TagID = TagObject->GetIntegerField("sortOrder");
                    
                    // GameplayTag 생성
                    FGameplayTag NewTag = FGameplayTag::RequestGameplayTag(*TagName);
                    if (NewTag.IsValid())
                    {
                        LoadedTags.Add(NewTag);
                        
                        CLog::Print(FString::Printf(TEXT("태그 생성: %s (ID: %d, 카테고리: %s)"), 
                            *TagName, TagID, *Category));
                    }
                }
                
                // 태그 로드 완료 이벤트 발생
                OnTagsLoaded.Broadcast(LoadedTags);
                
                CLog::Print(FString::Printf(TEXT("총 %d개의 태그를 성공적으로 가져왔습니다."), LoadedTags.Num()));
            }
        }
        else
        {
            FString ErrorMessage = JsonObject->GetStringField("error");
            CLog::Log("API 오류: " + ErrorMessage);
        }
    }
    else
    {
        CLog::Log("JSON 파싱 실패");
    }
}

TArray<FGameplayTag> UCDataLoadGameInstance::GetTagsByCategory(const FString& Category) const
{
    TArray<FGameplayTag> CategoryTags;
    
    // 카테고리별 태그 필터링 (구현 필요)
    // 현재는 전체 태그 반환
    CategoryTags = LoadedTags;
    
    return CategoryTags;
}