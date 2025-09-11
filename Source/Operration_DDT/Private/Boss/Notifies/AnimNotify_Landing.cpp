// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/Notifies/AnimNotify_Landing.h"
#include "Global.h"
#include "Boss/Component/FlyingComponent.h"

UAnimNotify_Landing::UAnimNotify_Landing()
{
	LandingSpeed = 200.0f;
}

FString UAnimNotify_Landing::GetNotifyName_Implementation() const
{
	return Super::GetNotifyName_Implementation();
}

void UAnimNotify_Landing::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	UFlyingComponent* FlyingComp = CHelpers::GetComponent<UFlyingComponent>(MeshComp->GetOwner());
	CheckNull(FlyingComp);
	
	// 착륙 속도 설정
	FlyingComp->LandingSpeed = LandingSpeed;
	
	// 착륙 시작
	FlyingComp->StartLanding();
}
