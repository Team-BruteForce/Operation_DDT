// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/Effect/BossEffectStructures.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"

void FBossEffectData::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotation)
{
	CheckNull(EffectSystem);
	
	FTransform transform;
	// 최종 위치 = 전달받은 위치(보스 위치 또는 커스텀 위치) + 회전된 오프셋
	transform.SetLocation(InLocation + InRotation.RotateVector(EffectLocation));
	transform.SetScale3D(EffectScale);
	// 최종 회전 = 전달받은 회전(보스 회전 또는 커스텀 회전) + 추가 회전
	transform.SetRotation((InRotation + EffectRotation).Quaternion());
	
	CHelpers::PlayEffect(InWorld, EffectSystem, transform);
}

void FBossEffectData::PlayEffect(UWorld* InWorld, const FVector& InLocation)
{
	CheckNull(EffectSystem);
	
	FTransform transform;
	// 최종 위치 = 전달받은 위치(보스 위치 또는 커스텀 위치) + 오프셋
	transform.SetLocation(InLocation + EffectLocation);
	transform.SetScale3D(EffectScale);
	// 최종 회전 = 추가 회전만 사용
	transform.SetRotation(EffectRotation.Quaternion());
	
	CHelpers::PlayEffect(InWorld, EffectSystem, transform);
}

