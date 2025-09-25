// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_AIController/CEnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AIPerceptionSystem.h"
#include "GenericTeamAgentInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"
#include "ODH/ODH_Enemy/Component/CEnemyHealthBarComponent.h"

ACEnemyAIController::ACEnemyAIController()
{
	// AIPerception 컴포넌트 생성
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	
	// 시각 감지 설정 생성
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	
	// 시각 감지 설정
	SightConfig->SightRadius = SightRadius;
	SightConfig->LoseSightRadius = LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = PeripheralVisionAngleDegrees;
	SightConfig->SetMaxAge(SightAge);
	
	// 모든 액터 타입 감지 (플레이어, 적, 중립 등)
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	
	// AIPerception 컴포넌트에 시각 감지 설정 추가
	AIPerceptionComponent->SetDominantSense(*SightConfig->GetSenseImplementation());
	AIPerceptionComponent->ConfigureSense(*SightConfig);
	
	// 팀 ID 초기화 (기본값: 1 - 적)
	TeamID = FGenericTeamId(1);
	
	// 이벤트 바인딩
	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ACEnemyAIController::OnTargetPerceptionUpdated);
}

void ACEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	// 팀 관계는 기본적으로 설정되어 있으므로 별도 설정 불필요
	// AAIController의 기본 팀 시스템을 사용
}

FGenericTeamId ACEnemyAIController::GetGenericTeamId() const
{
	return TeamID;
}

void ACEnemyAIController::SetGenericTeamId(const FGenericTeamId& NewTeamID)
{
	TeamID = NewTeamID;
}

void ACEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor || !IsValid(Actor))
	{
		return;
	}

    // 플레이어가 방금 사망한 상태라면 모든 감지 무시
    if (bIsNowPlayerDead)
    {
        return;
    }
	
	// IsPlayerInBoss가 true이면 탐색하지 않음
	UBlackboardComponent* BlackboardComp = GetBlackboardComponent();
	if (BlackboardComp)
	{
		static const FName KeyIsPlayerInBoss = TEXT("IsPlayerInBoss");
		const bool bIsPlayerInBoss = BlackboardComp->GetValueAsBool(KeyIsPlayerInBoss);
		if (bIsPlayerInBoss)
		{
			return; // 보스 구역에 플레이어가 있으면 탐색 중단
		}
	}
	
	// 시각 감지인지 확인
	if (Stimulus.Type == UAISense::GetSenseID<UAISense_Sight>())
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			// 감지 성공 - 적 또는 아군으로 분류
			if (IsEnemy(Actor))
			{
				AddToEnemyList(Actor);
				OnEnemyDetected(Actor);
				
				// 블랙보드에 타겟 플레이어 설정
				if (BlackboardComp)
				{
					BlackboardComp->SetValueAsObject("TargetPlayer", Actor);
					BlackboardComp->SetValueAsBool("IsInCombat", true);
					BlackboardComp->SetValueAsVector("LastSeenLocation", Actor->GetActorLocation());
					
					// 기존 타이머가 있다면 취소 (새로운 타겟을 찾았으므로)
					if (TargetLostTimerHandle.IsValid())
					{
						GetWorldTimerManager().ClearTimer(TargetLostTimerHandle);
					}
				}
			}
			else if (IsAlly(Actor))
			{
				AddToAllyList(Actor);
				OnAllyDetected(Actor);
			}
		}
		else
		{
			// 감지 실패 - 목록에서 제거
			RemoveFromEnemyList(Actor);
			RemoveFromAllyList(Actor);
			// 시야 상실 시에도 전투 이탈을 처리하지 않음 (TargetPlayer/IsInCombat 유지)
		}
	}
}

void ACEnemyAIController::SetNowPlayerDead()
{
    bIsNowPlayerDead = true;
}

