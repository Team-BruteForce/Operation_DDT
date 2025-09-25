/**
 * @file AnimNotify_BeginFlying.cpp
 * @brief 비행 시작 애니메이션 노티파이 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 비행을 시작하는 노티파이입니다.
 * 호버링을 중지하고 이륙을 시작합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotify_BeginFlying.h"
#include "Global.h"
#include "Boss/CBoss.h"
#include "Boss/Component/CBossMovementComponent.h"
#include "Boss/Component/FlyingComponent.h"

/**
 * @brief 노티파이 이름을 반환하는 함수
 * 
 * @return FString 노티파이 이름
 */
FString UAnimNotify_BeginFlying::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

/**
 * @brief 노티파이 실행 함수
 * 
 * @param MeshComp 스켈레탈 메시 컴포넌트
 * @param Animation 애니메이션 시퀀스
 * 
 * 보스의 비행을 시작하고 이륙을 실행합니다.
 */
void UAnimNotify_BeginFlying::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	ACBoss* Owner = Cast<ACBoss>(MeshComp->GetOwner());
	CheckNull(Owner);
	UFlyingComponent* FlyingComponent = CHelpers::GetComponent<UFlyingComponent>(Owner);
	CheckNull(FlyingComponent);
	FlyingComponent->StopHovering(); 
	FlyingComponent->StartTakeoff(Height,speed);
}
