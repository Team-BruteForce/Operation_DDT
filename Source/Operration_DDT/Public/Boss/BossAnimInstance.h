/**
 * @file CEnemyAnimInstance.h
 * @brief 적 캐릭터 애니메이션 인스턴스 헤더 파일
 * 
 * @details
 * 적 캐릭터들의 애니메이션을 관리하는 클래스입니다.
 * 적의 상태에 따른 애니메이션 전환을 처리합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * @brief 적 캐릭터 애니메이션 인스턴스 클래스
 * 
 * 적 캐릭터들의 애니메이션을 관리하는 클래스입니다.
 * 적의 상태에 따른 애니메이션 전환을 처리합니다.
 */
UCLASS()
class OPERRATION_DDT_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	virtual void NativeBeginPlay() override;
};
