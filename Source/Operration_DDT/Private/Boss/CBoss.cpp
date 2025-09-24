/**
 * @file CBoss.cpp
 * @brief 보스 캐릭터의 핵심 로직을 구현하는 파일
 * 
 * @details
 * 보스 캐릭터의 생성, 초기화, 공격 애니메이션 재생 등의 기능을 담당합니다.
 * StateTree와 연동하여 보스의 AI 행동을 제어하는 기반이 되는 클래스입니다.
 * 
 * @author [이효원]
 * @date 2024-12-19
 */

#include "Boss/CBoss.h"
#include "Global.h"
#include "Boss/Component/CBossWeaponComponent.h"
#include "MotionWarpingComponent.h"
#include "Blueprint/UserWidget.h"
#include "Boss/Component/BossEffectComponent.h"
#include "Boss/Component/CBossStatusComponent.h"
#include "Boss/Component/FlyingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StateTreeComponent.h"
#include "Engine/DamageEvents.h"
#include "Boss/Actor/BossDissolve.h"
#include "Boss/Component/BossStateComponent.h"
#include "Boss/Component/CBossMovementComponent.h"
#include "Boss/Component/BossProjectileComponent.h"
#include "Boss/Component/CBossTargetingComponent.h"
#include "Boss/Component/BossDebugComponent.h"
#include "Boss/Widget/BossStatusWidget.h"
#include "Player/DDTGameMode.h"
#include "Utilities/AssetPath.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

/**
 * @brief 보스 캐릭터 생성자
 * 
 * 보스 캐릭터의 기본 설정을 초기화합니다.
 */
ACBoss::ACBoss()
{
	// 스켈레탈 메시 에셋 로드 및 설정
	USkeletalMesh* Bodymesh;
	CHelpers::GetAsset<USkeletalMesh>(&Bodymesh, AssetPaths::Boss_MESH);
	GetMesh()->SetSkeletalMesh(Bodymesh);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeScale3D(FVector(1.8f));


	// 애님 인스턴스 클래스 에셋 로드 및 설정
	// TSubclassOf<UAnimInstance> Anim;
	// CHelpers::GetClass<UAnimInstance>(&Anim, AssetPaths::Boss_ANIM);
	// if (Anim)
	// {
	// 	GetMesh()->SetAnimInstanceClass(Anim);
	// }


	CHelpers::CreateActorComponent<UBossStateComponent>(this,&BossStateComponent,"StateComp");
	CHelpers::CreateActorComponent<UCBossMovementComponent>(this,&BossMovementComponent,"MovementComp");
	CHelpers::CreateActorComponent<UCBossWeaponComponent>(this,&BossWeaponComponent,"WeaponComp");
	CHelpers::CreateActorComponent<UCBossStatusComponent>(this,&BossStatusComponent,"StatusComp");
	CHelpers::CreateActorComponent<UMotionWarpingComponent>(this,&BossMotionWarping,"MotionWarpComp");
	CHelpers::CreateActorComponent<UBossProjectileComponent>(this,&ProjectileComp,"ProjectileComp");
	CHelpers::CreateActorComponent<UCBossTargetingComponent>(this,&TargetingComp,"TargetingComp");
	CHelpers::CreateActorComponent<UBossDebugComponent>(this,&DebugComp,"DebugComp");
	CHelpers::CreateActorComponent<UBossEffectComponent>(this,&EffectComponent,"EffectComp");
	CHelpers::CreateActorComponent<UFlyingComponent>(this,&FlyingComponent,"FlyingComponent");

	// 보스 배경음악 컴포넌트 생성
	CHelpers::CreateActorComponent<UAudioComponent>(this,&BossBGMComponent,"BGMComponent");

/**
 *보스 위젯 가져오기
 **/
	CHelpers::GetClass<UUserWidget>(&BossWidgetClass, AssetPaths::Boss_Widget);
	
	// USkeletalMesh* WingsMesh;
	// CHelpers::GetAsset<USkeletalMesh>(&WingsMesh, AssetPaths::Boss_Wings);
	// CHelpers::CreateComponent<USkeletalMeshComponent>(this,&WingMeshComp,"WingMeshComp");
	// WingMeshComp->SetSkeletalMesh(WingsMesh);
	// WingMeshComp->SetupAttachment(GetMesh(), FName("Wings_Socket"));
	//
	// TSubclassOf<UAnimInstance> WingAnim;
	// CHelpers::GetClass<UAnimInstance>(&WingAnim, AssetPaths::Boss_WingsAnim);
	// WingMeshComp->SetAnimInstanceClass(WingAnim);
}

