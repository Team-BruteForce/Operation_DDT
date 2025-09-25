/**
 * @file BossEffectExecute.cpp
 * @brief 보스 이펙트 실행기 구현 파일
 * 
 * @details
 * 보스의 이펙트 실행을 담당하는 컴포넌트입니다.
 * 이펙트 매니저와 연동하여 이펙트를 실행하고 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Effect/BossEffectExecute.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Boss/Effect/BossEffectManager.h"

UBossEffectExecute::UBossEffectExecute()
{
	OwnerCharacter = nullptr;
	World = nullptr;
	EffectManager = nullptr;
}

void UBossEffectExecute::BeginPlay(ACharacter* InOwner, TMap<FGameplayTag, FBossEffectData>& InEffectDatas, UBossEffectManager* InEffectManager)
{
	OwnerCharacter = InOwner;
	World = OwnerCharacter->GetWorld();
	EffectDatas = InEffectDatas;
	EffectManager = InEffectManager;
}

void UBossEffectExecute::ExecuteEffect(FGameplayTag EffectTag, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CheckNull(EffectManager);
	
	if (!EffectDatas.Contains(EffectTag))
	{
		CLog::Log("BossEffectExecute::ExecuteEffect - EffectTag not found: " + EffectTag.ToString());
		return;
	}

	// EffectManager를 통해 이펙트 실행 (스케일은 EffectManager에서 처리)
	EffectManager->PlayEffect(EffectTag, InLocation, InRotation, InScale);
}

void UBossEffectExecute::ExecuteEffects(TArray<FGameplayTag> EffectTags, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CheckNull(EffectManager);
	
	for (const FGameplayTag& effectTag : EffectTags)
	{
		ExecuteEffect(effectTag, InLocation, InRotation, InScale);
	}
}

void UBossEffectExecute::ExecuteEffectWithDelay(FGameplayTag EffectTag, const FVector& InLocation, const FRotator& InRotation, float Delay, const FVector& InScale)
{
	CheckNull(World);
	
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([EffectTag,InLocation,InRotation,InScale,this]()
	{
		ExecuteEffect(EffectTag, InLocation, InRotation, InScale);
	});
	
	FTimerHandle handle;
	World->GetTimerManager().SetTimer(handle, timerDelegate, Delay, false);
}

void UBossEffectExecute::ExecuteEffectLoop(FGameplayTag EffectTag, const FVector& InLocation, const FRotator& InRotation, float Interval, int32 LoopCount, const FVector& InScale)
{
	CheckNull(World);
	CheckNull(EffectManager);
	
	if (!EffectDatas.Contains(EffectTag))
	{
		CLog::Log("BossEffectExecute::ExecuteEffectLoop - EffectTag not found: " + EffectTag.ToString());
		return;
	}

	// 타이머 핸들을 멤버 변수로 저장하여 안전하게 관리
	FTimerHandle* handle = new FTimerHandle();
	int32* currentLoop = new int32(0);
	
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this, EffectTag, InLocation, InRotation, LoopCount, InScale, currentLoop, handle]()
	{
		ExecuteEffect(EffectTag, InLocation, InRotation, InScale);
		(*currentLoop)++;
		
		// 루프 횟수가 설정되어 있고 도달했으면 정지
		if (LoopCount > 0 && (*currentLoop) >= LoopCount)
		{
			World->GetTimerManager().ClearTimer(*handle);
			delete currentLoop;
			delete handle;
		}
	});
	
	World->GetTimerManager().SetTimer(*handle, timerDelegate, Interval, true);
}

void UBossEffectExecute::ExecuteEffectAttachedToSocketLoop(FGameplayTag EffectTag, const FName& SocketName, float Interval, int32 LoopCount, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CLog::Log("ExecuteEffectAttachedToSocketLoop called - EffectTag: " + EffectTag.ToString() + ", SocketName: " + SocketName.ToString());
	
	CheckNull(World);
	CheckNull(OwnerCharacter);
	
	USkeletalMeshComponent* mesh = OwnerCharacter->GetMesh();
	if (!mesh)
	{
		CLog::Log("BossEffectExecute::ExecuteEffectAttachedToSocketLoop - Mesh not found");
		return;
	}

	// 소켓이 존재하는지 확인
	if (!mesh->DoesSocketExist(SocketName))
	{
		CLog::Log("BossEffectExecute::ExecuteEffectAttachedToSocketLoop - Socket does not exist: " + SocketName.ToString());
		return;
	}

	// 소켓 회전 가져오기
	FRotator socketRotation = mesh->GetSocketRotation(SocketName);
	FRotator finalRotation = InRotation.IsZero() ? socketRotation : InRotation;
	
	CLog::Log("ExecuteEffectAttachedToSocketLoop - Starting loop with Interval: " + FString::SanitizeFloat(Interval) + ", LoopCount: " + FString::FromInt(LoopCount));
	
	// 타이머 핸들을 동적 할당하여 안전하게 관리
	FTimerHandle* handle = new FTimerHandle();
	int32* currentLoop = new int32(0);
	
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this, EffectTag, SocketName, LoopCount, InLocation, finalRotation, InScale, currentLoop, handle]()
	{
		CLog::Log("ExecuteEffectAttachedToSocketLoop - Loop iteration: " + FString::FromInt((*currentLoop) + 1));
		ExecuteEffectAttachedToSocket(EffectTag, SocketName, InLocation, finalRotation, InScale);
		(*currentLoop)++;
		
		// 루프 횟수가 설정되어 있고 도달했으면 정지
		if (LoopCount > 0 && (*currentLoop) >= LoopCount)
		{
			CLog::Log("ExecuteEffectAttachedToSocketLoop - Loop completed, stopping timer");
			World->GetTimerManager().ClearTimer(*handle);
			delete currentLoop;
			delete handle;
		}
	});
	
	World->GetTimerManager().SetTimer(*handle, timerDelegate, Interval, true);
}

void UBossEffectExecute::Begin_ExecuteEffect()
{
	// 이펙트 실행 시작 시 필요한 로직
}

void UBossEffectExecute::End_ExecuteEffect()
{
	// 이펙트 실행 종료 시 필요한 로직
}

void UBossEffectExecute::ExecuteEffectAtSocket(FGameplayTag EffectTag, const FName& SocketName, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CLog::Log("ExecuteEffectAtSocket called - EffectTag: " + EffectTag.ToString() + ", SocketName: " + SocketName.ToString());
	
	CheckNull(OwnerCharacter);
	CheckNull(EffectManager);
	
	USkeletalMeshComponent* mesh = OwnerCharacter->GetMesh();
	if (!mesh)
	{
		CLog::Log("BossEffectExecute::ExecuteEffectAtSocket - Mesh not found");
		return;
	}

	// 소켓이 존재하는지 확인
	if (!mesh->DoesSocketExist(SocketName))
	{
		CLog::Log("BossEffectExecute::ExecuteEffectAtSocket - Socket does not exist: " + SocketName.ToString());
		return;
	}

	// 소켓 위치와 회전 가져오기
	FVector socketLocation = mesh->GetSocketLocation(SocketName);
	FRotator socketRotation = mesh->GetSocketRotation(SocketName);
	
	// 최종 위치: 소켓 위치 + 커스텀 오프셋
	FVector finalLocation = socketLocation + InLocation;
	
	CLog::Log("ExecuteEffectAtSocket - Socket location: " + socketLocation.ToString());
	CLog::Log("ExecuteEffectAtSocket - Custom offset: " + InLocation.ToString());
	CLog::Log("ExecuteEffectAtSocket - Final location: " + finalLocation.ToString());
	
	// 회전이 기본값이면 소켓 회전 사용
	FRotator finalRotation = InRotation.IsZero() ? socketRotation : InRotation;
	
	// EffectManager를 통해 이펙트 실행
	EffectManager->PlayEffect(EffectTag, finalLocation, finalRotation, InScale);
}

void UBossEffectExecute::ExecuteEffectAttachedToSocket(FGameplayTag EffectTag, const FName& SocketName, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CheckNull(OwnerCharacter);
	CheckNull(EffectManager);
	
	USkeletalMeshComponent* mesh = OwnerCharacter->GetMesh();
	if (!mesh)
	{
		CLog::Log("BossEffectExecute::ExecuteEffectAttachedToSocket - Mesh not found");
		return;
	}

	// 소켓 위치와 회전 가져오기
	FVector socketLocation = mesh->GetSocketLocation(SocketName);
	FRotator socketRotation = mesh->GetSocketRotation(SocketName);
	
	// 최종 위치: 소켓 위치 + 커스텀 오프셋
	FVector finalLocation = socketLocation + InLocation;
	
	// 회전이 기본값이면 소켓 회전 사용
	FRotator finalRotation = InRotation.IsZero() ? socketRotation : InRotation;
	
	// EffectManager를 통해 이펙트 실행 (소켓에 붙여서)
	EffectManager->PlayEffectAttachedToSocket(EffectTag, SocketName, finalLocation, finalRotation, InScale);
}

void UBossEffectExecute::ExecuteEffectAtSocketWithDelay(FGameplayTag EffectTag, const FName& SocketName, float Delay, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CheckNull(World);
	CheckNull(OwnerCharacter);
	
	USkeletalMeshComponent* mesh = OwnerCharacter->GetMesh();
	if (!mesh)
	{
		CLog::Log("BossEffectExecute::ExecuteEffectAtSocketWithDelay - Mesh not found");
		return;
	}

	// 소켓 회전 가져오기
	FRotator socketRotation = mesh->GetSocketRotation(SocketName);
	FRotator finalRotation = InRotation.IsZero() ? socketRotation : InRotation;
	
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this,EffectTag,SocketName,InLocation,finalRotation,InScale]()
	{
		ExecuteEffectAtSocket(EffectTag, SocketName, InLocation, finalRotation, InScale);
	});
	
	FTimerHandle handle;
	World->GetTimerManager().SetTimer(handle, timerDelegate, Delay, false);
}

void UBossEffectExecute::ExecuteEffectAtSocketLoop(FGameplayTag EffectTag, const FName& SocketName, float Interval, int32 LoopCount, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CLog::Log("ExecuteEffectAtSocketLoop called - EffectTag: " + EffectTag.ToString() + ", SocketName: " + SocketName.ToString());
	
	CheckNull(World);
	CheckNull(OwnerCharacter);
	
	USkeletalMeshComponent* mesh = OwnerCharacter->GetMesh();
	if (!mesh)
	{
		CLog::Log("BossEffectExecute::ExecuteEffectAtSocketLoop - Mesh not found");
		return;
	}

	// 소켓이 존재하는지 확인
	if (!mesh->DoesSocketExist(SocketName))
	{
		CLog::Log("BossEffectExecute::ExecuteEffectAtSocketLoop - Socket does not exist: " + SocketName.ToString());
		return;
	}

	// 소켓 회전 가져오기
	FRotator socketRotation = mesh->GetSocketRotation(SocketName);
	FRotator finalRotation = InRotation.IsZero() ? socketRotation : InRotation;
	
	CLog::Log("ExecuteEffectAtSocketLoop - Starting loop with Interval: " + FString::SanitizeFloat(Interval) + ", LoopCount: " + FString::FromInt(LoopCount));
	
	// 타이머 핸들을 동적 할당하여 안전하게 관리
	FTimerHandle* handle = new FTimerHandle();
	int32* currentLoop = new int32(0);
	
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this, EffectTag, SocketName, LoopCount, InLocation, finalRotation, InScale, currentLoop, handle]()
	{
		CLog::Log("ExecuteEffectAtSocketLoop - Loop iteration: " + FString::FromInt((*currentLoop) + 1));
		ExecuteEffectAtSocket(EffectTag, SocketName, InLocation, finalRotation, InScale);
		(*currentLoop)++;
		
		// 루프 횟수가 설정되어 있고 도달했으면 정지
		if (LoopCount > 0 && (*currentLoop) >= LoopCount)
		{
			CLog::Log("ExecuteEffectAtSocketLoop - Loop completed, stopping timer");
			World->GetTimerManager().ClearTimer(*handle);
			delete currentLoop;
			delete handle;
		}
	});
	
	World->GetTimerManager().SetTimer(*handle, timerDelegate, Interval, true);
}
