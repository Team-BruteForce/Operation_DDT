/**
 * @file BossEffect.cpp
 * @brief 보스 이펙트 구현 파일
 * 
 * @details
 * 보스의 이펙트 시스템을 구현하는 파일입니다.
 * 나이아가라 컴포넌트와 이펙트 매니저를 통해 이펙트를 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Effect/BossEffect.h"
#include "Global.h"
#include "Components/SceneComponent.h"
#include "NiagaraComponent.h"
#include "Boss/Effect/BossEffectManager.h"
#include "GameFramework/Character.h"

// Sets default values
ABossEffect::ABossEffect()
{
	PrimaryActorTick.bCanEverTick = true;

	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	CHelpers::CreateComponent<UNiagaraComponent>(this, &NiagaraComponent, "NiagaraComponent", Root);
	
	SetRootComponent(Root);
	NiagaraComponent->SetupAttachment(Root);
	
	// 초기에는 비활성화 상태
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

// Called when the game starts or when spawned
void ABossEffect::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABossEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 활성화된 이펙트의 시간 업데이트
	if (bIsActive)
	{
		ElapsedTime += DeltaTime;
		
		// Duration이 0이면 Niagara 시스템 시간을 따름 (한 번만 실행), 양수면 설정된 Duration을 따름
		if (CurrentDuration == 0.0f)
		{
			// Niagara 시스템이 비활성화되었는지 확인 (자연스러운 종료)
			if (NiagaraComponent && !NiagaraComponent->IsActive())
			{
				CLog::Log("ABossEffect::Tick - Niagara system finished naturally after " + FString::SanitizeFloat(ElapsedTime) + " seconds (Duration=0, following Niagara time)");
				DeactivateEffect();
			}
			else
			{
				// 1초마다 진행 상황 로그 (인스턴스별로 관리)
				LastLogTime += DeltaTime;
				if (LastLogTime >= 1.0f)
				{
					CLog::Log("ABossEffect::Tick - Niagara system running for " + FString::SanitizeFloat(ElapsedTime) + " seconds (Duration=0, following Niagara time)");
					LastLogTime = 0.0f;
				}
			}
		}
		else if (CurrentDuration > 0.0f)
		{
			// 설정된 Duration을 따름
			if (ElapsedTime >= CurrentDuration)
			{
				CLog::Log("ABossEffect::Tick - Effect finished by Duration! ElapsedTime: " + FString::SanitizeFloat(ElapsedTime) + ", Duration: " + FString::SanitizeFloat(CurrentDuration));
				DeactivateEffect();
			}
			else
			{
				// 1초마다 남은 시간 로그 (인스턴스별로 관리)
				LastLogTime += DeltaTime;
				if (LastLogTime >= 1.0f)
				{
					float RemainingTime = CurrentDuration - ElapsedTime;
					CLog::Log("ABossEffect::Tick - Elapsed: " + FString::SanitizeFloat(ElapsedTime) + "s, Remaining: " + FString::SanitizeFloat(RemainingTime) + "s (following Duration)");
					LastLogTime = 0.0f;
				}
			}
		}
		else
		{
			// Duration이 음수면 무한 실행 (Niagara가 무한이면 계속 실행)
			if (NiagaraComponent && !NiagaraComponent->IsActive())
			{
				CLog::Log("ABossEffect::Tick - Niagara system finished naturally after " + FString::SanitizeFloat(ElapsedTime) + " seconds (Duration<0, infinite mode)");
				DeactivateEffect();
			}
			else
			{
				// 1초마다 진행 상황 로그 (인스턴스별로 관리)
				LastLogTime += DeltaTime;
				if (LastLogTime >= 1.0f)
				{
					CLog::Log("ABossEffect::Tick - Niagara system running for " + FString::SanitizeFloat(ElapsedTime) + " seconds (Duration<0, infinite mode)");
					LastLogTime = 0.0f;
				}
			}
		}
	}
}

void ABossEffect::ActivateEffect(const FBossEffectData& InEffectData, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CLog::Log("ABossEffect::ActivateEffect called - EffectTag: " + InEffectData.EffectTag.ToString());
	
	CheckNull(NiagaraComponent);
	CheckNull(InEffectData.EffectSystem);

	// 이펙트 데이터 설정
	CurrentEffectTag = InEffectData.EffectTag;
	CurrentDuration = InEffectData.Duration;
	ElapsedTime = 0.0f;
	bIsActive = true;

	CLog::Log("ABossEffect::ActivateEffect - Setting Niagara asset: " + InEffectData.EffectSystem->GetName());
	CLog::Log("ABossEffect::ActivateEffect - Duration: " + FString::SanitizeFloat(InEffectData.Duration));

	// Niagara System 설정
	NiagaraComponent->SetAsset(InEffectData.EffectSystem);
	
	// 스케일 설정 (커스텀 스케일이 기본값이면 데이터 스케일만 사용)
	FVector finalScale = InScale.IsZero() ? InEffectData.EffectScale : InEffectData.EffectScale * InScale;
	NiagaraComponent->SetRelativeScale3D(finalScale);

	CLog::Log("ABossEffect::ActivateEffect - Final scale: " + finalScale.ToString());

	// 위치 설정 - 소켓 위치 + 이펙트 데이터 위치 오프셋
	FVector finalLocation = InLocation + InRotation.RotateVector(InEffectData.EffectLocation);
	FRotator finalRotation = InRotation + InEffectData.EffectRotation;
	
	PlaceInWorld(finalLocation, finalRotation);

	CLog::Log("ABossEffect::ActivateEffect - Placed in world at: " + GetActorLocation().ToString());

	// 액터 활성화
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	// 이펙트 재생
	NiagaraComponent->Activate();
	
	CLog::Log("ABossEffect::ActivateEffect - Niagara component activated");
}

void ABossEffect::ActivateEffectAttachedToSocket(const FBossEffectData& InEffectData, const FName& SocketName, const FVector& InLocation, const FRotator& InRotation, const FVector& InScale)
{
	CheckNull(NiagaraComponent);
	CheckNull(InEffectData.EffectSystem);

	// 이펙트 데이터 설정
	CurrentEffectTag = InEffectData.EffectTag;
	CurrentDuration = InEffectData.Duration;
	ElapsedTime = 0.0f;
	bIsActive = true;

	// Niagara System 설정
	NiagaraComponent->SetAsset(InEffectData.EffectSystem);
	
	// 스케일 설정 (커스텀 스케일이 기본값이면 데이터 스케일만 사용)
	FVector finalScale = InScale.IsZero() ? InEffectData.EffectScale : InEffectData.EffectScale * InScale;
	NiagaraComponent->SetRelativeScale3D(finalScale);

	// 소켓에 붙이기
	AttachToSocket(SocketName, InLocation, InRotation);

	// 액터 활성화
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	// 이펙트 재생
	NiagaraComponent->Activate();
}

void ABossEffect::DeactivateEffect()
{
	if (!bIsActive) return;

	CLog::Log("ABossEffect::DeactivateEffect called - EffectTag: " + CurrentEffectTag.ToString() + ", ElapsedTime: " + FString::SanitizeFloat(ElapsedTime));

	bIsActive = false;
	CurrentEffectTag = FGameplayTag::EmptyTag;
	CurrentDuration = 0.0f;
	ElapsedTime = 0.0f;

	// 이펙트 정지
	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
		// 스케일을 기본값으로 복원
		NiagaraComponent->SetRelativeScale3D(FVector::OneVector);
	}

	// 액터 비활성화
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	// 풀로 반환 - EffectManager에 직접 알림
	if (UBossEffectManager* Manager = Cast<UBossEffectManager>(GetOwner()->GetComponentByClass(UBossEffectManager::StaticClass())))
	{
		Manager->OnEffectFinished(this);
	}
	else
	{
		// 델리게이트로 백업
		OnEffectFinishedDelegate.Broadcast(this);
	}
}

// OnEffectFinished 함수 제거 - Tick에서 지속시간으로 관리

void ABossEffect::AttachToBoss(ACharacter* InBoss, const FName& InSocketName)
{
	CheckNull(InBoss);
	
	AttachToComponent(InBoss->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void ABossEffect::AttachToSocket(const FName& SocketName, const FVector& InLocation, const FRotator& InRotation)
{
	// 소켓에 붙이기 (OwnerCharacter의 메시에)
	if (AActor* owner = GetOwner())
	{
		if (ACharacter* character = Cast<ACharacter>(owner))
		{
			if (USkeletalMeshComponent* mesh = character->GetMesh())
			{
				AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), SocketName);
				
				// 커스텀 위치 오프셋 설정
				if (!InLocation.IsZero())
				{
					SetActorRelativeLocation(InLocation);
				}
				
				// 회전 설정
				if (!InRotation.IsZero())
				{
					SetActorRelativeRotation(InRotation);
				}
			}
		}
	}
}

void ABossEffect::PlaceInWorld(const FVector& InLocation, const FRotator& InRotation)
{
	SetActorLocation(InLocation);
	SetActorRotation(InRotation);
}

