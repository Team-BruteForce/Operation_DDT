/**
 * @file CBossEndAttack.h
 * @brief 보스 공격 종료 애니메이션 노티파이 헤더 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 공격 종료를 처리하는 노티파이입니다.
 * 보스의 공격 상태를 종료하고 다음 행동을 준비합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CBossEndAttack.generated.h"

/**
 * @brief 보스 공격 종료 애니메이션 노티파이
 * 
 * 애니메이션 시퀀스에서 보스의 공격 종료를 처리하는 노티파이입니다.
 * 보스의 공격 상태를 종료하고 다음 행동을 준비합니다.
 */
UCLASS(Blueprintable)
class OPERRATION_DDT_API UCBossEndAttack : public UAnimNotify
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation ( ) const override;

	void Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation ) override;
	
	UPROPERTY(EditAnywhere,Category="Tag")
	FGameplayTag EventTag;
};
