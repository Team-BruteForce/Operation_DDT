/**
 * @file AnimNotify_PaseChangeDissolve.cpp
 * @brief 페이즈 변경 디졸브 애니메이션 노티파이 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 페이즈 변경 시 디졸브 효과를 처리하는 노티파이입니다.
 * 보스의 페이즈 전환 시 디졸브 애니메이션을 실행합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotify_PaseChangeDissolve.h"

FString UAnimNotify_PaseChangeDissolve::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_PaseChangeDissolve::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
}
