/**
 * @file AnimNotify_Groggying.cpp
 * @brief 그로기 상태 애니메이션 노티파이 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 그로기 상태를 처리하는 노티파이입니다.
 * 보스 상태 컴포넌트를 통해 그로기 상태를 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotify_Groggying.h"
#include "Global.h"
#include "Boss/Component/CBossStatusComponent.h"

FString UAnimNotify_Groggying::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_Groggying::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	UCBossStatusComponent* Status = CHelpers::GetComponent<UCBossStatusComponent>(MeshComp->GetOwner());
	CheckNull(Status);
	Status->OnGroggy();
}

void UAnimNotify_Groggying::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UCBossStatusComponent* Status = CHelpers::GetComponent<UCBossStatusComponent>(MeshComp->GetOwner());
	CheckNull(Status);
	Status->OffGroggy();
}
