/**
 * @file AnimNotify_SelectCollisionOnOff.h
 * @brief 선택 콜리전 온/오프 애니메이션 노티파이 헤더 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 선택된 콜리전을 활성화/비활성화하는 노티파이입니다.
 * 웨폰 컴포넌트를 통해 특정 콜리전을 제어합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotify_SelectCollisionOnOff.generated.h"

/**
 * @brief 선택 콜리전 온/오프 애니메이션 노티파이
 * 
 * 애니메이션 시퀀스에서 보스의 선택된 콜리전을 활성화/비활성화하는 노티파이입니다.
 * 웨폰 컴포넌트를 통해 특정 콜리전을 제어합니다.
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotify_SelectCollisionOnOff : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation ( ) const override;
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName CollisionName;
};
