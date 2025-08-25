/**
 * @file EditorPlugin_DataSync.h
 * @brief 언리얼 엔진 에디터용 데이터 동기화 플러그인 메인 헤더 파일
 * 
 * @details
 * 언리얼 엔진 에디터에서 외부 데이터베이스나 API와의
 * 실시간 데이터 동기화를 담당하는 플러그인의 메인 모듈 클래스를 정의합니다.
 * 
 * @author [이효원]
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Http.h"
#include "Json.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"

/**
 * @brief 언리얼 엔진 에디터용 데이터 동기화 플러그인 메인 모듈 클래스
 * 
 * 언리얼 엔진 에디터에서 외부 데이터베이스나 API와의
 * 실시간 데이터 동기화를 담당하는 플러그인의 메인 모듈입니다.
 */
class FEditorPlugin_DataSyncModule : public IModuleInterface
{
public:
	/**
	 * @brief 모듈 시작 시 호출되는 함수
	 * 
	 * 플러그인이 로드될 때 초기화 작업을 수행합니다.
	 */
	virtual void StartupModule() override;

	/**
	 * @brief 모듈 종료 시 호출되는 함수
	 * 
	 * 플러그인이 언로드될 때 정리 작업을 수행합니다.
	 */
	virtual void ShutdownModule() override;

	/**
	 * @brief GameplayTags 동기화 함수
	 * 
	 * 외부 API에서 GameplayTags 데이터를 가져와서
	 * 언리얼 엔진의 데이터 테이블에 동기화합니다.
	 */
	void SyncGameplayTags();

	/**
	 * @brief 보스 스탯 동기화 함수
	 * 
	 * 외부 API에서 보스 스탯 데이터를 가져와서
	 * 언리얼 엔진의 데이터 테이블에 동기화합니다.
	 */
	void SyncBossStats();

	/**
	 * @brief HTTP API 요청 함수
	 * 
	 * @param Endpoint API 엔드포인트 (예: "tags", "stats")
	 * @param DataType 데이터 타입 식별자
	 * 
	 * 지정된 엔드포인트로 HTTP GET 요청을 전송합니다.
	 */
	void MakeAPIRequest(const FString& Endpoint, const FString& DataType);

	/**
	 * @brief HTTP 응답 처리 함수
	 * 
	 * @param Request HTTP 요청 객체
	 * @param Response HTTP 응답 객체
	 * @param bWasSuccessful 요청 성공 여부
	 * 
	 * HTTP 요청에 대한 응답을 처리합니다.
	 */
	void OnDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/**
	 * @brief GameplayTags 데이터 테이블 업데이트 함수
	 * 
	 * @param TagTable 업데이트할 데이터 테이블
	 * @param Data JSON 데이터 배열
	 * 
	 * JSON 데이터를 파싱하여 GameplayTags 데이터 테이블을 업데이트합니다.
	 */
	void UpdateGameplayTagsTable(UDataTable* TagTable, const TArray<TSharedPtr<FJsonValue>>& Data);

	/**
	 * @brief 보스 스탯 데이터 테이블 업데이트 함수 (간단 버전)
	 * 
	 * @param BossTable 업데이트할 데이터 테이블
	 * @param Data JSON 데이터 배열
	 * 
	 * JSON 데이터를 파싱하여 보스 스탯 데이터 테이블을 업데이트합니다.
	 */
	void UpdateBossStatsTableSimple(UDataTable* BossTable, const TArray<TSharedPtr<FJsonValue>>& Data);

	/**
	 * @brief 플러그인 버튼 클릭 처리 함수
	 * 
	 * 플러그인 UI에서 버튼이 클릭되었을 때 호출되는 함수입니다.
	 */
	void PluginButtonClicked();

	/**
	 * @brief 플러그인 탭 생성 함수
	 * 
	 * @param SpawnTabArgs 탭 생성 인수
	 * @return TSharedRef<SDockTab> 생성된 탭 위젯
	 * 
	 * 플러그인의 메인 UI 탭을 생성합니다.
	 */
	TSharedRef<SDockTab> OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs);

	/**
	 * @brief GameplayTag 데이터 파싱 함수
	 * 
	 * @param RowData JSON 객체
	 * @return FGameplayTagTableRow 파싱된 태그 데이터
	 * 
	 * JSON 객체에서 GameplayTag 데이터를 추출하여
	 * FGameplayTagTableRow 구조체로 변환합니다.
	 */
	FGameplayTagTableRow ParseGameplayTagData(const TSharedPtr<FJsonObject>& RowData);

private:
	/**
	 * @brief 에디터 메뉴 등록 함수
	 * 
	 * 언리얼 엔진 에디터에 플러그인 메뉴를 등록합니다.
	 */
	void RegisterMenus();

	/**
	 * @brief 에디터 툴바 등록 함수
	 * 
	 * 언리얼 엔진 에디터에 플러그인 툴바를 등록합니다.
	 */
	void RegisterToolbar();

	/**
	 * @brief 플러그인 UI 생성 함수
	 * 
	 * 플러그인의 독립 탭 UI를 생성합니다.
	 */
	void CreatePluginUI();

	// UI 관련 변수들
	TSharedPtr<class FUICommandList> PluginCommands; ///< 플러그인 명령어 리스트
	TSharedPtr<class SWidget> PluginTabContent; ///< 플러그인 탭 콘텐츠
};