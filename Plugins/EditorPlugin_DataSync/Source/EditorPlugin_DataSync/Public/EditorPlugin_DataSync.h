// Copyright Epic Games, Inc. All Rights Reserved.

/**
 * @file EditorPlugin_DataSync.h
 * @brief 언리얼 엔진 에디터용 데이터 동기화 플러그인 헤더 파일
 * 
 * @date 2024-12-19
 * @author [이효원]
 * @version 1.0
 * 
 * @section overview 개요
 * 이 헤더 파일은 언리얼 엔진 에디터에서 외부 데이터와의
 * 실시간 동기화를 담당하는 플러그인의 클래스 선언을 포함합니다.
 * 기획자가 데이터를 변경할 때 즉시 게임에 반영되도록 하는
 * 핵심 시스템의 인터페이스를 정의합니다.
 * 
 * @section purpose 개발 목적
 * - 기획자가 데이터를 변경할 때 즉시 게임에 반영되도록 실시간 데이터 동기화 시스템 구축
 * - 언리얼 엔진 플러그인 아키텍처를 활용한 확장 가능한 시스템 설계
 * - HTTP 통신을 통한 외부 API 연동 및 데이터 동적 로딩
 * - 개발자와 기획자 간의 협업 효율성 극대화
 * 
 * @section class_structure 클래스 구조
 * - FEditorPlugin_DataSyncModule: 메인 모듈 클래스
 *   - IModuleInterface를 상속받아 언리얼 엔진 모듈 시스템과 연동
 *   - 플러그인의 생명주기 관리 (StartupModule, ShutdownModule)
 *   - 에디터 UI 통합 및 메뉴/툴바 등록
 *   - 데이터 동기화 핵심 기능 구현
 * 
 * @section key_functions 주요 함수
 * - StartupModule(): 플러그인 초기화 및 에디터 통합
 * - ShutdownModule(): 플러그인 정리 및 리소스 해제
 * - PluginButtonClicked(): 플러그인 UI 활성화
 * - SyncGameplayTags(): GameplayTags 데이터 동기화
 * - MakeAPIRequest(): HTTP API 요청 처리
 * - OnDataReceived(): API 응답 처리 및 데이터 파싱
 * - UpdateGameplayTagsTable(): 데이터 테이블 업데이트
 * - ParseGameplayTagData(): JSON 데이터를 언리얼 엔진 구조체로 변환
 * 
 * @section technical_details 기술적 세부사항
 * - 언리얼 엔진 모듈 시스템 활용
 * - HTTP 모듈을 통한 REST API 통신
 * - JSON 파싱 및 데이터 테이블 시스템 연동
 * - 언리얼 엔진 에디터 UI 프레임워크 활용
 * - 컴포넌트 기반 아키텍처로 확장성 확보
 * 
 * @section portfolio_highlight 포트폴리오 하이라이트
 * 이 헤더 파일은 게임 개발에서 핵심적인 실시간 데이터 동기화 시스템의
 * 설계 능력을 보여줍니다. 특히 언리얼 엔진의 플러그인 아키텍처를
 * 이해하고 활용한 점과, HTTP 통신을 통한 외부 시스템 연동 능력을
 * 잘 보여줍니다.
 * 
 * 기획자가 데이터를 변경할 때마다 게임을 재시작할 필요 없이
 * 즉시 반영되는 시스템은 현대 게임 개발에서 요구되는
 * 빠른 반복 개발과 실시간 협업의 핵심 요소이며,
 * 이는 대규모 게임 프로젝트에서 필수적인 시스템입니다.
 */

#pragma once

#include "GameplayTagsManager.h"
#include "Modules/ModuleManager.h"
#include "Engine/DataTable.h"
#include "Interfaces/IHttpRequest.h"

class FToolBarBuilder;
class FMenuBuilder;

class FEditorPlugin_DataSyncModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up plugin window) */
	void PluginButtonClicked();
	
private:

	void RegisterMenus();

	TSharedRef<class SDockTab> OnSpawnPluginTab(const class FSpawnTabArgs& SpawnTabArgs);

	// 데이터 동기화 함수들
	void SyncGameplayTags();
	void MakeAPIRequest(const FString& Endpoint, const FString& DataType);
	void OnDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void UpdateGameplayTagsTable(UDataTable* TagTable, const TArray<TSharedPtr<FJsonValue>>& Data);
	FGameplayTagTableRow ParseGameplayTagData(const TSharedPtr<FJsonObject>& RowData);

private:
	TSharedPtr<class FUICommandList> PluginCommands;
};