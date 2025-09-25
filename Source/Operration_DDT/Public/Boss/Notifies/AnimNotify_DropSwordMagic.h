/**
 * @file AnimNotify_DropSwordMagic.h
 * @brief 검 마법 드롭 애니메이션 노티파이 헤더 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스가 검 마법을 드롭하는 노티파이입니다.
 * 프로젝타일 컴포넌트를 통해 검 마법을 생성합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_DropSwordMagic.generated.h"

/**
 * @brief 검 마법 드롭 애니메이션 노티파이
 * 
 * 애니메이션 시퀀스에서 보스가 검 마법을 드롭하는 노티파이입니다.
 * 프로젝타일 컴포넌트를 통해 검 마법을 생성합니다.
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotify_DropSwordMagic : public UAnimNotify
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
