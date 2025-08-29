// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CFireComponent.h"
#include "Global.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CPlayerBullet.h"
#include "Player/DDTPlayer.h"
#include "Weapons/CAttachment.h"

// Sets default values for this component's properties
UCFireComponent::UCFireComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCFireComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ADDTPlayer>(GetOwner());
	FName HandSocketName = FName("Hand_Rifle");
	RifleSocketName = FName("MuzzlePos");
	Rifle = Cast<ACAttachment>(GetActorAttachedToSocket(HandSocketName));
	
	
}


// Called every frame
void UCFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

AActor* UCFireComponent::GetActorAttachedToSocket(const FName& SocketName)
{
	if (!OwnerCharacter)
	{
		CLog::Log("OwnerCharacter is null");
		return nullptr;
	}

	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
	if (!MeshComponent)
	{
		CLog::Log("MeshComponent is null");
		return nullptr;
	}

	TArray<USceneComponent*> AttachedComponents;
	MeshComponent->GetChildrenComponents(true, AttachedComponents);

	for (USceneComponent* Comp : AttachedComponents)
	{
		if (Comp->GetAttachSocketName() == SocketName)
		{
			AActor* AttachedActor = Comp->GetOwner();
			if (AttachedActor)
			{
				CLog::Log("Found Actor : "+ AttachedActor->GetName());
				return AttachedActor;
			}
		}
	}
	
	// 소켓에 붙어있는 액터를 찾지 못한 경우
	CLog::Log("No actor found attached to socket: " + SocketName.ToString());
	return nullptr;
}

void UCFireComponent::SetMuzzleVector(const FName& SocketName)
{
	if (!Rifle)
	{
		CLog::Log("Rifle is null");
		return;
	}
	USkeletalMeshComponent* MeshComponent = Rifle->GetMeshComp();
	if (!MeshComponent)
	{
		CLog::Log("MeshComponent is null");
		return;
	}
	if (MeshComponent->DoesSocketExist(SocketName))
	{
		MuzzleVector = MeshComponent->GetSocketLocation(SocketName);
		CLog::Log(MuzzleVector);
		
	}
	else
	{
		CLog::Log("Socket not found: " + SocketName.ToString());
	}
}

void UCFireComponent::Fire()
{
	
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	spawnParams.bNoFail = true;
	spawnParams.Owner = OwnerCharacter;

	FVector tmp = FVector(OwnerCharacter->GetActorLocation().X, OwnerCharacter->GetActorLocation().Y,OwnerCharacter->GetActorLocation().Z);

	//MuzzleVector = tmp;

	SetMuzzleVector(RifleSocketName);
	
	ACPlayerBullet* Bullet = GetWorld()->SpawnActor<ACPlayerBullet>(PlayerBulletClass, MuzzleVector, FRotator(0.f,0.f,0.f), spawnParams);
	if (Bullet)
	{
		CLog::Log(MuzzleVector);
		CLog::Log("Spawned Bullet");
		
		Bullet->SetVelocity(OwnerCharacter->GetActorForwardVector());
		
	}
}

