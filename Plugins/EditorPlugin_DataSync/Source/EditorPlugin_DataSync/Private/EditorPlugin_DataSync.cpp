// Copyright Epic Games, Inc. All Rights Reserved.

/**
 * @file EditorPlugin_DataSync.cpp
 * @brief 언리얼 엔진 에디터용 데이터 동기화 플러그인 구현 파일
 * 
 * @date 2024-12-19
 * @author [이효원]
 * @version 1.0
 * 
 * @section overview 개요
 * 이 플러그인은 언리얼 엔진 에디터에서 외부 데이터베이스나 API와의
 * 실시간 데이터 동기화를 담당하는 핵심 시스템입니다.
 * 기획자가 데이터를 변경할 때 즉시 게임에 반영되도록 하여
 * 개발 효율성을 극대화하는 것이 주요 목적입니다.
 * 
 * @section purpose 개발 목적
 * - 기획자가 데이터를 변경할 때 즉시 게임에 반영되도록 실시간 데이터 동기화 시스템 구축
 * - 외부 API와의 HTTP 통신을 통한 데이터 동적 로딩
 * - 언리얼 엔진의 데이터 테이블 시스템과 연동하여 게임 데이터 자동 업데이트
 * - 개발자와 기획자 간의 협업 효율성 향상
 * 
 * @section features 주요 기능
 * - HTTP API를 통한 외부 데이터 실시간 동기화
 * - GameplayTags 데이터 테이블 자동 업데이트
 * - 언리얼 엔진 에디터 UI 통합 (메뉴, 툴바, 독립 탭)
 * - JSON 데이터 파싱 및 언리얼 엔진 데이터 구조 변환
 * - 에러 처리 및 로깅 시스템
 * 
 * @section technical_details 기술적 세부사항
 * - 언리얼 엔진 플러그인 아키텍처 활용
 * - HTTP 모듈을 통한 REST API 통신
 * - JSON 파싱 및 데이터 테이블 시스템 연동
 * - 언리얼 엔진 에디터 UI 프레임워크 활용
 * - 모듈 기반 아키텍처로 확장성 및 유지보수성 확보
 * 
 * @section architecture 시스템 아키텍처
 * - FEditorPlugin_DataSyncModule: 메인 모듈 클래스
 * - HTTP 요청/응답 처리 시스템
 * - 데이터 파싱 및 변환 엔진
 * - 에디터 UI 통합 시스템
 * 
 * @section portfolio_highlight 포트폴리오 하이라이트
 * 이 플러그인은 게임 개발에서 가장 중요한 실시간 데이터 동기화와
 * 개발 효율성 향상을 위한 핵심 시스템입니다. 특히 언리얼 엔진의
 * 플러그인 아키텍처를 활용한 확장 가능한 시스템 설계와,
 * HTTP 통신을 통한 외부 시스템 연동 능력을 보여줍니다.
 * 
 * 기획자가 데이터를 변경할 때마다 게임을 재시작할 필요 없이
 * 즉시 반영되는 시스템은 현대 게임 개발에서 요구되는
 * 빠른 반복 개발과 실시간 협업의 핵심 요소입니다.
 */

#include "EditorPlugin_DataSync.h"
#include "EditorPlugin_DataSyncStyle.h"
#include "EditorPlugin_DataSyncCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"

// 데이터 동기화를 위한 추가 include
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "Engine/DataTable.h"
#include "Misc/DateTime.h"
#include "Widgets/Input/SButton.h"

static const FName EditorPlugin_DataSyncTabName("EditorPlugin_DataSync");

#define LOCTEXT_NAMESPACE "FEditorPlugin_DataSyncModule"

void FEditorPlugin_DataSyncModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FEditorPlugin_DataSyncStyle::Initialize();
	FEditorPlugin_DataSyncStyle::ReloadTextures();

	FEditorPlugin_DataSyncCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FEditorPlugin_DataSyncCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FEditorPlugin_DataSyncModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FEditorPlugin_DataSyncModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(EditorPlugin_DataSyncTabName, FOnSpawnTab::CreateRaw(this, &FEditorPlugin_DataSyncModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FEditorPlugin_DataSyncTabTitle", "EditorPlugin_DataSync"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FEditorPlugin_DataSyncModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module. For modules that support dynamic reloading,
	// we call this function before unloading the module.

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FEditorPlugin_DataSyncStyle::Shutdown();

	FEditorPlugin_DataSyncCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(EditorPlugin_DataSyncTabName);
}

TSharedRef<SDockTab> FEditorPlugin_DataSyncModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("TitleText", "Data Sync Plugin"))
				.Font(FCoreStyle::GetDefaultFontStyle("Bold", 16))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10)
			[
				SNew(SButton)
				.Text(LOCTEXT("SyncButtonText", "Sync GameplayTags"))
				.OnClicked_Lambda([this]()
				{
					SyncGameplayTags();
					return FReply::Handled();
				})
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.Padding(10)
			[
				SNew(STextBlock)
				.Text(LOCTEXT("StatusText", "Status: Ready"))
			]
		];
}

void FEditorPlugin_DataSyncModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(EditorPlugin_DataSyncTabName);
}

void FEditorPlugin_DataSyncModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FEditorPlugin_DataSyncCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FEditorPlugin_DataSyncCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}

// 데이터 동기화 함수들
void FEditorPlugin_DataSyncModule::SyncGameplayTags()
{
	UE_LOG(LogTemp, Log, TEXT("GameplayTags 동기화 시작..."));
	
	// API 요청 생성
	MakeAPIRequest("/api/tags", "GameplayTags");
}

