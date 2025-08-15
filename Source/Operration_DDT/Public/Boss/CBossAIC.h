// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CBossAIC.generated.h"

/**
 * @brief 보스 AI 컨트롤러 클래스
 * 
 * 보스 캐릭터의 AI 동작을 제어하는 컨트롤러입니다.
 * 보스의 행동 패턴과 의사결정을 관리합니다.
 */
UCLASS()
class OPERRATION_DDT_API ACBossAIC : public AAIController
{
	GENERATED_BODY()

protected:
	/**
	 * @brief Pawn 소유 시 호출되는 함수
	 * 
	 * @param InPawn 소유할 Pawn 객체
	 * 
	 * AI 컨트롤러가 Pawn을 소유할 때 초기화 작업을 수행합니다.
	 */
	virtual void OnPossess(APawn* InPawn) override;
};
