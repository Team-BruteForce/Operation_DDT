## 보스 AI 및 전투 시스템 구현 가이드

본 가이드는 언리얼 엔진 5 (UE5) 환경에서 보스 AI 및 전투 시스템을 구축하는 데 필요한 상세한 구현 단계를 제공합니다.  각 클래스와 함수에 대한 설명뿐만 아니라, 실제 코드를 포함하여 개발자가 직접 따라하며 구현할 수 있도록 구성되었습니다. 본 가이드라인을 통해 복잡한 보스 AI 및 전투 시스템을 효율적으로 구축하고, 게임의 완성도를 높일 수 있을 것입니다.

**1. 프로젝트 설정**

본격적인 구현에 앞서, 개발 환경을 설정하고 프로젝트 구조를 정의해야 합니다. 이는 개발의 효율성을 높이고, 유지보수를 용이하게 하는 데 중요한 역할을 합니다.

### 1.1 개발 환경 구성

*   **언리얼 엔진 5 설치:** 언리얼 엔진 공식 웹사이트([https://www.unrealengine.com/](https://www.unrealengine.com/))에서 언리얼 엔진 5를 다운로드하여 설치합니다.
*   **Visual Studio 설치:** C++ 코드를 컴파일하고 디버깅하기 위해 Visual Studio를 설치합니다. 언리얼 엔진과 호환되는 버전을 선택해야 합니다.  Visual Studio 설치 시 "게임 개발(C++)" 워크로드를 반드시 포함해야 합니다.
*   **언리얼 엔진 프로젝트 생성:** 언리얼 에디터를 실행하고, "게임" 템플릿을 사용하여 새로운 프로젝트를 생성합니다.  프로젝트 이름은 "BossBattle"과 같이 적절하게 설정합니다.  C++ 프로젝트로 생성해야 합니다.

### 1.2 프로젝트 구조 설정

프로젝트 폴더 구조를 체계적으로 관리하는 것은 매우 중요합니다.  다음과 같은 구조를 권장합니다.

```
BossBattle/
├── Content/
│   ├── Boss/             # 보스 관련 에셋 (모델, 텍스처, 머티리얼, 애니메이션 등)
│   │   ├── Meshes/
│   │   ├── Textures/
│   │   ├── Materials/
│   │   ├── Animations/
│   │   ├── Blueprints/    # 보스 관련 블루프린트
│   │   └── Effects/       # 보스 관련 이펙트
│   ├── UI/               # UI 관련 에셋
│   ├── Audio/            # 오디오 관련 에셋
│   └── ...
├── Source/
│   ├── BossBattle/        # 메인 게임 모듈
│   │   ├── Public/       # 헤더 파일 (.h)
│   │   ├── Private/      # 소스 파일 (.cpp)
│   │   ├── ...
│   └── BossBattleEditor/  # 에디터 모듈 (플러그인)
└── ...
```

### 1.3 의존성 관리

본 프로젝트에서는 특별한 외부 라이브러리 의존성은 없지만, 필요에 따라 다음과 같은 방법으로 의존성을 관리할 수 있습니다.

*   **언리얼 엔진 내장 모듈:** 언리얼 엔진에서 제공하는 다양한 모듈(예: AIModule, GameplayTasksModule)을 사용합니다.  `Build.cs` 파일에 필요한 모듈을 추가하여 의존성을 선언합니다.
*   **NuGet 패키지:** Visual Studio의 NuGet 패키지 관리자를 사용하여 외부 라이브러리를 설치하고 관리합니다.
*   **수동 라이브러리 추가:** 필요한 경우, 외부 라이브러리의 헤더 파일과 라이브러리 파일을 프로젝트에 직접 추가하고, 빌드 설정을 수정합니다.

### 1.4 빌드 설정

`BossBattle.Build.cs` 파일을 열고, 다음과 같이 필요한 모듈을 추가합니다.

```csharp
using UnrealBuildTool;

public class BossBattle : ModuleRules
{
	public BossBattle(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "GameplayTasks", "StateTreeModule", "NavigationSystem", "UMG" }); // UMG 추가
		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
```

**2. 핵심 클래스 구현**

이제 보스 AI 및 전투 시스템의 핵심 클래스를 구현할 차례입니다.  각 클래스의 역할과 주요 기능을 명확히 이해하고, 제공된 정보를 바탕으로 코드를 작성해야 합니다.

본 가이드에서는 `ABossEffect`, `ABossManager`, `ABossProjectileActor`, `ABossProjectileOrb`, `ACBoss`, `ACBossAIC`, `ACBossWeapon`, `AFlySpline`, `AGateOfBabylon`, `AGateOfBabyonProjectile`, `AHolySwordMagic`, `AProjectile_LightSpear`, `FEditorPlugin_DataSyncCommands`, `FEditorPlugin_DataSyncModule`, `FEditorPlugin_DataSyncStyle`, `UAnimNotify_ArmorDissolve`, `UAnimNotify_BeginFlying`, `UAnimNotify_BossWeaponCollision`, `UAnimNotify_ChaseRotation`, `UAnimNotify_DeadDissolve`, `UAnimNotify_DropSwordMagic`, `UAnimNotify_EndFlying`, `UAnimNotify_GateOfBabylonSpawn`, `UAnimNotify_Groggying`, `UAnimNotify_Landing`, `UAnimNotify_LineTraceOnOff`, `UAnimNotify_OrbSpawn`, `UAnimNotify_PaseChangeDissolve`, `UAnimNotify_PlayEffect`, `UAnimNotify_SelectCollisionOnOff`, `UAnimNotify_SpawnLightningSpear`, `UAnimNotifyState_PaseChange`, `UBossAnimInstance`, `UBossEffectExecute`, `UBossEffectManager`, `UBossProjectileComponent`, `UBossStatusWidget`, `UBossTagStructure`, `UCBossDoAction`, `UCBossDoAction_ComBo`, `UCBossEndAttack`, `UCBossEnemyStateTreeEvaluator`, `UCBossEquipment`, `UCBossWeaponAsset`, `UCBossWeaponStuctures`, `UDDTLoadingWidget`, `UDDTMainThemeWidget`, `USTC_CheckPase`, `USTC_DistanceCheck`, `USTC_IsBossActionInProgress`, `USTC_IsBossDistanceGreaterThan`, `USTC_IsBossDistanceLessThan`, `USTC_IsBossInDashRange`, `USTC_IsBossInMeleeRange`, `USTC_IsBossInRangedRange`, `USTC_IsDeadState`, `USTC_ProbabilityCheck`, `USTC_RandLocationCheck`, `USTC_Stun`, `USTC_TimerCheck`, `UTask_BossChase`, `UTask_Dead`, `UTask_FlyKeepingDistance`, `UTask_FlySetLocation`, `UTask_Hovering`, `UTask_IncreaseAP`, `UTask_KeepingDistance`, `UTask_Log`, `UTask_PlayMontage`, `UTask_ResetAP`, `UTask_RotateTowardsPlayer`, `UTask_SetCurrentAction`, `UTask_SetVectorTargetLocation`, `UTask_SideMoveGetLocation`, `UTask_SwitchPase`, `UTask_SwitchState`, `UTask_TargetLocationFeet`, `UTask_TargetLocatonGap` 클래스에 대한 구현 가이드를 제공합니다.

다음 섹션에서는 `ABossEffect` 클래스를 시작으로, 각 클래스의 구현 과정을 상세하게 설명합니다.  각 클래스별로 헤더 파일(.h)과 소스 파일(.cpp)에 대한 코드 예제를 제공하며, 주요 메서드와 프로퍼티에 대한 설명을 포함합니다.


알겠습니다. 이전 내용을 바탕으로 `ABossEffect` 클래스부터 시작하여 각 클래스의 구현 과정을 상세하게 설명하는 가이드를 작성하겠습니다.

## 1. 프로젝트 설정

본 가이드에서는 언리얼 엔진 5 (UE5) 환경을 기준으로 설명합니다.

### 1.1 개발 환경 구성

1.  **언리얼 엔진 설치**: 언리얼 엔진 공식 웹사이트 ([https://www.unrealengine.com/](https://www.unrealengine.com/))에서 언리얼 엔진 5를 다운로드하여 설치합니다.
2.  **Visual Studio 설치**: C++ 코드를 컴파일하기 위해 Visual Studio 2019 이상 버전을 설치합니다.  언리얼 엔진 설치 시 Visual Studio 관련 구성 요소를 함께 설치하는 것을 권장합니다.
3.  **에디터 설정**: 언리얼 에디터를 실행하고, C++ 프로젝트를 생성하거나 기존 프로젝트를 엽니다.  `편집 > 편집기 개인설정 > 일반 > 소스 코드`에서 Visual Studio를 소스 코드 에디터로 설정합니다.

### 1.2 프로젝트 구조 설정

본 가이드에서는 다음과 같은 폴더 구조를 권장합니다.

```
MyProject/
├── Content/
│   ├── Blueprints/
│   ├── Materials/
│   ├── Meshes/
│   ├── Textures/
│   └── ...
├── Source/
│   ├── MyProject/
│   │   ├── Public/
│   │   │   ├── Boss/
│   │   │   │   ├── ABossEffect.h
│   │   │   │   ├── ... (다른 보스 관련 헤더 파일들)
│   │   │   ├── ... (다른 헤더 파일들)
│   │   ├── Private/
│   │   │   ├── Boss/
│   │   │   │   ├── ABossEffect.cpp
│   │   │   │   ├── ... (다른 보스 관련 소스 파일들)
│   │   │   ├── ... (다른 소스 파일들)
│   │   ├── MyProject.Build.cs
│   │   └── MyProject.h
│   └── MyProjectEditor.Target.cs
└── MyProject.uproject
```

`Boss` 폴더 내에 보스 관련 클래스들을 관리하여 코드를 체계적으로 구성합니다.

### 1.3 의존성 관리

`MyProject.Build.cs` 파일에 필요한 모듈을 추가하여 의존성을 관리합니다.  예를 들어, AI 관련 기능을 사용하려면 다음과 같이 `AIModule`을 추가합니다.

```csharp
using UnrealBuildTool;

public class MyProject : ModuleRules
{
    public MyProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "GameplayTasks", "NavigationSystem" }); // AIModule 추가

        PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
```

### 1.4 빌드 설정

언리얼 에디터에서 `파일 > C++ 클래스 새로 만들기`를 통해 C++ 클래스를 생성하면 자동으로 빌드 설정이 업데이트됩니다.  만약 수동으로 빌드 설정을 변경해야 하는 경우, `MyProject.Build.cs` 파일을 수정하고 언리얼 에디터에서 프로젝트를 다시 빌드합니다.  `빌드 > 솔루션 파일 생성`을 통해 Visual Studio 솔루션 파일을 업데이트할 수도 있습니다.

## 2. 핵심 클래스 구현

이제 각 클래스의 구현 과정을 상세하게 설명하겠습니다. `ABossEffect` 클래스부터 시작합니다.

### 2.1 `ABossEffect` 클래스 구현

`ABossEffect` 클래스는 이펙트 활성화 및 관리를 담당하는 액터 클래스입니다.

#### 2.1.1 헤더 파일 (ABossEffect.h)

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "ABossEffect.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectFinished, ABossEffect*, Effect);

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API ABossEffect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossEffect();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 이펙트 완료 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Effect")
	FOnEffectFinished OnEffectFinished;

	// 이펙트 활성화
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ActivateEffect();

	// 소켓에 붙여서 이펙트 활성화
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ActivateEffectAttachedToSocket(USceneComponent* Parent, FName SocketName);

	// 이펙트 비활성화
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void DeactivateEffect();

	// 이펙트가 활성화되어 있는지 확인
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Effect")
	bool IsActive() const;

	// 현재 이펙트 태그 반환
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Effect")
	FGameplayTag GetCurrentEffectTag() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 이펙트를 보스에 어태치
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void AttachToBoss(AActor* BossActor, FName SocketName);

	// 소켓에 이펙트 붙이기
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void AttachToSocket(USceneComponent* Parent, FName SocketName);

	// 이펙트를 월드에 배치
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlaceInWorld(FVector Location, FRotator Rotation);

private:
	// 루트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComponent;

	// 스태틱 메시 컴포넌트 (선택 사항)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMeshComponent;

	// 파티클 시스템 컴포넌트 (선택 사항)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UParticleSystemComponent* ParticleSystemComponent;

	// 이펙트 활성화 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	bool bIsActive;

	// 현재 이펙트 태그
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	FGameplayTag CurrentEffectTag;

public:
	// 이펙트 태그 설정
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void SetCurrentEffectTag(FGameplayTag Tag);
};
```

#### 2.1.2 소스 파일 (ABossEffect.cpp)

```c++
#include "Boss/ABossEffect.h"

// Sets default values
ABossEffect::ABossEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트 컴포넌트 생성
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComponent);

	// 스태틱 메시 컴포넌트 생성
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetVisibility(false); // 초기에는 숨김

	// 파티클 시스템 컴포넌트 생성
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);
	ParticleSystemComponent->SetAutoActivate(false); // 초기에는 자동 활성화 비활성화
	ParticleSystemComponent->SetVisibility(false); // 초기에는 숨김

	bIsActive = false;
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

}

void ABossEffect::ActivateEffect()
{
	bIsActive = true;

	// 스태틱 메시 컴포넌트 활성화
	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetVisibility(true);
	}

	// 파티클 시스템 컴포넌트 활성화
	if (ParticleSystemComponent)
	{
		ParticleSystemComponent->SetVisibility(true);
		ParticleSystemComponent->Activate(true);
	}

	// 이펙트 종료 후 비활성화 (예시)
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossEffect::DeactivateEffect, 3.0f, false); // 3초 후 비활성화
}

void ABossEffect::ActivateEffectAttachedToSocket(USceneComponent* Parent, FName SocketName)
{
	if (!Parent) return;

	AttachToSocket(Parent, SocketName);
	ActivateEffect();
}

