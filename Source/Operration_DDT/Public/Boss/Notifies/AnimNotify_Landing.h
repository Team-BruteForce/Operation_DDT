// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_Landing.generated.h"

/**
 * @brief 착륙 애니메이션 노티파이
 * 
 * 보스가 착륙할 때 FlyingComponent의 착륙 기능을 호출합니다.
 * 에디터에서 착륙 속도를 설정할 수 있습니다.
 */
UCLASS()
class OPERRATION_DDT_API UAnimNotify_Landing : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAnimNotify_Landing();

	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	/**
	 * @brief 착륙 속도
	 * 
	 * 에디터에서 설정 가능한 착륙 속도입니다.
	 * 기본값: 200.0f
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Landing Settings")
	float LandingSpeed = 200.0f;
};
