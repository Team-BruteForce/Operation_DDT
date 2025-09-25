/**
 * @file AnimNotify_Landing.cpp
 * @brief 착륙 애니메이션 노티파이 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 착륙을 처리하는 노티파이입니다.
 * 비행 컴포넌트를 통해 착륙 기능을 실행합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotify_Landing.h"
#include "Global.h"
#include "Boss/Component/FlyingComponent.h"

UAnimNotify_Landing::UAnimNotify_Landing()
{
	LandingSpeed = 200.0f;
}

FString UAnimNotify_Landing::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_Landing::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	UFlyingComponent* FlyingComp = CHelpers::GetComponent<UFlyingComponent>(MeshComp->GetOwner());
	CheckNull(FlyingComp);
	
	// 착륙 속도 설정
	FlyingComp->LandingSpeed = LandingSpeed;
	
	// 착륙 시작
	FlyingComp->StartLanding();
}