void ABossEffect::DeactivateEffect()
{
	bIsActive = false;

	// 스태틱 메시 컴포넌트 비활성화
	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetVisibility(false);
	}

	// 파티클 시스템 컴포넌트 비활성화
	if (ParticleSystemComponent)
	{
		ParticleSystemComponent->SetVisibility(false);
		ParticleSystemComponent->Deactivate();
	}

	// 이펙트 종료 델리게이트 호출
	OnEffectFinished.Broadcast(this);
}

bool ABossEffect::IsActive() const
{
	return bIsActive;
}

FGameplayTag ABossEffect::GetCurrentEffectTag() const
{
	return CurrentEffectTag;
}

void ABossEffect::AttachToBoss(AActor* BossActor, FName SocketName)
{
	if (!BossActor) return;

	AttachToSocket(BossActor->GetRootComponent(), SocketName);
}

void ABossEffect::AttachToSocket(USceneComponent* Parent, FName SocketName)
{
	if (!Parent) return;

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	AttachToComponent(Parent, AttachmentRules, SocketName);
}

void ABossEffect::PlaceInWorld(FVector Location, FRotator Rotation)
{
	SetActorLocationAndRotation(Location, Rotation);
}

void ABossEffect::SetCurrentEffectTag(FGameplayTag Tag)
{
	CurrentEffectTag = Tag;
}
```

#### 2.1.3 주요 메서드 및 프로퍼티 설명

*   **`RootComponent`**: 액터의 루트 컴포넌트입니다.  모든 컴포넌트는 이 컴포넌트에 Attach됩니다.
*   **`StaticMeshComponent`**: 이펙트를 표현하기 위한 스태틱 메시 컴포넌트입니다.
*   **`ParticleSystemComponent`**: 이펙트를 표현하기 위한 파티클 시스템 컴포넌트입니다.
*   **`bIsActive`**: 이펙트가 현재 활성화되어 있는지 여부를 나타내는 불리언 변수입니다.
*   **`CurrentEffectTag`**: 현재 이펙트의 태그를 저장합니다.  이 태그를 사용하여 이펙트를 식별하고 관리할 수 있습니다.
*   **`OnEffectFinished`**: 이펙트가 종료되었을 때 호출되는 델리게이트입니다.
*   **`ActivateEffect()`**: 이펙트를 활성화합니다. 스태틱 메시와 파티클 시스템의 visibility를 true로 설정하고, 파티클 시스템을 활성화합니다.
*   **`ActivateEffectAttachedToSocket()`**:  지정된 소켓에 이펙트를 부착하고 활성화합니다.
*   **`DeactivateEffect()`**: 이펙트를 비활성화합니다. 스태틱 메시와 파티클 시스템의 visibility를 false로 설정하고, 파티클 시스템을 비활성화합니다.  `OnEffectFinished` 델리게이트를 호출하여 이펙트 종료를 알립니다.
*   **`IsActive()`**: 이펙트가 활성화되어 있는지 확인합니다.
*   **`GetCurrentEffectTag()`**: 현재 이펙트의 태그를 반환합니다.
*   **`AttachToBoss()`**: 보스 액터의 특정 소켓에 이펙트를 부착합니다.
*   **`AttachToSocket()`**: 지정된 씬 컴포넌트의 특정 소켓에 이펙트를 부착합니다.
*   **`PlaceInWorld()`**: 지정된 위치와 회전으로 이펙트를 월드에 배치합니다.
*   **`SetCurrentEffectTag()`**: 현재 이펙트의 태그를 설정합니다.

#### 2.1.4 초기화 및 정리

*   `ABossEffect()` 생성자에서 컴포넌트를 생성하고 초기화합니다.
*   `BeginPlay()` 함수에서 게임 시작 시 필요한 초기화 작업을 수행합니다.
*   `DeactivateEffect()` 함수에서 이펙트 종료 시 필요한 정리 작업을 수행합니다.

#### 2.1.5 실제 사용 예제

다음은 `ABossEffect` 클래스를 사용하는 예제입니다.

```c++
// 보스 액터 클래스 (예시)
#include "Boss/ABossEffect.h"
#include "GameFramework/Actor.h"

void AMyBoss::SpawnAndActivateEffect(FGameplayTag EffectTag, FVector Location)
{
    // 이펙트 액터 스폰
    ABossEffect* EffectActor = GetWorld()->SpawnActor<ABossEffect>(ABossEffect::StaticClass());

    if (EffectActor)
    {
        // 이펙트 태그 설정
        EffectActor->SetCurrentEffectTag(EffectTag);

        // 위치 설정
        EffectActor->PlaceInWorld(Location, FRotator::ZeroRotator);

        // 이펙트 활성화
        EffectActor->ActivateEffect();

        // 이펙트 종료 콜백 설정 (예시)
        EffectActor->OnEffectFinished.AddDynamic(this, &AMyBoss::OnEffectActorFinished);
    }
}

void AMyBoss::OnEffectActorFinished(ABossEffect* Effect)
{
    // 이펙트 종료 처리
    if (Effect)
    {
        Effect->Destroy();
    }
}
```

이 코드는 `AMyBoss` 클래스에서 `SpawnAndActivateEffect` 함수를 통해 이펙트 액터를 스폰하고, 위치를 설정한 후 활성화하는 방법을 보여줍니다.  또한, 이펙트가 종료되었을 때 호출되는 `OnEffectActorFinished` 콜백 함수를 등록하여 이펙트 액터를 제거합니다.

## 3. 시스템 통합

이어서 `ABossManager`, `ABossProjectileActor`, `ABossProjectileOrb`, `ACBoss`, `ACBossAIC`, `ACBossWeapon` 클래스에 대한 구현 가이드를 제공하겠습니다. 각 클래스별로 헤더 파일(.h)과 소스 파일(.cpp)에 대한 코드 예제를 제공하며, 주요 메서드와 프로퍼티에 대한 설명을 포함할 것입니다.

이어서 `ABossManager` 클래스 구현 가이드를 작성하겠습니다.


## 3. 시스템 통합: `ABossManager` 클래스 구현 가이드

이전에는 이펙트 액터를 스폰하고 활성화하는 방법을 다루었습니다. 이제 보스 전체를 관리하는 핵심 클래스인 `ABossManager`의 구현 가이드를 제공하겠습니다. `ABossManager`는 보스의 스폰, 초기화, 상태 관리, 그리고 중요한 이벤트들을 처리하는 역할을 담당합니다.

### 3.1. `ABossManager` 클래스 설계

`ABossManager`는 게임 월드에 존재하는 보스를 관리하는 액터 클래스입니다.  주요 기능은 다음과 같습니다.

*   **보스 스폰 및 초기화:** 게임 시작 시 또는 특정 조건 발생 시 보스를 월드에 스폰하고 초기 상태로 설정합니다.
*   **보스 상태 관리:** 보스의 HP, 페이즈, 액션 상태 등을 추적하고 관리합니다.
*   **이벤트 처리:** 보스 관련 이벤트(예: 문 열림, 보스 사망)를 처리하고 다른 시스템에 알립니다.
*   **보스 완전 초기화:** 보스의 상태, 컴포넌트, StateTree 등을 초기 상태로 되돌립니다.

### 3.2. `ABossManager` 클래스 구현

다음은 `ABossManager` 클래스의 헤더 파일(`BossManager.h`)과 소스 파일(`BossManager.cpp`)에 대한 코드 예제입니다.

#### 3.2.1. `BossManager.h`

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossManager.generated.h"

UCLASS()
class MYPROJECT_API ABossManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 보스 완전 초기화 (메인 함수)
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	void ResetBossCompletely();

	// 문 열기
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	void OpenDoor();

	// 월드에서 보스 찾기
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	class ACBoss* FindBossInWorld();

	// 각 컴포넌트별 초기화 실행
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	void ResetAllBossComponents();

	// StateTree 완전 리스타트
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	void ResetBossStateTree();

	// 콜리전 오버랩 시작 이벤트
	UFUNCTION()
	void OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// 스폰할 보스 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
	TSubclassOf<class ACBoss> BossClass;

	// 스폰된 보스 액터
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss")
	class ACBoss* SpawnedBoss;

	// 보스 스폰 위치
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
	FTransform BossSpawnTransform;

	// 문 액터 (예: AGateOfBabylon)
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Door")
	class AActor* DoorActor;

	// 트리거 박스 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	// 보스 스탯 데이터 테이블
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Data")
	class UDataTable* BossStatsDataTable;

public:
	// 보스 스폰 함수 (C++에서 호출)
	ACBoss* SpawnBoss();
};
```

#### 3.2.2. `BossManager.cpp`

```c++
#include "BossManager.h"
#include "CBoss.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ABossManager::ABossManager()
{
	PrimaryActorTick.bCanEverTick = true;

	// 트리거 박스 컴포넌트 생성
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("OverlapAll"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABossManager::OnTriggerBoxOverlapBegin);
}

void ABossManager::BeginPlay()
{
	Super::BeginPlay();

	// 보스 스폰
	SpawnedBoss = SpawnBoss();

	if (SpawnedBoss)
	{
		UE_LOG(LogTemp, Warning, TEXT("Boss Spawned Successfully!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Spawn Boss!"));
	}
}

void ABossManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

ACBoss* ABossManager::SpawnBoss()
{
	if (BossClass)
	{
		// 월드에 보스 스폰
		ACBoss* Boss = GetWorld()->SpawnActor<ACBoss>(BossClass, BossSpawnTransform);
		return Boss;
	}

	return nullptr;
}

void ABossManager::ResetBossCompletely()
{
	UE_LOG(LogTemp, Warning, TEXT("ResetBossCompletely Called"));

	// 1. 보스 찾기
	ACBoss* Boss = FindBossInWorld();
	if (!Boss)
	{
		UE_LOG(LogTemp, Error, TEXT("No Boss Found in World!"));
		return;
	}

	// 2. 각 컴포넌트별 초기화
	ResetAllBossComponents();

	// 3. StateTree 리스타트
	ResetBossStateTree();

	// 4. HP 리셋 (예시)
	// Boss->HPUpdate(); // HPUpdate 함수가 있다면 호출

	UE_LOG(LogTemp, Warning, TEXT("Boss Reset Complete"));
}

void ABossManager::OpenDoor()
{
	if (DoorActor)
	{
		// 문 액터에 OpenDoor 함수가 있다면 호출 (예시)
		// Cast<AGateOfBabylon>(DoorActor)->ActivateGate();
		UE_LOG(LogTemp, Warning, TEXT("Opening Door!"));
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DoorActor is not set!"));
	}
}

ACBoss* ABossManager::FindBossInWorld()
{
	// 월드에서 ACBoss 타입의 액터 찾기
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACBoss::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		// 첫 번째로 찾은 보스 반환
		return Cast<ACBoss>(FoundActors[0]);
	}

	return nullptr;
}

void ABossManager::ResetAllBossComponents()
{
	// 1. 보스 찾기
	ACBoss* Boss = FindBossInWorld();
	if (!Boss)
	{
		UE_LOG(LogTemp, Error, TEXT("No Boss Found in World!"));
		return;
	}

	// 2. 각 컴포넌트 초기화 (예시)
	// Boss->FindComponentByClass<UBossStateComponent>()->ResetState();
	// Boss->FindComponentByClass<UBossMovementComponent>()->ResetMovementSystem();

	UE_LOG(LogTemp, Warning, TEXT("All Boss Components Reset"));
}

void ABossManager::ResetBossStateTree()
{
	// 1. 보스 찾기
	ACBoss* Boss = FindBossInWorld();
	if (!Boss)
	{
		UE_LOG(LogTemp, Error, TEXT("No Boss Found in World!"));
		return;
	}

	// 2. AIController 찾기
	AAIController* AIController = Cast<AAIController>(Boss->GetController());
	if (!AIController)
	{
		UE_LOG(LogTemp, Error, TEXT("No AIController Found!"));
		return;
	}

	// 3. StateTree 컴포넌트 찾기 (가정)
	// UStateTreeComponent* StateTreeComponent = AIController->FindComponentByClass<UStateTreeComponent>();
	// if (StateTreeComponent)
	// {
	// 	StateTreeComponent->RestartTree();
	// 	UE_LOG(LogTemp, Warning, TEXT("Boss StateTree Restarted"));
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("No StateTreeComponent Found!"));
	// }

	UE_LOG(LogTemp, Warning, TEXT("Boss StateTree Reset"));
}

void ABossManager::OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어가 트리거 박스에 진입했을 때 이벤트 처리 (예시)
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (OtherActor == PlayerPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Entered Boss Area!"));
		OpenDoor();
		// 추가적인 로직 (보스 활성화 등)
	}
}
```

### 3.3. 주요 메서드 설명

*   **`ABossManager()`**: 생성자입니다. 트리거 박스 컴포넌트를 생성하고 초기화합니다.
*   **`BeginPlay()`**: 액터가 스폰되었을 때 호출됩니다. 보스를 스폰합니다.
*   **`Tick()`**: 매 프레임마다 호출됩니다. 필요에 따라 보스 상태를 업데이트하거나 이벤트를 처리할 수 있습니다.
*   **`ResetBossCompletely()`**: 보스를 초기 상태로 되돌립니다. 컴포넌트 초기화, StateTree 리스타트 등을 포함합니다.
*   **`OpenDoor()`**: 문 액터를 활성화합니다.
*   **`FindBossInWorld()`**: 월드에서 보스 액터를 찾습니다.
*   **`ResetAllBossComponents()`**: 보스의 모든 컴포넌트를 초기화합니다.
*   **`ResetBossStateTree()`**: 보스의 StateTree를 리스타트합니다.
*   **`OnTriggerBoxOverlapBegin()`**: 플레이어가 트리거 박스에 진입했을 때 호출됩니다. 문을 열거나 보스를 활성화하는 등의 이벤트를 처리합니다.
*   **`SpawnBoss()`**:  `BossClass`에 지정된 보스 액터를 `BossSpawnTransform` 위치에 스폰합니다.

### 3.4. 프로퍼티 설정

