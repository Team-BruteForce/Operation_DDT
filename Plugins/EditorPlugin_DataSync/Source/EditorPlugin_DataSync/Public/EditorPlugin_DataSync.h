// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file EditorPlugin_DataSync.h
 * @brief 언리얼 엔진 에디터용 데이터 동기화 플러그인 메인 헤더 파일
 * 
 * @section overview 개요
 * 이 파일은 언리얼 엔진 에디터에서 외부 데이터베이스나 API와의
 * 실시간 데이터 동기화를 담당하는 플러그인의 메인 모듈 클래스를 정의합니다.
 * 
 * @section purpose 개발 목적
 * - 기획자가 데이터를 변경할 때 즉시 게임에 반영되도록 실시간 데이터 동기화 시스템 구축
 * - 외부 API와의 HTTP 통신을 통한 데이터 동적 로딩
 * - 언리얼 엔진의 데이터 테이블 시스템과 연동하여 게임 데이터 자동 업데이트
 * - 개발자와 기획자 간의 협업 효율성 향상
 * 
 * @section architecture 시스템 아키텍처
 * - FEditorPlugin_DataSyncModule: 메인 모듈 클래스
 * - HTTP 요청/응답 처리 시스템
 * - 데이터 파싱 및 변환 엔진
 * - 에디터 UI 통합 시스템
 * 
 * @section features 주요 기능
 * - HTTP API를 통한 외부 데이터 실시간 동기화
 * - GameplayTags 데이터 테이블 자동 업데이트
 * - 보스 스탯 데이터 테이블 자동 업데이트
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
 * @section api_integration API 연동
 * - GameplayTags: /api/tags 엔드포인트
 * - BossStats: /api/stats 엔드포인트
 * - JSON 응답 형식: {"success": true, "data": [...]}
 * 
 * @section data_flow 데이터 흐름
 * 1. 사용자가 동기화 버튼 클릭
 * 2. HTTP 요청을 외부 API로 전송
 * 3. JSON 응답을 받아서 파싱
 * 4. 데이터 테이블에 결과 저장
 * 5. 성공/실패 로그 출력
 * 
 * @section error_handling 에러 처리
 * - HTTP 연결 실패 처리
 * - JSON 파싱 오류 처리
 * - 데이터 테이블 로드 실패 처리
 * - 사용자 친화적 에러 메시지 출력
 * 
 * @author [이효원]
 * @date 2024-12-19
 * @version 1.0
 */

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Http.h"
#include "Json.h"
#include "Engine/DataTable.h"
#include "GameplayTagContainer.h"

/**
 * @brief 언리얼 엔진 에디터용 데이터 동기화 플러그인 메인 모듈 클래스
 * 
 * @details
 * 이 클래스는 언리얼 엔진 에디터에서 외부 데이터베이스나 API와의
 * 실시간 데이터 동기화를 담당하는 플러그인의 메인 모듈입니다.
 * 
 * @section design_patterns 설계 패턴
 * - 모듈 패턴: 언리얼 엔진 플러그인 아키텍처 활용
 * - 옵저버 패턴: HTTP 응답 비동기 처리
 * - 팩토리 패턴: 데이터 타입별 처리 로직 분리
 * - 싱글톤 패턴: 모듈 인스턴스 관리
 * 
 * @section responsibilities 책임
 * - 플러그인 초기화 및 종료
 * - 에디터 UI 메뉴 및 툴바 등록
 * - HTTP 요청/응답 처리
 * - JSON 데이터 파싱 및 변환
 * - 데이터 테이블 업데이트
 * - 에러 처리 및 로깅
 * 
 * @section integration 연동 시스템
 * - 언리얼 엔진 에디터 프레임워크
 * - HTTP 모듈 (FHttpModule, IHttpRequest, IHttpResponse)
 * - JSON 모듈 (FJsonSerializer, FJsonObject, FJsonValue)
 * - 데이터 테이블 시스템 (UDataTable)
 * - 게임플레이 태그 시스템 (FGameplayTag)
 * 
 * @section usage 사용법
 * 1. 플러그인을 언리얼 엔진에 설치
 * 2. 에디터에서 플러그인 활성화
 * 3. 메뉴나 툴바에서 플러그인 UI 열기
 * 4. 동기화 버튼을 클릭하여 데이터 업데이트
 * 
 * @section future_enhancements 향후 개선 사항
 * - 추가 데이터 타입 지원
 * - 실시간 자동 동기화
 * - 데이터 변경 감지 및 알림
 * - 백업 및 복원 기능
 */
class FEditorPlugin_DataSyncModule : public IModuleInterface
{
public:
	/**
	 * @brief 모듈 시작 시 호출되는 함수
	 * 
	 * @details
	 * 플러그인이 로드될 때 초기화 작업을 수행합니다.
	 * 에디터 UI 메뉴 및 툴바를 등록하고 기본 설정을 적용합니다.
	 * 
	 * @section initialization 초기화 과정
	 * 1. HTTP 모듈 초기화
	 * 2. 에디터 메뉴 등록
	 * 3. 툴바 버튼 등록
	 * 4. 기본 설정 적용
	 * 5. 로그 시스템 초기화
	 */
	virtual void StartupModule() override;

