/**
 * @file AnimNotify_LineTraceOnOff.h
 * @brief 라인 트레이스 온/오프 애니메이션 노티파이 헤더 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 라인 트레이스를 활성화/비활성화하는 노티파이입니다.
 * 웨폰 컴포넌트를 통해 라인 트레이스 기능을 제어합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotify_LineTraceOnOff.generated.h"

/**
 * @brief 라인 트레이스 온/오프 애니메이션 노티파이
 * 
 * 애니메이션 시퀀스에서 보스의 라인 트레이스를 활성화/비활성화하는 노티파이입니다.
 * 웨폰 컴포넌트를 통해 라인 트레이스 기능을 제어합니다.
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotify_LineTraceOnOff : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation ( ) const override;
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsMove=true;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsEndMove=true;
};