*   **`BossClass`**: 스폰할 보스 액터의 클래스를 지정합니다. 에디터에서 설정할 수 있습니다.
*   **`SpawnedBoss`**: 스폰된 보스 액터의 인스턴스를 저장합니다.
*   **`BossSpawnTransform`**: 보스 액터의 스폰 위치와 회전을 지정합니다. 에디터에서 설정할 수 있습니다.
*   **`DoorActor`**: 문 액터를 지정합니다. 에디터에서 설정할 수 있습니다.
*   **`TriggerBox`**: 플레이어 감지를 위한 트리거 박스 컴포넌트입니다.
*	**`BossStatsDataTable`**: 보스 스탯 데이터 테이블을 지정합니다. 에디터에서 설정할 수 있습니다.

### 3.5. 실제 사용 예제

1.  **`ABossManager` 액터 배치:** 레벨 에디터에서 `ABossManager` 액터를 씬에 배치합니다.
2.  **프로퍼티 설정:** `ABossManager` 액터의 디테일 패널에서 `BossClass`, `BossSpawnTransform`, `DoorActor`, `BossStatsDataTable` 등의 프로퍼티를 설정합니다.
3.  **트리거 박스 조정:** `TriggerBox` 컴포넌트의 크기와 위치를 조정하여 플레이어 감지 영역을 설정합니다.
4.  **보스 스폰:** 게임 시작 시 `ABossManager`가 자동으로 보스를 스폰합니다.
5.  **보스 초기화:** `ResetBossCompletely` 함수를 호출하여 보스를 초기 상태로 되돌릴 수 있습니다.
6.  **문 열기:** 플레이어가 트리거 박스에 진입하면 `OpenDoor` 함수가 호출되어 문이 열립니다.

### 3.6. 다음 단계

다음으로는 `ABossProjectileActor` 클래스의 구현 가이드를 제공하여, 보스의 공격 패턴을 구현하는 방법을 자세히 설명하겠습니다.


## 4. `ABossProjectileActor` 클래스 구현 가이드

이제 보스의 공격 패턴을 구현하는 데 핵심적인 역할을 하는 `ABossProjectileActor` 클래스의 구현 가이드를 제공합니다. 이 클래스는 보스가 발사하는 투사체의 기본 동작을 정의하며, 투사체의 발사, 효과, 충돌 처리 등을 담당합니다.

### 4.1. 프로젝트 설정 (이전 단계와 동일)

이전 단계에서 설정한 프로젝트 환경을 그대로 사용합니다.  새로운 프로젝트를 시작하는 경우, 3.1 단계를 참고하여 개발 환경을 구성하고 프로젝트 구조를 설정하십시오.

### 4.2. 핵심 클래스 구현: `ABossProjectileActor`

#### 4.2.1. 클래스 설계 및 구현

`ABossProjectileActor` 클래스는 `AActor` 클래스를 상속받아 구현됩니다. 투사체의 이동, 충돌, 효과 재생 등의 기능을 담당하는 컴포넌트들을 포함합니다.

```c++
// BossProjectileActor.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BossProjectileActor.generated.h"

UCLASS()
class YOUR_PROJECT_API ABossProjectileActor : public AActor
{
	GENERATED_BODY()

public:
	// 생성자
	ABossProjectileActor();

	// 매 프레임 호출
	virtual void Tick(float DeltaTime) override;

	// 투사체 발사 함수
	UFUNCTION(BlueprintCallable, Category = "BossProjectile")
	void FireProjectile(const FVector& Direction);

	// 특정 위치로 투사체 발사 함수
	UFUNCTION(BlueprintCallable, Category = "BossProjectile")
	void FireProjectileToLocation(const FVector& TargetLocation);

protected:
	// BeginPlay 오버라이드
	virtual void BeginPlay() override;

	// 투사체 충돌 시 호출되는 함수
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	// 스폰 이펙트 재생
	UFUNCTION(BlueprintNativeEvent, Category = "BossProjectile")
	void PlaySpawnEffect();
	virtual void PlaySpawnEffect_Implementation();

	// 파괴 이펙트 재생
	UFUNCTION(BlueprintNativeEvent, Category = "BossProjectile")
	void PlayDestroyEffect();
	virtual void PlayDestroyEffect_Implementation();

public:
	// 투사체 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	// 충돌체
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

	// 투사체 이동 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	// 스폰 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* SpawnEffect;

	// 파괴 이펙트
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* DestroyEffect;

	// 데미지량
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float DamageAmount;
};
```

```c++
// BossProjectileActor.cpp
#include "BossProjectileActor.h"
#include "Kismet/GameplayStatics.h"

ABossProjectileActor::ABossProjectileActor()
{
	PrimaryActorTick.bCanEverTick = true;

	// 메시 컴포넌트 생성 및 설정
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// 충돌체 컴포넌트 생성 및 설정
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetSphereRadius(32.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABossProjectileActor::OnProjectileHit);

	// 투사체 이동 컴포넌트 생성 및 설정
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	// 초기 데미지량 설정
	DamageAmount = 10.0f;
}

void ABossProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	// 초기에는 투사체를 비활성화
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

void ABossProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossProjectileActor::FireProjectile(const FVector& Direction)
{
	// 투사체 활성화
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	// 방향 설정 및 발사
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;

	// 스폰 이펙트 재생
	PlaySpawnEffect();
}

void ABossProjectileActor::FireProjectileToLocation(const FVector& TargetLocation)
{
	// 투사체 활성화
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);

	// 방향 계산
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();

	// 방향 설정 및 발사
	ProjectileMovement->Velocity = Direction * ProjectileMovement->InitialSpeed;

	// 스폰 이펙트 재생
	PlaySpawnEffect();
}

void ABossProjectileActor::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 데미지 적용 로직 (예시)
	if (OtherActor && OtherActor != this)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, DamageAmount, NormalImpulse, Hit, GetInstigatorController(), this, UDamageType::StaticClass());
	}

	// 파괴 이펙트 재생
	PlayDestroyEffect();

	// 투사체 비활성화
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);

	// 투사체 이동 정지
	ProjectileMovement->Velocity = FVector::ZeroVector;

	// 액터 위치 초기화 (풀링 시스템 사용 시)
	SetActorLocation(FVector::ZeroVector);
}

void ABossProjectileActor::PlaySpawnEffect_Implementation()
{
	if (SpawnEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, GetActorTransform(), true);
	}
}

void ABossProjectileActor::PlayDestroyEffect_Implementation()
{
	if (DestroyEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyEffect, GetActorTransform(), true);
	}
}
```

#### 4.2.2. 주요 메서드 구현

*   **`FireProjectile(const FVector& Direction)`**: 주어진 방향으로 투사체를 발사합니다.  `ProjectileMovement` 컴포넌트의 `Velocity`를 설정하여 투사체를 이동시킵니다.
*   **`FireProjectileToLocation(const FVector& TargetLocation)`**: 특정 위치를 향해 투사체를 발사합니다.  타겟 위치와 현재 위치를 이용하여 방향을 계산하고, `FireProjectile` 함수를 호출합니다.
*   **`OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)`**: 투사체가 다른 액터와 충돌했을 때 호출됩니다. 충돌한 액터에 데미지를 적용하고, 파괴 이펙트를 재생하며, 투사체를 비활성화합니다.
*   **`PlaySpawnEffect_Implementation()`**: 투사체가 발사될 때 스폰 이펙트를 재생합니다.
*   **`PlayDestroyEffect_Implementation()`**: 투사체가 파괴될 때 파괴 이펙트를 재생합니다.

#### 4.2.3. 프로퍼티 설정

*   **`ProjectileMesh`**: 투사체의 메시를 설정합니다. 에디터에서 원하는 스태틱 메시를 할당할 수 있습니다.
*   **`CollisionComponent`**: 투사체의 충돌 영역을 설정합니다.  `SphereComponent`를 사용하여 구 형태의 충돌 영역을 정의합니다.
*   **`ProjectileMovement`**: 투사체의 이동 속도, 가속도 등을 설정합니다.  `ProjectileMovementComponent`를 사용하여 투사체의 물리적인 움직임을 제어합니다.
*   **`SpawnEffect`**: 투사체가 생성될 때 재생할 파티클 시스템을 설정합니다.
*   **`DestroyEffect`**: 투사체가 파괴될 때 재생할 파티클 시스템을 설정합니다.
*   **`DamageAmount`**: 투사체가 충돌 시 가할 데미지 양을 설정합니다.

#### 4.2.4. 초기화 및 정리

*   **`BeginPlay()`**: 액터가 생성될 때 호출됩니다.  초기에는 투사체를 비활성화하고, 풀링 시스템을 사용하는 경우 액터를 풀에 반환합니다.
*   **`OnProjectileHit()`**: 충돌 발생 시 투사체를 비활성화하고, 풀링 시스템을 사용하는 경우 액터를 풀에 반환합니다.

#### 4.2.5. 실제 사용 예제

1.  **`ABossProjectileActor` 액터 생성:** 콘텐츠 브라우저에서 `ABossProjectileActor` 클래스를 상속받는 블루프린트 클래스를 생성합니다.
2.  **메시 및 효과 설정:** 생성된 블루프린트 클래스에서 `ProjectileMesh`, `SpawnEffect`, `DestroyEffect` 프로퍼티에 원하는 메시와 파티클 시스템을 할당합니다.
3.  **발사 함수 호출:** 보스 AI 또는 애니메이션 이벤트에서 `FireProjectile` 또는 `FireProjectileToLocation` 함수를 호출하여 투사체를 발사합니다.
4.  **충돌 처리:** 투사체가 충돌하면 `OnProjectileHit` 함수가 호출되어 데미지를 적용하고, 파괴 이펙트를 재생합니다.

### 4.3. 시스템 통합 (다음 단계에서 다룰 내용)

다음 단계에서는 `ABossProjectileActor` 클래스를 보스 시스템에 통합하는 방법을 설명합니다.  보스 AI에서 투사체를 발사하고, 풀링 시스템을 사용하여 투사체를 효율적으로 관리하는 방법을 다룹니다.


## 3. 시스템 통합

이제까지 개별적인 액터와 컴포넌트들을 구현했습니다. 이제 이들을 통합하여 보스 시스템을 완성하는 단계입니다. 이 단계에서는 보스 AI가 투사체를 발사하고, 이펙트를 재생하며, 풀링 시스템을 통해 리소스를 효율적으로 관리하는 방법을 다룹니다.

### 3.1. 보스 AI와 투사체 발사 연동

보스 AI는 StateTree를 사용하여 구현됩니다. StateTree의 Task에서 투사체를 발사하는 기능을 호출하도록 설정합니다. `UBossProjectileComponent`의 `ShotProjectile` 또는 `ShotProjectileToLocation` 함수를 호출하여 투사체를 발사할 수 있습니다.

**단계별 구현:**

1.  **StateTree Task 생성:** `UTask_PlayMontage`와 유사하게, `UBossProjectileComponent`의 투사체 발사 함수를 호출하는 새로운 StateTree Task를 생성합니다. `UTask_FireProjectile`이라는 이름으로 새로운 C++ 클래스를 생성하고, `UStateTreeTask`를 상속받습니다.

    ```c++
    // Task_FireProjectile.h
    #pragma once

    #include "CoreMinimal.h"
    #include "StateTreeTask.h"
    #include "Task_FireProjectile.generated.h"

    UCLASS()
    class YOURPROJECT_API UTask_FireProjectile : public UStateTreeTask
    {
        GENERATED_BODY()

    public:
        UTask_FireProjectile();

        virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
        FName SocketName;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
        TSubclassOf<ABossProjectileActor> ProjectileClass;
    };

    // Task_FireProjectile.cpp
    #include "Task_FireProjectile.h"
    #include "CBoss.h"
    #include "BossProjectileComponent.h"
    #include "Components/SkeletalMeshComponent.h"

    UTask_FireProjectile::UTask_FireProjectile()
    {
        // Task가 항상 실행되도록 설정
        bShouldCallTick = false;
    }

    EStateTreeRunStatus UTask_FireProjectile::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
    {
        ACBoss* Boss = Cast<ACBoss>(Context.GetOwner());
        if (Boss == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("UTask_FireProjectile::EnterState - Boss is null"));
            return EStateTreeRunStatus::Failed;
        }

        UBossProjectileComponent* ProjectileComponent = Boss->FindComponentByClass<UBossProjectileComponent>();
        if (ProjectileComponent == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("UTask_FireProjectile::EnterState - ProjectileComponent is null"));
            return EStateTreeRunStatus::Failed;
        }

        USkeletalMeshComponent* MeshComponent = Boss->GetMesh();
        if (MeshComponent == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("UTask_FireProjectile::EnterState - MeshComponent is null"));
            return EStateTreeRunStatus::Failed;
        }

        FVector SocketLocation = MeshComponent->GetSocketLocation(SocketName);

        if (ProjectileClass == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("UTask_FireProjectile::EnterState - ProjectileClass is null"));
            return EStateTreeRunStatus::Failed;
        }

        ProjectileComponent->ShotProjectileToLocation(ProjectileClass, SocketLocation, Boss->GetActorRotation());

        return EStateTreeRunStatus::Succeeded;
    }
    ```

2.  **StateTree에 Task 추가:** 보스의 StateTree 에디터에서 새로운 Task를 추가하고, 방금 생성한 `UTask_FireProjectile` 클래스를 선택합니다.

3.  **Task 설정:** StateTree 에디터에서 Task의 프로퍼티를 설정합니다. 발사할 투사체의 `ProjectileClass`와 발사 위치의 `SocketName`을 설정합니다.

4.  **애니메이션 연동:** 몽타주 애니메이션의 특정 시점에 투사체를 발사하도록 애니메이션 노티파이를 추가합니다.  애니메이션 노티파이에서 StateTree의 해당 State로 전환되도록 설정합니다.  (또는, `UTask_PlayMontage` Task에서 직접 투사체를 발사할 수도 있습니다.)

### 3.2. 이펙트 재생 연동

