// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CMagazineComponent.h"
#include "Global.h"
#include "IO/OnDemandToc.h"
#include "Player/DDTPlayer.h"

// Sets default values for this component's properties
UCMagazineComponent::UCMagazineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


// Called when the game starts
void UCMagazineComponent::BeginPlay()
{
	Super::BeginPlay();

	// AnimInstance 할당
	OwnerCharacter = Cast<ADDTPlayer>(GetOwner());

	TotalRifleBullets <= RifleMagazines ? CurrentRifleBullets = TotalRifleBullets : CurrentRifleBullets = RifleMagazines;
	TotalRifleBullets -= CurrentRifleBullets;

	OnCurrentBulletChanged.Broadcast(CurrentRifleBullets);
	OnTotalBulletChanged.Broadcast(TotalRifleBullets);
	
}


// Called every frame
void UCMagazineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	

	// ...
}

bool UCMagazineComponent::CheckCanReload()
{
	if (TotalRifleBullets <= 0 || CurrentRifleBullets == RifleMagazines) return false;
	return true;
}

void UCMagazineComponent::ReloadRifleMagazine()
{
	if (CheckCanReload() == false ) return;
	
	++CurrentRifleBullets;
	OnCurrentBulletChanged.Broadcast(CurrentRifleBullets);
	--TotalRifleBullets;
	OnTotalBulletChanged.Broadcast(TotalRifleBullets);
}

void UCMagazineComponent::LootRifleBullets(int32 InValue)
{
	TotalRifleBullets += InValue;
	OnTotalBulletChanged.Broadcast(TotalRifleBullets);
}

void UCMagazineComponent::Reloading()
{
	if (CheckCanReload() == false)
		return;
	
	// 장전 가능한 탄약 수 계산
	MaxReloadLoopCount = RifleMagazines - CurrentRifleBullets ;
	ReloadLoopCount = 0;
	
	// Reload_Start 몽타주 재생
	OwnerCharacter->PlayAnimMontage(Reload_Start);
}

void UCMagazineComponent::ResetMagazines()
{
	TotalRifleBullets = 50;
	
	TotalRifleBullets <= RifleMagazines ? CurrentRifleBullets = TotalRifleBullets : CurrentRifleBullets = RifleMagazines;
	TotalRifleBullets -= CurrentRifleBullets;

	OnCurrentBulletChanged.Broadcast(CurrentRifleBullets);
	OnTotalBulletChanged.Broadcast(TotalRifleBullets);
}

