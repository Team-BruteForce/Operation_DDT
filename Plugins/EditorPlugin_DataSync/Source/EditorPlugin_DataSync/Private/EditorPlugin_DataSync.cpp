/**
 * @file EditorPlugin_DataSync.cpp
 * @brief 언리얼 엔진 에디터용 데이터 동기화 플러그인 구현 파일
 * 
 * @details
 * 이 플러그인은 언리얼 엔진 에디터에서 외부 데이터베이스나 API와의
 * 실시간 데이터 동기화를 담당하는 핵심 시스템입니다.
 * 기획자가 데이터를 변경할 때 즉시 게임에 반영되도록 하여
 * 개발 효율성을 극대화하는 것이 주요 목적입니다.
 * 
 * @author [이효원]
 * @date 2024-12-19
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
#include "GameplayTagsManager.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "Engine/DataTable.h"
#include "Misc/DateTime.h"
#include "Widgets/Input/SButton.h"

static const FName EditorPlugin_DataSyncTabName("EditorPlugin_DataSync");

#define LOCTEXT_NAMESPACE "FEditorPlugin_DataSyncModule"

/**
 * @brief 모듈 시작 시 호출되는 함수
 * 
 * 플러그인이 로드될 때 초기화 작업을 수행합니다.
 */
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

/**
 * @brief 모듈 종료 시 호출되는 함수
 * 
 * 플러그인이 언로드될 때 정리 작업을 수행합니다.
 */
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

/**
 * @brief 플러그인 탭 생성 함수
 * 
 * @param SpawnTabArgs 탭 생성 인수
 * @return TSharedRef<SDockTab> 생성된 탭 위젯
 * 
 * 플러그인의 메인 UI 탭을 생성합니다.
 */
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
				SNew(SButton)
				.Text(LOCTEXT("SyncBossStatsButtonText", "Sync Boss Stats"))
				.OnClicked_Lambda([this]()
				{
					SyncBossStats();
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

/**
 * @brief 플러그인 버튼 클릭 처리 함수
 * 
 * 플러그인 UI에서 버튼이 클릭되었을 때 호출되는 함수입니다.
 */
void FEditorPlugin_DataSyncModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(EditorPlugin_DataSyncTabName);
}

/**
 * @brief 에디터 메뉴 등록 함수
 * 
 * 언리얼 엔진 에디터에 플러그인 메뉴를 등록합니다.
 */
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
/**
 * @brief GameplayTags 동기화 함수
 * 
 * 외부 API에서 GameplayTags 데이터를 가져와서
 * 언리얼 엔진의 데이터 테이블에 동기화합니다.
 */
void FEditorPlugin_DataSyncModule::SyncGameplayTags()
{
	UE_LOG(LogTemp, Log, TEXT("GameplayTags 동기화 시작..."));
	
	// API 요청 생성
	MakeAPIRequest("/api/tags", "GameplayTags");
}

/**
 * @brief Boss Stats 동기화 함수
 * 
 * 외부 API에서 보스 스탯 데이터를 가져와서
 * 언리얼 엔진의 데이터 테이블에 동기화합니다.
 */
void FEditorPlugin_DataSyncModule::SyncBossStats()
{
	UE_LOG(LogTemp, Log, TEXT("Boss Stats 동기화 시작..."));
	
	// API 요청 생성
	MakeAPIRequest("/api/stats", "BossStats");
}

/**
 * @brief HTTP API 요청 함수
 * 
 * @param Endpoint API 엔드포인트 (예: "tags", "stats")
 * @param DataType 데이터 타입 식별자
 * 
 * 지정된 엔드포인트로 HTTP GET 요청을 전송합니다.
 */
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
	Request->SetURL(FString::Printf(TEXT("http://localhost:3000%s?type=%s"), *Endpoint, *DataType));
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	
	// 콜백 바인딩
	Request->OnProcessRequestComplete().BindRaw(this, &FEditorPlugin_DataSyncModule::OnDataReceived);
	
	// 요청 전송
	Request->ProcessRequest();
}

/**
 * @brief HTTP 응답 처리 함수
 * 
 * @param Request HTTP 요청 객체
 * @param Response HTTP 응답 객체
 * @param bWasSuccessful 요청 성공 여부
 * 
 * HTTP 요청에 대한 응답을 처리합니다.
 */
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
			// URL에서 type 파라미터 확인
			FString RequestURL = Request->GetURL();
			if (RequestURL.Contains("type=GameplayTags"))
			{
				// "tags" 필드에서 데이터 가져오기
				TArray<TSharedPtr<FJsonValue>> TagsData = JsonObject->GetArrayField("tags");
				UE_LOG(LogTemp, Log, TEXT("태그 데이터 %d개 받음"), TagsData.Num());
				
				if (TagsData.Num() > 0)
				{
					// GameplayTags 데이터 테이블 업데이트
					UDataTable* TagTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("/Game/LHW/BossData/DT_Tag")));
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
			else if (RequestURL.Contains("type=BossStats"))
			{
				// "bossStats" 필드에서 데이터 가져오기
				if (JsonObject->HasField("bossStats"))
				{
					TArray<TSharedPtr<FJsonValue>> StatsData = JsonObject->GetArrayField("bossStats");
					UE_LOG(LogTemp, Log, TEXT("보스 스탯 데이터 %d개 받음"), StatsData.Num());
				
					if (StatsData.Num() > 0)
					{
						// 보스 스탯 데이터 테이블 업데이트
						UDataTable* BossTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, TEXT("/Game/LHW/BossData/DT_BossStats")));
						if (BossTable)
						{
							UpdateBossStatsTableSimple(BossTable, StatsData);
							UE_LOG(LogTemp, Log, TEXT("Boss Stats 동기화 완료"));
						}
						else
						{
							UE_LOG(LogTemp, Log, TEXT("DT_BossStats 테이블을 찾을 수 없습니다."));
						}
					}
					else
					{
						UE_LOG(LogTemp, Log, TEXT("bossStats 배열이 비어있습니다."));
					}
				}
				else
				{
					UE_LOG(LogTemp, Log, TEXT("bossStats 필드를 찾을 수 없습니다."));
				}
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