`UBossEffectComponent`를 사용하여 투사체 발사 시 이펙트를 재생합니다. 투사체가 발사되는 시점에 `PlayEffect` 또는 `PlayEffectAttachedToSocket` 함수를 호출하여 이펙트를 재생할 수 있습니다.

**단계별 구현:**

1.  **`UTask_FireProjectile` 수정:** `UTask_FireProjectile` Task에 이펙트 재생 기능을 추가합니다.

    ```c++
    // Task_FireProjectile.h (수정)
    #pragma once

    #include "CoreMinimal.h"
    #include "StateTreeTask.h"
    #include "Task_FireProjectile.generated.h"

    UCLASS()
    class YOURPROJECT_API UTask_FireProjectile : public UStateTreeTask
    {
        GENERATED_BODY()

    public:
        UTask_FireProjectile();

        virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

    protected:
        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
        FName SocketName;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
        TSubclassOf<ABossProjectileActor> ProjectileClass;

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
        FName EffectTag; // 이펙트 태그 추가
    };

    // Task_FireProjectile.cpp (수정)
    #include "Task_FireProjectile.h"
    #include "CBoss.h"
    #include "BossProjectileComponent.h"
    #include "BossEffectComponent.h" // BossEffectComponent 포함
    #include "Components/SkeletalMeshComponent.h"

    UTask_FireProjectile::UTask_FireProjectile()
    {
        // Task가 항상 실행되도록 설정
        bShouldCallTick = false;
    }

    EStateTreeRunStatus UTask_FireProjectile::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
    {
        ACBoss* Boss = Cast<ACBoss>(Context.GetOwner());
        if (Boss == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("UTask_FireProjectile::EnterState - Boss is null"));
            return EStateTreeRunStatus::Failed;
        }

        UBossProjectileComponent* ProjectileComponent = Boss->FindComponentByClass<UBossProjectileComponent>();
        if (ProjectileComponent == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("UTask_FireProjectile::EnterState - ProjectileComponent is null"));
            return EStateTreeRunStatus::Failed;
        }

        UBossEffectComponent* EffectComponent = Boss->FindComponentByClass<UBossEffectComponent>();
        if (EffectComponent == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("UTask_FireProjectile::EnterState - EffectComponent is null"));
            return EStateTreeRunStatus::Failed;
        }

        USkeletalMeshComponent* MeshComponent = Boss->GetMesh();
        if (MeshComponent == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("UTask_FireProjectile::EnterState - MeshComponent is null"));
            return EStateTreeRunStatus::Failed;
        }

        FVector SocketLocation = MeshComponent->GetSocketLocation(SocketName);

        if (ProjectileClass == nullptr)
        {
            UE_LOG(LogTemp, Error, TEXT("UTask_FireProjectile::EnterState - ProjectileClass is null"));
            return EStateTreeRunStatus::Failed;
        }

        ProjectileComponent->ShotProjectileToLocation(ProjectileClass, SocketLocation, Boss->GetActorRotation());

        // 이펙트 재생
        if (!EffectTag.IsNone())
        {
            EffectComponent->PlayEffectAttachedToSocket(EffectTag, SocketName);
        }

        return EStateTreeRunStatus::Succeeded;
    }
    ```

2.  **StateTree Task 설정:** StateTree 에디터에서 Task의 `EffectTag` 프로퍼티를 설정하여 재생할 이펙트의 태그를 지정합니다.

### 3.3. 풀링 시스템을 활용한 리소스 관리

`UBossProjectileComponent`는 투사체, 오브, 성검 마법 액터들을 풀링 시스템을 사용하여 관리합니다.  `InitializeObjectPool` 함수를 통해 풀을 초기화하고, `GetProjectileFromPool`, `GetOrbFromPool`, `GetHolySwordMagicFromPool` 함수를 통해 풀에서 액터를 가져옵니다.  액터가 더 이상 필요하지 않으면 `ReturnProjectileToPool`, `ReturnOrbToPool`, `ReturnHolySwordMagicToPool` 함수를 통해 풀로 반환합니다.

**단계별 구현:**

1.  **`AGateOfBabylon` 클래스 수정:** `AGateOfBabylon` 클래스에서 `SpawnProjectile` 함수를 수정하여 `UBossProjectileComponent`의 풀링 시스템을 사용하도록 합니다.

    ```c++
    // GateOfBabylon.cpp (수정)
    void AGateOfBabylon::SpawnProjectile()
    {
        // ... 기존 로직 ...

        // 풀링 시스템 사용
        if (BossProjectileComponent)
        {
            AGateOfBabyonProjectile* Projectile = Cast<AGateOfBabyonProjectile>(BossProjectileComponent->GetGateFromPool(ProjectileClass));

            if (Projectile)
            {
                FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
                FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

                Projectile->SetActorLocation(SpawnLocation);
                Projectile->SetActorRotation(SpawnRotation);
                Projectile->ActivateProjectile();

                // 투사체 이동 로직 (예: MoveToRandomLocationAroundPlayer)
                Projectile->MoveToRandomLocationAroundPlayer();
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("No available projectile in pool!"));
            }
        }
    }

    // AGateOfBabyonProjectile::DeactivateProjectile 수정 (풀로 반환)
    void AGateOfBabyonProjectile::DeactivateProjectile()
    {
        // ... 기존 로직 ...

        // 풀로 반환
        if (GateOfBabylonOwner && GateOfBabylonOwner->BossProjectileComponent)
        {
            GateOfBabylonOwner->BossProjectileComponent->ReturnGateToPool(this);
        }
        else
        {
            Destroy(); // 풀이 없는 경우 파괴
        }
    }
    ```

2.  **`ABossProjectileActor`, `ABossProjectileOrb`, `AHolySwordMagic` 클래스 수정:**  각 클래스의 `BeginPlay` 함수에서 액터를 비활성화하고, `OnProjectileHit` (또는 유사한 함수)에서 액터를 풀로 반환하도록 수정합니다.

    ```c++
    // ABossProjectileActor.cpp (수정)
    void ABossProjectileActor::BeginPlay()
    {
        Super::BeginPlay();

        // 초기에는 투사체를 비활성화
        SetActorTickEnabled(false);
        SetActorHiddenInGame(true);
        GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    void ABossProjectileActor::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
    {
        // ... 데미지 처리 로직 ...

        // 투사체 비활성화 및 풀로 반환
        SetActorTickEnabled(false);
        SetActorHiddenInGame(true);
        GetCollisionComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

        UBossProjectileComponent* ProjectileComponent = GetOwner()->FindComponentByClass<UBossProjectileComponent>();
        if (ProjectileComponent)
        {
            ProjectileComponent->ReturnBossProjectileToPool(this);
        }
        else
        {
            Destroy(); // 풀이 없는 경우 파괴
        }
    }
    ```

3.  **`UBossProjectileComponent` 초기화:** `ABossManager` 또는 보스 액터의 `BeginPlay` 함수에서 `UBossProjectileComponent`의 `InitializeObjectPool` 함수를 호출하여 풀을 초기화합니다.  풀 크기를 적절하게 설정합니다.

    ```c++
    // ABossManager.cpp (예시)
    void ABossManager::BeginPlay()
    {
        Super::BeginPlay();

        // 보스 찾기
        ACBoss* Boss = FindBossInWorld();
        if (Boss)
        {
            // UBossProjectileComponent 찾기
            UBossProjectileComponent* ProjectileComponent = Boss->FindComponentByClass<UBossProjectileComponent>();
            if (ProjectileComponent)
            {
                // 풀 초기화
                ProjectileComponent->InitializeObjectPool(50, ABossProjectileActor::StaticClass()); // 예시: 투사체 풀 크기 50
                ProjectileComponent->InitializeObjectPool(20, ABossProjectileOrb::StaticClass()); // 예시: 오브 풀 크기 20
                ProjectileComponent->InitializeObjectPool(10, AHolySwordMagic::StaticClass()); // 예시: 성검 마법 풀 크기 10
                ProjectileComponent->InitializeObjectPool(30, AGateOfBabyonProjectile::StaticClass()); // 예시: 바빌론의 문 프로젝타일 풀 크기 30
            }
        }
    }
    ```

### 3.4. 데이터 플로우 및 이벤트 시스템

클래스 간의 데이터 플로우는 주로 컴포넌트와 액터 간의 함수 호출을 통해 이루어집니다. 예를 들어, `UBossProjectileComponent`는 투사체 발사 요청을 받으면 풀에서 액터를 가져와 발사하고, 충돌 시 데미지를 적용하는 등의 작업을 수행합니다.

이벤트 시스템은 `DECLARE_DYNAMIC_MULTICAST_DELEGATE` 매크로를 사용하여 정의된 델리게이트를 통해 구현됩니다. 예를 들어, `CBossEquipment` 클래스에서 장비 장착/해제 이벤트가 발생하면 해당 델리게이트를 통해 다른 컴포넌트나 액터에 알릴 수 있습니다.

### 3.5. 상태 관리

보스의 상태는 `BossStateComponent`를 통해 관리됩니다. `BossStateComponent`는 현재 상태를 나타내는 태그를 저장하고, 상태가 변경될 때 이벤트를 발생시킵니다. StateTree의 조건 노드는 `STC_CheckPase`, `STC_IsDeadState` 등과 같은 클래스를 사용하여 보스의 현재 상태를 확인하고, StateTree의 흐름을 제어합니다.

## 4. 마무리

이 가이드에서는 보스 시스템을 구축하기 위한 핵심 액터, 컴포넌트, 애니메이션 노티파이, StateTree Task들을 구현하고 통합하는 방법을 상세하게 설명했습니다.  이 가이드를 통해 개발자는 자신만의 독특한 보스 시스템을 구축하고, 게임의 재미를 한층 더 끌어올릴 수 있을 것입니다.  다음 단계에서는 이 시스템을 더욱 확장하고 개선하는 방법을 다룰 수 있습니다. 예를 들어, 더 복잡한 AI 패턴, 더 다양한 이펙트, 더 정교한 풀링 시스템 등을 구현할 수 있습니다.


## 1. 프로젝트 설정

### 1.1 개발 환경 구성

언리얼 엔진 5 (UE5)가 설치되어 있어야 합니다. Visual Studio 2019 이상 또는 Xcode (macOS)가 설치되어 언리얼 엔진 프로젝트를 빌드할 수 있도록 설정합니다. 언리얼 엔진 에디터에서 C++ 프로젝트를 생성할 때 필요한 툴체인이 올바르게 구성되었는지 확인하십시오.

### 1.2 프로젝트 구조 설정

새로운 언리얼 엔진 프로젝트를 생성하거나 기존 프로젝트를 사용합니다. "BossSystem"과 같은 의미 있는 이름으로 폴더를 생성하여 보스 관련 모든 코드를 구성합니다.  이 폴더 안에 `Actors`, `Components`, `AnimNotifies`, `StateTreeTasks`, `Conditions`, `UI` 등의 하위 폴더를 생성하여 코드를 더욱 체계적으로 관리합니다.

```
BossSystem/
├── Actors/
├── Components/
├── AnimNotifies/
├── StateTreeTasks/
├── Conditions/
└── UI/
```

### 1.3 의존성 관리

`Build.cs` 파일에서 필요한 모듈을 추가하여 프로젝트의 의존성을 관리합니다. 예를 들어, AI 기능을 사용하려면 `AIModule`을 추가해야 합니다.

```csharp
// MyProject.Build.cs
using UnrealBuildTool;

public class MyProject : ModuleRules
{
    public MyProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "AIModule", "GameplayTasks", "NavigationSystem", "UMG", "Slate", "SlateCore" });

        PrivateDependencyModuleNames.AddRange(new string[] {  });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
```

### 1.4 빌드 설정

언리얼 엔진 에디터에서 "File" -> "Refresh Visual Studio Project"를 선택하여 Visual Studio 프로젝트 파일을 업데이트합니다.  프로젝트를 빌드하여 모든 설정이 올바른지 확인합니다.  빌드 오류가 발생하면 의존성, 헤더 파일 포함 경로, 빌드 설정 등을 확인하십시오.

## 2. 핵심 클래스 구현

### 2.1. `ABossEffect` 클래스 구현

**설명**: 이펙트 액터 클래스. 풀링 시스템과 연동하여 효율적인 이펙트 관리를 제공합니다.

**헤더 파일 (BossEffect.h)**:

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossEffect.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectFinished, ABossEffect*, Effect);

UCLASS()
class MYPROJECT_API ABossEffect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossEffect();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** 이펙트 종료 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "Effect")
	FOnEffectFinished OnEffectFinished;

	/** 이펙트 활성화 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ActivateEffect();

	/** 소켓에 붙여서 이펙트 활성화 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ActivateEffectAttachedToSocket(USceneComponent* Parent, FName SocketName);

	/** 이펙트 비활성화 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void DeactivateEffect();

	/** 이펙트가 활성화되어 있는지 확인 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Effect")
	bool IsActive() const;

	/** 현재 이펙트 태그 반환 */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Effect")
	FGameplayTag GetCurrentEffectTag() const;

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

