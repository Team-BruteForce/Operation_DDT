/**
 * @file AnimNotify_PaseChangeDissolve.h
 * @brief 페이즈 변경 디졸브 애니메이션 노티파이 헤더 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 페이즈 변경 시 디졸브 효과를 처리하는 노티파이입니다.
 * 보스의 페이즈 전환 시 디졸브 애니메이션을 실행합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PaseChangeDissolve.generated.h"

/**
 * @brief 페이즈 변경 디졸브 애니메이션 노티파이
 * 
 * 애니메이션 시퀀스에서 보스의 페이즈 변경 시 디졸브 효과를 처리하는 노티파이입니다.
 * 보스의 페이즈 전환 시 디졸브 애니메이션을 실행합니다.
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotify_PaseChangeDissolve : public UAnimNotify
{
	GENERATED_BODY()
	FString GetNotifyName_Implementation ( ) const override;

	void Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation ) override;
};
