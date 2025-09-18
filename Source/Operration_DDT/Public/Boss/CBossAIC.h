/**
 * @file CBossAIC.h
 * @brief 보스 AI 컨트롤러 헤더 파일
 * 
 * @details
 * 보스 캐릭터의 AI 동작을 제어하는 컨트롤러 클래스를 정의합니다.
 * StateTree 기반 AI 시스템을 사용하여 보스의 행동 패턴과 의사결정을 관리합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CBossAIC.generated.h"

/**
 * @brief 보스 AI 컨트롤러 클래스
 * 
 * 보스 캐릭터의 AI 동작을 제어하는 컨트롤러입니다.
 * StateTree 기반 AI 시스템을 사용하여 보스의 행동 패턴을 관리합니다.
 */
UCLASS()
class OPERRATION_DDT_API ACBossAIC : public AAIController
{
	GENERATED_BODY()

	/**
	 * @brief 생성자
	 * 
	 * AI 컨트롤러의 기본 설정을 초기화합니다.
	 */
	ACBossAIC();

protected:
	/**
	 * @brief Pawn 소유 시 호출되는 함수
	 * 
	 * @param InPawn 소유할 Pawn 객체
	 * 
	 * AI 컨트롤러가 Pawn을 소유할 때 초기화 작업을 수행합니다.
	 */
	virtual void OnPossess(APawn* InPawn) override;

	/**
	 * @brief StateTree AI 컴포넌트
	 * 
	 * StateTree 기반 AI 시스템을 실행하는 컴포넌트입니다.
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UStateTreeAIComponent* StateTreeComp;
};
