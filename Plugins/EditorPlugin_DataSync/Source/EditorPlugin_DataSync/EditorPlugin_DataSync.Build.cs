// Copyright Epic Games, Inc. All Rights Reserved.

/**
 * @file EditorPlugin_DataSync.Build.cs
 * @brief 언리얼 엔진 에디터용 데이터 동기화 플러그인 빌드 설정 파일
 * 
 * @section overview 개요
 * 이 파일은 EditorPlugin_DataSync 플러그인의 빌드 설정을 정의합니다.
 * 모듈 의존성, 포함 경로, 빌드 옵션 등을 설정합니다.
 * 
 * @section purpose 개발 목적
 * - 플러그인의 빌드 의존성 관리
 * - 필요한 언리얼 엔진 모듈 연결
 * - 컴파일러 설정 및 옵션 정의
 * - 플러그인 개발 환경 구성
 * 
 * @section dependencies 주요 의존성
 * - Core: 언리얼 엔진 핵심 기능
 * - HTTP: HTTP 통신 모듈
 * - Json: JSON 파싱 모듈
 * - GameplayTags: 게임플레이 태그 시스템
 * - UnrealEd: 언리얼 에디터 기능
 * - Slate: UI 프레임워크
 * 
 * @section build_configuration 빌드 설정
 * - PCH 사용: 명시적 또는 공유 PCH 사용
 * - 공개 의존성: 다른 모듈에서 접근 가능한 모듈
 * - 비공개 의존성: 내부적으로만 사용하는 모듈
 * - 동적 로드: 런타임에 로드되는 모듈
 * 
 * @section technical_details 기술적 세부사항
 * - 언리얼 빌드 도구(UBT) 설정
 * - 모듈 규칙(ModuleRules) 상속
 * - 타겟별 빌드 설정
 * - 플랫폼별 최적화
 * 
 * @author [이효원]
 * @date 2024-12-19
 * @version 1.0
 */

using UnrealBuildTool;

/**
 * @brief EditorPlugin_DataSync 모듈 규칙 클래스
 * 
 * @details
 * 언리얼 엔진의 빌드 시스템에서 플러그인 모듈의 빌드 규칙을 정의합니다.
 * 모듈 의존성, 포함 경로, 컴파일러 옵션 등을 설정합니다.
 * 
 * @section inheritance 상속 구조
 * - ModuleRules: 언리얼 엔진 기본 모듈 규칙 클래스
 * - ReadOnlyTargetRules: 읽기 전용 타겟 규칙
 * 
 * @section responsibilities 책임
 * - 모듈 의존성 정의
 * - 포함 경로 설정
 * - 빌드 옵션 구성
 * - 플랫폼별 설정
 * 
 * @section usage 사용법
 * - 언리얼 빌드 도구에서 자동으로 사용
 * - 플러그인 컴파일 시 참조
 * - 의존성 해결 및 링크 처리
 */
