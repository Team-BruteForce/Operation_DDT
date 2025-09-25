/**
 * @file BossEffectStructures.cpp
 * @brief 보스 이펙트 구조체 구현 파일
 * 
 * @details
 * 보스의 이펙트 관련 구조체들을 구현하는 파일입니다.
 * 이펙트 데이터와 나이아가라 시스템을 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

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

