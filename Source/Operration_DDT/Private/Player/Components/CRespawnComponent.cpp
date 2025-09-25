// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CRespawnComponent.h"
#include "Global.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CStateComponent.h"
#include "Player/Components/CMontageComponent.h"
#include "Player/Components/CStatusComponent.h"
#include "Player/Components/CMovementComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Boss/BossManager.h"
#include "Components/CapsuleComponent.h"
// BossManager BP 자동 탐색용
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ODH/ODH_Enemy/Interface/AllEnemyRestart.h"
#include "ODH/ODH_Enemy/CCombatEncounterManager.h"
#include "ODH/Component/CItemPoolManager.h"
#include "ODH/Component/CItemDropObjectComponent.h"
#include "Player/DDTGameMode.h"
#include "Player/Widget/CPlayerUI.h"
#include "Player/Components/CMagazineComponent.h"
#include "Player/Components/CStaminaComponent.h"
#include "Player/Components/CUIComponent.h"

// Sets default values for this component's properties
UCRespawnComponent::UCRespawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCRespawnComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	OwnerCharacter = Cast<ADDTPlayer>(GetOwner());
	RespawnLocation = OwnerCharacter->GetActorLocation();
	
	State = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	Capsule = CHelpers::GetComponent<UCapsuleComponent>(OwnerCharacter);
	Movement = CHelpers::GetComponent<UCMovementComponent>(OwnerCharacter);
	Status = CHelpers::GetComponent<UCStatusComponent>(OwnerCharacter);
	Magazine = CHelpers::GetComponent<UCMagazineComponent>(OwnerCharacter);
	Stamina = CHelpers::GetComponent<UCStaminaComponent>(OwnerCharacter);
	UIComp = CHelpers::GetComponent<UCUIComponent>(OwnerCharacter);
	
	// DieDelegate 구독
	if (OwnerCharacter && OwnerCharacter->Montages)
	{
		OwnerCharacter->Montages->DieDelegate.AddDynamic(this, &UCRespawnComponent::OnPlayerDied);
		CLog::Log("RespawnComponent: DieDelegate subscribed successfully");
	}
	else
	{
		CLog::Log("RespawnComponent: Failed to subscribe to DieDelegate");
	}
	
	// 클래스 기반 탐색 (레벨에 하나만 배치되어 있다는 가정)
	if (!BossManager)
	{
		TArray<AActor*> FoundManagers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABossManager::StaticClass(), FoundManagers);
		if (FoundManagers.Num() > 0)
		{
			BossManager = Cast<ABossManager>(FoundManagers[0]);
		}
	}

	if (BossManager)
	{
		CLog::Log("RespawnComponent: BossManager found in level: " + BossManager->GetName());
	}
	else
	{
		CLog::Log("RespawnComponent: Cannot find BossManager in level");
	}

	// CCombatEncounterManager 찾기
	if (!CombatEncounterManager)
	{
		TArray<AActor*> FoundEncounterManagers;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACCombatEncounterManager::StaticClass(), FoundEncounterManagers);
		if (FoundEncounterManagers.Num() > 0)
		{
			CombatEncounterManager = Cast<ACCombatEncounterManager>(FoundEncounterManagers[0]);
		}
	}

	if (CombatEncounterManager)
	{
		CLog::Log("RespawnComponent: CombatEncounterManager found in level: " + CombatEncounterManager->GetName());
	}
	else
	{
		CLog::Log("RespawnComponent: Cannot find CombatEncounterManager in level");
	}
}


// Called every frame
void UCRespawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// CLog::Log("RespawnLocation: " + RespawnLocation.ToString());
	// ...
}

void UCRespawnComponent::OnPlayerDied() 
{
	if (OwnerCharacter && UIComp && UIComp->playerUI)
	{
		CLog::Log("OnPlayerDied) Owner, UIComp, PlayerUI");
		ADDTGameMode* GM = GetWorld()->GetAuthGameMode<ADDTGameMode>();
		if (GM)
		{
			CLog::Log("OnPlayerDied)  GM Successed");
			UIComp->playerUI->OnAnimFinishedDelegate.AddDynamic(GM, &ADDTGameMode::LinkedMaintoLoading);
			CLog::Log("OnPlayerDied)  AddDynamic");
		}
		
	}
	
	// 플레이어가 사망했을 때 호출되는 함수
	CLog::Log("RespawnComponent: Player Died! Starting respawn timer...");
	
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Movement->Stop();
	OwnerCharacter->GetCharacterMovement()->StopActiveMovement();

	// Call Animation
	OnPlayerDeath.Broadcast();
	
	if (BossManager)
	{
		BossManager->ResetBossCompletely();
	}
	else
	{
		CLog::Log("RespawnComponent: BossManager is null, cannot reset boss");
	}

	// 플레이어가 방금 사망했음을 모든 에너미 AI에 알림(감지 차단)
	if (CombatEncounterManager)
	{
		CombatEncounterManager->SetAllAIsNowPlayerDead();
	}
	
	/*// 기존 타이머가 있다면 클리어
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimerHandle);
		
		// 일정 시간 후 부활하도록 타이머 설정
		GetWorld()->GetTimerManager().SetTimer(
			RespawnTimerHandle,
			this,
			&UCRespawnComponent::RespawnPlayer,
			RespawnDelay,
			false  // 한 번만 실행
		);
		
		CLog::Log("RespawnComponent: Respawn timer set for " + FString::SanitizeFloat(RespawnDelay) + " seconds");
	}*/
}