private:
	/** 이펙트를 보스에 어태치 */
	UFUNCTION()
	void AttachToBoss(AActor* BossActor);

	/** 소켓에 이펙트 붙이기 */
	UFUNCTION()
	void AttachToSocket(USceneComponent* Parent, FName SocketName);

	/** 이펙트를 월드에 배치 */
	UFUNCTION()
	void PlaceInWorld(FVector Location, FRotator Rotation);

	/** 이펙트 활성화 여부 */
	bool bIsActive;

	/** 이펙트 태그 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FGameplayTag EffectTag;

	/** 이펙트 지속 시간 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float EffectDuration;

	/** 이펙트 파티클 시스템 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* ParticleSystemComponent;

	/** 이펙트 사운드 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class USoundBase* EffectSound;

	/** 이펙트 오디오 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	class UAudioComponent* AudioComponent;
};
```

**구현 파일 (BossEffect.cpp)**:

```c++
#include "BossEffect.h"
#include "Components/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
ABossEffect::ABossEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 파티클 시스템 컴포넌트 생성
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystem"));
	RootComponent = ParticleSystemComponent;

	// 오디오 컴포넌트 생성
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false; // 자동 실행 비활성화

	bIsActive = false;
	EffectDuration = 3.0f; // 기본 지속 시간
}

// Called when the game starts or when spawned
void ABossEffect::BeginPlay()
{
	Super::BeginPlay();

	// 초기에는 이펙트를 비활성화
	DeactivateEffect();
}

// Called every frame
void ABossEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActive)
	{
		EffectDuration -= DeltaTime;
		if (EffectDuration <= 0.0f)
		{
			DeactivateEffect();
		}
	}
}

void ABossEffect::ActivateEffect()
{
	bIsActive = true;
	ParticleSystemComponent->Activate(true);
	if (EffectSound)
	{
		AudioComponent->SetSound(EffectSound);
		AudioComponent->Play();
	}
	EffectDuration = 3.0f; // 지속 시간 초기화
	SetActorTickEnabled(true);
}

void ABossEffect::ActivateEffectAttachedToSocket(USceneComponent* Parent, FName SocketName)
{
	AttachToSocket(Parent, SocketName);
	ActivateEffect();
}

void ABossEffect::DeactivateEffect()
{
	bIsActive = false;
	ParticleSystemComponent->Deactivate();
	AudioComponent->Stop();
	SetActorTickEnabled(false);

	// 이펙트 종료 델리게이트 호출
	OnEffectFinished.Broadcast(this);
}

bool ABossEffect::IsActive() const
{
	return bIsActive;
}

FGameplayTag ABossEffect::GetCurrentEffectTag() const
{
	return EffectTag;
}

void ABossEffect::AttachToBoss(AActor* BossActor)
{
	if (BossActor)
	{
		AttachToActor(BossActor, FAttachmentTransformRules::KeepWorldTransform);
	}
}

void ABossEffect::AttachToSocket(USceneComponent* Parent, FName SocketName)
{
	if (Parent)
	{
		AttachToComponent(Parent, FAttachmentTransformRules::KeepWorldTransform, SocketName);
	}
}

void ABossEffect::PlaceInWorld(FVector Location, FRotator Rotation)
{
	SetActorLocationAndRotation(Location, Rotation);
}
```

### 2.2. `ABossManager` 클래스 구현

**설명**: 보스 액터의 생성, 초기화, 상태 관리 등을 담당하는 핵심 클래스입니다.

**헤더 파일 (BossManager.h)**:

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIController.h"
#include "BossManager.generated.h"

UCLASS()
class MYPROJECT_API ABossManager : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossManager();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** 보스 완전 초기화 (메인 함수) */
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void ResetBossCompletely();

	/** 문 열기 */
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void OpenDoor();

	/** 월드에서 보스 찾기 */
	UFUNCTION(BlueprintCallable, Category = "Boss")
	AActor* FindBossInWorld();

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

private:
	/** 각 컴포넌트별 초기화 실행 */
	UFUNCTION()
	void ResetAllBossComponents();

	/** StateTree 완전 리스타트 */
	UFUNCTION()
	void ResetBossStateTree();

	/** 콜리전 오버랩 시작 이벤트 */
	UFUNCTION()
	void OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** 스폰할 보스 타입 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	TSubclassOf<AActor> BossType;

	/** 스폰 위치 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	FTransform SpawnTransform;

	/** 보스 액터 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss")
	AActor* SpawnedBoss;

	/** 보스 AI 컨트롤러 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss")
	AAIController* BossAIController;

	/** 트리거 박스 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox;

	/** 문 액터 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	AActor* DoorActor;

	/** 보스 초기 위치 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	FVector InitialBossLocation;

	/** 보스 초기 회전 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	FRotator InitialBossRotation;
};
```

**구현 파일 (BossManager.cpp)**:

```c++
#include "BossManager.h"
#include "Components/BoxComponent.h"
#include "AIController.h"

// Sets default values
ABossManager::ABossManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 트리거 박스 컴포넌트 생성
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABossManager::OnTriggerBoxOverlapBegin);
}

// Called when the game starts or when spawned
void ABossManager::BeginPlay()
{
	Super::BeginPlay();

	// 보스 스폰
	if (BossType)
	{
		SpawnedBoss = GetWorld()->SpawnActor<AActor>(BossType, SpawnTransform);

		if (SpawnedBoss)
		{
			// AI 컨트롤러 생성 및 소유
			BossAIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass());
			if (BossAIController)
			{
				BossAIController->Possess(Cast<APawn>(SpawnedBoss));
			}

			// 초기 위치 저장
			InitialBossLocation = SpawnedBoss->GetActorLocation();
			InitialBossRotation = SpawnedBoss->GetActorRotation();
		}
	}
}

// Called every frame
void ABossManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossManager::ResetBossCompletely()
{
	// 1. 보스 액터 파괴
	if (SpawnedBoss)
	{
		SpawnedBoss->Destroy();
		SpawnedBoss = nullptr;
	}

	// 2. AI 컨트롤러 파괴
	if (BossAIController)
	{
		BossAIController->Destroy();
		BossAIController = nullptr;
	}

	// 3. 보스 액터 재생성
	if (BossType)
	{
		SpawnedBoss = GetWorld()->SpawnActor<AActor>(BossType, SpawnTransform);

		if (SpawnedBoss)
		{
			// AI 컨트롤러 생성 및 소유
			BossAIController = GetWorld()->SpawnActor<AAIController>(AAIController::StaticClass());
			if (BossAIController)
			{
				BossAIController->Possess(Cast<APawn>(SpawnedBoss));
			}

			// 초기 위치 복원
			SpawnedBoss->SetActorLocation(InitialBossLocation);
			SpawnedBoss->SetActorRotation(InitialBossRotation);
		}
	}

	// 4. 컴포넌트 초기화
	ResetAllBossComponents();

	// 5. StateTree 리스타트
	ResetBossStateTree();
}

void ABossManager::OpenDoor()
{
	if (DoorActor)
	{
		DoorActor->Destroy();
	}
}

AActor* ABossManager::FindBossInWorld()
{
	return SpawnedBoss;
}

void ABossManager::ResetAllBossComponents()
{
	if (SpawnedBoss)
	{
		// 모든 컴포넌트를 순회하며 초기화 함수 호출 (예시)
		TArray<UActorComponent*> Components;
		SpawnedBoss->GetComponents(Components);

		for (UActorComponent* Component : Components)
		{
			// 특정 컴포넌트 타입에 따라 초기화 로직 실행
			if (UBossProjectileComponent* ProjectileComponent = Cast<UBossProjectileComponent>(Component))
			{
				ProjectileComponent->ResetProjectileSystem();
			}
			// 다른 컴포넌트 타입에 대한 초기화 로직 추가
		}
	}
}

void ABossManager::ResetBossStateTree()
{
	if (BossAIController)
	{
		// StateTree 컴포넌트 가져오기 (가정)
		UBehaviorTreeComponent* BehaviorTreeComponent = BossAIController->FindComponentByClass<UBehaviorTreeComponent>();
		if (BehaviorTreeComponent && BehaviorTreeComponent->IsRunning())
		{
			BehaviorTreeComponent->StopTree(EAIVReason::Restarted);
			BehaviorTreeComponent->StartTree();
		}
	}
}

void ABossManager::OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어가 트리거 박스에 들어오면 보스 활성화 (예시)
	if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
	{
		// 보스 활성화 로직 (예: StateTree 시작)
		if (BossAIController)
		{
			UBehaviorTreeComponent* BehaviorTreeComponent = BossAIController->FindComponentByClass<UBehaviorTreeComponent>();
			if (BehaviorTreeComponent && !BehaviorTreeComponent->IsRunning())
			{
				BehaviorTreeComponent->StartTree();
			}
		}
	}
}
```

### 2.3. `ABossProjectileActor` 클래스 구현

**설명**: 보스 투사체 액터의 기본 클래스입니다. 투사체 발사, 충돌 처리, 이펙트 재생 등을 담당합니다.

**헤더 파일 (BossProjectileActor.h)**:

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BossProjectileActor.generated.h"

UCLASS()
class MYPROJECT_API ABossProjectileActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossProjectileActor();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** 투사체 발사 */
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void FireProjectile(FVector Direction, float Speed);

	/** 특정 위치로 투사체 발사 */
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void FireProjectileToLocation(FVector TargetLocation, float Speed);

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

	/** 투사체 충돌 이벤트 */
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** 스폰 이펙트 재생 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlaySpawnEffect();

	/** 파괴 이펙트 재생 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayDestroyEffect();

private:
	/** 투사체 이동 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	/** 콜리전 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComponent;

	/** 스폰 이펙트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UParticleSystem* SpawnEffect;

	/** 파괴 이펙트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UParticleSystem* DestroyEffect;

	/** 데미지 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float DamageAmount;
};
```

**구현 파일 (BossProjectileActor.cpp)**:

```c++
#include "BossProjectileActor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossProjectileActor::ABossProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 콜리전 컴포넌트 생성
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABossProjectileActor::OnProjectileHit);

	// 투사체 이동 컴포넌트 생성
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f;

	DamageAmount = 10.0f;
}

// Called when the game starts or when spawned
void ABossProjectileActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABossProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossProjectileActor::FireProjectile(FVector Direction, float Speed)
{
	ProjectileMovement->Velocity = Direction * Speed;
	ProjectileMovement->Activate();
}

void ABossProjectileActor::FireProjectileToLocation(FVector TargetLocation, float Speed)
{
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	FireProjectile(Direction, Speed);
}

void ABossProjectileActor::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 데미지 적용
	if (OtherActor && OtherActor != this)
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, DamageAmount, NormalImpulse, Hit, GetInstigatorController(), this, UDamageType::StaticClass());
		PlayDestroyEffect();
		Destroy(); // 또는 풀에 반환
	}
}

void ABossProjectileActor::PlaySpawnEffect()
{
	if (SpawnEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, GetActorTransform(), true);
	}
}

void ABossProjectileActor::PlayDestroyEffect()
{
	if (DestroyEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyEffect, GetActorTransform(), true);
	}
}
```

### 2.4. `ABossProjectileOrb` 클래스 구현

**설명**: 보스 프로젝타일 오브 액터 클래스.  특정 위치에 생성되어 주변에 투사체를 발사하는 역할을 합니다.

**헤더 파일 (BossProjectileOrb.h)**:

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossProjectileOrb.generated.h"

UCLASS()
class MYPROJECT_API ABossProjectileOrb : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossProjectileOrb();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

private:
	/** 투사체 스폰 */
	UFUNCTION(BlueprintCallable, Category = "Orb")
	void SpawnProjectile();

	/** 오브 파괴 */
	UFUNCTION(BlueprintCallable, Category = "Orb")
	void DestroyOrb();

	/** 오버랩 이벤트 */
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** 스폰 사운드 재생 */
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlaySpawnSound();

	/** 풀 반환 사운드 재생 */
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayReturnToPoolSound();

	/** 충돌 사운드 재생 */
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayCollisionSound();

	/** 스폰 이펙트 재생 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlaySpawnEffect();

	/** 풀 반환 이펙트 재생 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayReturnToPoolEffect();

	/** 충돌 이펙트 재생 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlayCollisionEffect();

	/** 지연 후 오브 파괴 */
	UFUNCTION(BlueprintCallable, Category = "Orb")
	void DestroyOrbWithDelay(float Delay);

	/** 오브 활성화 */
	UFUNCTION(BlueprintCallable, Category = "Orb")
	void ActivateOrb();

	/** 콜리전 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* CollisionComponent;

	/** 스폰 사운드 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* SpawnSound;

	/** 풀 반환 사운드 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* ReturnToPoolSound;

	/** 충돌 사운드 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class USoundBase* CollisionSound;

	/** 스폰 이펙트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UParticleSystem* SpawnEffect;

	/** 풀 반환 이펙트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UParticleSystem* ReturnToPoolEffect;

	/** 충돌 이펙트 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	class UParticleSystem* CollisionEffect;

	/** 투사체 타입 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb")
	TSubclassOf<AActor> ProjectileType;

	/** 투사체 스폰 간격 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb")
	float SpawnInterval;

	/** 투사체 속도 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb")
	float ProjectileSpeed;

	/** 투사체 데미지 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Orb")
	float ProjectileDamage;

	/** 활성화 여부 */
	bool bIsActive;

	/** 스폰 타이머 핸들 */
	FTimerHandle SpawnTimerHandle;
};
```

**구현 파일 (BossProjectileOrb.cpp)**:

```c++
#include "BossProjectileOrb.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BossProjectileActor.h"

// Sets default values
ABossProjectileOrb::ABossProjectileOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 콜리전 컴포넌트 생성
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetSphereRadius(100.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossProjectileOrb::OnOverlap);

	SpawnInterval = 1.0f;
	ProjectileSpeed = 1000.0f;
	ProjectileDamage = 10.0f;
	bIsActive = false;
}

// Called when the game starts or when spawned
void ABossProjectileOrb::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABossProjectileOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossProjectileOrb::SpawnProjectile()
{
	if (ProjectileType)
	{
		// 랜덤 방향 생성
		FVector RandomDirection = FMath::VRand().GetSafeNormal();

		// 투사체 스폰
		AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileType, GetActorLocation(), RandomDirection.Rotation());

		if (Projectile)
		{
			// 투사체 속도 설정 (BossProjectileActor로 캐스팅하여 설정)
			if (ABossProjectileActor* BossProjectile = Cast<ABossProjectileActor>(Projectile))
			{
				BossProjectile->FireProjectile(RandomDirection, ProjectileSpeed);
			}
		}
	}
}

void ABossProjectileOrb::DestroyOrb()
{
	PlayReturnToPoolSound();
	PlayReturnToPoolEffect();
	Destroy(); // 또는 풀에 반환
}