	/**
	 * @brief 모듈 종료 시 호출되는 함수
	 * 
	 * @details
	 * 플러그인이 언로드될 때 정리 작업을 수행합니다.
	 * 등록된 UI 요소들을 제거하고 리소스를 정리합니다.
	 * 
	 * @section cleanup 정리 과정
	 * 1. 등록된 메뉴 제거
	 * 2. 툴바 버튼 제거
	 * 3. HTTP 모듈 정리
	 * 4. 리소스 해제
	 * 5. 로그 출력
	 */
	virtual void ShutdownModule() override;

	/**
	 * @brief GameplayTags 동기화 함수
	 * 
	 * @details
	 * 외부 API에서 GameplayTags 데이터를 가져와서
	 * 언리얼 엔진의 데이터 테이블에 동기화합니다.
	 * 
	 * @section process 처리 과정
	 * 1. HTTP 요청을 /api/tags 엔드포인트로 전송
	 * 2. JSON 응답을 받아서 파싱
	 * 3. GameplayTags 데이터 테이블 업데이트
	 * 4. 성공/실패 로그 출력
	 * 
	 * @section integration 연동 시스템
	 * - HTTP 모듈: API 통신
	 * - JSON 모듈: 데이터 파싱
	 * - 데이터 테이블: DT_Tag 테이블 업데이트
	 * - 게임플레이 태그: FGameplayTag 구조체
	 */
	void SyncGameplayTags();

	/**
	 * @brief 보스 스탯 동기화 함수
	 * 
	 * @details
	 * 외부 API에서 보스 스탯 데이터를 가져와서
	 * 언리얼 엔진의 데이터 테이블에 동기화합니다.
	 * 
	 * @section process 처리 과정
	 * 1. HTTP 요청을 /api/stats 엔드포인트로 전송
	 * 2. JSON 응답을 받아서 파싱
	 * 3. 보스 스탯 데이터 테이블 업데이트
	 * 4. 성공/실패 로그 출력
	 * 
	 * @section integration 연동 시스템
	 * - HTTP 모듈: API 통신
	 * - JSON 모듈: 데이터 파싱
	 * - 데이터 테이블: DT_BossStats 테이블 업데이트
	 * - 보스 스탯: FBossStatSimple 구조체
	 */
	void SyncBossStats();

	/**
	 * @brief HTTP API 요청 함수
	 * 
	 * @param Endpoint API 엔드포인트 (예: "tags", "stats")
	 * @param DataType 데이터 타입 식별자
	 * 
	 * @details
	 * 지정된 엔드포인트로 HTTP GET 요청을 전송합니다.
	 * 비동기 응답 처리를 위해 콜백 함수를 설정합니다.
	 * 
	 * @section parameters 매개변수
	 * - Endpoint: API 엔드포인트 경로
	 * - DataType: 데이터 타입 식별자 (응답 처리 시 사용)
	 * 
	 * @section process 처리 과정
	 * 1. HTTP 요청 객체 생성
	 * 2. 요청 URL 설정 (/api/{endpoint}?type={DataType})
	 * 3. 콜백 함수 설정 (OnDataReceived)
	 * 4. 요청 전송
	 * 5. 로그 출력
	 */
	void MakeAPIRequest(const FString& Endpoint, const FString& DataType);

	/**
	 * @brief HTTP 응답 처리 함수
	 * 
	 * @param Request HTTP 요청 객체
	 * @param Response HTTP 응답 객체
	 * @param bWasSuccessful 요청 성공 여부
	 * 
	 * @details
	 * HTTP 요청에 대한 응답을 처리합니다.
	 * JSON 데이터를 파싱하고 데이터 타입에 따라 적절한 처리 함수를 호출합니다.
	 * 
	 * @section process 처리 과정
	 * 1. 응답 성공 여부 확인
	 * 2. JSON 응답 파싱
	 * 3. 데이터 타입 확인 (type 파라미터)
	 * 4. 데이터 타입별 처리 함수 호출
	 * 5. 에러 처리 및 로그 출력
	 * 
	 * @section data_types 지원 데이터 타입
	 * - GameplayTags: 태그 데이터 처리
	 * - BossStats: 보스 스탯 데이터 처리
	 */
	void OnDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	/**
	 * @brief GameplayTags 데이터 테이블 업데이트 함수
	 * 
	 * @param TagTable 업데이트할 데이터 테이블
	 * @param Data JSON 데이터 배열
	 * 
	 * @details
	 * JSON 데이터를 파싱하여 GameplayTags 데이터 테이블을 업데이트합니다.
	 * 각 JSON 객체를 FGameplayTagTableRow 구조체로 변환합니다.
	 * 
	 * @section process 처리 과정
	 * 1. 데이터 테이블 유효성 검사
	 * 2. 테이블 초기화 (기존 데이터 삭제)
	 * 3. JSON 데이터 순회
	 * 4. 각 JSON 객체를 FGameplayTagTableRow로 변환
	 * 5. 데이터 테이블에 행 추가
	 * 6. 성공/실패 통계 출력
	 * 
	 * @section data_structure 데이터 구조
	 * - JSON: {"tag": "TagName", "description": "Tag Description"}
	 * - FGameplayTagTableRow: {Tag, Description}
	 */
	void UpdateGameplayTagsTable(UDataTable* TagTable, const TArray<TSharedPtr<FJsonValue>>& Data);

