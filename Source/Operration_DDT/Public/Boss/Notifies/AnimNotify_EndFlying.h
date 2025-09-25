/**
 * @file AnimNotify_EndFlying.h
 * @brief 비행 종료 애니메이션 노티파이 헤더 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 비행을 종료하는 노티파이입니다.
 * 비행 컴포넌트의 비행 중지 기능을 호출합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"

#include "AnimNotify_EndFlying.generated.h"

/**
 * @brief 비행 종료 애니메이션 노티파이
 * 
 * 애니메이션 시퀀스에서 보스의 비행을 종료하는 노티파이입니다.
 * 비행 컴포넌트의 비행 중지 기능을 호출합니다.
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotify_EndFlying : public UAnimNotify
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

};
