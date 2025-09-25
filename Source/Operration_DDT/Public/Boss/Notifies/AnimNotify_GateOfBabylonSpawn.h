/**
 * @file AnimNotify_GateOfBabylonSpawn.h
 * @brief 바빌론의 문 스폰 애니메이션 노티파이 헤더 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 바빌론의 문을 스폰하는 노티파이입니다.
 * 프로젝타일 컴포넌트를 통해 바빌론의 문을 생성합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_GateOfBabylonSpawn.generated.h"

/**
 * @brief 바빌론의 문 스폰 애니메이션 노티파이
 * 
 * 애니메이션 시퀀스에서 바빌론의 문을 스폰하는 노티파이입니다.
 * 프로젝타일 컴포넌트를 통해 바빌론의 문을 생성합니다.
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotify_GateOfBabylonSpawn : public UAnimNotify
{
	GENERATED_BODY()
	FString GetNotifyName_Implementation ( ) const override;

	void Notify ( USkeletalMeshComponent* MeshComp , UAnimSequenceBase* Animation ) override;
};
