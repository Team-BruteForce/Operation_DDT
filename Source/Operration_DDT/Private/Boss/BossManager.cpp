// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/BossManager.h"
#include "Global.h"
#include "Boss/CBoss.h"
#include "Boss/Component/CBossStatusComponent.h"
#include "Boss/Component/BossStateComponent.h"
#include "Boss/Component/BossProjectileComponent.h"
#include "Boss/Component/CBossWeaponComponent.h"
#include "Boss/Component/FlyingComponent.h"
#include "Boss/Component/CBossMovementComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Components/StateTreeAIComponent.h"
#include "Engine/World.h"

ABossManager::ABossManager()
{
	PrimaryActorTick.bCanEverTick = true;

	// 박스 콜리전 생성
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	
	// 콜리전 설정
	TriggerBox->SetBoxExtent(FVector(200.0f, 200.0f, 100.0f));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
	// 오버랩 이벤트 바인딩
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABossManager::OnTriggerBoxOverlapBegin);
}

void ABossManager::BeginPlay()
{
	Super::BeginPlay();
	
	// 게임 시작 시 보스 찾기 (스폰 대신)
	FindBossInWorld();
}

void ABossManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	static int32 TickCount = 0;
	TickCount++;
	
	// 5초마다 틱 로그 출력
	if (TickCount % 300 == 0) // 60fps 기준 5초
	{
		UE_LOG(LogTemp, Warning, TEXT("🔄 BossManager Tick 작동 중 - SpawnedBoss: %s"), 
			SpawnedBoss ? *SpawnedBoss->GetName() : TEXT("nullptr"));
	}
	
	// 보스가 없으면 계속 찾기
	if (!SpawnedBoss)
	{
		UE_LOG(LogTemp, Warning, TEXT("🔍 보스 없음, 찾는 중..."));
		FindBossInWorld();
	}
}

/**
 * @brief 보스 완전 초기화 (메인 함수)
 * 
 * 이 함수 하나만 호출하면 모든 보스 시스템이 초기화됩니다!
 */
