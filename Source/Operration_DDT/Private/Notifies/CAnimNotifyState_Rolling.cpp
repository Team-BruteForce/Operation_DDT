// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotifyState_Rolling.h"
#include "Global.h"
#include "Player/Components/CCameraActionComponent.h"
#include "Player/Components/CStateComponent.h"

FString UCAnimNotifyState_Rolling::GetNotifyAction() const
{
	return "Rolling";
}

void UCAnimNotifyState_Rolling::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());
	State = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CameraAction = CHelpers::GetComponent<UCCameraActionComponent>(MeshComp->GetOwner());
	CheckNull(State);
	CheckNull(CameraAction);

	CheckFalse(State->IsRollingMode());

	if (State->IsRifleAimMode())
	{
		State->SetRifleAimMode(false);
		CameraAction->SetIdlePosition();
	}
	State->SetCanDodge(true);

}

void UCAnimNotifyState_Rolling::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::NotifyEnd(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	CheckNull(State);
	
	State->SetCanDodge(false);
}