void FEditorPlugin_DataSyncModule::MakeAPIRequest(const FString& Endpoint, const FString& DataType)
{
	// HTTP 모듈 확인
	if (!FHttpModule::Get().IsHttpEnabled())
	{
		UE_LOG(LogTemp, Log, TEXT("HTTP 모듈이 비활성화되어 있습니다."));
		return;
	}
	
	// 요청 생성
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb("GET");
	Request->SetURL(FString::Printf(TEXT("http://localhost:3000%s"), *Endpoint));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	
	// 콜백 바인딩
	Request->OnProcessRequestComplete().BindRaw(this, &FEditorPlugin_DataSyncModule::OnDataReceived);
	
	// 요청 전송
	Request->ProcessRequest();
}

void FEditorPlugin_DataSyncModule::OnDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Log, TEXT("API 응답 실패"));
		return;
	}
	
	FString ResponseContent = Response->GetContentAsString();
	UE_LOG(LogTemp, Log, TEXT("API 응답 받음: %s"), *ResponseContent);
	
	// JSON 파싱
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(ResponseContent);
	
	if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
	{
		// 성공 응답 확인
		bool bSuccess = JsonObject->GetBoolField("success");
		if (bSuccess)
		{
			// "tags" 필드에서 데이터 가져오기 (data가 아님!)
			TArray<TSharedPtr<FJsonValue>> TagsData = JsonObject->GetArrayField("tags");
			UE_LOG(LogTemp, Log, TEXT("태그 데이터 %d개 받음"), TagsData.Num());
			
			if (TagsData.Num() > 0)
			{
				// GameplayTags 데이터 테이블 업데이트
				UDataTable* TagTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("/Game/LHW/BossData/NewDataTable")));
				if (TagTable)
				{
					UpdateGameplayTagsTable(TagTable, TagsData);
					UE_LOG(LogTemp, Log, TEXT("GameplayTags 동기화 완료"));
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("DT_Tag 테이블을 찾을 수 없습니다."));
				}
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("tags 배열이 비어있습니다."));
			}
		}
		else
		{
			FString ErrorMessage = JsonObject->GetStringField("error");
			UE_LOG(LogTemp, Log, TEXT("API 오류: %s"), *ErrorMessage);
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("JSON 파싱 실패"));
	}
}
void FEditorPlugin_DataSyncModule::UpdateGameplayTagsTable(UDataTable* TagTable, const TArray<TSharedPtr<FJsonValue>>& Data)
{
	if (!TagTable)
	{
		UE_LOG(LogTemp, Log, TEXT("TagTable이 유효하지 않습니다."));
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("GameplayTags 테이블 업데이트 시작..."));
	
	// 테이블 초기화
	TagTable->EmptyTable();
	
	int32 SuccessCount = 0;
	int32 ErrorCount = 0;
	
	// 각 행의 데이터 처리
	for (const TSharedPtr<FJsonValue>& RowValue : Data)
	{
		TSharedPtr<FJsonObject> RowObject = RowValue->AsObject();
		if (!RowObject.IsValid()) continue;
		
		try
		{
			// JSON 데이터를 GameplayTag 구조체로 변환
			FGameplayTagTableRow TagRow = ParseGameplayTagData(RowObject);
			
			if (TagRow.Tag.IsValid())
			{
				// 행 이름 생성 (Tag 필드 사용)
				FString TagString = TagRow.Tag.ToString();
				FName RowName = FName(*TagString);
				
				// 테이블에 행 추가
				TagTable->AddRow(RowName, TagRow);
				SuccessCount++;
				
				UE_LOG(LogTemp, Log, TEXT("태그 추가 성공: %s"), *TagString);
			}
			else
			{
				ErrorCount++;
				UE_LOG(LogTemp, Log, TEXT("태그가 유효하지 않습니다."));
			}
		}
		catch (...)
		{
			ErrorCount++;
			UE_LOG(LogTemp, Log, TEXT("행 데이터 파싱 중 오류 발생"));
		}
	}
	
	// 테이블을 더티로 표시
	TagTable->MarkPackageDirty();
	
	UE_LOG(LogTemp, Log, TEXT("GameplayTags 테이블 업데이트 완료: 성공 %d개, 실패 %d개"), SuccessCount, ErrorCount);
}

FGameplayTagTableRow FEditorPlugin_DataSyncModule::ParseGameplayTagData(const TSharedPtr<FJsonObject>& RowData)
{
	FGameplayTagTableRow TagRow;
	
	// Tag 필드에서 태그 이름 가져오기
	FString TagName = RowData->GetStringField("Tag");
	if (!TagName.IsEmpty())
	{
		// Tag가 FName 타입이므로 이렇게 할당
		TagRow.Tag = FName(TagName);
	}
	
	// Description 필드에서 설명 가져오기
	FString Description = RowData->GetStringField("Description");
	if (!Description.IsEmpty())
	{
		TagRow.DevComment = Description;
	}
	
	// Category 필드에서 카테고리 정보 가져오기 (선택사항)
	FString Category = RowData->GetStringField("Category");
	if (!Category.IsEmpty())
	{
		// 카테고리 정보를 DevComment에 추가
		if (!TagRow.DevComment.IsEmpty())
		{
			TagRow.DevComment = FString::Printf(TEXT("[%s] %s"), *Category, *TagRow.DevComment);
		}
		else
		{
			TagRow.DevComment = FString::Printf(TEXT("[%s]"), *Category);
		}
	}
	
	return TagRow;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorPlugin_DataSyncModule, EditorPlugin_DataSync)