void ACBoss::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	// 1순위: 죽음 체크 (한 번만 실행)
	if (BossStatusComponent->BossCurrentStats.CurrentHP <= 0 && !bIsDead)
	{
		if (bIsDead)return;
		bIsDead = true; // 죽음 상태로 설정
		LowerBossBGMVolume(); // 보스 죽음 시 배경음악 볼륨 감소
		UStateTreeComponent* StateTreeComp = GetController()->FindComponentByClass<UStateTreeComponent>();
		StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag("BOSS.State.Dead"));
		BossWidget->SwitchBossCompleteUI();
		return; // 죽음 처리 후 다른 로직 실행 안함
	}
	
	// 2순위: 페이즈 전환 체크 (스턴보다 우선)
	if (BossStatusComponent->BossCurrentStats.CurrentHP <= 1200 && 
		BossWeaponComponent->GetCurrentWeaponMode() == FGameplayTag::RequestGameplayTag("BOSS.Pase.One"))
	{
		UStateTreeComponent* StateTreeComp = GetController()->FindComponentByClass<UStateTreeComponent>();
		StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag("BOSS.Event.ChangePase"));
		IsPaseChange=true;
		return; // 페이즈 전환 처리 후 스턴 로직 실행 안함
	}
	
	// 3순위: 스턴 체크 (페이즈 전환 후에만)
	if (BossStatusComponent->BossCurrentStats.CurrentGroggyGauge >= BossStatusComponent->BossCurrentStats.MaxGroggyGauge)
	{
		if (BossStatusComponent->GetIsPaseChange()) return;
		
		// 현재 스턴 상태가 아닐 때만 스턴 상태로 전환
		if (!BossStatusComponent->IsGroggy)
		{
			UStateTreeComponent* StateTreeComp = GetController()->FindComponentByClass<UStateTreeComponent>();
			StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag("BOSS.State.Stunned"));
			BossStatusComponent->BossCurrentStats.CurrentGroggyGauge = 0;
		}
	}
}

float ACBoss::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator,
                         AActor* DamageCauser)
{
	const FPointDamageEvent* PointDamageEvent = static_cast<const FPointDamageEvent*>(&DamageEvent);
	FName HitBone = PointDamageEvent->HitInfo.BoneName;
	FVector HitLocation = PointDamageEvent->HitInfo.ImpactPoint;
	bool isCritical = false;
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		if (PointDamageEvent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Bone: %s"), *HitBone.ToString());

			// 🎯 헤드샷 판정
			if (HitBone == FName("head") || HitBone == FName("Head") || HitBone.ToString().Contains("Head"))
			{
				DamageAmount *= 2.0f; // 데미지 배율
				isCritical = true;
			}
		}
	}

	// 기존 보스 상태 처리
	if (!BossStatusComponent->GetIsPaseChange())
	{
		BossStatusComponent->SetDamage(DamageAmount, isCritical);
		BossStatusComponent->IncreaseAP(DamageAmount);
		BossStatusComponent->IncreaseGroggyGauge(DamageAmount);
		PlayHitMotion(HitBone);
	}
	HPUpdate();


	return DamageAmount;
}


/** 
 * @brief 게임 시작 또는 스폰 시 호출되는 함수
 * 
 * 캐릭터가 게임에 스폰될 때 초기화 작업을 수행합니다.
 */
