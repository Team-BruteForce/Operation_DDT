/**
 * @file AnimNotifyState_PaseChange.cpp
 * @brief 페이즈 변경 애니메이션 노티파이 상태 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 페이즈 변경을 처리하는 노티파이 상태입니다.
 * 보스 상태 컴포넌트를 통해 페이즈 변경을 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotifyState_PaseChange.h"
#include "Global.h"
#include "Boss/Component/CBossStatusComponent.h"
FString UAnimNotifyState_PaseChange::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotifyState_PaseChange::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                              float TotalDuration)
{
	CheckNull(MeshComp->GetOwner());
	UCBossStatusComponent* Status = CHelpers::GetComponent<UCBossStatusComponent>(MeshComp->GetOwner());
	CheckNull(Status);
	Status->OnPaseChange();
}

void UAnimNotifyState_PaseChange::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	CheckNull(MeshComp->GetOwner());
	UCBossStatusComponent* Status = CHelpers::GetComponent<UCBossStatusComponent>(MeshComp->GetOwner());
	CheckNull(Status);
	Status->OffPaseChange();
}
