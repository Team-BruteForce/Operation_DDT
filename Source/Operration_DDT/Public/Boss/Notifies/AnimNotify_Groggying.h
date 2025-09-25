/**
 * @file AnimNotify_Groggying.h
 * @brief 그로기 상태 애니메이션 노티파이 헤더 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 그로기 상태를 처리하는 노티파이입니다.
 * 보스 상태 컴포넌트를 통해 그로기 상태를 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotify_Groggying.generated.h"

/**
 * @brief 그로기 상태 애니메이션 노티파이
 * 
 * 애니메이션 시퀀스에서 보스의 그로기 상태를 처리하는 노티파이입니다.
 * 보스 상태 컴포넌트를 통해 그로기 상태를 관리합니다.
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotify_Groggying : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation ( ) const override;
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};
