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
#include "Boss/CBossAIC.h" 
#include "Boss/Component/CBossWeaponComponent.h"
#include "MotionWarpingComponent.h"
#include "Boss/Component/BossEffectComponent.h"
#include "Boss/Component/CBossStatusComponent.h"
#include "Boss/Component/FlyingComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StateTreeComponent.h"
#include "Engine/DamageEvents.h"

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
	TSubclassOf<UAnimInstance> Anim;
	// CHelpers::GetClass<UAnimInstance>(&Anim, AssetPaths::Boss_ANIM);
	GetMesh()->SetAnimInstanceClass(Anim);

	// AI 컨트롤러 클래스 에셋 로드 및 설정
	TSubclassOf<ACBossAIC> AIC;
	CHelpers::GetClass<ACBossAIC>(&AIC, AssetPaths::Boss_AIC);
	AIControllerClass = AIC;

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
	if (BossStatusComponent->BossCurrentStats.CurrentHP<=0)
	{
		UStateTreeComponent* StateTreeComp = GetController()->FindComponentByClass<UStateTreeComponent>();
		StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag("BOSS.State.Dead"));
		BossStatusComponent->BossCurrentStats.CurrentHP=1;
	}
	if (BossStatusComponent->BossCurrentStats.CurrentHP <= 1200 && BossWeaponComponent->GetCurrentWeaponMode()==FGameplayTag::RequestGameplayTag("BOSS.Pase.One"))
	{
		UStateTreeComponent* StateTreeComp = GetController()->FindComponentByClass<UStateTreeComponent>();
		StateTreeComp->SendStateTreeEvent(FGameplayTag::RequestGameplayTag("BOSS.Event.ChangePase"));
	}
	else if (BossStatusComponent->BossCurrentStats.CurrentGroggyGauge >= BossStatusComponent->BossCurrentStats.MaxGroggyGauge)
	{
		if (BossStatusComponent->GetIsPaseChange())return;
		// 현재 스턴 상태가 아닐 때만 스턴 상태로 전환
		if (!BossStatusComponent->IsGroggy) // 또는 적절한 스턴 상태 체크
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
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		if (PointDamageEvent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Bone: %s"), *HitBone.ToString());

			// 🎯 헤드샷 판정
			if (HitBone == FName("head") || HitBone == FName("Head") || HitBone.ToString().Contains("Head"))
			{
				DamageAmount *= 2.0f; // 데미지 배율
			}
		}
	}

	// 기존 보스 상태 처리
	if (!BossStatusComponent->GetIsPaseChange())
	{
		BossStatusComponent->SetDamage(DamageAmount);
		BossStatusComponent->IncreaseAP(DamageAmount);
		BossStatusComponent->IncreaseGroggyGauge(DamageAmount);
		PlayHitMotion(HitBone);
	}


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
}

void ACBoss::PlayHitMotion(FName BoneName)
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
