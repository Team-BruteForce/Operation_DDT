/**
 * @file AnimNotify_ChaseRotation.cpp
 * @brief 추적 회전 애니메이션 노티파이 구현 파일
 * 
 * @details
 * 애니메이션 시퀀스에서 보스가 플레이어를 추적하며 회전하는 노티파이입니다.
 * 보스의 회전 기능을 활성화/비활성화합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Notifies/AnimNotify_ChaseRotation.h"
#include "Global.h"
#include "Boss/Component/CBossMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"

FString UAnimNotify_ChaseRotation::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_ChaseRotation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	if (!MeshComp) return;
	
	ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (!OwnerCharacter) return;
	
	MovementComponent = CHelpers::GetComponent<UCBossMovementComponent>(OwnerCharacter);
	if (MovementComponent)
	{
		bIsChasing = true;
		CLog::Log("Chase Rotation Started");
	}
}

void UAnimNotify_ChaseRotation::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
	
	if (MovementComponent && bIsChasing)
	{
		MovementComponent->RotateTowardsPlayer(FrameDeltaTime, RotationSpeed);
	}
}

void UAnimNotify_ChaseRotation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	if (MovementComponent && bIsChasing)
	{
		bIsChasing = false;
		MovementComponent = nullptr;
		CLog::Log("Chase Rotation Ended");
	}
}
