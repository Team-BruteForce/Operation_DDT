/**
 * @file AnimNotify_BeginFlying.h
 * @brief 비행 시작 애니메이션 노티파이 헤더 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 비행을 시작하는 노티파이입니다.
 * 호버링을 중지하고 이륙을 시작합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_BeginFlying.generated.h"

/**
 * @brief 비행 시작 애니메이션 노티파이
 * 
 * 애니메이션 시퀀스에서 보스의 비행을 시작하는 노티파이입니다.
 * 호버링을 중지하고 이륙을 시작합니다.
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotify_BeginFlying : public UAnimNotify
{
	GENERATED_BODY()
public:
	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Height= 300.0f;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float speed=300.0f;
};
