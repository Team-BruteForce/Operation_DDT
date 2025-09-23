// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CDamageUIManageComponent.h"
#include "Global.h"
#include "Blueprint/UserWidget.h"
#include "Player/DDTPlayer.h"
#include "Player/Widget/CDamageWidget_Normal.h"

// Sets default values for this component's properties
UCDamageUIManageComponent::UCDamageUIManageComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCDamageUIManageComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCharacter = Cast<ADDTPlayer>(GetOwner());

	// 오브젝트 풀 초기화
	for (int32 i = 0; i < MaxPoolSize; i++)
	{
		UCDamageWidget_Normal* ui = CreateDamageUI();
		if (ui)
			DamageUIPool.Add(ui);
	}
	
}


// Called every frame
void UCDamageUIManageComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


UCDamageWidget_Normal* UCDamageUIManageComponent::GetInactiveUI()
{
	// 순환 방식으로 비활성화된 총알 찾기
	for (int32 i = 0; i < DamageUIPool.Num(); i++)
	{
		int32 index = (CurrentDamageUIPoolIndex + i) % DamageUIPool.Num();
		UCDamageWidget_Normal* damageUI = DamageUIPool[i];
		
		// 더 엄격한 체크 - 사용 중이 아닌 총알만 반환
		if (!damageUI->IsActive())
		{
			CurrentDamageUIPoolIndex = (index + 1) % DamageUIPool.Num();
			CLog::Log("Pool) Return UI : " + FString::FromInt(CurrentDamageUIPoolIndex));
			return damageUI;
		}
	}
	
	UCDamageWidget_Normal* newUI = CreateDamageUI();
	//MaxMagazinePool++;
	DamageUIPool.Add(newUI);
	return newUI;
}

UCDamageWidget_Normal* UCDamageUIManageComponent::CreateDamageUI()
{
	UCDamageWidget_Normal* ui =  CreateWidget<UCDamageWidget_Normal>(GetWorld(),DamageWidget);
	ui->SetActive(false);	
	return ui;
}

