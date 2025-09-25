/**
 * @file AnimNotify_BossWeaponCollision.h
 * @brief 보스 웨폰 콜리전 애니메이션 노티파이 헤더 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스 웨폰의 콜리전을 활성화/비활성화하는 노티파이입니다.
 * 보스의 공격 범위를 제어합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotify_BossWeaponCollision.generated.h"

/**
 * @brief 보스 웨폰 콜리전 애니메이션 노티파이
 * 
 * 애니메이션 시퀀스에서 보스 웨폰의 콜리전을 활성화/비활성화하는 노티파이입니다.
 * 보스의 공격 범위를 제어합니다.
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotify_BossWeaponCollision : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation ( ) const override;
	
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
};