public class EditorPlugin_DataSync : ModuleRules
{
	/**
	 * @brief 생성자
	 * 
	 * @param Target 빌드 타겟 규칙
	 * 
	 * @details
	 * 모듈의 빌드 설정을 초기화합니다.
	 * PCH 사용 모드와 기본 설정을 적용합니다.
	 * 
	 * @section initialization 초기화 과정
	 * 1. 부모 클래스 생성자 호출
	 * 2. PCH 사용 모드 설정
	 * 3. 포함 경로 설정
	 * 4. 모듈 의존성 정의
	 * 5. 빌드 옵션 적용
	 */
	public EditorPlugin_DataSync(ReadOnlyTargetRules Target) : base(Target)
	{
		/**
		 * @brief PCH 사용 모드 설정
		 * 
		 * @details
		 * 명시적 또는 공유 PCH를 사용하도록 설정합니다.
		 * 컴파일 시간을 단축하고 빌드 성능을 향상시킵니다.
		 * 
		 * @section pch_modes PCH 모드
		 * - UseExplicitOrSharedPCHs: 명시적 또는 공유 PCH 사용
		 * - UseSharedPCHs: 공유 PCH만 사용
		 * - NoPCHs: PCH 사용하지 않음
		 */
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		/**
		 * @brief 공개 포함 경로 설정
		 * 
		 * @details
		 * 다른 모듈에서 접근할 수 있는 헤더 파일 경로를 정의합니다.
		 * 현재는 추가 경로가 없으므로 빈 배열로 설정됩니다.
		 * 
		 * @section usage 사용법
		 * - 공개 API 헤더 파일 경로 추가
		 * - 다른 모듈에서 포함할 수 있는 경로
		 * - 인터페이스 정의 파일 경로
		 */
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		/**
		 * @brief 비공개 포함 경로 설정
		 * 
		 * @details
		 * 이 모듈 내부에서만 사용하는 헤더 파일 경로를 정의합니다.
		 * 현재는 추가 경로가 없으므로 빈 배열로 설정됩니다.
		 * 
		 * @section usage 사용법
		 * - 내부 구현 헤더 파일 경로 추가
		 * - 모듈 내부에서만 사용하는 경로
		 * - 구현 세부사항 파일 경로
		 */
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		/**
		 * @brief 공개 의존성 모듈 설정
		 * 
		 * @details
		 * 다른 모듈에서 이 모듈을 사용할 때 필요한 의존성을 정의합니다.
		 * HTTP, JSON, GameplayTags 등 핵심 기능 모듈을 포함합니다.
		 * 
		 * @section core_modules 핵심 모듈
		 * - Core: 언리얼 엔진 기본 기능
		 * - CoreUObject: 언리얼 객체 시스템
		 * - Engine: 언리얼 엔진 핵심 기능
		 * - InputCore: 입력 처리 시스템
		 * 
		 * @section communication_modules 통신 모듈
		 * - HTTP: HTTP 요청/응답 처리
		 * - Json: JSON 데이터 파싱
		 * - JsonUtilities: JSON 유틸리티 함수
		 * 
		 * @section gameplay_modules 게임플레이 모듈
		 * - GameplayTags: 게임플레이 태그 시스템
		 * 
		 * @section editor_modules 에디터 모듈
		 * - UnrealEd: 언리얼 에디터 기능
		 * - EditorUtilityWidgets: 에디터 유틸리티 위젯
		 */
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",              ///< 언리얼 엔진 기본 기능
				"CoreUObject",       ///< 언리얼 객체 시스템
				"Engine",            ///< 언리얼 엔진 핵심 기능
				"InputCore",         ///< 입력 처리 시스템
				"HTTP",              ///< HTTP 통신 모듈
				"Json",              ///< JSON 파싱 모듈
				"JsonUtilities",     ///< JSON 유틸리티 함수
				"GameplayTags",      ///< 게임플레이 태그 시스템
				"UnrealEd",          ///< 언리얼 에디터 기능
				"EditorUtilityWidgets" ///< 에디터 유틸리티 위젯
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		/**
		 * @brief 비공개 의존성 모듈 설정
		 * 
		 * @details
		 * 이 모듈 내부에서만 사용하는 의존성을 정의합니다.
		 * 에디터 기능, UI 프레임워크, 프로젝트 관리 모듈을 포함합니다.
		 * 
		 * @section project_modules 프로젝트 모듈
		 * - Projects: 프로젝트 관리 기능
		 * 
		 * @section editor_framework_modules 에디터 프레임워크 모듈
		 * - EditorFramework: 에디터 프레임워크
		 * - UnrealEd: 언리얼 에디터 기능
		 * - ToolMenus: 도구 메뉴 시스템
		 * 
		 * @section ui_modules UI 모듈
		 * - Slate: UI 프레임워크
		 * - SlateCore: UI 핵심 기능
		 * 
		 * @section utility_modules 유틸리티 모듈
		 * - InputCore: 입력 처리 시스템
		 * - CoreUObject: 언리얼 객체 시스템
		 * - Engine: 언리얼 엔진 핵심 기능
		 * - HTTP: HTTP 통신 모듈
		 * - Json: JSON 파싱 모듈
		 * - JsonUtilities: JSON 유틸리티 함수
		 * - GameplayTags: 게임플레이 태그 시스템
		 */
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Projects",          ///< 프로젝트 관리 기능
				"InputCore",         ///< 입력 처리 시스템
				"EditorFramework",   ///< 에디터 프레임워크
				"UnrealEd",          ///< 언리얼 에디터 기능
				"ToolMenus",         ///< 도구 메뉴 시스템
				"CoreUObject",       ///< 언리얼 객체 시스템
				"Engine",            ///< 언리얼 엔진 핵심 기능
				"Slate",             ///< UI 프레임워크
				"SlateCore",         ///< UI 핵심 기능
				"HTTP",              ///< HTTP 통신 모듈
				"Json",              ///< JSON 파싱 모듈
				"JsonUtilities",     ///< JSON 유틸리티 함수
				"GameplayTags"       ///< 게임플레이 태그 시스템
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		/**
		 * @brief 동적 로드 모듈 설정
		 * 
		 * @details
		 * 런타임에 동적으로 로드되는 모듈을 정의합니다.
		 * 현재는 동적 로드 모듈이 없으므로 빈 배열로 설정됩니다.
		 * 
		 * @section usage 사용법
		 * - 플러그인 형태의 모듈 추가
		 * - 런타임에 로드되는 기능
		 * - 선택적 기능 모듈
		 */
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}