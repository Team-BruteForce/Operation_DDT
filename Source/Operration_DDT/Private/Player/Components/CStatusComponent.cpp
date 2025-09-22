// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CStatusComponent.h"
#include "Global.h"
#include "Player/DDTPlayer.h"

// Sets default values for this component's properties
UCStatusComponent::UCStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCStatusComponent::BeginPlay()
{
	Super::BeginPlay();

	
	OwnerCharater = Cast<ADDTPlayer>(GetOwner());
	StateComp = CHelpers::GetComponent<UCStateComponent>(OwnerCharater);

	// 초기 체력 설정
	NowHp = MaxHp;
	OnPlayerHealthChanged.Broadcast(NowHp, NowHp, MaxHp);
	OnHealItemChanged.Broadcast(HealItemCount);
	
}


// Called every frame
void UCStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 회복 중이면 점진적으로 체력 증가
	if (bIsHealing)
	{
		HealElapsedTime += DeltaTime;
		
		// 경과 시간에 따른 회복 진행률 계산 (0.0 ~ 1.0)
		float HealProgress = FMath::Clamp(HealElapsedTime / HealDuration, 0.0f, 1.0f);
		
		// 부드러운 곡선을 위한 EaseOut 함수 적용
		float EasedProgress = 1.0f - FMath::Pow(1.0f - HealProgress, 3.0f);
		
		// 현재 체력 = 시작 체력 + (목표 회복량 * 진행률)
		float prevHP = NowHp;
		NowHp = HealStartAmount + (HealTargetAmount * EasedProgress);
		OnPlayerHealthChanged.Broadcast(prevHP,NowHp, MaxHp);
		
		// 회복 완료 체크
		if (HealElapsedTime >= HealDuration)
		{
			NowHp = HealStartAmount + HealTargetAmount; // 정확한 목표값으로 설정
			bIsHealing = false;
			HealElapsedTime = 0.0f;

			HealItemCount--;
			OnHealItemChanged.Broadcast(HealItemCount);
			CLog::Log("회복 완료! 현재 체력: " + FString::SanitizeFloat(NowHp));
		}
	}
}

void UCStatusComponent::GainHealItem()
{
	HealItemCount = FMath::Clamp(HealItemCount + 1, 0, 3);
	OnHealItemChanged.Broadcast(HealItemCount);
}

void UCStatusComponent::GetDamage(float value)
{
	float prevHp = NowHp;
	NowHp = FMath::Clamp(NowHp - value, 0.0f, MaxHp);
	OnPlayerHealthChanged.Broadcast(prevHp, NowHp, MaxHp);
}

void UCStatusComponent::GetHeal(float value)
{
	float prevHp = NowHp;
	NowHp = FMath::Clamp(NowHp + value, 0.0f, MaxHp);
	OnPlayerHealthChanged.Broadcast(prevHp,NowHp, MaxHp);
}

void UCStatusComponent::CalculateHealing()
{
	CheckTrue(StateComp->IsHealingMode());
	CheckTrue(StateComp->IsRollingMode());
	// 회복 가능 여부 체크
	if (!CanHeal())
	{
		CLog::Log("회복할 수 없습니다!");
		return;
	}
	
	// 기존 회복 중단
	StopHealing();
	
	// 회복량 계산 (최대 체력의 설정된 비율%)
	HealAmount = MaxHp * HealPercentage;
	
	// 실제 회복 가능한 양 계산 (최대 체력 초과 방지)
	ActualHealAmount = FMath::Min(HealAmount, MaxHp - NowHp);
	
	if (ActualHealAmount <= 0.0f || NowHp == MaxHp)
	{
		CLog::Log("이미 최대 체력입니다!");
		StateComp->SetIdleMode();
		return;
	}

	else
	{
		StateComp->SetHealingMode();
	}
}

void UCStatusComponent::StartHealing()
{
	
	// 회복 시작
	bIsHealing = true;
	HealStartAmount = NowHp;
	HealTargetAmount = ActualHealAmount;
	HealElapsedTime = 0.0f;
	
	CLog::Log("회복 시작! " + FString::SanitizeFloat(NowHp) + " → " + 
		FString::SanitizeFloat(NowHp + ActualHealAmount) + " (" + 
		FString::SanitizeFloat(HealDuration) + "초 소요)");
}

void UCStatusComponent::StopHealing()
{
	if (bIsHealing)
	{
		bIsHealing = false;
		HealElapsedTime = 0.0f;
		CLog::Log("회복 중단");
	}
}

bool UCStatusComponent::CanHeal() const
{
	// 이미 최대 체력이거나, 이미 회복 중이면 회복 불가
	return (NowHp < MaxHp) && !bIsHealing;
}

float UCStatusComponent::GetHealthPercentage() const
{
	return (MaxHp > 0.0f) ? (NowHp / MaxHp) : 0.0f;
}

void UCStatusComponent::ResetStatus()
{
	SetFullHealth();
	OnPlayerHealthChanged.Broadcast(NowHp,NowHp, MaxHp);
	HealItemCount = 3;
	OnHealItemChanged.Broadcast(HealItemCount);
}

