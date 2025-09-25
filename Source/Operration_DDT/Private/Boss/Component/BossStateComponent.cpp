/**
 * @file BossStateComponent.cpp
 * @brief 보스 상태 관리 컴포넌트 구현 파일
 * 
 * @details
 * 보스 캐릭터의 상태를 관리하는 컴포넌트의 구현 파일입니다.
 * StateTree와 연동하여 보스의 현재 상태를 추적하고 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Component/BossStateComponent.h"

#include "AIController.h"
#include "Global.h"
#include "Components/StateTreeComponent.h"

/**
 * @brief 보스 상태 컴포넌트 생성자
 * 
 * 컴포넌트의 기본 설정을 초기화합니다.
 */
UBossStateComponent::UBossStateComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}
/**
 * @brief 컴포넌트 시작 시 호출되는 함수
 * 
 * 소유자 Pawn을 캐스팅하고 초기화 작업을 수행합니다.
 */
void UBossStateComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<APawn>(GetOwner());
	CheckNull(Owner);
	
	// AI 컨트롤러는 지연 초기화 (Tick에서 안전하게 처리)
	// 초기 상태는 나중에 AI 컴포넌트가 준비되면 설정
}

/**
 * @brief 컴포넌트 틱 함수
 * 
 * @param DeltaTime 델타 타임
 * @param TickType 틱 타입
 * @param ThisTickFunction 틱 함수
 * 
 * AI 컴포넌트 초기화 및 상태 업데이트를 처리합니다.
 */
void UBossStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// AI 컴포넌트가 초기화되지 않았다면 시도
	if (!CachedStateTree && Owner)
	{
		AIC = Cast<AAIController>(Owner->GetController());
		if (AIC)
		{
			CachedStateTree = AIC->GetComponentByClass<UStateTreeComponent>();
			if (CachedStateTree)
			{
				// 초기 상태 설정
				if (!CurrentState.IsValid() && Tag_State_Idle.IsValid())
				{
					SetStateTag(Tag_State_Idle, false);
				}
			}
		}
	}
}

void UBossStateComponent::SetStateTag(FGameplayTag NewState, bool bSendEvent)
{
	if (!NewState.IsValid() || NewState == CurrentState) return;

	const FGameplayTag Prev = CurrentState;

	// 컨테이너 갱신(이전 상태 제거,새 상태 추가)
	if (Prev.IsValid()) { OwnedTags.RemoveTag(Prev); }
	CurrentState = NewState;
	OwnedTags.AddTag(NewState);

	OnStateTagChanged.Broadcast(Prev,NewState);

	if (bSendEvent) SendEventToStateTree(NewState);
}

void UBossStateComponent::AddTag(FGameplayTag Tag, bool bSendEvent)
{
	if (!Tag.IsValid()) return;
	if (!OwnedTags.HasTag(Tag))
	{
		OwnedTags.AddTag(Tag);
		if (bSendEvent) SendEventToStateTree(Tag);
	}
}

void UBossStateComponent::RemoveTag(FGameplayTag Tag, bool bSendEvent)
{
	if (!Tag.IsValid()) return;
	if (OwnedTags.HasTag(Tag))
	{
		OwnedTags.RemoveTag(Tag);
		if (bSendEvent) SendEventToStateTree(Tag); // 필요시 End용 별도 태그를 보내도록 분기
	}
}
void UBossStateComponent::SendEventToStateTree(const FGameplayTag& Tag)
{
	if (!CachedStateTree) return;
	// ① 단순 태그 이벤트
	CachedStateTree->SendStateTreeEvent(Tag);

	// ② 페이로드를 쓰고 싶다면:
	// FMyPayload Payload{...};
	// CachedStateTree->SendStateTreeEvent(Tag, FConstStructView::Make(Payload));
}

/**
 * @brief 보스 상태 완전 초기화 (매니저용)
 * 
 * 보스의 모든 상태와 태그를 초기 상태로 리셋합니다.
 * 보스 매니저에서 보스 리셋 시 사용됩니다.
 */
void UBossStateComponent::ResetBossState()
{
	// 모든 소유 태그 클리어
	OwnedTags.Reset();
	
	// 현재 상태 초기화
	CurrentState = FGameplayTag();
	
	// 기본 Idle 상태로 설정
	if (Tag_State_Idle.IsValid())
	{
		SetStateTag(Tag_State_Idle, true);
	}
	
	// StateTree 초기화 이벤트 발송 (있다면)
	if (CachedStateTree)
	{
		FGameplayTag ResetTag = FGameplayTag::RequestGameplayTag("BOSS.Event.Reset");
		if (ResetTag.IsValid())
		{
			SendEventToStateTree(ResetTag);
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("보스 상태 완전 초기화 완료"));
}