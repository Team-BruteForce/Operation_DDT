/**
 * @file BossEffectManager.cpp
 * @brief 보스 이펙트 매니저 구현 파일
 * 
 * @details
 * 보스의 이펙트 시스템을 관리하는 매니저입니다.
 * 데이터 테이블과 이펙트 풀링을 통해 이펙트를 효율적으로 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Effect/BossEffectManager.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Engine/DataTable.h"
#include "Boss/Effect/BossEffect.h"
#include "Boss/Effect/BossEffectExecute.h"

UBossEffectManager::UBossEffectManager()
{
	OwnerCharacter = nullptr;
	World = nullptr;
	EffectDataTable = nullptr;
	MaxPoolSize = 20;
	bAutoExpandPool = true;
	EffectExecute = nullptr;
}

void UBossEffectManager::BeginPlay(ACharacter* InOwner, UDataTable* InEffectDataTable, int32 InMaxPoolSize, bool InAutoExpandPool)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();
	EffectDataTable = InEffectDataTable;
	MaxPoolSize = InMaxPoolSize;
	bAutoExpandPool = InAutoExpandPool;

	// 데이터 테이블에서 데이터 로드
	LoadDataFromTables();

	// 이펙트 풀 초기화
	InitializeEffectPool();
}

void UBossEffectManager::PlayEffect(FGameplayTag EffectTag, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CLog::Log("BossEffectManager::PlayEffect called - EffectTag: " + EffectTag.ToString() + ", Location: " + InLocation.ToString());
	
	CheckNull(World);

	if (!EffectDatas.Contains(EffectTag))
	{
		CLog::Log("BossEffectManager::PlayEffect - EffectTag not found: " + EffectTag.ToString());
		return;
	}

	FBossEffectData& effectData = EffectDatas[EffectTag];
	CLog::Log("BossEffectManager::PlayEffect - Found effect data, EffectSystem: " + (effectData.EffectSystem ? effectData.EffectSystem->GetName() : TEXT("NULL")));

	// 풀에서 이펙트 가져오기
	ABossEffect* effect = GetEffectFromPool();
	if (!effect)
	{
		CLog::Log("BossEffectManager::PlayEffect - No available effect in pool");
		return;
	}

	CLog::Log("BossEffectManager::PlayEffect - Got effect from pool: " + effect->GetName());

	// 이펙트 활성화
	effect->ActivateEffect(effectData, InLocation, InRotation, InScale);

	// 활성화된 이펙트 목록에 추가
	ActiveEffects.Add(effect);
	
	CLog::Log("BossEffectManager::PlayEffect - Effect activated successfully");
}

void UBossEffectManager::PlayEffectAttachedToSocket(FGameplayTag EffectTag, const FName& SocketName, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CheckNull(World);
	CheckNull(OwnerCharacter);

	if (!EffectDatas.Contains(EffectTag))
	{
		CLog::Log("BossEffectManager::PlayEffectAttachedToSocket - EffectTag not found: " + EffectTag.ToString());
		return;
	}

	FBossEffectData& effectData = EffectDatas[EffectTag];

	// 풀에서 이펙트 가져오기
	ABossEffect* effect = GetEffectFromPool();
	if (!effect)
	{
		CLog::Log("BossEffectManager::PlayEffectAttachedToSocket - No available effect in pool");
		return;
	}

	// 소켓에 붙여서 이펙트 활성화
	effect->ActivateEffectAttachedToSocket(effectData, SocketName, InLocation, InRotation, InScale);

	// 활성화된 이펙트 목록에 추가
	ActiveEffects.Add(effect);
}

void UBossEffectManager::SetMaxPoolSize(int32 NewMaxPoolSize)
{
	MaxPoolSize = FMath::Max(1, NewMaxPoolSize);
	
	// 현재 풀 크기가 새로운 최대 크기보다 크면 조정
	if (EffectPool.Num() > MaxPoolSize)
	{
		// 초과분 제거
		for (int32 i = EffectPool.Num() - 1; i >= MaxPoolSize; --i)
		{
			if (EffectPool[i])
			{
				EffectPool[i]->Destroy();
			}
			EffectPool.RemoveAt(i);
		}
	}
}

void UBossEffectManager::SetAutoExpandPool(bool bAutoExpand)
{
	bAutoExpandPool = bAutoExpand;
}

// ExecuteEffect 관련 함수들 제거 - BossEffectExecute에서 직접 처리

void UBossEffectManager::StopAllEffects()
{
	for (ABossEffect* effect : ActiveEffects)
	{
		if (effect && effect->IsActive())
		{
			effect->DeactivateEffect();
		}
	}
}

void UBossEffectManager::StopEffect(FGameplayTag EffectTag)
{
	for (ABossEffect* effect : ActiveEffects)
	{
		if (effect && effect->IsActive() && effect->GetCurrentEffectTag() == EffectTag)
		{
			effect->DeactivateEffect();
		}
	}
}

void UBossEffectManager::LoadDataFromTables()
{
	// 이펙트 데이터 테이블 로드
	if (EffectDataTable)
	{
		TArray<FBossEffectData*> effectRows;
		EffectDataTable->GetAllRows<FBossEffectData>(TEXT("LoadEffectData"), effectRows);
		
		for (FBossEffectData* row : effectRows)
		{
			if (row)
			{
				EffectDatas.Add(row->EffectTag, *row);
			}
		}
	}
}

void UBossEffectManager::InitializeEffectPool()
{
	CheckNull(World);

	// 최대 풀 크기만큼 이펙트 액터 생성
	for (int32 i = 0; i < MaxPoolSize; ++i)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = OwnerCharacter;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ABossEffect* effect = World->SpawnActor<ABossEffect>(ABossEffect::StaticClass(), spawnParams);
		if (effect)
		{
			// 이펙트 완료 델리게이트 바인딩
			effect->OnEffectFinishedDelegate.AddDynamic(this, &UBossEffectManager::OnEffectFinished);
			EffectPool.Add(effect);
		}
	}

	CLog::Log("BossEffectManager::InitializeEffectPool - Created " + FString::FromInt(EffectPool.Num()) + " effects");
}

ABossEffect* UBossEffectManager::GetEffectFromPool()
{
	// 사용 가능한 이펙트가 있으면 반환
	if (EffectPool.Num() > 0)
	{
		ABossEffect* effect = EffectPool.Pop();
		return effect;
	}

	// 풀이 비어있으면 확장
	ExpandEffectPool();
	
	if (EffectPool.Num() > 0)
	{
		ABossEffect* effect = EffectPool.Pop();
		return effect;
	}

	return nullptr;
}

void UBossEffectManager::ReturnEffectToPool(ABossEffect* InEffect)
{
	CheckNull(InEffect);

	// 활성화된 이펙트 목록에서 제거
	ActiveEffects.Remove(InEffect);

	// 풀로 반환
	EffectPool.Add(InEffect);
}

void UBossEffectManager::ExpandEffectPool()
{
	CheckNull(World);

	// 자동 확장이 비활성화되어 있으면 확장하지 않음
	if (!bAutoExpandPool)
	{
		CLog::Log("BossEffectManager::ExpandEffectPool - Auto expand is disabled");
		return;
	}

	// 풀 크기를 2배로 확장
	int32 currentSize = EffectPool.Num();
	for (int32 i = 0; i < currentSize; ++i)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = OwnerCharacter;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ABossEffect* effect = World->SpawnActor<ABossEffect>(ABossEffect::StaticClass(), spawnParams);
		if (effect)
		{
			// 이펙트 완료 델리게이트 바인딩
			effect->OnEffectFinishedDelegate.AddDynamic(this, &UBossEffectManager::OnEffectFinished);
			EffectPool.Add(effect);
		}
	}

	MaxPoolSize = EffectPool.Num();
	CLog::Log("BossEffectManager::ExpandEffectPool - Expanded pool to " + FString::FromInt(MaxPoolSize) + " effects");
}

void UBossEffectManager::OnEffectFinished(ABossEffect* InEffect)
{
	CheckNull(InEffect);
	
	// 이펙트를 풀로 반환
	ReturnEffectToPool(InEffect);
}

