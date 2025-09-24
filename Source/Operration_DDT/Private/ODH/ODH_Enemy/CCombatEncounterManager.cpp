#include "ODH/ODH_Enemy/CCombatEncounterManager.h"

#include "Components/BoxComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DDTPlayer.h"
#include "ODH/ODH_Enemy/Interface/AllResettable.h"
#include "ODH/ODH_AIController/CEnemyAIController.h"

ACCombatEncounterManager::ACCombatEncounterManager()
{
	PrimaryActorTick.bCanEverTick = false;

    // 루트 및 트리거
    PlayerTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("PlayerTrigger"));
    SetRootComponent(PlayerTrigger);
	PlayerTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PlayerTrigger->SetCollisionObjectType(ECC_WorldDynamic);
	PlayerTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	PlayerTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	PlayerTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACCombatEncounterManager::OnPlayerTriggerBeginOverlap);
}

void ACCombatEncounterManager::BeginPlay()
{
	Super::BeginPlay();
}

void ACCombatEncounterManager::CollectActiveEnemies(TArray<TWeakObjectPtr<APawn>>& OutEnemies) const
{
    OutEnemies.Reset();

    for (const TWeakObjectPtr<APawn>& W : RegisteredEnemies)
    {
        APawn* Pawn = W.Get();
        if (!IsValid(Pawn) || Pawn->IsActorBeingDestroyed())
            continue;

        AAIController* AI = Cast<AAIController>(Pawn->GetController());
        if (!AI)
            continue;

        if (UBlackboardComponent* BB = AI->GetBlackboardComponent())
        {
            static const FName KeyIsInCombat = TEXT("IsInCombat");
            const bool bInCombat = BB->GetValueAsBool(KeyIsInCombat);
            if (bInCombat)
            {
                OutEnemies.Add(Pawn);
            }
        }
    }
}

void ACCombatEncounterManager::CollectActiveEnemiesInZone_BP(TArray<APawn*>& OutEnemies) const
{
    TArray<TWeakObjectPtr<APawn>> Temp;
    CollectActiveEnemies(Temp);
	OutEnemies.Reset(Temp.Num());
	for (const TWeakObjectPtr<APawn>& W : Temp)
	{
		if (APawn* P = W.Get())
		{
			OutEnemies.Add(P);
		}
	}
}

void ACCombatEncounterManager::ResetCombatInZone(AActor* InstigatorActor)
{
	TArray<TWeakObjectPtr<APawn>> Targets;
    CollectActiveEnemies(Targets);

	if (Targets.Num() == 0)
		return;

	const int32 LocalBatchSize = FMath::Max(1, BatchSize);
	const float Interval = FMath::Max(0.0f, BatchIntervalSeconds);

	for (int32 Index = 0; Index < Targets.Num(); Index += LocalBatchSize)
	{
		TArray<TWeakObjectPtr<APawn>> Batch;
		const int32 End = FMath::Min(Index + LocalBatchSize, Targets.Num());
		for (int32 i = Index; i < End; ++i)
		{
			Batch.Add(Targets[i]);
		}

		if (Interval <= KINDA_SMALL_NUMBER)
		{
			ProcessResetBatch(Batch, InstigatorActor);
		}
		else
		{
			FTimerHandle Handle;
			GetWorld()->GetTimerManager().SetTimer(
				Handle,
				[this, Batch, InstigatorActor]()
				{
					ProcessResetBatch(Batch, InstigatorActor);
				},
				(Index / LocalBatchSize) * Interval,
				false);
		}
	}
}

void ACCombatEncounterManager::AllEnemyRestart()
{
    // 3초 지연 후 실행되도록 타이머 설정
    GetWorld()->GetTimerManager().ClearTimer(AllEnemyRestartTimerHandle);
    GetWorld()->GetTimerManager().SetTimer(
        AllEnemyRestartTimerHandle,
        this,
        &ACCombatEncounterManager::DoAllEnemyRestart,
        3.0f,
        false
    );
}

void ACCombatEncounterManager::DoAllEnemyRestart()
{
    int32 RestartCount = 0;

    for (const TWeakObjectPtr<APawn>& WeakPawn : RegisteredEnemies)
    {
        APawn* Pawn = WeakPawn.Get();
        if (!Pawn || Pawn->IsActorBeingDestroyed())
            continue;

        if (Pawn->Implements<UAllEnemyRestart>())
        {
            IAllEnemyRestart::Execute_EnemyRestart(Pawn);
            RestartCount++;
        }
    }

    UE_LOG(LogTemp, Log, TEXT("CombatEncounterManager: Restarted %d enemies with AllEnemyRestart interface (delayed)"), RestartCount);
}