void ABossProjectileOrb::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 충돌 처리 (예: 플레이어에게 데미지)
	if (OtherActor && OtherActor != this)
	{
		PlayCollisionSound();
		PlayCollisionEffect();
		UGameplayStatics::ApplyPointDamage(OtherActor, ProjectileDamage, SweepResult.ImpactNormal, SweepResult, GetInstigatorController(), this, UDamageType::StaticClass());
		DestroyOrb(); // 또는 풀에 반환
	}
}

void ABossProjectileOrb::PlaySpawnSound()
{
	if (SpawnSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation());
	}
}

void ABossProjectileOrb::PlayReturnToPoolSound()
{
	if (ReturnToPoolSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ReturnToPoolSound, GetActorLocation());
	}
}

void ABossProjectileOrb::PlayCollisionSound()
{
	if (CollisionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CollisionSound, GetActorLocation());
	}
}

void ABossProjectileOrb::PlaySpawnEffect()
{
	if (SpawnEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, GetActorTransform(), true);
	}
}

void ABossProjectileOrb::PlayReturnToPoolEffect()
{
	if (ReturnToPoolEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ReturnToPoolEffect, GetActorTransform(), true);
	}
}

void ABossProjectileOrb::PlayCollisionEffect()
{
	if (CollisionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollisionEffect, GetActorTransform(), true);
	}
}

void ABossProjectileOrb::DestroyOrbWithDelay(float Delay)
{
	FTimerHandle DestroyTimerHandle;
	FTimerDelegate DestroyDelegate;
	DestroyDelegate.BindUFunction(this, FName("DestroyOrb"));
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, DestroyDelegate, Delay, false);
}

void ABossProjectileOrb::ActivateOrb()
{
	bIsActive = true;
	PlaySpawnSound();
	PlaySpawnEffect();

	// 투사체 스폰 시작
	GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &ABossProjectileOrb::SpawnProjectile, SpawnInterval, true);
}
```

### 2.5. `ACBoss` 클래스 구현

**설명**: 보스 캐릭터의 기본 클래스입니다. 데미지 처리, 애니메이션 재생, UI 업데이트 등을 담당합니다.

**헤더 파일 (CBoss.h)**:

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CBoss.generated.h"

UCLASS()
class MYPROJECT_API ACBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACBoss();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	/** 데미지 함수 */
	UFUNCTION(BlueprintCallable, Category = "Boss")
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

private:
	/** 피격 모션 재생 */
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayHitMotion();

	/** 보스 상태 위젯 표시 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowBossStatusWidget();

	/** HP 업데이트 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HPUpdate();

	/** UI 리스타트 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RestartUI();

	/** 보스 배경음악 재생 */
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayBossBGM();

	/** 보스 배경음악 정지 */
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void StopBossBGM();

	/** 보스 배경음악 볼륨 감소 (죽음 시) */
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void LowerBossBGMVolume();

	/** 보스 초기 위치 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	FVector InitialLocation;

	/** 보스 상태 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> BossStatusWidgetClass;

	/** 보스 상태 위젯 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UBossStatusWidget* BossStatusWidget;

	/** 최대 HP */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHP;

	/** 현재 HP */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentHP;

	/** 보스 배경음악 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	class

## 2. 핵심 클래스 구현

이제 추출된 데이터와 설명을 바탕으로 핵심 클래스들을 구현해 보겠습니다. 각 클래스별로 헤더 파일(.h)과 소스 파일(.cpp)을 생성하고, 필요한 멤버 변수와 함수들을 정의하고 구현합니다.

### 2.1. `ACBoss` (보스 캐릭터)

**헤더 파일 (CBoss.h):**

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/AudioComponent.h" // 배경음악 재생을 위해 추가
#include "CBossStatusWidget.h" // 보스 상태 위젯 포함
#include "CBoss.generated.h"

UCLASS()
class YOUR_PROJECT_API ACBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACBoss();

	/** Called every frame */
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** 데미지 함수 */
	UFUNCTION(BlueprintCallable, Category = "Combat")
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;

public:

	/** 히트 모션 재생 */
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void PlayHitMotion();

	/** 보스 상태 위젯 표시 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void ShowBossStatusWidget();

	/** HP 업데이트 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void HPUpdate();

	/** UI 리스타트 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RestartUI();

	/** 보스 배경음악 재생 */
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayBossBGM();

	/** 보스 배경음악 정지 */
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void StopBossBGM();

	/** 보스 배경음악 볼륨 감소 (죽음 시) */
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void LowerBossBGMVolume();

	/** 보스 초기 위치 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
	FVector InitialLocation;

	/** 보스 상태 위젯 클래스 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> BossStatusWidgetClass;

	/** 보스 상태 위젯 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	class UBossStatusWidget* BossStatusWidget;

	/** 최대 HP */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	float MaxHP;

	/** 현재 HP */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Status")
	float CurrentHP;

	/** 보스 배경음악 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* BossBGM; // USoundBase로 변경. USoundWave는 레거시.

	/** 오디오 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Sound", meta = (AllowPrivateAccess = "true"))
	UAudioComponent* BGMComponent;
};
```

**소스 파일 (CBoss.cpp):**

```c++
#include "CBoss.h"
#include "Components/WidgetComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "CBossStatusWidget.h"

ACBoss::ACBoss()
{
	PrimaryActorTick.bCanEverTick = true;

	// 오디오 컴포넌트 생성
	BGMComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMComponent"));
	BGMComponent->SetupAttachment(RootComponent);
	BGMComponent->bAutoActivate = false; // BeginPlay에서 직접 활성화
}

void ACBoss::BeginPlay()
{
	Super::BeginPlay();

	// 초기 위치 저장
	InitialLocation = GetActorLocation();

	// 보스 상태 위젯 생성
	if (BossStatusWidgetClass)
	{
		BossStatusWidget = CreateWidget<UBossStatusWidget>(GetWorld(), BossStatusWidgetClass);
		if (BossStatusWidget)
		{
			BossStatusWidget->AddToViewport();
			BossStatusWidget->SetVisibility(ESlateVisibility::Hidden); // 초기에는 숨김
		}
	}

	// HP 초기화
	CurrentHP = MaxHP;

	// BGM 컴포넌트 설정 (BGM이 설정되어 있다면)
	if (BossBGM)
	{
		BGMComponent->SetSound(BossBGM);
	}
}

void ACBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float ACBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	CurrentHP -= DamageAmount;
	CurrentHP = FMath::Clamp(CurrentHP, 0.0f, MaxHP); // HP가 범위를 벗어나지 않도록

	HPUpdate();

	if (CurrentHP <= 0.0f)
	{
		// 사망 처리 로직 (애니메이션 재생, UI 변경 등)
		// 예시: PlayDeadAnimation();
		StopBossBGM(); // 보스 사망 시 BGM 정지
	}

	return DamageAmount;
}

void ACBoss::PlayHitMotion()
{
	// TODO: 히트 모션 재생 로직 구현 (애니메이션 몽타주 재생 등)
	UE_LOG(LogTemp, Warning, TEXT("PlayHitMotion called"));
}

void ACBoss::ShowBossStatusWidget()
{
	if (BossStatusWidget)
	{
		BossStatusWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

void ACBoss::HPUpdate()
{
	if (BossStatusWidget)
	{
		BossStatusWidget->UpdateBossHP(CurrentHP / MaxHP); // HP 비율을 위젯에 전달
	}
}

void ACBoss::RestartUI()
{
	// TODO: UI 리스타트 로직 구현
	UE_LOG(LogTemp, Warning, TEXT("RestartUI called"));
}

void ACBoss::PlayBossBGM()
{
	if (BGMComponent && BossBGM)
	{
		BGMComponent->Play();
	}
}

void ACBoss::StopBossBGM()
{
	if (BGMComponent)
	{
		BGMComponent->Stop();
	}
}

void ACBoss::LowerBossBGMVolume()
{
	if (BGMComponent)
	{
		BGMComponent->SetVolumeMultiplier(0.2f); // 볼륨을 20%로 감소 (예시)
	}
}
```

**설명:**

*   `BGMComponent`: 보스 배경음악을 재생하는 `UAudioComponent`입니다.  `CreateDefaultSubobject`로 생성하고, `RootComponent`에 Attach합니다.  `bAutoActivate`는 `false`로 설정하여 `BeginPlay`에서 직접 활성화하도록 합니다.
*   `BossBGM`:  에디터에서 설정할 수 있는 `USoundBase` 타입의 변수입니다.  `USoundWave`는 레거시 타입이므로 `USoundBase`를 사용하는 것이 좋습니다.
*   `BeginPlay`:  `InitialLocation`을 저장하고, `BossStatusWidget`을 생성하여 뷰포트에 추가합니다.  `CurrentHP`를 `MaxHP`로 초기화하고, `BGMComponent`에 `BossBGM`을 설정합니다.
*   `TakeDamage`: 데미지를 처리하고, `CurrentHP`를 업데이트합니다.  `HPUpdate` 함수를 호출하여 UI를 업데이트합니다.  HP가 0 이하가 되면 사망 처리 로직을 실행하고, `StopBossBGM`을 호출하여 배경음악을 정지합니다.
*   `PlayBossBGM`, `StopBossBGM`, `LowerBossBGMVolume`:  `BGMComponent`를 사용하여 배경음악을 재생, 정지, 볼륨 조절합니다.

**주의 사항:**

*   `YOUR_PROJECT_API`를 실제 프로젝트 이름으로 변경해야 합니다.
*   `TODO` 주석이 있는 부분은 실제 게임 로직에 맞게 구현해야 합니다.
*   `USoundWave`는 레거시 타입이므로 `USoundBase`를 사용하는 것이 좋습니다.

### 2.2. `UBossStatusWidget` (보스 상태 위젯)

**헤더 파일 (CBossStatusWidget.h):**

```c++
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CBossStatusWidget.generated.h"

UCLASS()
class YOUR_PROJECT_API UBossStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** 위젯 초기화 */
	virtual void NativeConstruct() override;

	/** 보스 HP 업데이트 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateBossHP(float HPRatio);

	/** 보스 완전체 UI 전환 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SwitchBossCompleteUI();

	/** 위젯 종료 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void EndWidget();

	/** 재시작 준비 */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void RestartReady();

protected:
	/** 페이드 인 핸들러 */
	UFUNCTION()
	void FadeInHandler();

	/** 완전체 UI 표시 */
	UFUNCTION()
	void ShowCompleteUI();

	/** 페이드 아웃 핸들러 */
	UFUNCTION()
	void FadeOutHandler();

	/** 부드러운 HP 업데이트 (딜레이) */
	UFUNCTION()
	void SmoothUpdateDelayHP();

private:
	// HP Bar (ProgressBar) - UMG 에디터에서 바인딩
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HPBar;

	// Complete UI (Image) - UMG 에디터에서 바인딩
	UPROPERTY(meta = (BindWidget))
	class UImage* CompleteUI;
};
```

**소스 파일 (BossStatusWidget.cpp):**

```c++
#include "BossStatusWidget.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"
#include "Animation/WidgetAnimation.h"

void UBossStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// HPBar가 유효한지 확인
	if (!HPBar)
	{
		UE_LOG(LogTemp, Warning, TEXT("HPBar is null in BossStatusWidget!"));
	}

	// CompleteUI가 유효한지 확인
	if (!CompleteUI)
	{
		UE_LOG(LogTemp, Warning, TEXT("CompleteUI is null in BossStatusWidget!"));
		if (CompleteUI)
		{
			CompleteUI->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UBossStatusWidget::UpdateBossHP(float HPRatio)
{
	if (HPBar)
	{
		HPBar->SetPercent(HPRatio);
	}
}

void UBossStatusWidget::SwitchBossCompleteUI()
{
	// TODO: 완전체 UI 전환 로직 구현 (애니메이션 재생, 위젯 표시 등)
	UE_LOG(LogTemp, Warning, TEXT("SwitchBossCompleteUI called"));
}

void UBossStatusWidget::EndWidget()
{
	// TODO: 위젯 종료 로직 구현 (위젯 제거, 메모리 해제 등)
	UE_LOG(LogTemp, Warning, TEXT("EndWidget called"));
}

void UBossStatusWidget::RestartReady()
{
	// TODO: 재시작 준비 로직 구현
	UE_LOG(LogTemp, Warning, TEXT("RestartReady called"));
}

void UBossStatusWidget::FadeInHandler()
{
	// TODO: 페이드 인 핸들러 로직 구현
	UE_LOG(LogTemp, Warning, TEXT("FadeInHandler called"));
}

void UBossStatusWidget::ShowCompleteUI()
{
	// TODO: 완전체 UI 표시 로직 구현
	UE_LOG(LogTemp, Warning, TEXT("ShowCompleteUI called"));
}

void UBossStatusWidget::FadeOutHandler()
{
	// TODO: 페이드 아웃 핸들러 로직 구현
	UE_LOG(LogTemp, Warning, TEXT("FadeOutHandler called"));
}

void UBossStatusWidget::SmoothUpdateDelayHP()
{
	// TODO: 부드러운 HP 업데이트 (딜레이) 로직 구현
	UE_LOG(LogTemp, Warning, TEXT("SmoothUpdateDelayHP called"));
}
```

**설명:**

*   `HPBar`: 보스의 HP를 표시하는 `UProgressBar`입니다. UMG 에디터에서 위젯과 바인딩해야 합니다.
*   `CompleteUI`: 보스가 완전체가 되었을 때 표시하는 `UImage`입니다. UMG 에디터에서 위젯과 바인딩해야 합니다.
*   `NativeConstruct`: 위젯이 생성될 때 호출되는 함수입니다. `HPBar`와 `CompleteUI`가 유효한지 확인합니다.
*   `UpdateBossHP`: `HPBar`의 Percent 값을 업데이트하여 보스의 HP를 표시합니다.

**주의 사항:**

*   `YOUR_PROJECT_API`를 실제 프로젝트 이름으로 변경해야 합니다.
*   UMG 에디터에서 `HPBar`와 `CompleteUI` 위젯을 생성하고, 이 클래스의 변수와 바인딩해야 합니다.
*   `TODO` 주석이 있는 부분은 실제 게임 로직에 맞게 구현해야 합니다.