void ACEnemyAIController::ClearNowPlayerDead()
{
    bIsNowPlayerDead = false;
    
    // 비헤이비어트리 재시작
    if (UBrainComponent* Brain = GetBrainComponent())
    {
        Brain->RestartLogic();
    }
    
    // 블랙보드 초기화
    if (UBlackboardComponent* BlackboardComp = GetBlackboardComponent())
    {
        static const FName KeyIsInCombat = TEXT("IsInCombat");
        static const FName KeyTargetPlayer = TEXT("TargetPlayer");
        static const FName KeyLastSeenLocation = TEXT("LastSeenLocation");
        
        BlackboardComp->SetValueAsBool(KeyIsInCombat, false);
        BlackboardComp->ClearValue(KeyTargetPlayer);
        BlackboardComp->ClearValue(KeyLastSeenLocation);
    }
    
    // 감지 목록 초기화
    DetectedEnemies.Empty();
    DetectedAllies.Empty();
    
    // AIPerception 재설정 - 감지된 모든 액터를 잊어버리기
    if (AIPerceptionComponent)
    {
        AIPerceptionComponent->ForgetAll();
    }
    
    // 이동 기능 재활성화
    if (APawn* MyPawn = GetPawn())
    {
        if (UCharacterMovementComponent* MoveComp = MyPawn->GetComponentByClass<UCharacterMovementComponent>())
        {
            // 이동 컴포넌트 재활성화
            MoveComp->SetMovementMode(MOVE_Walking);
            
            // AI 컨트롤러 이동 재시작
            StopMovement();
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("AI Controller restarted after player respawn: %s"), *GetName());
}

AActor* ACEnemyAIController::GetNearestEnemy() const
{
	if (DetectedEnemies.Num() == 0)
	{
		return nullptr;
	}
	
	AActor* NearestEnemy = nullptr;
	float NearestDistance = FLT_MAX;
	FVector MyLocation = GetPawn()->GetActorLocation();
	
	for (AActor* Enemy : DetectedEnemies)
	{
		if (IsValid(Enemy))
		{
			float Distance = FVector::Dist(MyLocation, Enemy->GetActorLocation());
			if (Distance < NearestDistance)
			{
				NearestDistance = Distance;
				NearestEnemy = Enemy;
			}
		}
	}
	
	return NearestEnemy;
}

AActor* ACEnemyAIController::GetNearestAlly() const
{
	if (DetectedAllies.Num() == 0)
	{
		return nullptr;
	}
	
	AActor* NearestAlly = nullptr;
	float NearestDistance = FLT_MAX;
	FVector MyLocation = GetPawn()->GetActorLocation();
	
	for (AActor* Ally : DetectedAllies)
	{
		if (IsValid(Ally))
		{
			float Distance = FVector::Dist(MyLocation, Ally->GetActorLocation());
			if (Distance < NearestDistance)
			{
				NearestDistance = Distance;
				NearestAlly = Ally;
			}
		}
	}
	
	return NearestAlly;
}

bool ACEnemyAIController::IsEnemy(AActor* Actor) const
{
	if (!Actor)
	{
		return false;
	}
	
	// GenericTeamAgentInterface를 구현한 액터인지 확인
	if (IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(Actor))
	{
		// 팀 ID를 가져올 수 있으면 팀 ID 비교
		FGenericTeamId ActorTeamID = TeamAgent->GetGenericTeamId();
		FGenericTeamId MyTeamID = GetGenericTeamId();
		
		// 다른 팀이면 적, 같은 팀이면 아군
		return (ActorTeamID != MyTeamID);
	}
	
	// GenericTeamAgentInterface를 구현하지 않은 액터는 적으로 간주
	// (플레이어 등)
	return true;
}

bool ACEnemyAIController::IsAlly(AActor* Actor) const
{
	if (!Actor)
	{
		return false;
	}
	
	// GenericTeamAgentInterface를 구현한 액터인지 확인
	if (IGenericTeamAgentInterface* TeamAgent = Cast<IGenericTeamAgentInterface>(Actor))
	{
		// 팀 ID를 가져올 수 있으면 팀 ID 비교
		FGenericTeamId ActorTeamID = TeamAgent->GetGenericTeamId();
		FGenericTeamId MyTeamID = GetGenericTeamId();
		
		// 같은 팀이면 아군
		return (ActorTeamID == MyTeamID);
	}
	
	// GenericTeamAgentInterface를 구현하지 않은 액터는 아군이 아님
	return false;
}

void ACEnemyAIController::AddToEnemyList(AActor* Actor)
{
	if (Actor && !DetectedEnemies.Contains(Actor))
	{
		DetectedEnemies.Add(Actor);
	}
}

void ACEnemyAIController::RemoveFromEnemyList(AActor* Actor)
{
	if (Actor)
	{
		DetectedEnemies.Remove(Actor);
	}
}

void ACEnemyAIController::AddToAllyList(AActor* Actor)
{
	if (Actor && !DetectedAllies.Contains(Actor))
	{
		DetectedAllies.Add(Actor);
	}
}

void ACEnemyAIController::RemoveFromAllyList(AActor* Actor)
{
	if (Actor)
	{
		DetectedAllies.Remove(Actor);
	}
}

void ACEnemyAIController::OnTargetLost()
{
	// 전투 지속 정책: 타겟을 포기하지 않음. 아무 것도 하지 않음.
	// 타이머 핸들만 정리
	if (TargetLostTimerHandle.IsValid())
	{
		TargetLostTimerHandle.Invalidate();
	}
}
