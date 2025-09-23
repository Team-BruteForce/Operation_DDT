// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Widget/CNormalDamageUIActor.h"
#include "Global.h"
#include "Components/WidgetComponent.h"
#include "Player/DDTPlayer.h"
#include "Camera/CameraComponent.h"
#include "Player/Widget/CDamageWidget_Normal.h"

// Sets default values
ACNormalDamageUIActor::ACNormalDamageUIActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	SetRootComponent(SceneComp);

	WidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComp"));
	WidgetComp->SetupAttachment(SceneComp);

}

// Called when the game starts or when spawned
void ACNormalDamageUIActor::BeginPlay()
{
	Super::BeginPlay();

	Player = Cast<ADDTPlayer>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		CameraComp = CHelpers::GetComponent<UCameraComponent>(Player);
	}

	if (UIWidgetClass)
	{
		WidgetComp->SetWidgetClass(UIWidgetClass);
		CLog::Log(TEXT("ACNormalDamageUIActor) WidgetClass Load Success"));
		UUserWidget* Widget = WidgetComp->GetWidget();
		DamageUI = Cast<UCDamageWidget_Normal>(Widget);
	}
	else
	{
		CLog::Log(TEXT("ACNormalDamageUIActor) WidgetClass Load Fail"));
	}

}

// Called every frame
void ACNormalDamageUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CameraComp)
	{
		FVector dir = CameraComp->GetComponentLocation() - GetActorLocation() ;
		FRotator LookRotation = FRotationMatrix::MakeFromX(dir).Rotator();
		SetActorRotation(LookRotation);
	}
}

void ACNormalDamageUIActor::StartLifeTimer()
{
	// 수명 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(
		LifeTimerHandle,
		this,
		&ACNormalDamageUIActor::OnLifeTimeExpired,
		LifeTime,
		false  // 한 번만 실행
	);
}

void ACNormalDamageUIActor::StopLifeTimer()
{
	// 수명 타이머 중지
	GetWorld()->GetTimerManager().ClearTimer(LifeTimerHandle);
}

void ACNormalDamageUIActor::OnLifeTimeExpired()
{
	// 수명이 만료되면 풀로 돌아가기
	ReturnToPool();
}

void ACNormalDamageUIActor::ReturnToPool()
{
	if (bIsActive)
	{
		// 타이머 먼저 정지
		StopLifeTimer();
		
		// 상태 초기화
		SetActive(false);
	}
}

void ACNormalDamageUIActor::SetActive(bool bValue)
{
	bIsActive = bValue;
	SceneComp->SetVisibility(bValue);

	if (bIsActive)
	{
		DamageUI->ShowDamageWidgetAnimation();
	}
	else
	{
		DamageUI->ResetTextPosition();
	}
}

bool ACNormalDamageUIActor::IsActive() const
{
	return bIsActive;
}

