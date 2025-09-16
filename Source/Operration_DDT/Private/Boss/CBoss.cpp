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
#include "AIController.h"

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
	
	// 보스 초기 위치 저장 (레벨에 배치된 위치)
	InitialLocation = GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("CBoss: 초기 위치 저장 완료 - %s"), 
		*InitialLocation.ToString());
}

/**
 * @brief 듀얼 레이어 디졸브 시작
 */
void ACBoss::StartDualLayerDissolve()
{
	if (bIsTransforming)
	{
		UE_LOG(LogTemp, Warning, TEXT("이미 변신 중입니다!"));
		return;
	}

	// 기존 타이머 정리
	if (DissolveTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DissolveTimerHandle);
		UE_LOG(LogTemp, Warning, TEXT("기존 디졸브 타이머 정리"));
	}

	// 디졸브 메테리얼 배열 확인
	if (MetalDissolveMaterials.Num() < 4 || GoldenDissolveMaterials.Num() < 4)
	{
		UE_LOG(LogTemp, Error, TEXT("디졸브 메테리얼이 설정되지 않았습니다! (Metal: %d, Golden: %d)"), 
			MetalDissolveMaterials.Num(), GoldenDissolveMaterials.Num());
		return;
	}

	bIsTransforming = true;
	UE_LOG(LogTemp, Warning, TEXT("🎨 듀얼 레이어 디졸브 시작: Metal → Golden"));

	// 원본 메테리얼 백업
	OriginalMaterials.Empty();
	DynamicMaterials.Empty();

	// 변경할 Element 인덱스들 (0,1,2,6)
	TArray<int32> TargetElements = {0, 1, 2, 6};

	for (int32 i = 0; i < TargetElements.Num(); i++)
	{
		int32 ElementIndex = TargetElements[i];
		
		// 원본 메테리얼 백업
		OriginalMaterials.Add(GetMesh()->GetMaterial(ElementIndex));

		// Metal Dissolve 메테리얼로 교체
		if (MetalDissolveMaterials[i])
		{
			UMaterialInstanceDynamic* DynMat = UMaterialInstanceDynamic::Create(MetalDissolveMaterials[i], this);
			DynamicMaterials.Add(DynMat);
			GetMesh()->SetMaterial(ElementIndex, DynMat);
			
			// 초기 디졸브 값 설정 (완전 보임)
			DynMat->SetScalarParameterValue(TEXT("DissolveOut"), 0.0f);
		}
	}

	// 10초간 부드러운 디졸브 (100번 업데이트)
	DissolveProgress = 0.0f;
	
	GetWorld()->GetTimerManager().SetTimer(DissolveTimerHandle, [this]()
	{
		DissolveProgress += 0.01f; // 1% 증가
		UpdateDualDissolve(DissolveProgress);
		
		if (DissolveProgress >= 1.0f)
		{
			// 완료되면 타이머 자동 정지 (더 이상 실행 안됨)
			GetWorld()->GetTimerManager().ClearTimer(DissolveTimerHandle);
			bIsTransforming = false;
			RestoreToOriginalMaterials();
			UE_LOG(LogTemp, Warning, TEXT("✅ 10초 디졸브 완료!"));
		}
		
	}, 0.1f, true); // 0.1초마다 반복, 하지만 1.0f 되면 자동 정지
}

/**
 * @brief 듀얼 디졸브 진행 상황 업데이트 (완전 동시 진행)
 */
