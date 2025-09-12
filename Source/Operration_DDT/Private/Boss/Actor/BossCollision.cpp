// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Actor/BossCollision.h"

#include "Boss/BossWeapon/CBossWeapon.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Global.h"

// Sets default values
ABossCollision::ABossCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CHelpers::CreateComponent(this,&BoxComp,"BoxComp",RootComponent);
	BoxComp->SetBoxExtent(FVector(80,80,700));
}

void ABossCollision::BeginPlay()
{
	Super::BeginPlay();
	BoxComp->OnComponentBeginOverlap.AddDynamic(this,&ABossCollision::OnBossComponentBeginOverlap);
	BoxComp->OnComponentEndOverlap.AddDynamic(this,&ABossCollision::OnBossComponentEndOverlap);
	Weapon=Cast<ACBossWeapon>(GetOwner());
}

void ABossCollision::OnBossComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckNull(Cast<ACharacter>(OtherActor));
	Weapon->OnBossAttachmentBeginOverlap.Broadcast(Cast<ACharacter>(Weapon->GetOwner()),Weapon,Cast<ACharacter>(OtherActor));
	
}

void ABossCollision::OnBossComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	CheckNull(Cast<ACharacter>(OtherActor));
	Weapon->OnBossAttachmentEndOverlap.Broadcast(Cast<ACharacter>(Weapon->GetOwner()),Cast<ACharacter>(OtherActor));
}