/**
 * @brief GameplayTags 데이터 테이블 업데이트 함수
 * 
 * @param TagTable 업데이트할 데이터 테이블
 * @param Data JSON 데이터 배열
 * 
 * JSON 데이터를 파싱하여 GameplayTags 데이터 테이블을 업데이트합니다.
 */
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

/**
 * @brief GameplayTag 데이터 파싱 함수
 * 
 * @param RowData JSON 객체
 * @return FGameplayTagTableRow 파싱된 태그 데이터
 * 
 * JSON 객체에서 GameplayTag 데이터를 추출하여
 * FGameplayTagTableRow 구조체로 변환합니다.
 */
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

/**
 * @brief 보스 스탯 데이터 테이블 업데이트 함수 (간단 버전)
 * 
 * @param BossTable 업데이트할 데이터 테이블
 * @param Data JSON 데이터 배열
 * 
 * JSON 데이터를 파싱하여 보스 스탯 데이터 테이블을 업데이트합니다.
 */
void FEditorPlugin_DataSyncModule::UpdateBossStatsTableSimple(UDataTable* BossTable, const TArray<TSharedPtr<FJsonValue>>& Data)
{
	if (!BossTable)
	{
		UE_LOG(LogTemp, Log, TEXT("BossTable이 유효하지 않습니다."));
		return;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Boss Stats 테이블 업데이트 시작..."));
	
	// 테이블 초기화
	BossTable->EmptyTable();
	
	int32 SuccessCount = 0;
	int32 ErrorCount = 0;
	
	// 각 행의 데이터 처리
	for (const TSharedPtr<FJsonValue>& RowValue : Data)
	{
		TSharedPtr<FJsonObject> RowObject = RowValue->AsObject();
		if (!RowObject.IsValid()) continue;
		
		try
		{
			// JSON에서 각 변수 직접 가져오기 (API 응답 필드명에 맞춤)
			FString BossName = RowObject->GetStringField("name");
			int32 MaxHP = RowObject->GetIntegerField("maxHP");
			float MaxAP = RowObject->GetNumberField("maxAP");
			float IncreaseAP = RowObject->GetNumberField("IncreaseAP");
			float ATK = RowObject->GetNumberField("atk");
			float DEF = RowObject->GetNumberField("def");
			int32 Speed = RowObject->GetIntegerField("speed");
			float MaxGroggyGauge = RowObject->GetNumberField("MaxGroggyGauge");
			
			// 행 이름 생성
			FName RowName = FName(*BossName);
			
			// 간단한 구조체 생성 (FTableRowBase 상속)
			struct FBossStatSimple : public FTableRowBase
			{
				int32 MaxHP;
				float MaxAP;
				float IncreaseAP;
				float ATK;
				float DEF;
				int32 Speed;
				float MaxGroggyGauge;
			};
			
			FBossStatSimple BossStat;
			BossStat.MaxHP = MaxHP;
			BossStat.MaxAP = MaxAP;
			BossStat.IncreaseAP = IncreaseAP;
			BossStat.ATK = ATK;
			BossStat.DEF = DEF;
			BossStat.Speed = Speed;
			BossStat.MaxGroggyGauge = MaxGroggyGauge;
			
			// 테이블에 행 추가
			BossTable->AddRow(RowName, BossStat);
			SuccessCount++;
			
			UE_LOG(LogTemp, Log, TEXT("보스 스탯 추가 성공: %s (HP:%d, AP:%f, ATK:%.1f, DEF:%.1f, Speed:%d, MaxGroggyGauge : %.1f)"), 
				*BossName, MaxHP, MaxAP, ATK, DEF, Speed,MaxGroggyGauge);
		}
		catch (...)
		{
			ErrorCount++;
			UE_LOG(LogTemp, Log, TEXT("행 데이터 파싱 중 오류 발생"));
		}
	}
	
	// 테이블을 더티로 표시
	BossTable->MarkPackageDirty();
	
	UE_LOG(LogTemp, Log, TEXT("Boss Stats 테이블 업데이트 완료: 성공 %d개, 실패 %d개"), SuccessCount, ErrorCount);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorPlugin_DataSyncModule, EditorPlugin_DataSync)