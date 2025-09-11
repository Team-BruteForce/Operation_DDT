// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CMagazineComponent.h"
#include "Global.h"
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
	AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();

	TotalRifleBullets <= RifleMagazines ? CurrentRifleBullets = TotalRifleBullets : CurrentRifleBullets = RifleMagazines;
	TotalRifleBullets -= CurrentRifleBullets;
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
	--TotalRifleBullets;
}

void UCMagazineComponent::LootRifleBullets(int32 InValue)
{
	TotalRifleBullets += InValue;
}

// 메인 장전 함수
void UCMagazineComponent::StartReloadSequence()
{
	if (CheckCanReload() == false)
		return;
	
	// 장전 가능한 탄약 수 계산
	MaxReloadLoopCount = RifleMagazines - CurrentRifleBullets;
	ReloadLoopCount = 0;
	bIsReloading = true;
	
	// Reload_Start 몽타주 재생
	PlayMontage(Reload_Start);
	bReloadStart = true;
}

// Reload_Start 완료 시 호출되는 콜백
void UCMagazineComponent::OnReloadStartFinished()
{
	bReloadStart = false;
	
	// Reload_Loop 시작
	StartReloadLoop();
}

// Reload_Loop 시작 함수
void UCMagazineComponent::StartReloadLoop()
{
	if (ReloadLoopCount < MaxReloadLoopCount)
	{
		PlayMontage(Reload_Loop);
		bReloadLoop = true;
	}
	else
	{
		// 모든 루프가 끝났으므로 Reload_End 시작
		StartReloadEnd();
	}
}

// Reload_Loop 완료 시 호출되는 콜백
void UCMagazineComponent::OnReloadLoopFinished()
{
	bReloadLoop = false;
	
	// 탄약 추가
	ReloadRifleMagazine();
	
	// 루프 카운트 증가
	ReloadLoopCount++;
	
	// 다음 루프 또는 종료
	StartReloadLoop();
}

// Reload_End 시작 함수
void UCMagazineComponent::StartReloadEnd()
{
	PlayMontage(Reload_End);
	bReloadEnd = true;
}

// Reload_End 완료 시 호출되는 콜백
void UCMagazineComponent::OnReloadEndFinished()
{
	bReloadEnd = false;
	bIsReloading = false;
	
	// 장전 시퀀스 완료
	InitializeReload();
}

// 몽타주 재생 함수
void UCMagazineComponent::PlayMontage(UAnimMontage* Montage)
{
	if (AnimInstance != nullptr && Montage != nullptr)
	{
		AnimInstance->Montage_Play(Montage);
		
		// 몽타주 완료 델리게이트 바인딩
		if (Montage == Reload_Start)
		{
			// Reload_Start 완료 시 OnReloadStartFinished 호출
			BindMontageEndDelegate();
		}
		else if (Montage == Reload_Loop)
		{
			// Reload_Loop 완료 시 OnReloadLoopFinished 호출
			BindMontageEndDelegate();
		}
		else if (Montage == Reload_End)
		{
			// Reload_End 완료 시 OnReloadEndFinished 호출
			BindMontageEndDelegate();
		}
	}
}

// 장전 중단 함수
void UCMagazineComponent::CancelReload()
{
	if (bIsReloading)
	{
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Stop(0.0f);
		}
		InitializeReload();
		bIsReloading = false;
	}
}

// 델리게이트 바인딩 함수
void UCMagazineComponent::BindMontageEndDelegate()
{
	if (AnimInstance != nullptr)
	{
		// 몽타주 완료 델리게이트 바인딩
		
		if (bReloadStart)
		{
			MontageEndedDelegate.BindUFunction(this, FName("OnReloadStartFinished"));
			
		}
		else if (bReloadLoop)
		{
			MontageEndedDelegate.BindUFunction(this, FName("OnReloadLoopFinished"));
		}
		else if (bReloadEnd)
		{
			MontageEndedDelegate.BindUFunction(this, FName("OnReloadEndFinished"));
		}
		
		AnimInstance->Montage_SetEndDelegate(MontageEndedDelegate);
	}
}

// 델리게이트 언바인딩 함수
void UCMagazineComponent::UnbindMontageEndDelegate()
{
	if (AnimInstance != nullptr)
	{
		FOnMontageEnded EmptyDelegate;
		AnimInstance->Montage_SetEndDelegate(EmptyDelegate);
	}
}

