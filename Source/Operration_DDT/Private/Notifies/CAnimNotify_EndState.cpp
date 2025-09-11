// Fill out your copyright notice in the Description page of Project Settings.


#include "Notifies/CAnimNotify_EndState.h"
#include "Global.h"
#include "Player/DDTPlayer.h"

FString UCAnimNotify_EndState::GetNotifyName_Implementation() const
{
	return "EndState";
}

void UCAnimNotify_EndState::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp);
	CheckNull(MeshComp->GetOwner());

	ADDTPlayer* OwnerCharater  = Cast<ADDTPlayer>(MeshComp->GetOwner());
	CheckNull(OwnerCharater);

	switch (StateType)
	{
		case EStateType::Rolling:
		{
			OwnerCharater->End_Rolling();
			break;
		}
		case EStateType::Hitted:
		{
			OwnerCharater->End_Hitted();
			break;
		}
		case EStateType::Healing:
		{
			OwnerCharater->End_Healing();
			break;
		}
		case EStateType::RifleReload:
		{
			OwnerCharater->End_Reload();
			break;
		}
	}
	
}
