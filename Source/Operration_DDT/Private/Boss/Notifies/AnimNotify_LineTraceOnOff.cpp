/**
 * @file AnimNotify_LineTraceOnOff.cpp
 * @brief 라인 트레이스 온/오프 애니메이션 노티파이 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스의 라인 트레이스를 활성화/비활성화하는 노티파이입니다.
 * 웨폰 컴포넌트를 통해 라인 트레이스 기능을 제어합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotify_LineTraceOnOff.h"
#include "Boss/BossWeapon/CBossWeapon.h"
#include "Boss/Component/CBossWeaponComponent.h"
#include "Global.h"

FString UAnimNotify_LineTraceOnOff::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_LineTraceOnOff::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	UCBossWeaponComponent* Weapon=CHelpers::GetComponent<UCBossWeaponComponent>(MeshComp->GetOwner());
	CheckNull(Weapon);
	// 노티파이 스테이트 Begin
	Weapon->GetBossWeapon()->StartCollisionAtSocket(FName("Slash"),IsMove);
}

void UAnimNotify_LineTraceOnOff::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	UCBossWeaponComponent* Weapon=CHelpers::GetComponent<UCBossWeaponComponent>(MeshComp->GetOwner());
	CheckNull(Weapon);
	// 노티파이 스테이트 Begin
	Weapon->GetBossWeapon()->EndCollisionToOwner(IsEndMove);
}