void ACBoss::UpdateDualDissolve(float Progress)
{
	// 변경할 Element 인덱스들 (0,1,2,6)
	TArray<int32> TargetElements = {0, 1, 2, 6};
	
	// 각 Element별로 Metal과 Golden을 동시에 처리
	for (int32 i = 0; i < TargetElements.Num(); i++)
	{
		int32 ElementIndex = TargetElements[i];
		
		// Metal 레이어 업데이트 (사라짐)
		if (i < DynamicMaterials.Num() && DynamicMaterials[i])
		{
			DynamicMaterials[i]->SetScalarParameterValue(TEXT("DissolveOut"), Progress);
		}
		
		// Golden 레이어 동시 시작 (처음부터!)
		if (Progress >= 0.0f && GoldenDissolveMaterials[i])
		{
			// Golden 메테리얼로 교체 (한 번만)
			if (bGoldenApplied.Num() < 4)
			{
				bGoldenApplied = {false, false, false, false};
			}
			
			if (!bGoldenApplied[i])
			{
				UMaterialInstanceDynamic* GoldenDynMat = UMaterialInstanceDynamic::Create(GoldenDissolveMaterials[i], this);
				GetMesh()->SetMaterial(ElementIndex, GoldenDynMat);
				
				// Golden 초기값 (완전 투명)
				GoldenDynMat->SetScalarParameterValue(TEXT("DissolveIn"), 0.0f);
				GoldenDynMat->SetScalarParameterValue(TEXT("EmissiveStrength"), 0.0f);
				
				bGoldenApplied[i] = true;
			}
			
			// Golden 디졸브 진행 (처음부터 동시 진행!)
			float GoldenProgress = Progress; // 0.0~1.0 (Metal과 동일한 진행도)
			
			// Golden 메테리얼 업데이트
			UMaterialInstanceDynamic* GoldenDynMat = Cast<UMaterialInstanceDynamic>(GetMesh()->GetMaterial(ElementIndex));
			if (GoldenDynMat)
			{
				GoldenDynMat->SetScalarParameterValue(TEXT("DissolveIn"), GoldenProgress);
				
				// 빛나는 효과
				float EmissiveStrength = GoldenProgress * 3.0f;
				GoldenDynMat->SetScalarParameterValue(TEXT("EmissiveStrength"), EmissiveStrength);
			}
		}
	}
	
	// 완료 시 배열 리셋
	if (Progress >= 1.0f)
	{
		for (int32 i = 0; i < bGoldenApplied.Num(); i++)
		{
			bGoldenApplied[i] = false;
		}
	}
	
	UE_LOG(LogTemp, Log, TEXT("듀얼 디졸브: %.1f%% (Metal 사라짐, Golden 나타남)"), Progress * 100.0f);
}

/**
 * @brief 기존 메테리얼로 복구 (Golden 버전)
 */
void ACBoss::RestoreToOriginalMaterials()
{
	// 변경할 Element 인덱스들 (0,1,2,6)
	TArray<int32> TargetElements = {0, 1, 2, 6};
	
	// Golden 기본 메테리얼로 복구
	TArray<FString> GoldenMaterialPaths = {
		TEXT("/Game/LHW/Asset/AngelOfDeath/Materials/M_AngelOfDeath_Head_Golden"),    // Element 0
		TEXT("/Game/LHW/Asset/AngelOfDeath/Materials/M_AngelOfDeath_Head_Golden"),    // Element 1  
		TEXT("/Game/LHW/Asset/AngelOfDeath/Materials/M_AngelOfDeath_Armor_Golden"),   // Element 2
		TEXT("/Game/LHW/Asset/AngelOfDeath/Materials/M_AngelOfDeath_Cloth_Golden")    // Element 6
	};
	
	for (int32 i = 0; i < TargetElements.Num(); i++)
	{
		int32 ElementIndex = TargetElements[i];
		
		// 기존 Golden 메테리얼 로드
		UMaterialInterface* OriginalMaterial = LoadObject<UMaterialInterface>(nullptr, *GoldenMaterialPaths[i]);
		
		if (OriginalMaterial)
		{
			GetMesh()->SetMaterial(ElementIndex, OriginalMaterial);
			UE_LOG(LogTemp, Log, TEXT("Element %d를 기존 Golden 메테리얼로 복구: %s"), 
				ElementIndex, *GoldenMaterialPaths[i]);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Element %d 복구 실패: %s 로드 불가"), 
				ElementIndex, *GoldenMaterialPaths[i]);
		}
	}
	
	// 정리
	DynamicMaterials.Empty();
	OriginalMaterials.Empty();
	
	// 멤버 배열 리셋
	for (int32 i = 0; i < bGoldenApplied.Num(); i++)
	{
		bGoldenApplied[i] = false;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("🔄 모든 메테리얼을 기존 Golden 버전으로 복구 완료"));
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