void ACCombatEncounterManager::SetAllAIsNowPlayerDead()
{
    for (const TWeakObjectPtr<APawn>& WeakPawn : RegisteredEnemies)
    {
        APawn* Pawn = WeakPawn.Get();
        if (!Pawn)
            continue;

        if (AAIController* AI = Cast<AAIController>(Pawn->GetController()))
        {
            if (ACEnemyAIController* EnemyAI = Cast<ACEnemyAIController>(AI))
            {
                EnemyAI->SetNowPlayerDead();
            }
        }
    }
}

void ACCombatEncounterManager::ClearAllAIsNowPlayerDead()
{
    for (const TWeakObjectPtr<APawn>& WeakPawn : RegisteredEnemies)
    {
        APawn* Pawn = WeakPawn.Get();
        if (!Pawn)
            continue;

        if (AAIController* AI = Cast<AAIController>(Pawn->GetController()))
        {
            if (ACEnemyAIController* EnemyAI = Cast<ACEnemyAIController>(AI))
            {
                EnemyAI->ClearNowPlayerDead();
            }
        }
    }
}

void ACCombatEncounterManager::RegisterEnemy(APawn* Enemy)
{
    if (!IsValid(Enemy)) return;
    RegisteredEnemies.Add(Enemy);
}

void ACCombatEncounterManager::UnregisterEnemy(APawn* Enemy)
{
    if (!Enemy) return;
    RegisteredEnemies.Remove(Enemy);
}

void ACCombatEncounterManager::ProcessResetBatch(const TArray<TWeakObjectPtr<APawn>>& Batch, AActor* InstigatorActor) const
{
	for (const TWeakObjectPtr<APawn>& WeakPawn : Batch)
	{
		APawn* Pawn = WeakPawn.Get();
		if (!Pawn)
			continue;

		ResetOneEnemy(Pawn, InstigatorActor);
	}
}

void ACCombatEncounterManager::ResetOneEnemy(APawn* Pawn, AActor* InstigatorActor) const
{
	AAIController* AI = Cast<AAIController>(Pawn->GetController());
	if (!AI)
		return;

	if (UAIPerceptionComponent* Perc = AI->FindComponentByClass<UAIPerceptionComponent>())
	{
		if (InstigatorActor)
		{
			Perc->ForgetActor(InstigatorActor);
		}
	}
	AI->ClearFocus(EAIFocusPriority::Gameplay);
	AI->StopMovement();

	if (UBlackboardComponent* BB = AI->GetBlackboardComponent())
	{
		static const FName KeyIsInCombat = TEXT("IsInCombat");
		static const FName KeyTargetPlayer = TEXT("TargetPlayer");
		static const FName KeyIsPlayerInBoss = TEXT("IsPlayerInBoss");
		BB->SetValueAsBool(KeyIsInCombat, false);
		BB->SetValueAsBool(KeyIsPlayerInBoss, true);
		BB->ClearValue(KeyTargetPlayer);
	}

	FVector Home;
	if (TryGetHomeLocationFromPatrolArray(Pawn, Home))
	{
		AI->MoveToLocation(Home, 5.0f, false);
	}

	// BP 인터페이스 AllReset 호출(있으면)
	if (Pawn->GetClass()->ImplementsInterface(UAllResettable::StaticClass()))
	{
		IAllResettable::Execute_AllReset(Pawn);
	}
}

bool ACCombatEncounterManager::TryGetHomeLocationFromPatrolArray(const AActor* Enemy, FVector& OutLocation) const
{
	if (!Enemy)
		return false;

	static const FName PatrolArrayName(TEXT("PatrolArray"));
	for (TFieldIterator<FProperty> It(Enemy->GetClass()); It; ++It)
	{
		FProperty* Prop = *It;
		if (!Prop || Prop->GetFName() != PatrolArrayName)
			continue;

		if (FArrayProperty* ArrayProp = CastField<FArrayProperty>(Prop))
		{
			if (FStructProperty* InnerStruct = CastField<FStructProperty>(ArrayProp->Inner))
			{
				if (InnerStruct->Struct == TBaseStructure<FVector>::Get())
				{
					const void* ValuePtr = ArrayProp->ContainerPtrToValuePtr<void>(Enemy);
					FScriptArrayHelper Helper(ArrayProp, ValuePtr);
					if (Helper.Num() > 0)
					{
						const FVector* VecPtr = reinterpret_cast<const FVector*>(Helper.GetRawPtr(0));
						if (VecPtr)
						{
							OutLocation = *VecPtr;
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void ACCombatEncounterManager::OnPlayerTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority())
		return;

	ADDTPlayer* Player = Cast<ADDTPlayer>(OtherActor);
	if (!Player)
		return;

	ResetCombatInZone(OtherActor);
}


