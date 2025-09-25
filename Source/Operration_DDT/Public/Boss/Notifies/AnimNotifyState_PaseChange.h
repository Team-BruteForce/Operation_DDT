/**
 * @file AnimNotifyState_PaseChange.h
 * @brief 페이즈 변경 애니메이션 노티파이 상태 헤더 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 페이즈 변경을 처리하는 노티파이 상태입니다.
 * 보스 상태 컴포넌트를 통해 페이즈 변경을 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_PaseChange.generated.h"

/**
 * @brief 페이즈 변경 애니메이션 노티파이 상태
 * 
 * 애니메이션 시퀀스에서 보스의 페이즈 변경을 처리하는 노티파이 상태입니다.
 * 보스 상태 컴포넌트를 통해 페이즈 변경을 관리합니다.
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotifyState_PaseChange : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation ( ) const override;
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};
