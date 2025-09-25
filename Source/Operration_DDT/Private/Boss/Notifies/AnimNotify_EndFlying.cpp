/**
 * @file AnimNotify_EndFlying.cpp
 * @brief 비행 종료 애니메이션 노티파이 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 비행을 종료하는 노티파이입니다.
 * 비행 컴포넌트의 비행 중지 기능을 호출합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotify_EndFlying.h"
#include "Global.h"
#include "Boss/Component/FlyingComponent.h"

FString UAnimNotify_EndFlying::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_EndFlying::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	UFlyingComponent* FlyingComponent = CHelpers::GetComponent<UFlyingComponent>(MeshComp->GetOwner());
	CheckNull(FlyingComponent);
	FlyingComponent->StopFlying();
}