void ABossManager::ResetBossCompletely()
{
	if (!SpawnedBoss)
	{
		UE_LOG(LogTemp, Error, TEXT("BossManager: 스폰된 보스가 없습니다!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("=== 보스 완전 초기화 시작 ==="));
	
	// 보스 상태 확인
	UE_LOG(LogTemp, Warning, TEXT("🔍 보스 상태 확인 - SpawnedBoss: %s"), 
		SpawnedBoss ? *SpawnedBoss->GetName() : TEXT("nullptr"));
	
	if (SpawnedBoss)
	{
		UE_LOG(LogTemp, Warning, TEXT("🔍 보스 컨트롤러 확인 - Controller: %s"), 
			SpawnedBoss->GetController() ? *SpawnedBoss->GetController()->GetName() : TEXT("nullptr"));
	}

	UE_LOG(LogTemp, Warning, TEXT("⏰ 3초 후 보스 완전 초기화 시작 예약"));

	// 3초 후 모든 초기화 실행
	FTimerHandle ResetTimer;
	GetWorld()->GetTimerManager().SetTimer(ResetTimer, [this]()
	{
		if (!SpawnedBoss || !IsValid(SpawnedBoss))
		{
			UE_LOG(LogTemp, Error, TEXT("❌ 3초 후 초기화 실행 시 보스가 유효하지 않음"));
			return;
		}

		UE_LOG(LogTemp, Warning, TEXT("🚀 3초 후 보스 완전 초기화 실행 시작"));

		// 1. 모든 컴포넌트 초기화
		ResetAllBossComponents();
		UE_LOG(LogTemp, Warning, TEXT("✅ 컴포넌트 초기화 완료"));

		// 2. StateTree 완전 리스타트
		ResetBossStateTree();
		UE_LOG(LogTemp, Warning, TEXT("✅ StateTree 초기화 완료"));

		// 3. 보스 위치 이동
		FVector OldLocation = SpawnedBoss->GetActorLocation();
		SpawnedBoss->SetActorLocation(BossInitialLocation);
		UE_LOG(LogTemp, Warning, TEXT("✅ 보스 위치 이동 완료: %s → %s"), 
			*OldLocation.ToString(), *BossInitialLocation.ToString());

		// 4. 트리거 콜리전 다시 활성화 (재사용 가능)
		TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		UE_LOG(LogTemp, Warning, TEXT("🔓 트리거 콜리전 활성화 (재사용 가능)"));

		UE_LOG(LogTemp, Warning, TEXT("=== 보스 완전 초기화 완료 ==="));

	}, 3.0f, false);
}

/**
 * @brief StateTree 완전 리스타트
 */
void ABossManager::ResetBossStateTree()
{
	UE_LOG(LogTemp, Warning, TEXT("🔄 ResetBossStateTree 함수 진입"));
	
	if (!SpawnedBoss)
	{
		UE_LOG(LogTemp, Error, TEXT("❌ BossManager: 보스가 없어서 StateTree 리스타트 불가"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("🔄 StateTree 리스타트 시작 - 보스: %s"), *SpawnedBoss->GetName());

	// 보스의 AI 컨트롤러 찾기
	AAIController* BossAIC = Cast<AAIController>(SpawnedBoss->GetController());
	if (!BossAIC)
	{
		UE_LOG(LogTemp, Error, TEXT("BossManager: 보스 AI 컨트롤러를 찾을 수 없습니다! Controller: %s"), 
			SpawnedBoss->GetController() ? *SpawnedBoss->GetController()->GetName() : TEXT("nullptr"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("BossManager: AI 컨트롤러 찾음: %s"), *BossAIC->GetName());

	// StateTree 컴포넌트 찾기
	UStateTreeAIComponent* StateTreeComp = BossAIC->GetComponentByClass<UStateTreeAIComponent>();
	if (!StateTreeComp)
	{
		UE_LOG(LogTemp, Error, TEXT("BossManager: StateTree 컴포넌트를 찾을 수 없습니다!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("BossManager: StateTree 컴포넌트 찾음: %s"), *StateTreeComp->GetName());

	GetWorld()->GetTimerManager().SetTimer(RestartTimer, [StateTreeComp]()
	{
		if (StateTreeComp && IsValid(StateTreeComp))
		{
			UE_LOG(LogTemp, Warning, TEXT("🔄 StateTree RestartLogic 실행 중..."));
			StateTreeComp->RestartLogic();
			UE_LOG(LogTemp, Warning, TEXT("✅ StateTree 재시작 완료"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("❌ StateTree 컴포넌트가 유효하지 않음"));
		}
	}, 5.0f, false);

	UE_LOG(LogTemp, Warning, TEXT("🔄 StateTree 재시작 3초 후 예약 완료"));
}

/**
 * @brief 콜리전 오버랩 시작 이벤트
 */
void ABossManager::OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어인지 확인
	ACharacter* Player = Cast<ACharacter>(OtherActor);
	if (!Player || !SpawnedBoss)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("BossManager: 플레이어가 트리거에 진입! 보스 이벤트 전송"));

	// 보스의 AI 컨트롤러 찾기
	AAIController* BossAIC = Cast<AAIController>(SpawnedBoss->GetController());
	if (!BossAIC)
	{
		UE_LOG(LogTemp, Error, TEXT("BossManager: 보스 AI 컨트롤러를 찾을 수 없습니다!"));
		return;
	}

	// StateTree 컴포넌트 찾기
	UStateTreeAIComponent* StateTreeComp = BossAIC->GetComponentByClass<UStateTreeAIComponent>();
	if (!StateTreeComp)
	{
		UE_LOG(LogTemp, Error, TEXT("BossManager: StateTree 컴포넌트를 찾을 수 없습니다!"));
		return;
	}

	// StateTree 이벤트 전송
	if (BossStartEventTag.IsValid())
	{
		StateTreeComp->SendStateTreeEvent(BossStartEventTag);
		UE_LOG(LogTemp, Warning, TEXT("BossManager: StateTree 이벤트 전송 완료: %s"), 
			*BossStartEventTag.ToString());
		
		// 보스 시작 후 콜리전 비활성화 (중복 트리거 방지)
		TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogTemp, Warning, TEXT("🔒 트리거 콜리전 비활성화 (보스 시작됨)"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BossManager: BossStartEventTag가 설정되지 않았습니다!"));
	}
}

/**
 * @brief 월드에서 보스 찾기
 */
void ABossManager::FindBossInWorld()
{
	// 월드에서 보스 찾기
	SpawnedBoss = CHelpers::FindActor<ACBoss>(GetWorld());
	
	if (SpawnedBoss)
		BossInitialLocation = SpawnedBoss->InitialLocation;
	else
		CLog::Log("보스 없음!");
}
/**
 * @brief 각 컴포넌트별 초기화 실행
 * 
 * 모든 보스 컴포넌트의 Reset 함수들을 순서대로 호출합니다.
 */
void ABossManager::ResetAllBossComponents()
{
	UE_LOG(LogTemp, Error, TEXT("⚠️⚠️⚠️ ResetAllBossComponents 직접 호출됨! ResetBossCompletely를 호출해야 합니다! ⚠️⚠️⚠️"));
	
	// 1. 스탯 컴포넌트 초기화
	if (UCBossStatusComponent* StatusComp = CHelpers::GetComponent<UCBossStatusComponent>(SpawnedBoss))
	{
		StatusComp->ResetAllStats();
		UE_LOG(LogTemp, Warning, TEXT("✅ 보스 스탯 초기화 완료"));
	}

	// 2. 상태 컴포넌트 초기화  
	if (UBossStateComponent* StateComp = CHelpers::GetComponent<UBossStateComponent>(SpawnedBoss))
	{
		StateComp->ResetBossState();
		UE_LOG(LogTemp, Warning, TEXT("✅ 보스 상태 초기화 완료"));
	}

	// 3. 투사체 시스템 초기화
	if (UBossProjectileComponent* ProjectileComp = CHelpers::GetComponent<UBossProjectileComponent>(SpawnedBoss))
	{
		ProjectileComp->ResetProjectileSystem();
		UE_LOG(LogTemp, Warning, TEXT("✅ 보스 투사체 시스템 초기화 완료"));
	}

	// 4. 무기 시스템 초기화
	if (UCBossWeaponComponent* WeaponComp = CHelpers::GetComponent<UCBossWeaponComponent>(SpawnedBoss))
	{
		WeaponComp->ResetWeaponSystem();
		UE_LOG(LogTemp, Warning, TEXT("✅ 보스 무기 시스템 초기화 완료"));
	}

	// 5. 비행 시스템 초기화
	if (UFlyingComponent* FlyingComp = CHelpers::GetComponent<UFlyingComponent>(SpawnedBoss))
	{
		FlyingComp->ResetFlyingSystem();
		UE_LOG(LogTemp, Warning, TEXT("✅ 보스 비행 시스템 초기화 완료"));
	}

	// 6. 이동 시스템 초기화
	if (UCBossMovementComponent* MovementComp = CHelpers::GetComponent<UCBossMovementComponent>(SpawnedBoss))
	{
		MovementComp->ResetMovementSystem();
		UE_LOG(LogTemp, Warning, TEXT("✅ 보스 이동 시스템 초기화 완료"));
	}
}


