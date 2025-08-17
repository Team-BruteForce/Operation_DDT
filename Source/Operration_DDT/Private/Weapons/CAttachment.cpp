// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/CAttachment.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ACAttachment::ACAttachment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent (this, &Root, "Root");

}

// Called when the game starts or when spawned
void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	Super::BeginPlay();
		
}

// Called every frame
void ACAttachment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACAttachment::AttachTo(FName InSocketName)
{
	CLog::Print(InSocketName.ToString());
	AttachToComponent (OwnerCharacter->GetMesh (), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}