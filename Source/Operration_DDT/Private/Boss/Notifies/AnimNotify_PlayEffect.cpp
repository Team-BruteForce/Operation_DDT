// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/Notifies/AnimNotify_PlayEffect.h"
#include "Global.h"
#include "Boss/Component/BossEffectComponent.h"

UAnimNotify_PlayEffect::UAnimNotify_PlayEffect()
{
	EffectFunctionType = EEffectFunctionType::PlayEffect;
	EffectTag = FGameplayTag::EmptyTag;
	SocketName = NAME_None;
	CustomLocation = FVector::ZeroVector;
	CustomRotation = FRotator::ZeroRotator;
	bUseCustomLocation = false;
	CustomScale = FVector::OneVector;
	bUseCustomScale = false;
	bUseCustomRotation = false;
	Delay = 0.0f;
	LoopInterval = 1.0f;
	LoopCount = 0;
}

FString UAnimNotify_PlayEffect::GetNotifyName_Implementation() const
{
	FString functionName;
	switch (EffectFunctionType)
	{
	case EEffectFunctionType::PlayEffect:
		functionName = TEXT("PlayEffect");
		break;
	case EEffectFunctionType::PlayEffects:
		functionName = TEXT("PlayEffects");
		break;
	case EEffectFunctionType::PlayEffectWithDelay:
		functionName = TEXT("PlayEffectWithDelay");
		break;
	case EEffectFunctionType::PlayEffectLoop:
		functionName = TEXT("PlayEffectLoop");
		break;
	case EEffectFunctionType::PlayEffectAtSocket:
		functionName = TEXT("PlayEffectAtSocket");
		break;
	case EEffectFunctionType::PlayEffectAttachedToSocket:
		functionName = TEXT("PlayEffectAttachedToSocket");
		break;
	case EEffectFunctionType::PlayEffectAtSocketWithDelay:
		functionName = TEXT("PlayEffectAtSocketWithDelay");
		break;
	case EEffectFunctionType::PlayEffectAtSocketLoop:
		functionName = TEXT("PlayEffectAtSocketLoop");
		break;
	case EEffectFunctionType::PlayEffectAttachedToSocketLoop:
		functionName = TEXT("PlayEffectAttachedToSocketLoop");
		break;
	}
	
	return FString::Printf(TEXT("PlayEffect: %s"), *functionName);
}

void UAnimNotify_PlayEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	APawn* Owner = Cast<APawn>(MeshComp->GetOwner());
	CheckNull(Owner);
	
	UBossEffectComponent* EffectComp = CHelpers::GetComponent<UBossEffectComponent>(Owner);
	CheckNull(EffectComp);
	
	// 위치, 회전, 스케일 설정
	FVector location = FVector::ZeroVector;  // 기본값으로 설정하여 BossEffectComponent에서 처리
	FRotator rotation = FRotator::ZeroRotator;  // 기본값으로 설정하여 BossEffectComponent에서 처리
	FVector scale = FVector::OneVector;
	
	// 커스텀 설정이 활성화된 경우에만 적용
	if (bUseCustomLocation)
	{
		location = CustomLocation;  // BossEffectComponent에서 보스 위치 기준으로 처리됨
	}
	
	if (bUseCustomRotation)
	{
		rotation = CustomRotation;  // BossEffectComponent에서 보스 회전 기준으로 처리됨
	}
	
	if (bUseCustomScale)
	{
		scale = CustomScale;
	}
	
	// 선택된 함수 타입에 따라 이펙트 실행
	switch (EffectFunctionType)
	{
	case EEffectFunctionType::PlayEffect:
		EffectComp->PlayEffect(EffectTag, location, rotation, scale);
		break;
		
	case EEffectFunctionType::PlayEffects:
		EffectComp->PlayEffects(EffectTags, location, rotation, scale);
		break;
		
	case EEffectFunctionType::PlayEffectWithDelay:
		EffectComp->PlayEffectWithDelay(EffectTag, Delay, location, rotation, scale);
		break;
		
	case EEffectFunctionType::PlayEffectLoop:
		EffectComp->PlayEffectLoop(EffectTag, LoopInterval, LoopCount, location, rotation, scale);
		break;
		
	case EEffectFunctionType::PlayEffectAtSocket:
		EffectComp->PlayEffectAtSocket(EffectTag, SocketName, location, rotation, scale);
		break;
		
	case EEffectFunctionType::PlayEffectAttachedToSocket:
		EffectComp->PlayEffectAttachedToSocket(EffectTag, SocketName, location, rotation, scale);
		break;
		
	case EEffectFunctionType::PlayEffectAtSocketWithDelay:
		EffectComp->PlayEffectAtSocketWithDelay(EffectTag, SocketName, Delay, location, rotation, scale);
		break;
		
	case EEffectFunctionType::PlayEffectAtSocketLoop:
		EffectComp->PlayEffectAtSocketLoop(EffectTag, SocketName, LoopInterval, LoopCount, location, rotation, scale);
		break;
		
	case EEffectFunctionType::PlayEffectAttachedToSocketLoop:
		EffectComp->PlayEffectAttachedToSocketLoop(EffectTag, SocketName, LoopInterval, LoopCount, location, rotation, scale);
		break;
	}
}