void ACBoss::BeginPlay()
{
	Super::BeginPlay();
	
	// 보스 초기 위치 저장 (레벨에 배치된 위치)
	InitialLocation = GetActorLocation();

	FActorSpawnParameters Parameters;
	Parameters.Owner=this;
	if (GetWorld() and BossDissolveClass)
	{
		BossDissolve=GetWorld()->SpawnActor<ABossDissolve>(BossDissolveClass,FVector::ZeroVector,FRotator::ZeroRotator,Parameters);
	}
	if (BossWidgetClass)
	{
		BossWidget=CreateWidget<UBossStatusWidget>(GetWorld(),BossWidgetClass);
		BossWidget->Owner=this;
		
		// 초기 체력 UI 업데이트
		BossWidget->UpdateBossHP(
			BossStatusComponent->BossCurrentStats.CurrentHP,
			BossStatusComponent->BossCurrentStats.MaxHP
		);
	}

	// 보스 배경음악 로드 및 사운드 컴포넌트 설정
	CHelpers::GetAssetDynamic(&BossBGM, AssetPaths::Boss_BGM);
	UE_LOG(LogTemp, Warning, TEXT("🔊 보스 배경음악 로드 - BossBGM: %s"), BossBGM ? TEXT("성공") : TEXT("실패"));
	UE_LOG(LogTemp, Warning, TEXT("🔊 사운드 컴포넌트 - BossBGMComponent: %s"), BossBGMComponent ? TEXT("유효함") : TEXT("nullptr"));
	
	if (BossBGMComponent && BossBGM)
	{
		BossBGMComponent->SetSound(BossBGM);
		BossBGMComponent->bAutoActivate = false; // 자동 재생 비활성화
		UE_LOG(LogTemp, Warning, TEXT("🔊 사운드 컴포넌트 설정 완료"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("❌ 사운드 컴포넌트 설정 실패 - BossBGMComponent: %s, BossBGM: %s"), 
			BossBGMComponent ? TEXT("유효함") : TEXT("nullptr"),
			BossBGM ? TEXT("유효함") : TEXT("nullptr"));
	}
}

void ACBoss::PlayHitMotion(FName BoneName="spine_03")
{
	GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("spine_03"), true, true);
	// 일정 시간 후 복구
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->AttachToComponent(GetCapsuleComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}, 0.05f, false);
}

void ACBoss::ShowBossStatusWidget()
{
	BossWidget->AddToViewport();
}

void ACBoss::HPUpdate()
{
	// UI 업데이트
	if (BossWidget)
	{
		BossWidget->UpdateBossHP(
			BossStatusComponent->BossCurrentStats.CurrentHP,
			BossStatusComponent->BossCurrentStats.MaxHP
		);
	}
}

void ACBoss::RestartUI()
{
	ADDTGameMode* GameMode=Cast<ADDTGameMode>(GetWorld()->GetAuthGameMode());
	GameMode->RestartGame();
}

void ACBoss::PlayBossBGM()
{
	if (BossBGMComponent && !bIsBGMPlaying)
	{
		BossBGMComponent->SetVolumeMultiplier(1.0f); // 볼륨을 원래대로 복원
		BossBGMComponent->Play();
		bIsBGMPlaying = true;
	}
}

void ACBoss::StopBossBGM()
{
	if (BossBGMComponent && bIsBGMPlaying)
	{
		// 볼륨 페이드아웃 타이머 정리
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(VolumeFadeTimer);
		}
		
		BossBGMComponent->Stop();
		bIsBGMPlaying = false;
	}
}

void ACBoss::LowerBossBGMVolume()
{
	if (BossBGMComponent && bIsBGMPlaying)
	{
		// 기존 타이머 정리
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().ClearTimer(VolumeFadeTimer);
		}
		
		// 15초 동안 점진적으로 볼륨을 0까지 감소
		float FadeDuration = 15.0f;
		float UpdateInterval = 0.1f; // 0.1초마다 업데이트
		float FadeSteps = FadeDuration / UpdateInterval; // 동적으로 계산
		float StepDuration = UpdateInterval;
		float VolumeStep = 1.0f / FadeSteps;
		
		float CurrentVolume = 1.0f;
		int32 StepCount = 0;
		// 타이머로 점진적 볼륨 감소
		GetWorld()->GetTimerManager().SetTimer(VolumeFadeTimer, [this, CurrentVolume, VolumeStep, StepCount, FadeSteps]() mutable
		{
			if (BossBGMComponent && bIsBGMPlaying)
			{
				CurrentVolume -= VolumeStep;
				CurrentVolume = FMath::Max(0.0f, CurrentVolume); // 0 이하로 가지 않도록
				
				BossBGMComponent->SetVolumeMultiplier(CurrentVolume);
				
				StepCount++;
				if (StepCount >= FadeSteps || CurrentVolume <= 0.0f)
				{
					// 페이드아웃 완료
					BossBGMComponent->SetVolumeMultiplier(0.0f);
					GetWorld()->GetTimerManager().ClearTimer(VolumeFadeTimer);
				}
			}
		}, StepDuration, true);
	}
}