	/**
	 * @brief 보스 스탯 데이터 테이블 업데이트 함수 (간단 버전)
	 * 
	 * @param BossTable 업데이트할 데이터 테이블
	 * @param Data JSON 데이터 배열
	 * 
	 * @details
	 * JSON 데이터를 파싱하여 보스 스탯 데이터 테이블을 업데이트합니다.
	 * 각 JSON 객체를 FBossStatSimple 구조체로 변환합니다.
	 * 
	 * @section process 처리 과정
	 * 1. 데이터 테이블 유효성 검사
	 * 2. 테이블 초기화 (기존 데이터 삭제)
	 * 3. JSON 데이터 순회
	 * 4. 각 JSON 객체를 FBossStatSimple로 변환
	 * 5. 데이터 테이블에 행 추가
	 * 6. 성공/실패 통계 출력
	 * 
	 * @section data_structure 데이터 구조
	 * - JSON: {"name": "BossName", "maxHP": 1000, "maxAP": 100, "atk": 50.0, "def": 30.0, "speed": 10}
	 * - FBossStatSimple: {MaxHP, MaxAP, ATK, DEF, Speed}
	 * 
	 * @section error_handling 에러 처리
	 * - JSON 필드 누락 시 기본값 사용
	 * - 파싱 오류 시 해당 행 건너뛰기
	 * - 상세한 에러 로그 출력
	 */
	void UpdateBossStatsTableSimple(UDataTable* BossTable, const TArray<TSharedPtr<FJsonValue>>& Data);

	/**
	 * @brief GameplayTag 데이터 파싱 함수
	 * 
	 * @param RowData JSON 객체
	 * @return FGameplayTagTableRow 파싱된 태그 데이터
	 * 
	 * @details
	 * JSON 객체에서 GameplayTag 데이터를 추출하여
	 * FGameplayTagTableRow 구조체로 변환합니다.
	 * 
	 * @section parsing 파싱 과정
	 * 1. "tag" 필드에서 태그 이름 추출
	 * 2. "description" 필드에서 설명 추출
	 * 3. FGameplayTagTableRow 구조체 생성
	 * 4. 반환
	 * 
	 * @section error_handling 에러 처리
	 * - 필수 필드 누락 시 기본값 사용
	 * - JSON 파싱 오류 시 예외 처리
	 */
	FGameplayTagTableRow ParseGameplayTagData(const TSharedPtr<FJsonObject>& RowData);

private:
	/**
	 * @brief 에디터 메뉴 등록 함수
	 * 
	 * @details
	 * 언리얼 엔진 에디터에 플러그인 메뉴를 등록합니다.
	 * 메뉴 항목과 명령어를 설정합니다.
	 * 
	 * @section menu_structure 메뉴 구조
	 * - Tools > Data Sync > Sync GameplayTags
	 * - Tools > Data Sync > Sync Boss Stats
	 * 
	 * @section commands 명령어
	 * - SyncGameplayTags: 태그 동기화 명령
	 * - SyncBossStats: 보스 스탯 동기화 명령
	 */
	void RegisterMenus();

	/**
	 * @brief 에디터 툴바 등록 함수
	 * 
	 * @details
	 * 언리얼 엔진 에디터에 플러그인 툴바를 등록합니다.
	 * 툴바 버튼과 아이콘을 설정합니다.
	 * 
	 * @section toolbar_structure 툴바 구조
	 * - Sync GameplayTags 버튼
	 * - Sync Boss Stats 버튼
	 * 
	 * @section icons 아이콘
	 * - 태그 동기화: 태그 아이콘
	 * - 보스 스탯 동기화: 스탯 아이콘
	 */
	void RegisterToolbar();

	/**
	 * @brief 플러그인 UI 생성 함수
	 * 
	 * @details
	 * 플러그인의 독립 탭 UI를 생성합니다.
	 * 동기화 버튼과 상태 표시를 포함합니다.
	 * 
	 * @section ui_components UI 구성 요소
	 * - 동기화 버튼들
	 * - 상태 표시 레이블
	 * - 로그 출력 영역
	 * 
	 * @section layout 레이아웃
	 * - 수직 박스 레이아웃
	 * - 버튼 그룹화
	 * - 스크롤 가능한 로그 영역
	 */
	void CreatePluginUI();

	// UI 관련 변수들
	TSharedPtr<class FUICommandList> PluginCommands; ///< 플러그인 명령어 리스트
	TSharedPtr<class SWidget> PluginTabContent; ///< 플러그인 탭 콘텐츠
};