// Fill out your copyright notice in the Description page of Project Settings.


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
