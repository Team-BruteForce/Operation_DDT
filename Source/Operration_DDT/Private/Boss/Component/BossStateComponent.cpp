// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/Component/BossStateComponent.h"

#include "AIController.h"
#include "Global.h"
#include "Components/StateTreeComponent.h"

// Sets default values for this component's properties
UBossStateComponent::UBossStateComponent()
{

}
void UBossStateComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner=Cast<APawn>(GetOwner());
	CheckNull(Owner);
	AIC=Cast<AAIController>(Owner->GetController());
	CheckNull(AIC);
	CachedStateTree = AIC->GetComponentByClass<UStateTreeComponent>();
	CheckNull(CachedStateTree);
	CLog::Log("CachedStateTree Get");
	if (!CurrentState.IsValid() && Tag_State_Idle.IsValid())
		SetStateTag(Tag_State_Idle,false);
	
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