### 2.3. `ABossEffect` (보스 이펙트)

**헤더 파일 (BossEffect.h):**

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "BossEffect.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectFinished, ABossEffect*, Effect);

UCLASS(Blueprintable)
class YOUR_PROJECT_API ABossEffect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** 이펙트 활성화 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ActivateEffect();

	/** 소켓에 붙여서 이펙트 활성화 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ActivateEffectAttachedToSocket(FName SocketName);

	/** 이펙트 비활성화 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void DeactivateEffect();

	/** 이펙트가 활성화되어 있는지 확인 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	bool IsActive() const;

	/** 현재 이펙트 태그 반환 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	FGameplayTag GetCurrentEffectTag() const;

	/** 이펙트를 보스에 어태치 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void AttachToBoss(AActor* BossActor, FName SocketName = NAME_None);

	/** 소켓에 이펙트 붙이기 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void AttachToSocket(USceneComponent* Component, FName SocketName);

	/** 이펙트를 월드에 배치 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlaceInWorld(FVector Location, FRotator Rotation);

	/** 이펙트 종료 델리게이트 */
	UPROPERTY(BlueprintAssignable, Category = "Effect")
	FOnEffectFinished OnEffectFinished;

protected:
	/** 루트 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* Root;

	/** 이펙트 활성화 여부 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Status", meta = (AllowPrivateAccess = "true"))
	bool bIsActive;

	/** 현재 이펙트 태그 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	FGameplayTag CurrentEffectTag;

private:
	/** 초기 트랜스폼 */
	FTransform InitialTransform;
};
```

**소스 파일 (BossEffect.cpp):**

```c++
#include "BossEffect.h"

ABossEffect::ABossEffect()
{
	PrimaryActorTick.bCanEverTick = true;

	// 루트 컴포넌트 생성
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	bIsActive = false;
}

void ABossEffect::BeginPlay()
{
	Super::BeginPlay();

	// 초기 트랜스폼 저장
	InitialTransform = GetActorTransform();
}

void ABossEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossEffect::ActivateEffect()
{
	bIsActive = true;
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
	UE_LOG(LogTemp, Warning, TEXT("Effect Activated: %s"), *GetName());
}

void ABossEffect::ActivateEffectAttachedToSocket(FName SocketName)
{
	// TODO: 소켓에 붙여서 이펙트 활성화 로직 구현
	bIsActive = true;
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);
	SetActorEnableCollision(true);
	UE_LOG(LogTemp, Warning, TEXT("Effect Activated Attached to Socket: %s, Socket: %s"), *GetName(), *SocketName.ToString());
}

void ABossEffect::DeactivateEffect()
{
	bIsActive = false;
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
	SetActorEnableCollision(false);
	UE_LOG(LogTemp, Warning, TEXT("Effect Deactivated: %s"), *GetName());

	// 이펙트 종료 델리게이트 호출
	OnEffectFinished.Broadcast(this);

	// 초기 트랜스폼으로 되돌리기
	SetActorTransform(InitialTransform);
}

bool ABossEffect::IsActive() const
{
	return bIsActive;
}

FGameplayTag ABossEffect::GetCurrentEffectTag() const
{
	return CurrentEffectTag;
}

void ABossEffect::AttachToBoss(AActor* BossActor, FName SocketName)
{
	if (BossActor)
	{
		AttachToActor(BossActor, FAttachmentTransformRules::KeepWorldTransform, SocketName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BossActor is null in AttachToBoss!"));
	}
}

void ABossEffect::AttachToSocket(USceneComponent* Component, FName SocketName)
{
	if (Component)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepWorld, true);
		AttachToComponent(Component, AttachmentRules, SocketName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Component is null in AttachToSocket!"));
	}
}

void ABossEffect::PlaceInWorld(FVector Location, FRotator Rotation)
{
	SetActorLocationAndRotation(Location, Rotation);
}
```

**설명:**

*   `Root`: 이펙트의 루트 컴포넌트입니다.
*   `bIsActive`: 이펙트의 활성화 여부를 나타냅니다.
*   `CurrentEffectTag`: 이펙트의 태그입니다.  `FGameplayTag`는 Gameplay Ability System에서 사용되는 태그 시스템입니다.
*   `OnEffectFinished`: 이펙트가 종료될 때 호출되는 델리게이트입니다.
*   `ActivateEffect`: 이펙트를 활성화합니다.  `bIsActive`를 `true`로 설정하고, 액터를 보이게 하고, 틱을 활성화하고, 콜리전을 활성화합니다.
*   `DeactivateEffect`: 이펙트를 비활성화합니다.  `bIsActive`를 `false`로 설정하고, 액터를 숨기고, 틱을 비활성화하고, 콜리전을 비활성화합니다.  `OnEffectFinished` 델리게이트를 호출합니다.  초기 트랜스폼으로 되돌립니다.
*   `AttachToBoss`: 이펙트를 보스 액터에 Attach합니다.
*   `AttachToSocket`: 이펙트를 SceneComponent의 소켓에 Attach합니다.
*   `PlaceInWorld`: 이펙트를 월드에 배치합니다.

**주의 사항:**

*   `YOUR_PROJECT_API`를 실제 프로젝트 이름으로 변경해야 합니다.
*   `TODO` 주석이 있는 부분은 실제 게임 로직에 맞게 구현해야 합니다.

### 2.4. `UBossEffectManager` (보스 이펙트 매니저)

**헤더 파일 (BossEffectManager.h):**

```c++
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameplayTagContainer.h"
#include "BossEffect.h"
#include "BossEffectManager.generated.h"

UCLASS()
class YOUR_PROJECT_API UBossEffectManager : public UObject
{
	GENERATED_BODY()

public:
	UBossEffectManager();

	/** 게임 시작 시 호출되는 함수 */
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Effect Manager")
	void BeginPlay();

	/** 이펙트 재생 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	ABossEffect* PlayEffect(FGameplayTag EffectTag, FVector Location, FRotator Rotation);

	/** 소켓에 붙여서 이펙트 재생 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	ABossEffect* PlayEffectAttachedToSocket(FGameplayTag EffectTag, USceneComponent* Component, FName SocketName);

	/** 모든 이펙트 정지 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void StopAllEffects();

	/** 특정 태그의 이펙트 정지 */
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void StopEffect(FGameplayTag EffectTag);

	/** 최대 풀 크기 설정 */
	UFUNCTION(BlueprintCallable, Category = "Effect Pool")
	void SetMaxPoolSize(int32 Size);

	/** 자동 풀 확장 설정 */
	UFUNCTION(BlueprintCallable, Category = "Effect Pool")
	void SetAutoExpandPool(bool bExpand);

	/** 이펙트가 완료되었을 때 호출 */
	UFUNCTION()
	void OnEffectFinished(ABossEffect* Effect);

	/** 현재 활성화된 이펙트 수 반환 */
	UFUNCTION(BlueprintCallable, Category = "Effect Pool")
	int32 GetActiveEffectCount() const;

	/** 풀의 사용 가능한 이펙트 수 반환 */
	UFUNCTION(BlueprintCallable, Category = "Effect Pool")
	int32 GetAvailableEffectCount() const;

protected:
	/** 데이터 테이블에서 데이터 로드 */
	UFUNCTION(CallInEditor, BlueprintCallable, Category = "Effect Manager")
	void LoadDataFromTables();

	/** 이펙트 풀 초기화 */
	UFUNCTION()
	void InitializeEffectPool();

	/** 풀에서 이펙트 가져오기 */
	ABossEffect* GetEffectFromPool(FGameplayTag EffectTag);

	/** 이펙트를 풀로 반환 */
	void ReturnEffectToPool(ABossEffect* Effect);

	/** 이펙트 풀 확장 */
	void ExpandEffectPool();

public:
	/** 이펙트 데이터 테이블 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	class UDataTable* EffectDataTable;

private:
	/** 이펙트 풀 */
	TMap<FGameplayTag, TArray<ABossEffect*>> EffectPool;

	/** 활성화된 이펙트 */
	TArray<ABossEffect*> ActiveEffects;

