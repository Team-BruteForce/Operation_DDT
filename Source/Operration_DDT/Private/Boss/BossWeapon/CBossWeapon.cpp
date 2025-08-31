// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossWeapon/CBossWeapon.h"
#include "Global.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Character.h"

// Sets default values
ACBossWeapon::ACBossWeapon()
{
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh");
	CHelpers::CreateComponent(this, &Root, "Root");
	SetRootComponent(Root);
	SkeletalMesh->SetupAttachment(Root);
    
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMesh->SetVisibility(false);
}

// Called when the game starts or when spawned
void ACBossWeapon::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	TArray<USceneComponent*> children;
	Root->GetChildrenComponents(true, children);
	for (USceneComponent* child : children)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(child);
		CLog::Log("ACBossWeapon"+child->GetName());
		if (!!shape)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACBossWeapon::OnBossComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACBossWeapon::OnBossComponentEndOverlap);

			Collisions.Add(shape);
		}
	}
	OffBossCollisions();

	Super::BeginPlay();
}

void ACBossWeapon::OnBossCollisions()
{
	if (OnBossAttachmentBeginCollision.IsBound())
		OnBossAttachmentBeginCollision.Broadcast();

	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACBossWeapon::OffBossCollisions()
{
	if (OnBossAttachmentEndCollision.IsBound())
		OnBossAttachmentEndCollision.Broadcast();

	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACBossWeapon::OnBossComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());

	if (OnBossAttachmentBeginOverlap.IsBound())
		OnBossAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));
	OffBossCollisions();
}
void ACBossWeapon::OnBossComponentEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());
	
	if (OnBossAttachmentEndOverlap.IsBound())
		OnBossAttachmentEndOverlap.Broadcast(OwnerCharacter, Cast<ACharacter>(OtherActor));
}

void ACBossWeapon::BossAttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void ACBossWeapon::BossAttachToCollision(FName InCollisionName, FName InSocketName)
{
	for (UShapeComponent* collision : Collisions)
	{
		if (collision->GetName() == InCollisionName.ToString())
		{
			collision->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);

			return;
		}
	}
}