void UCRespawnComponent::RespawnPlayer()
{
	
	// 실제 부활 처리
	if (!OwnerCharacter || !State) 
	{
		CLog::Log("RespawnComponent: Cannot respawn - OwnerCharacter or State is null");
		return;
	}
	OwnerCharacter->GetCharacterMovement()->StopActiveMovement();
	
	CLog::Log("RespawnComponent: Starting respawn process...");
	
	// 1. 플레이어 위치를 RespawnLocation으로 이동
	OwnerCharacter->SetActorLocation(RespawnLocation);
	CLog::Log("RespawnComponent: Player moved to respawn location: " + RespawnLocation.ToString());
	
	// 2. 플레이어 상태를 Idle로 변경 및 사망 상태 리셋
	State->SetIdleMode();
	State->ResetDeadState();
	CLog::Log("RespawnComponent: Player state set to Idle and dead state reset");

	Movement->Move();
	
	// 3. HP를 최대치로 복구 (Status 컴포넌트가 있다면)
	if (Status)
	{
		Status->ResetStatus();
		CLog::Log("RespawnComponent: Player health restored to full");
	}
	if (Stamina)
	{
		Stamina->ResetStamina();
	}
	if (Magazine)
	{
		Magazine->ResetMagazines();
	}
	if (State)
	{
		State->SetIsHammerPulled(true);
	}
	// 4. 컨트롤러 회전 다시 활성화
	if (OwnerCharacter->Movement)
	{
		OwnerCharacter->Movement->EnableControlRotation();
		CLog::Log("RespawnComponent: Control rotation enabled");
	}
	
	//OwnerCharacter->SetActorEnableCollision(ECollisionEnabled::QueryAndPhysics);
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
	// 5. 부활 델리게이트 브로드캐스트
	//OnPlayerRespawned.Broadcast();
	CLog::Log("RespawnComponent: Player respawned successfully!");

	// CCombatEncounterManager를 통해 에너미들 초기화
	if (CombatEncounterManager)
	{
		CombatEncounterManager->AllEnemyRestart();
		CombatEncounterManager->ClearAllAIsNowPlayerDead();
		CLog::Log("RespawnComponent: All enemies restarted via CombatEncounterManager");
	}
	else
	{
		CLog::Log("RespawnComponent: CombatEncounterManager is null, cannot restart enemies");
	}

	// 모든 드랍 아이템들을 풀로 반환
	if (AItemPoolManager* ItemPool = Cast<AItemPoolManager>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AItemPoolManager::StaticClass())))
	{
		ItemPool->ResetAllActiveItems();
		CLog::Log("RespawnComponent: All active items returned to pool");
	}
	else
	{
		CLog::Log("RespawnComponent: ItemPoolManager not found, cannot reset items");
	}

	// 모든 ItemDropObject 초기화
	ResetAllItemDropObjects();
}

void UCRespawnComponent::SetRespawnLocation(FVector NewLocation)
{
	RespawnLocation = NewLocation;
	CLog::Log("RespawnComponent: Respawn location updated to: " + RespawnLocation.ToString());
}

void UCRespawnComponent::SetRespawnDelay(float NewDelay)
{
	RespawnDelay = FMath::Max(0.1f, NewDelay); // 최소 0.1초
	CLog::Log("RespawnComponent: Respawn delay updated to: " + FString::SanitizeFloat(RespawnDelay) + " seconds");
}

void UCRespawnComponent::ResetAllItemDropObjects()
{
	if (!GetWorld()) return;
	
	int32 ResetCount = 0;
	
	// 월드의 모든 Actor에서 UCItemDropObjectComponent 찾기
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
	
	for (AActor* Actor : AllActors)
	{
		if (!IsValid(Actor)) continue;
		
		// 해당 Actor에서 UCItemDropObjectComponent 찾기
		if (UCItemDropObjectComponent* ItemDropObjComp = Actor->FindComponentByClass<UCItemDropObjectComponent>())
		{
			// 파괴된 상태가 아니더라도 강제로 리스폰 (초기 상태로 복구)
			ItemDropObjComp->RespawnObject();
			ResetCount++;
		}
	}
	
	CLog::Log(FString::Printf(TEXT("RespawnComponent: Reset %d ItemDropObjects"), ResetCount));
}