	/** 최대 풀 크기 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Pool")
	int32 MaxPoolSize;

	/** 자동 풀 확장 여부 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect Pool")
	bool bAutoExpandPool;
};
```

**소스 파일 (BossEffectManager.cpp):**

```c++
#include "BossEffectManager.h"
#include "Kismet/GameplayStatics.h"

UBossEffectManager::UBossEffectManager()
{
	MaxPoolSize = 10;
	bAutoExpandPool = true;
}

void UBossEffectManager::BeginPlay()
{
	LoadDataFromTables();
	InitializeEffectPool();
}

ABossEffect* UBossEffectManager::PlayEffect(FGameplayTag EffectTag, FVector Location, FRotator Rotation)
{
	ABossEffect* Effect = GetEffectFromPool(EffectTag);
	if (Effect)
	{
		Effect->PlaceInWorld(Location, Rotation);
		Effect->ActivateEffect();
		ActiveEffects.Add(Effect);
		return Effect;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to play effect: %s"), *EffectTag.ToString());
		return nullptr;
	}
}

ABossEffect* UBossEffectManager::PlayEffectAttachedToSocket(FGameplayTag EffectTag, USceneComponent* Component, FName SocketName)
{
	ABossEffect* Effect = GetEffectFromPool(EffectTag);
	if (Effect)
	{
		Effect->AttachToSocket(Component, SocketName);
		Effect->ActivateEffectAttachedToSocket(SocketName);
		ActiveEffects.Add(Effect);
		return Effect;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to play effect attached to socket: %s, Socket: %s"), *EffectTag.ToString(), *SocketName.ToString());
		return nullptr;
	}
}

void UBossEffectManager::StopAllEffects()
{
	for (ABossEffect* Effect : ActiveEffects)
	{
		if (Effect)
		{
			Effect->DeactivateEffect();
		}
	}
	ActiveEffects.Empty();
}

void UBossEffectManager::StopEffect(FGameplayTag EffectTag)
{
	for (int32 i = ActiveEffects.Num() - 1; i >= 0; --i)
	{
		ABossEffect* Effect = ActiveEffects[i];
		if (Effect && Effect->GetCurrentEffectTag() == EffectTag)
		{
			Effect->DeactivateEffect();
			ActiveEffects.RemoveAt(i);
		}
	}
}

void UBossEffectManager::SetMaxPoolSize(int32 Size)
{
	MaxPoolSize = Size;
}

void UBossEffectManager::SetAutoExpandPool(bool bExpand)
{
	bAutoExpandPool = bExpand;
}

void UBossEffectManager::OnEffectFinished(ABossEffect* Effect)
{
	ReturnEffectToPool(Effect);
	ActiveEffects.Remove(Effect);
}

int32 UBossEffectManager::GetActiveEffectCount() const
{
	return ActiveEffects.Num();
}

int32 UBossEffectManager::GetAvailableEffectCount() const
{
	int32 Count = 0;
	for (const auto& Pair : EffectPool)
	{
		Count += Pair.Value.Num();
	}
	return Count;
}

void UBossEffectManager::LoadDataFromTables()
{
	// TODO: EffectDataTable에서 데이터를 로드하는 로직 구현
	// EffectDataTable의 각 행을 순회하면서 EffectPool에 필요한 이펙트를 생성하고 추가합니다.
	UE_LOG(LogTemp, Warning, TEXT("LoadDataFromTables called"));
}

void UBossEffectManager::InitializeEffectPool()
{
	// TODO: EffectPool을 초기화하는 로직 구현
	// LoadDataFromTables에서 로드한 데이터를 기반으로 EffectPool에 이펙트를 미리 생성해둡니다.
	UE_LOG(LogTemp, Warning, TEXT("InitializeEffectPool called"));
}

ABossEffect* UBossEffectManager::GetEffectFromPool(FGameplayTag EffectTag)
{
	if (EffectPool.Contains(EffectTag) && EffectPool[EffectTag].Num() > 0)
	{
		ABossEffect* Effect = EffectPool[EffectTag].Pop();
		return Effect;
	}
	else
	{
		// 풀에 이펙트가 없으면 새로 생성하거나, 자동 확장 여부에 따라 처리
		if (bAutoExpandPool)
		{
			ExpandEffectPool();
			return GetEffectFromPool(EffectTag); // 재귀 호출
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Effect pool is empty for tag: %s, and auto expand is disabled."), *EffectTag.ToString());
			return nullptr;
		}
	}
}

void UBossEffectManager::ReturnEffectToPool(ABossEffect* Effect)
{
	if (Effect)
	{
		Effect->DeactivateEffect();
		EffectPool.FindOrAdd(Effect->GetCurrentEffectTag()).Add(Effect);
	}
}

void UBossEffectManager::ExpandEffectPool()
{
	// TODO: 이펙트 풀을 확장하는 로직 구현
	// 새로운 이펙트를 생성하여 EffectPool에 추가합니다.
	UE_LOG(LogTemp, Warning, TEXT("ExpandEffectPool called"));
}
```

**설명:**

*   `EffectDataTable`: 이펙트 데이터를 담고 있는 데이터 테이블입니다.  각 행은 이펙트의 태그, 클래스, 풀 사이즈 등을 정의합니다.
*   `EffectPool`: 이펙트를 저장하는 풀입니다.  `TMap<FGameplayTag, TArray<ABossEffect*>>` 형태로, 각 태그별로 이펙트 배열을 관리합니다.
*   `ActiveEffects`: 현재 활성화된 이펙트들을 저장하는 배열입니다.
*   `MaxPoolSize`: 각 이펙트 태그별로 풀에 저장할 수 있는 최대 이펙트 개수입니다.
*   `bAutoExpandPool`: 풀에 이펙트가 없을 때 자동으로 풀을 확장할지 여부를 결정합니다.
*   `PlayEffect`: 이펙트를 재생합니다.  `GetEffectFromPool`을 사용하여 풀에서 이펙트를 가져오고, `PlaceInWorld`를 사용하여 이펙트를 월드에 배치하고, `ActivateEffect`를 사용하여 이펙트를 활성화합니다.
*   `StopAllEffects`: 모든 활성화된 이펙트를 정지합니다.
*   `StopEffect`: 특정 태그의 이펙트를 정지합니다.
*   `GetEffectFromPool`: 풀에서 이펙트를 가져옵니다.  풀에 이펙트가 없으면 새로 생성하거나, 자동 확장 여부에 따라 처리합니다.
*   `ReturnEffectToPool`: 이펙트를 풀로 반환합니다.  `DeactivateEffect`를 사용하여 이펙트를 비활성화하고, 풀에 다시 추가합니다.
*   `ExpandEffectPool`: 이펙트 풀을 확장합니다.  새로운 이펙트를 생성하여 풀에 추가합니다.

**주의 사항:**

*   `YOUR_PROJECT_API`를 실제 프로젝트 이름으로 변경해야 합니다.
*   `TODO` 주석이 있는 부분은 실제 게임 로직에 맞게 구현해야 합니다.
*   `EffectDataTable`을 생성하고, 이펙트 데이터를 채워야 합니다.

### 2.5. `ACBossAIC` (보스 AI 컨트롤러)

**헤더 파일 (CBossAIC.h):**

```c++
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CBossAIC.generated.h"

/**
 *
 */
UCLASS()
class YOUR_PROJECT_API ACBossAIC : public AAIController
{
	GENERATED_BODY()

public:
	ACBossAIC();

protected:
	virtual void OnPossess(APawn* InPawn) override;

};
```

**소스 파일 (CBossAIC.cpp):**

```c++
#include "CBossAIC.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"

ACBossAIC::ACBossAIC()
{
	// BlackBoardComponent 생성
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	// BlackBoardComponent가 유효한지 확인
	ensure(BlackboardComponent);

	// BehaviorTreeComponent 생성
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	// BehaviorTreeComponent가 유효한지 확인
	ensure(BehaviorTreeComponent);
}

void ACBossAIC::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// Pawn이 유효한지 확인
	if (InPawn)
	{
		// Pawn을 소유하면 BlackBoard를 사용하고 BehaviorTree를 실행합니다.
		UBehaviorTree* BehaviorTree = Cast<UBehaviorTree>(InPawn->GetClass()->GetDefaultObject<APawn>()->FindComponentByClass<UBehaviorTreeComponent>()->GetBehaviorTreeAsset());

		if (BehaviorTree)
		{
			// BlackBoard 초기화
			BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);

			// BehaviorTree 실행
			BehaviorTreeComponent->StartTree(*BehaviorTree);
		}
	}
}
```

**설명:**

*   `BlackboardComponent`: AI의 상태를 저장하는 Blackboard 컴포넌트입니다.
*   `BehaviorTreeComponent`: AI의 행동을 정의하는 BehaviorTree 컴포넌트입니다.
*   `OnPossess`: AIController가 Pawn을 소유하게 되면 호출되는 함수입니다.  BehaviorTree를 실행합니다.

**주의 사항:**

*   `YOUR_PROJECT_API`를 실제 프로젝트 이름으로 변경해야 합니다.
*   BehaviorTree와 Blackboard를 생성하고, AI에 필요한 행동과 상태를 정의해야 합니다.
*   Pawn에 `UBehaviorTreeComponent`를 추가하고, 사용할 BehaviorTree Asset을 설정해야 합니다.

### 2.6. `ACBossWeapon` (보스 웨폰

Okay! 이전 내용에 이어 `ACBossWeapon`에 대한 상세 구현 가이드를 제공하겠습니다.

### 2.7. `ACBossWeapon` (보스 웨폰) 구현 가이드

**2.7.1. 클래스 설계 및 구현**

`ACBossWeapon`은 보스가 사용하는 무기의 기본 클래스입니다. 이 클래스는 무기의 장착, 해제, 충돌 처리, 소켓 부착 등의 기능을 담당합니다.

**헤더 파일 (CBossWeapon.h):**

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "CBossWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossWeaponBeginCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBossWeaponEndCollision);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnBossWeaponBeginOverlap, UPrimitiveComponent*, UPrimitiveComponent*, AActor*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBossWeaponEndOverlap, UPrimitiveComponent*, AActor*);

UCLASS()
class YOUR_PROJECT_API ACBossWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACBossWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 보스가 무기를 장착할 때 호출됩니다.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BossWeapon")
	void OnBossBeginEquip();
	virtual void OnBossBeginEquip_Implementation();

	// 보스가 무기를 해제할 때 호출됩니다.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "BossWeapon")
	void OnBossUnequip();
	virtual void OnBossUnequip_Implementation();

	// 콜리전 활성화
	UFUNCTION(BlueprintCallable, Category = "BossWeapon")
	void OnBossCollisions();

	// 특정 콜리전 활성화
	UFUNCTION(BlueprintCallable, Category = "BossWeapon")
	void OnSelectCollision(FName SocketName);

	// 콜리전 비활성화
	UFUNCTION(BlueprintCallable, Category = "BossWeapon")
	void OffBossCollisions();

	// 콜리전을 보스에 부착
	UFUNCTION(BlueprintCallable, Category = "BossWeapon")
	void BossAttachToCollision(USceneComponent* InParent, FName InSocketName);

	// 소켓에서 콜리전 시작
	UFUNCTION(BlueprintCallable, Category = "BossWeapon")
	void StartCollisionAtSocket(FName SocketName);

	// 콜리전을 소유자에게 종료
	UFUNCTION(BlueprintCallable, Category = "BossWeapon")
	void EndCollisionToOwner();

	// 무기를 보스에 부착합니다.
	UFUNCTION(BlueprintCallable, Category = "BossWeapon")
	void BossAttachTo(USceneComponent* InParent, FName InSocketName);

	// 콜리전 시작 이벤트
	UFUNCTION()
	void OnBossComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 콜리전 종료 이벤트
	UFUNCTION()
	void OnBossComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// 무기 메시
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* WeaponMesh;

	// 충돌체
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	// 콜리전 시작 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnBossWeaponBeginCollision OnWeaponBeginCollision;

	// 콜리전 종료 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnBossWeaponEndCollision OnWeaponEndCollision;

	// 오버랩 시작 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnBossWeaponBeginOverlap OnWeaponBeginOverlap;

	// 오버랩 종료 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "Delegates")
	FOnBossWeaponEndOverlap OnWeaponEndOverlap;

private:
	// 현재 활성화된 콜리전 소켓 이름
	FName CurrentCollisionSocketName;
};
```

**구현 파일 (CBossWeapon.cpp):**

```c++
#include "CBossWeapon.h"

// Sets default values
ACBossWeapon::ACBossWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 메시 컴포넌트 생성
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	RootComponent = WeaponMesh;

	// 콜리전 박스 컴포넌트 생성
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(RootComponent);
	CollisionBox->SetCollisionProfileName(TEXT("NoCollision")); // 초기에는 콜리전 비활성화
	CollisionBox->SetGenerateOverlapEvents(false);

	// 오버랩 이벤트 활성화
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACBossWeapon::OnBossComponentBeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACBossWeapon::OnBossComponentEndOverlap);

	CurrentCollisionSocketName = NAME_None;
}

// Called when the game starts or when spawned
void ACBossWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ACBossWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACBossWeapon::OnBossBeginEquip_Implementation()
{
	// 무기 장착 시 수행할 로직 (애니메이션 재생, 파티클 효과 등)
	UE_LOG(LogTemp, Warning, TEXT("무기 장착!"));
}

void ACBossWeapon::OnBossUnequip_Implementation()
{
	// 무기 해제 시 수행할 로직
	UE_LOG(LogTemp, Warning, TEXT("무기 해제!"));
	OffBossCollisions(); // 해제 시 콜리전 비활성화
}

void ACBossWeapon::OnBossCollisions()
{
	if (CollisionBox)
	{
		CollisionBox->SetCollisionProfileName(TEXT("BossWeapon"));
		CollisionBox->SetGenerateOverlapEvents(true);
	}
}

void ACBossWeapon::OnSelectCollision(FName SocketName)
{
	if (CollisionBox)
	{
		CurrentCollisionSocketName = SocketName;
		CollisionBox->SetCollisionProfileName(TEXT("BossWeapon"));
		CollisionBox->SetGenerateOverlapEvents(true);

		// 소켓에 콜리전 부착
		StartCollisionAtSocket(SocketName);
	}
}

void ACBossWeapon::OffBossCollisions()
{
	if (CollisionBox)
	{
		CollisionBox->SetCollisionProfileName(TEXT("NoCollision"));
		CollisionBox->SetGenerateOverlapEvents(false);
		EndCollisionToOwner();
		CurrentCollisionSocketName = NAME_None;
	}
}

void ACBossWeapon::BossAttachToCollision(USceneComponent* InParent, FName InSocketName)
{
	if (CollisionBox && InParent)
	{
		CollisionBox->AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, InSocketName);
	}
}

void ACBossWeapon::StartCollisionAtSocket(FName SocketName)
{
	if (WeaponMesh && CollisionBox)
	{
		// 소켓 위치에 콜리전 박스 부착
		BossAttachToCollision(WeaponMesh, SocketName);
	}
}

void ACBossWeapon::EndCollisionToOwner()
{
	if (WeaponMesh && CollisionBox)
	{
		// 콜리전 박스를 무기 메시의 루트 컴포넌트에 부착
		CollisionBox->AttachToComponent(WeaponMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
	}
}

void ACBossWeapon::BossAttachTo(USceneComponent* InParent, FName InSocketName)
{
	if (WeaponMesh && InParent)
	{
		WeaponMesh->AttachToComponent(InParent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, InSocketName);
	}
}

void ACBossWeapon::OnBossComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 오버랩 시작 시 이벤트 발생
	OnWeaponBeginOverlap.Broadcast(OverlappedComponent, OtherComp, OtherActor);
	OnWeaponBeginCollision.Broadcast();
}

void ACBossWeapon::OnBossComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 오버랩 종료 시 이벤트 발생
	OnWeaponEndOverlap.Broadcast(OverlappedComponent, OtherActor);
	OnWeaponEndCollision.Broadcast();
}
```

**2.7.2. 주요 메서드 구현**

*   `OnBossBeginEquip()`: 보스가 무기를 장착할 때 호출됩니다.  무기 장착 애니메이션 재생, 파티클 효과 등을 구현합니다.
*   `OnBossUnequip()`: 보스가 무기를 해제할 때 호출됩니다. 무기 해제 애니메이션 재생, 콜리전 비활성화 등을 구현합니다.
*   `OnBossCollisions()`: 무기의 콜리전을 활성화합니다.
*   `OnSelectCollision(FName SocketName)`: 특정 소켓에 콜리전을 활성화합니다.
*   `OffBossCollisions()`: 무기의 콜리전을 비활성화합니다.
*   `BossAttachTo(USceneComponent* InParent, FName InSocketName)`: 무기를 보스의 특정 소켓에 부착합니다.
*	`BossAttachToCollision(USceneComponent* InParent, FName InSocketName)`: 콜리전을 보스의 특정 소켓에 부착합니다.
*   `OnBossComponentBeginOverlap()`: 콜리전 오버랩 시작 시 호출됩니다.
*   `OnBossComponentEndOverlap()`: 콜리전 오버랩 종료 시 호출됩니다.

**2.7.3. 프로퍼티 설정**

*   `WeaponMesh`: 무기의 스켈레탈 메시 컴포넌트입니다.
*   `CollisionBox`: 무기의 충돌체입니다.

**2.7.4. 초기화 및 정리**

*   `BeginPlay()`: 액터가 생성될 때 호출됩니다. 초기 상태를 설정합니다.
*   콜리전 박스는 초기 상태에서 비활성화됩니다.

**2.7.5. 실제 사용 예제**

1.  **보스 블루프린트에서 무기 설정:**

    *   보스 캐릭터 블루프린트에서 `ACBossWeapon` 클래스의 변수를 생성합니다.
    *   `BeginPlay` 이벤트에서 `SpawnActor` 노드를 사용하여 `ACBossWeapon` 액터를 생성하고, 해당 변수에 저장합니다.
    *   `AttachActorToComponent` 노드를 사용하여 무기를 보스의 메시 컴포넌트의 특정 소켓에 부착합니다.

2.  **애니메이션 노티파이에서 콜리전 제어:**

    *   애니메이션 시퀀스에 `AnimNotify_BossWeaponCollision` 노티파이를 추가합니다.
    *   노티파이 이벤트에서 `OnBossCollisions` 또는 `OffBossCollisions` 함수를 호출하여 무기의 콜리전을 제어합니다.
    *   `OnSelectCollision` 함수를 호출하여 특정 소켓의 콜리전을 활성화합니다.

3.  **데미지 처리:**

    *   `OnBossComponentBeginOverlap` 이벤트에서 겹치는 액터가 데미지를 받을 수 있는지 확인합니다.
    *   `ApplyPointDamage` 또는 `ApplyRadialDamage` 함수를 사용하여 데미지를 적용합니다.

**2.7.6. 추가 고려 사항**

*   무기의 콜리전 프로파일을 적절하게 설정해야 합니다.
*   무기의 애니메이션과 콜리전 타이밍을 일치시켜야 합니다.
*   다양한 무기 유형을 지원하기 위해 `ACBossWeapon` 클래스를 상속받아 새로운 무기 클래스를 만들 수 있습니다.
*   `CurrentCollisionSocketName` 변수를 사용하여 현재 활성화된 콜리전 소켓을 추적하고, 필요에 따라 콜리전을 비활성화할 수 있습니다.

이 가이드를 통해 `ACBossWeapon` 클래스를 효과적으로 구현하고, 보스 캐릭터에 다양한 무기 시스템을 적용할 수 있습니다.

**다음 단계:**

이제 `AFlySpline` 부터 남은 구현 대상들에 대한 가이드를 이어서 작성할 수 있습니다. 각 클래스와 함수에 대한 설명을 참고하여, 상세한 구현 방법과 코드 예제를 제공해주세요.


