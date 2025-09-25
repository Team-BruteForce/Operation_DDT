## C++ 클래스 설계 및 분석 문서

본 문서는 제공된 데이터에 기반하여 C++로 구현된 보스 전투 시스템과 관련된 클래스들을 상세히 분석합니다. 각 클래스의 기능, 역할, 상호작용, 성능 특성 등을 심층적으로 파악하여, 시스템의 전체적인 구조와 설계 의도를 이해하고, 잠재적인 개선점 및 최적화 방안을 도출하는 것을 목표로 합니다.

### 1. 클래스 개요

이 프로젝트는 언리얼 엔진 기반의 게임에서 보스 캐릭터의 행동, 능력, 상태 등을 관리하기 위한 시스템을 구축하는 것을 목표로 합니다. 제공된 클래스 목록을 살펴보면, 보스의 AI, 전투 로직, 이펙트, 투사체, 애니메이션, UI 등 다양한 측면을 다루고 있음을 알 수 있습니다.

**전체적인 역할:**

*   **보스 관리 및 제어:** `ABossManager`, `ACBossAIC`, `ACBoss` 등의 클래스는 보스 캐릭터의 생성, 초기화, AI 제어, 상태 관리 등 핵심적인 기능을 담당합니다.
*   **전투 로직:** `UCBossDoAction`, `UCBossEquipment`, `ACBossWeapon`, `UBossProjectileComponent` 등의 클래스는 보스의 공격 패턴, 무기 사용, 투사체 발사 등 전투와 관련된 로직을 구현합니다.
*   **시각 효과:** `ABossEffect`, `UBossEffectManager`, `ABossProjectileActor`, `ABossProjectileOrb`, `AGateOfBabylon`, `AHolySwordMagic`, `AProjectile_LightSpear` 등의 클래스는 보스 전투의 시각적인 측면을 담당하며, 이펙트, 투사체, 마법 등을 생성하고 관리합니다.
*   **애니메이션:** `UBossAnimInstance`, `UAnimNotify_*` 클래스들은 보스 캐릭터의 애니메이션을 제어하고, 특정 애니메이션 이벤트에 따라 게임 로직을 실행합니다.
*   **UI:** `UBossStatusWidget`, `UDDTLoadingWidget`, `UDDTMainThemeWidget` 클래스들은 보스의 상태를 표시하거나, 로딩 화면, 메인 테마 화면 등을 제공하는 UI 요소를 관리합니다.
*   **StateTree 기반 AI:** `UCBossEnemyStateTreeEvaluator`, `UTask_*`, `USTC_*` 클래스들은 언리얼 엔진의 StateTree 시스템을 활용하여 보스의 AI를 구현합니다. StateTree는 보스의 행동을 계층적으로 정의하고, 다양한 조건에 따라 행동을 전환할 수 있도록 해줍니다.
*   **에디터 플러그인:** `FEditorPlugin_DataSyncModule`, `FEditorPlugin_DataSyncCommands`, `FEditorPlugin_DataSyncStyle` 클래스들은 언리얼 엔진 에디터에서 보스 관련 데이터를 동기화하고 관리하는 데 사용되는 플러그인을 구현합니다.

**클래스 간 관계 및 의존성:**

클래스 간의 관계는 상당히 복잡하며, 계층적인 구조를 가지고 있습니다. 예를 들어, `ABossManager`는 `ACBoss`를 생성하고, `ACBoss`는 `ACBossAIC`를 통해 AI 제어를 받습니다. `ACBoss`는 `UBossProjectileComponent`를 사용하여 투사체를 발사하고, `UBossEffectComponent`를 사용하여 이펙트를 재생합니다. `UBossAnimInstance`는 `ACBoss`의 애니메이션을 제어하며, `UAnimNotify_*` 클래스들은 애니메이션 이벤트에 따라 특정 로직을 실행합니다.

StateTree 관련 클래스들은 `ACBossAIC`에 의해 사용되며, 보스의 행동을 결정하는 데 중요한 역할을 합니다. `UTask_*` 클래스들은 StateTree의 각 상태에서 실행되는 작업을 정의하고, `USTC_*` 클래스들은 상태 전환 조건을 정의합니다.

**설계 철학과 아키텍처 패턴:**

제공된 클래스 목록과 함수 이름들을 통해 다음과 같은 설계 철학과 아키텍처 패턴이 적용되었음을 추론할 수 있습니다.

*   **컴포넌트 기반 설계:** 보스 캐릭터의 기능들을 컴포넌트(`UBossProjectileComponent`, `UBossEffectComponent`, `CBossMovementComponent` 등)로 분리하여, 코드의 재사용성과 유지보수성을 높였습니다.
*   **오브젝트 풀링:** `UBossEffectManager`, `UBossProjectileComponent`, `AGateOfBabylon` 등의 클래스에서 오브젝트 풀링을 사용하여, 이펙트나 투사체 생성/소멸에 따른 성능 저하를 방지했습니다.
*   **State Machine (StateTree):** 언리얼 엔진의 StateTree 시스템을 사용하여 보스의 AI를 구현했습니다. StateTree는 복잡한 AI 로직을 계층적으로 구성하고, 상태 전환 조건을 명확하게 정의할 수 있도록 해줍니다.
*   **애니메이션 노티파이:** `UAnimNotify_*` 클래스들을 사용하여, 애니메이션 이벤트에 따라 게임 로직을 실행했습니다. 이를 통해 애니메이션과 게임 로직을 효과적으로 연동할 수 있습니다.
*   **데이터 테이블:** `FEditorPlugin_DataSyncModule` 클래스를 통해, 보스 스탯이나 Gameplay Tag 등의 데이터를 데이터 테이블에 저장하고 관리하는 방식을 사용하고 있음을 알 수 있습니다. 이는 데이터 관리의 효율성을 높이고, 에디터에서 데이터를 쉽게 수정할 수 있도록 해줍니다.

이제 각 클래스별로 독립적인 섹션을 만들어 상세히 분석하도록 하겠습니다. 먼저, 보스 전투 시스템의 핵심적인 역할을 담당하는 `ABossEffect` 클래스부터 분석을 시작합니다. 다음 섹션에서는 `ABossEffect` 클래스의 목적, 책임, 주요 메서드, 사용 예제, 성능 특성 등을 자세히 살펴보겠습니다.


## 상세 클래스 분석

이제 제공된 클래스 목록과 함수 정보를 바탕으로 각 클래스별 상세 분석을 진행하겠습니다. 각 섹션에서는 클래스의 목적, 책임, 주요 메서드 분석, 사용 예제, 성능 특성, 확장성, 실무 적용 가이드 등을 다룹니다.

### 1. `ABossEffect` 클래스 분석

**1.1 클래스 개요**

`ABossEffect` 클래스는 보스 캐릭터와 관련된 시각적, 청각적 이펙트(파티클, 사운드 등)를 관리하고 활성화/비활성화하는 역할을 담당합니다. 이 클래스는 언리얼 엔진의 `AActor` 클래스를 상속받아 게임 월드에 배치될 수 있는 액터로 구현됩니다. `ABossEffect`는 재사용성을 높이기 위해 오브젝트 풀링 시스템과 함께 사용될 가능성이 높으며, `UBossEffectManager`를 통해 관리될 것으로 예상됩니다.

**1.2 목적과 책임**

*   **이펙트 활성화 및 비활성화:** 특정 조건(애니메이션 이벤트, 보스 상태 변화 등)에 따라 이펙트를 활성화하거나 비활성화합니다.
*   **이펙트 위치 설정:** 이펙트를 보스 캐릭터에 부착하거나 특정 소켓에 부착하여 이펙트가 보스와 함께 움직이도록 합니다.
*   **이펙트 상태 관리:** 이펙트가 현재 활성화되어 있는지 여부를 추적합니다.
*   **이펙트 태그 관리:** 이펙트를 식별하고 관리하기 위한 태그를 제공합니다.

**1.3 주요 메서드 분석**

*   `ABossEffect()`: 생성자. 이펙트의 기본 속성(파티클 시스템, 사운드 큐 등)을 초기화합니다.
*   `Tick(float DeltaTime)`: 매 프레임마다 호출되는 함수. 이펙트의 지속 시간, 위치 업데이트 등을 처리할 수 있습니다.
*   `ActivateEffect()`: 이펙트를 활성화하는 함수. 파티클 시스템을 시작하고, 사운드를 재생하는 등의 작업을 수행합니다.
*   `ActivateEffectAttachedToSocket(USceneComponent* Component, FName SocketName)`: 지정된 컴포넌트의 소켓에 이펙트를 부착하고 활성화하는 함수.
*   `DeactivateEffect()`: 이펙트를 비활성화하는 함수. 파티클 시스템을 중지하고, 사운드를 정지하는 등의 작업을 수행합니다.
*   `IsActive()`: 이펙트가 현재 활성화되어 있는지 여부를 반환하는 함수.
*   `GetCurrentEffectTag()`: 현재 이펙트의 태그를 반환하는 함수.
*   `BeginPlay()`: 액터가 게임에 스폰될 때 호출되는 함수. 이펙트 초기화 작업을 수행합니다.
*   `AttachToBoss(AActor* BossActor)`: 이펙트를 보스 캐릭터에 부착하는 함수.
*   `AttachToSocket(USceneComponent* Component, FName SocketName)`: 이펙트를 지정된 컴포넌트의 소켓에 부착하는 함수.
*   `PlaceInWorld(FVector Location, FRotator Rotation)`: 이펙트를 월드의 특정 위치에 배치하는 함수.

**1.4 클래스의 생명주기와 상태 관리**

`ABossEffect`의 생명주기는 다음과 같습니다.

1.  **생성:** `UBossEffectManager` 또는 다른 클래스에서 `NewObject` 등을 사용하여 `ABossEffect` 인스턴스를 생성합니다. 오브젝트 풀링을 사용하는 경우, 풀에서 사용 가능한 이펙트를 가져옵니다.
2.  **초기화:** `BeginPlay` 함수에서 이펙트의 기본 속성을 초기화합니다.
3.  **활성화:** `ActivateEffect` 또는 `ActivateEffectAttachedToSocket` 함수를 호출하여 이펙트를 활성화합니다. 이 때, 파티클 시스템을 시작하고, 사운드를 재생하는 등의 작업을 수행합니다.
4.  **업데이트:** `Tick` 함수에서 이펙트의 지속 시간, 위치 업데이트 등을 처리합니다.
5.  **비활성화:** `DeactivateEffect` 함수를 호출하여 이펙트를 비활성화합니다. 파티클 시스템을 중지하고, 사운드를 정지하는 등의 작업을 수행합니다.
6.  **소멸/반환:** 오브젝트 풀링을 사용하는 경우, 이펙트를 풀로 반환합니다. 오브젝트 풀링을 사용하지 않는 경우, `Destroy` 함수를 호출하여 이펙트를 소멸시킵니다.

`ABossEffect`는 활성화/비활성화 상태를 가지며, `IsActive` 함수를 통해 현재 상태를 확인할 수 있습니다.

**1.5 실제 사용 예제와 코드**

다음은 `ABossEffect` 클래스의 사용 예제입니다.

```c++
// 보스 이펙트 컴포넌트에서 이펙트를 활성화하는 예제
void UBossEffectComponent::PlayEffect(FName EffectTag, USceneComponent* AttachComponent, FName SocketName)
{
    ABossEffect* Effect = BossEffectManager->GetEffectFromPool(EffectTag);
    if (Effect)
    {
        if (AttachComponent)
        {
            Effect->ActivateEffectAttachedToSocket(AttachComponent, SocketName);
        }
        else
        {
            Effect->ActivateEffect();
        }
    }
}

// 애니메이션 노티파이에서 이펙트를 활성화하는 예제
void UAnimNotify_PlayEffect::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
    if (MeshComp && MeshComp->GetOwner())
    {
        ACBoss* Boss = Cast<ACBoss>(MeshComp->GetOwner());
        if (Boss)
        {
            UBossEffectComponent* EffectComponent = Boss->FindComponentByClass<UBossEffectComponent>();
            if (EffectComponent)
            {
                EffectComponent->PlayEffect(EffectTag, MeshComp, SocketName);
            }
        }
    }
}

// 이펙트 비활성화 예제
void ABossEffect::DeactivateEffect()
{
    // 파티클 시스템 중지
    if (ParticleSystemComponent)
    {
        ParticleSystemComponent->Deactivate();
    }

    // 사운드 정지
    if (AudioComponent)
    {
        AudioComponent->Stop();
    }

    bIsActive = false;

    // 이펙트 매니저에게 반환 (오브젝트 풀링 사용 시)
    if (EffectManager)
    {
        EffectManager->ReturnEffectToPool(this);
    }
    else
    {
        Destroy(); // 오브젝트 풀링 미사용 시
    }
}
```

**1.6 성능 특성과 최적화 포인트**

*   **오브젝트 풀링:** 이펙트 생성/소멸 비용을 줄이기 위해 오브젝트 풀링을 사용하는 것이 좋습니다.
*   **파티클 시스템 최적화:** 파티클 시스템의 파티클 수, 렌더링 비용 등을 최적화하여 성능을 향상시킬 수 있습니다.
*   **사운드 최적화:** 사운드 큐의 압축 포맷, 샘플 레이트 등을 최적화하여 메모리 사용량을 줄일 수 있습니다.
*   **불필요한 Tick 제거:** 이펙트의 상태 업데이트가 필요하지 않은 경우, `Tick` 함수를 비활성화하여 성능을 향상시킬 수 있습니다.

**1.7 사용 시나리오와 베스트 프랙티스**

*   **애니메이션 이벤트에 따른 이펙트 재생:** 애니메이션 노티파이를 사용하여 애니메이션 이벤트에 맞춰 이펙트를 재생합니다.
*   **보스 상태 변화에 따른 이펙트 재생:** 보스의 상태(공격, 피격, 사망 등)에 따라 다른 이펙트를 재생합니다.
*   **무기 이펙트:** 보스의 무기에 이펙트를 부착하여 공격 시 시각적인 효과를 강화합니다.
*   **피격 이펙트:** 보스가 공격에 맞았을 때 피격 이펙트를 재생하여 피드백을 제공합니다.

**베스트 프랙티스:**

*   **이펙트 태그 사용:** 이펙트를 식별하고 관리하기 위해 고유한 태그를 사용합니다.
*   **오브젝트 풀링 사용:** 이펙트 생성/소멸 비용을 줄이기 위해 오브젝트 풀링을 사용합니다.
*   **파라미터화:** 이펙트의 속성(색상, 크기, 속도 등)을 파라미터화하여 다양한 이펙트를 쉽게 만들 수 있도록 합니다.
*   **재사용성:** 이펙트를 재사용할 수 있도록 설계하여 코드 중복을 줄입니다.

**1.8 다른 클래스와의 상호작용**

*   **`UBossEffectManager`:** 이펙트 풀을 관리하고, 이펙트 인스턴스를 생성/반환합니다.
*   **`UBossEffectComponent`:** 보스 캐릭터에 부착되어 이펙트 재생을 담당합니다.
*   **`UAnimNotify_*`:** 애니메이션 이벤트에 따라 이펙트 재생을 트리거합니다.
*   **`ACBoss`:** 보스 캐릭터 클래스. 이펙트가 부착될 대상입니다.

### 2. `ABossManager` 클래스 분석

**2.1 클래스 개요**

`ABossManager` 클래스는 보스 전투 전체를 관리하는 핵심 클래스입니다. 보스 캐릭터의 스폰, 초기화, 상태 관리, 전투 로직 등을 담당하며, 게임 내에서 단일 인스턴스로 존재할 가능성이 높습니다 (싱글톤 패턴).

**2.2 목적과 책임**

*   **보스 스폰:** 게임 월드에 보스 캐릭터를 스폰합니다. 스폰할 보스 타입은 데이터 테이블 또는 변수를 통해 설정될 수 있습니다.
*   **보스 초기화:** 스폰된 보스 캐릭터의 컴포넌트, StateTree, 스탯 등을 초기화합니다.
*   **보스 상태 관리:** 보스의 현재 상태(HP, 페이즈 등)를 추적하고 관리합니다.
*   **전투 로직 관리:** 보스 전투의 시작, 종료, 페이즈 전환 등을 관리합니다.
*   **UI 관리:** 보스 상태 위젯(HP 바 등)을 표시하고 업데이트합니다.
*   **트리거 이벤트 처리:** 특정 트리거(플레이어 진입 등)에 대한 이벤트를 처리합니다.

**2.3 주요 메서드 분석**

*   `ABossManager()`: 생성자. 보스 매니저의 기본 속성을 초기화합니다.
*   `Tick(float DeltaTime)`: 매 프레임마다 호출되는 함수. 보스 상태 업데이트, 전투 로직 등을 처리합니다.
*   `ResetBossCompletely()`: 보스 캐릭터를 완전 초기화하는 함수. 보스 스폰, 컴포넌트 초기화, StateTree 리스타트 등을 수행합니다. 이 함수는 전투 시작 시 또는 보스 사망 후 재시작 시 호출될 수 있습니다.
*   `OpenDoor()`: 보스 전투 시작 시 문을 여는 함수.
*   `FindBossInWorld()`: 게임 월드에서 보스 캐릭터를 찾는 함수.
*   `ResetAllBossComponents()`: 보스 캐릭터의 모든 컴포넌트를 초기화하는 함수.
*   `ResetBossStateTree()`: 보스 캐릭터의 StateTree를 완전 리스타트하는 함수.
*   `BeginPlay()`: 액터가 게임에 스폰될 때 호출되는 함수. 보스 매니저 초기화 작업을 수행합니다.
*   `OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)`: 트리거 박스에 플레이어가 진입했을 때 호출되는 함수. 보스 전투 시작을 트리거할 수 있습니다.

**2.4 클래스의 생명주기와 상태 관리**

`ABossManager`의 생명주기는 다음과 같습니다.

1.  **생성:** 게임 시작 시 또는 레벨 로드 시 `ABossManager` 인스턴스를 생성합니다.
2.  **초기화:** `BeginPlay` 함수에서 보스 매니저의 기본 속성을 초기화합니다. 데이터 테이블에서 보스 스탯, 스폰할 보스 타입 등을 로드할 수 있습니다.
3.  **보스 스폰:** `ResetBossCompletely` 함수를 통해 보스 캐릭터를 스폰합니다.
4.  **전투 시작:** 트리거 이벤트(`OnTriggerBoxOverlapBegin`) 또는 다른 조건에 따라 보스 전투를 시작합니다.
5.  **전투 진행:** `Tick` 함수에서 보스 상태 업데이트, 전투 로직 등을 처리합니다.
6.  **전투 종료:** 보스 사망 또는 다른 조건에 따라 보스 전투를 종료합니다.
7.  **재시작/소멸:** 보스 전투를 재시작하거나, 레벨을 언로드하는 경우 `ABossManager` 인스턴스를 소멸시킵니다.

`ABossManager`는 보스 전투의 진행 상태, 보스 캐릭터의 HP, 페이즈 등의 상태를 관리합니다.

**2.5 실제 사용 예제와 코드**

```c++
// 보스 스폰 및 초기화 예제
void ABossManager::ResetBossCompletely()
{
    // 기존 보스 캐릭터가 존재하면 파괴
    if (CurrentBoss)
    {
        CurrentBoss->Destroy();
        CurrentBoss = nullptr;
    }

    // 데이터 테이블에서 보스 정보 로드 (예시)
    // FBossStats* BossStats = BossDataTable->FindRow<FBossStats>(BossName, "");

    // 보스 스폰
    if (BossClass) // BossClass는 스폰할 보스 캐릭터의 클래스
    {
        CurrentBoss = GetWorld()->SpawnActor<ACBoss>(BossClass, SpawnLocation, SpawnRotation);
    }

    if (CurrentBoss)
    {
        // 컴포넌트 초기화
        ResetAllBossComponents();

        // StateTree 리스타트
        ResetBossStateTree();

        // UI 업데이트
        // ShowBossStatusWidget();
    }
}

// 트리거 이벤트 처리 예제
void ABossManager::OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (OtherActor && OtherActor->ActorHasTag(TEXT("Player")))
    {
        // 플레이어가 트리거 박스에 진입했을 때
        // 전투 시작 로직 구현
        UE_LOG(LogTemp, Warning, TEXT("Boss Battle Started!"));
        // OpenDoor();
        // PlayBossBGM();
    }
}
```

**2.6 성능 특성과 최적화 포인트**

*   **불필요한 Tick 제거:** 보스 매니저의 상태 업데이트가 필요하지 않은 경우, `Tick` 함수를 비활성화하여 성능을 향상시킬 수 있습니다.
*   **데이터 테이블 캐싱:** 데이터 테이블에서 자주 사용되는 데이터를 캐싱하여 접근 속도를 향상시킬 수 있습니다.
*   **최적화된 보스 스폰:** 보스 스폰 시 불필요한 리소스 로딩을 최소화하여 스폰 시간을 단축할 수 있습니다.

**2.7 사용 시나리오와 베스트 프랙티스**

*   **싱글톤 패턴:** `ABossManager`를 싱글톤 패턴으로 구현하여 게임 내에서 단일 인스턴스만 존재하도록 합니다.
*   **데이터 테이블 활용:** 보스 스탯, 스폰할 보스 타입 등의 데이터를 데이터 테이블에 저장하고 관리합니다.
*   **상태 머신:** 보스 전투의 진행 상태를 상태 머신으로 관리하여 복잡한 전투 로직을 효과적으로 구현합니다.
*   **이벤트 기반 아키텍처:** 보스 상태 변화, 페이즈 전환 등을 이벤트로 발생시켜 다른 시스템과 느슨하게 결합합니다.

**2.8 다른 클래스와의 상호작용**

*   **`ACBoss`:** 보스 캐릭터 클래스. `ABossManager`는 보스 캐릭터를 스폰하고 관리합니다.
*   **`UBossStatusWidget`:** 보스 상태 위젯 클래스. `ABossManager`는 보스 상태 위젯을 표시하고 업데이트합니다.
*   **`UBossStateComponent`:** 보스 상태 관리 컴포넌트 클래스. `ABossManager`는 보스 상태를 추적하고 관리합니다.
*   **`ACBossAIC`:** 보스 AI 컨트롤러 클래스. `ABossManager`는 보스 AI를 초기화하고 관리합니다.
*   **`FEditorPlugin_DataSyncModule`:** 에디터 플러그인 클래스. `ABossManager`에 필요한 데이터 테이블을 동기화하고 관리합니다.

### 3. `ABossProjectileActor` 클래스 분석

**3.1 클래스 개요**

`ABossProjectileActor` 클래스는 보스가 사용하는 투사체를 나타내는 액터 클래스입니다. 언리얼 엔진의 `AActor` 클래스를 상속받아 게임 월드에 배치될 수 있으며, 투사체의 이동, 충돌, 이펙트 재생 등을 담당합니다. 오브젝트 풀링과 함께 사용되어 투사체 생성 및 소멸에 따른 성능 저하를 방지할 가능성이 높습니다.

**3.2 목적과 책임**

*   **투사체 발사:** 지정된 방향 또는 위치로 투사체를 발사합니다.
*   **투사체 이동:** 투사체의 속도, 가속도 등을 제어하여 이동을 구현합니다.
*   **충돌 처리:** 투사체가 다른 액터와 충돌했을 때 충돌 이벤트를 처리합니다.
*   **이펙트 재생:** 투사체 스폰, 소멸, 충돌 시 이펙트를 재생합니다.
*   **투사체 생명주기 관리:** 투사체의 수명을 관리하고, 수명이 다하면 소멸시킵니다.

**3.3 주요 메서드 분석**

*   `ABossProjectileActor()`: 생성자. 투사체의 기본 속성(속도, 데미지, 파티클 시스템 등)을 초기화합니다.
*   `Tick(float DeltaTime)`: 매 프레임마다 호출되는 함수. 투사체의 이동, 수명 등을 업데이트합니다.
*   `FireProjectile(FVector Direction)`: 지정된 방향으로 투사체를 발사하는 함수.
*   `FireProjectileToLocation(FVector Location)`: 지정된 위치로 투사체를 발사하는 함수.
*   `PlaySpawnEffect()`: 투사체가 스폰될 때 이펙트를 재생하는 함수.
*   `PlayDestroyEffect()`: 투사체가 소멸될 때 이펙트를 재생하는 함수.
*   `OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)`: 투사체가 다른 액터와 충돌했을 때 호출되는 함수. 데미지 처리, 이펙트 재생 등을 수행합니다.
*   `BeginPlay()`: 액터가 게임에 스폰될 때 호출되는 함수. 투사체 초기화 작업을 수행합니다.

**3.4 클래스의 생명주기와 상태 관리**

`ABossProjectileActor`의 생명주기는 다음과 같습니다.

1.  **생성:** `UBossProjectileComponent` 또는 다른 클래스에서 `NewObject` 등을 사용하여 `ABossProjectileActor` 인스턴스를 생성합니다. 오브젝트 풀링을 사용하는 경우, 풀에서 사용 가능한 투사체를 가져옵니다.
2.  **초기화:** `BeginPlay` 함수에서 투사체의 기본 속성을 초기화합니다.
3.  **발사:** `FireProjectile` 또는 `FireProjectileToLocation` 함수를 호출하여 투사체를 발사합니다.
4.  **이동 및 업데이트:** `Tick` 함수에서 투사체의 이동, 수명 등을 업데이트합니다.
5.  **충돌 처리:** `OnProjectileHit` 함수에서 투사체가 다른 액터와 충돌했을 때 충돌 이벤트를 처리합니다.
6.  **소멸/반환:** 투사체의 수명이 다하거나, 다른 조건에 따라 투사체를 소멸시키거나 풀로 반환합니다.

`ABossProjectileActor`는 활성화/비활성화 상태를 가지며, 투사체의 이동, 충돌 여부, 수명 등을 상태로 관리합니다.

**3.5 실제 사용 예제와 코드**

```c++
// 투사체 발사 예제
void ABossProjectileActor::FireProjectile(FVector Direction)
{
    // 투사체의 초기 속도 설정
    ProjectileMovementComponent->Velocity = Direction * InitialSpeed;

    // 스폰 이펙트 재생
    PlaySpawnEffect();
}

// 충돌 처리 예제
void ABossProjectileActor::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // 데미지 처리
    UGameplayStatics::ApplyPointDamage(OtherActor, DamageAmount, NormalImpulse, Hit, GetInstigatorController(), this, DamageType);

    // 충돌 이펙트 재생
    PlayDestroyEffect();

    // 투사체 소멸 (오브젝트 풀링 사용 시 풀로 반환)
    Destroy();
}
```

**3.6 성능 특성과 최적화 포인트**

*   **오브젝트 풀링:** 투사체 생성/소멸 비용을 줄이기 위해 오브젝트 풀링을 사용하는 것이 좋습니다.
*   **충돌 처리 최적화:** 불필요한 충돌 검사를 피하고, 충돌 채널을 적절하게 설정하여 성능을 향상시킬 수 있습니다.
*   **파티클 시스템 최적화:** 파티클 시스템의 파티클 수, 렌더링 비용 등을 최적화하여 성능을 향상시킬 수 있습니다.
*   **불필요한 Tick 제거:** 투사체의 상태 업데이트가 필요하지 않은 경우, `Tick` 함수를 비활성화하여 성능을 향상시킬 수 있습니다.

**3.7 사용 시나리오와 베스트 프랙티스**

*   **원거리 공격:** 보스가 플레이어를 공격하기 위해 투사체를 발사합니다.
*   **패턴 공격:** 보스가 특정 패턴으로 투사체를 발사합니다.
*   **특수 능력:** 보스가 특수 능력을 사용할 때 투사체를 발사합니다.

**베스트 프랙티스:**

*   **투사체 종류별 클래스 분리:** 투사체의 종류(직선, 곡선, 유도 등)에 따라 클래스를 분리하여 코드 재사용성을 높입니다.
*   **파라미터화:** 투사체의 속도, 데미지, 이펙트 등을 파라미터화하여 다양한 투사체를 쉽게 만들 수 있도록 합니다.
*   **오브젝트 풀링 사용:** 투사체 생성/소멸 비용을 줄이기 위해 오브젝트 풀링을 사용합니다.

**3.8 다른 클래스와의 상호작용**

*   **`UBossProjectileComponent`:** 투사체 풀을 관리하고, 투사체 인스턴스를 생성/반환합니다.
*   **`ACBoss`:** 보스 캐릭터 클래스. 투사체를 발사하는 주체입니다.
*   **`ACharacter`:** 플레이어 캐릭터 클래스. 투사체의 타겟이 될 수 있습니다.

### 4. `ABossProjectileOrb` 클래스 분석

**4.1 클래스 개요**

`ABossProjectileOrb` 클래스는 보스가 사용하는 투사체 중에서도 특정한 형태인 "오브(Orb)" 형태의 투사체를 나타내는 액터 클래스입니다.  `ABossProjectileActor`와 마찬가지로 `AActor`를 상속받아 월드에 배치되며, 오브의 이동, 충돌, 특수한 행동(예: 플레이어 주변을 맴돌기, 폭발 등) 및 이펙트 재생을 담당합니다.  `ABossProjectileActor`보다 더 복잡한 로직과 시각적 효과를 가질 가능성이 높습니다.

**4.2 목적과 책임**

*   **오브 발사 및 활성화:** 특정 위치 또는 패턴에 따라 오브를 생성하고 활성화합니다.
*   **오브 이동:** 오브의 속도, 궤적, 특수한 움직임(예: 플레이어 추적, 회전 등)을 제어합니다.
*   **충돌 처리:** 오브가 다른 액터와 충돌했을 때의 로직을 처리합니다 (데미지, 이펙트, 특수 효과 등).
*   **특수 행동:** 오브가 특정 조건(예: 시간이 지남, 플레이어와의 거리 등)에 따라 특수한 행동(예: 폭발, 분열, 다른 투사체 생성 등)을 수행하도록 합니다.
*   **사운드 재생:** 오브의 스폰, 충돌, 소멸 시 관련 사운드를 재생합니다.
*   **이펙트 재생:** 오브의 스폰, 충돌, 소멸, 특수 행동 시 관련 이펙트를 재생합니다.
*   **오브 생명주기 관리:** 오브의 수명을 관리하고, 수명이 다하면 오브를 소멸시키거나 오브젝트 풀로 반환합니다.

**4.3 주요 메서드 분석**

*   `ABossProjectileOrb()`: 생성자. 오브의 기본 속성(속도, 데미지, 궤적, 파티클 시스템, 사운드 큐 등)을 초기화합니다.
*   `Tick(float DeltaTime)`: 매 프레임마다 호출되는 함수. 오브의 이동, 수명, 특수 행동 조건 등을 업데이트합니다.
*   `SpawnProjectile()`: 오브에서 다른 투사체를 생성하는 함수 (예: 오브가 폭발하면서 작은 투사체들을 흩뿌리는 경우).
*   `DestroyOrb()`: 오브를 즉시 소멸시키는 함수.
*   `OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)`: 오브가 다른 액터와 겹쳤을 때 호출되는 함수. 충돌 처리 로직을 구현합니다.
*   `PlaySpawnSound()`: 오브가 스폰될 때 사운드를 재생하는 함수.
*   `PlayReturnToPoolSound()`: 오브가 오브젝트 풀로 반환될 때 사운드를 재생하는 함수.
*   `PlayCollisionSound()`: 오브가 충돌했을 때 사운드를 재생하는 함수.
*   `PlaySpawnEffect()`: 오브가 스폰될 때 이펙트를 재생하는 함수.
*   `PlayReturnToPoolEffect()`: 오브가 오브젝트 풀로 반환될 때 이펙트를 재생하는 함수.
*   `PlayCollisionEffect()`: 오브가 충돌했을 때 이펙트를 재생하는 함수.
*   `DestroyOrbWithDelay(float Delay)`: 지정된 시간 후에 오브를 소멸시키는 함수.
*   `ActivateOrb()`: 오브를 활성화하는 함수 (이동 시작, 특수 행동 시작 등).
*   `BeginPlay()`: 액터가 게임에 스폰될 때 호출되는 함수. 오브 초기화 작업을 수행합니다.

**4.4 클래스의 생명주기와 상태 관리**

`ABossProjectileOrb`의 생명주기는 다음과 같습니다.

1.  **생성:** `UBossProjectileComponent` 또는 다른 클래스에서 `NewObject` 등을 사용하여 `ABossProjectileOrb` 인스턴스를 생성합니다. 오브젝트 풀링을 사용하는 경우, 풀에서 사용 가능한 오브를 가져옵니다.
2.  **초기화:** `BeginPlay` 함수에서 오브의 기본 속성을 초기화합니다.
3.  **활성화:** `ActivateOrb` 함수를 호출하여 오브를 활성화합니다.
4.  **이동 및 업데이트:** `Tick` 함수에서 오브의 이동, 수명, 특수 행동 조건 등을 업데이트합니다.
5.  **충돌 처리:** `OnOverlap` 함수에서 오브가 다른 액터와 충돌했을 때 충돌 이벤트를 처리합니다.
6.  **특수 행동:** `Tick` 함수 또는 다른 이벤트 핸들러에서 오브의 특수 행동을 트리거합니다.
7.  **소멸/반환:** 오브의 수명이 다하거나, 다른 조건에 따라 오브를 소멸시키거나 풀로 반환합니다.

`ABossProjectileOrb`는 활성화/비활성화 상태, 이동 상태, 특수 행동 상태 등을 상태로 관리합니다.

**4.5 실제 사용 예제와 코드**

```c++
// 오브 활성화 예제
void ABossProjectileOrb::ActivateOrb()
{
    // 이동 시작
    ProjectileMovementComponent->Activate();
    ProjectileMovementComponent->Velocity = InitialDirection * InitialSpeed;

    // 스폰 사운드 및 이펙트 재생
    PlaySpawnSound();
    PlaySpawnEffect();
}

// 충돌 처리 예제
void ABossProjectileOrb::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    // 데미지 처리
    UGameplayStatics::ApplyPointDamage(OtherActor, DamageAmount, SweepResult.ImpactPoint, SweepResult, GetInstigatorController(), this, DamageType);

    // 충돌 사운드 및 이펙트 재생
    PlayCollisionSound();
    PlayCollisionEffect();

    // 오브 소멸 (오브젝트 풀링 사용 시 풀로 반환)
    DestroyOrb();
}

// 특수 행동 예제 (일정 시간 후 폭발)
void ABossProjectileOrb::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    LifeTime += DeltaTime;

    if (LifeTime >= ExplosionDelay)
    {
        // 폭발 로직 구현 (다른 투사체 생성, 이펙트 재생 등)
        SpawnProjectile();

        // 오브 소멸
        DestroyOrb();
    }
}
```

**4.6 성능 특성과 최적화 포인트**

*   **오브젝트 풀링:** 오브 생성/소멸 비용을 줄이기 위해 오브젝트 풀링을 사용하는 것이 좋습니다.
*   **복잡한 이동 로직 최적화:** 오브의 이동 로직(추적, 회전 등)이 복잡한 경우, 성능 프로파일링을 통해 병목 지점을 찾고 최적화합니다.
*   **파티클 시스템 최적화:** 오브의 파티클 시스템의 파티클 수, 렌더링 비용 등을 최적화하여 성능을 향상시킬 수 있습니다.
*   **사운드 최적화:** 오브의 사운드 큐의 압축 포맷, 샘플 레이트 등을 최적화하여 메모리 사용량을 줄일 수 있습니다.
*   **불필요한 Tick 제거:** 오브의 상태 업데이트가 필요하지 않은 경우, `Tick` 함수를 비활성화하여 성능을 향상시킬 수 있습니다.

**4.7 사용 시나리오와 베스트 프랙티스**

*   **유도 투사체:** 플레이어를 추적하는 오브를 발사합니다.
*   **패턴 공격:** 특정 패턴으로 오브를 발사합니다 (예: 나선형, 원형 등).
*   **특수 능력:** 오브를 사용하여 플레이어에게 디버프를 걸거나, 특정 영역을 봉쇄합니다.
*   **폭발 공격:** 오브가 일정 시간 후 폭발하여 주변에 데미지를 줍니다.

**베스트 프랙티스:**

*   **오브의 행동 패턴 분리:** 오브의 이동, 충돌, 특수 행동 등을 독립적인 함수 또는 컴포넌트로 분리하여 코드 재사용성을 높입니다.
*   **파라미터화:** 오브의 속도, 데미지, 궤적, 이펙트 등을 파라미터화하여 다양한 오브를 쉽게 만들 수 있도록 합니다.
*   **오브젝트 풀링 사용:** 오브 생성/소멸 비용을 줄이기 위해 오브젝트 풀링을 사용합니다.
*   **시각적 피드백:** 오브의 행동에 대한 명확한 시각적 피드백(이펙트, 사운드)을 제공하여 플레이어가 오브를 쉽게 인지하고 대처할 수 있도록 합니다.

**4.8 다른 클래스와의 상호작용**

*   **`UBossProjectileComponent`:** 오브 풀을 관리하고, 오브 인스턴스를 생성/반환합니다.
*   **`ACBoss`:** 보스 캐릭터 클래스. 오브를 발사하는 주체입니다.
*   **`ACharacter`:** 플레이어 캐릭터 클래스. 오브의 타겟이 될 수 있습니다.

### 5. `ACBoss` 클래스 분석

**5.1 클래스 개요**

`ACBoss` 클래스는 게임 내 보스 캐릭터의 기본적인 행동과 속성을 정의하는 핵심 클래스입니다. 언리얼 엔진의 `ACharacter` 클래스를 상속받아 기본적인 캐릭터 이동, 애니메이션 재생, 데미지 처리 등의 기능을 제공하며, 보스 캐릭터 특유의 AI, 공격 패턴, 스탯 등을 구현합니다.

**5.2 목적과 책임**

*   **기본 캐릭터 기능 제공:** 이동, 점프, 애니메이션 재생 등 `ACharacter` 클래스의 기본 기능을 제공합니다.
*   **보스 AI 구현:** `ACBossAIC`를 사용하여 보스 캐릭터의 AI를 제어합니다.
*   **공격 패턴 구현:** 보스 캐릭터의 다양한 공격 패턴을 구현하고 관리합니다.
*   **스탯 관리:** 보스 캐릭터의 HP, 공격력, 방어력 등의 스탯을 관리합니다.
*   **데미지 처리:** 보스 캐릭터가 받는 데미지를 처리하고, HP를 업데이트합니다.
*   **애니메이션 제어:** 보스 캐릭터의 애니메이션을 제어하고, 애니메이션 이벤트(애니메이션 노티파이)를 처리합니다.
*   **UI 표시:** 보스 캐릭터의 상태 위젯(HP 바 등)을 표시하고 업데이트합니다.
*   **사운드 재생:** 보스 캐릭터의 관련 사운드(공격, 피격, 사망 등)를 재생합니다.

**5.3 주요 메서드 분석**

*   `ACBoss()`: 생성자. 보스 캐릭터의 기본 속성(스탯, AI 컨트롤러, 애니메이션 블루프린트 등)을 초기화합니다.
*   `Tick(float DeltaTime)`: 매 프레임마다 호출되는 함수. 보스 캐릭터의 상태 업데이트, AI 제어, 애니메이션 업데이트 등을 처리합니다.
*   `TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)`: 보스 캐릭터가 데미지를 받았을 때 호출되는 함수. 데미지를 처리하고, HP를 업데이트하며, 피격 애니메이션을 재생합니다.
*   `BeginPlay()`: 액터가 게임에 스폰될 때 호출되는 함수. 보스 캐릭터 초기화 작업을 수행합니다.
*   `PlayHitMotion()`: 보스 캐릭터가 피격 당했을 때 재생하는 애니메이션 몽타주를 실행하는 함수.
*   `ShowBossStatusWidget()`: 보스 캐릭터의 상태 위젯(HP 바 등)을 표시하는 함수.
*   `HPUpdate()`: 보스 캐릭터의 HP를 업데이트하고, 상태 위젯에 반영하는 함수.
*   `RestartUI()`: 보스 캐릭터의 UI를 재시작하는 함수.
*   `PlayBossBGM()`: 보스 캐릭터의 배경 음악을 재생하는 함수.
*   `StopBossBGM()`: 보스 캐릭터의 배경 음악을 정지하는 함수.
*   `LowerBossBGMVolume()`: 보스 캐릭터 사망 시 배경 음악 볼륨을 낮추는 함수.

**5.4 클래스의 생명주기와 상태 관리**

`ACBoss`의 생명주기는 다음과 같습니다.

1.  **생성:** `ABossManager` 또는 다른 클래스에서 `SpawnActor` 등을 사용하여 `ACBoss` 인스턴스를 생성합니다.
2.  **초기화:** `BeginPlay` 함수에서 보스 캐릭터의 기본 속성을 초기화합니다.
3.  **AI 제어:** `ACBossAIC`를 사용하여 보스 캐릭터의 AI를 제어합니다.
4.  **전투 진행:** `Tick` 함수에서 보스 캐릭터

## 클래스별 상세 분석

지금부터 위에서 나열된 클래스들을 기능과 역할 중심으로 상세하게 분석하겠습니다. 각 클래스별로 목적, 주요 메서드 및 프로퍼티, 생명주기, 사용 예제, 성능, 확장성, 유지보수성 등을 자세히 살펴보겠습니다.

### 1. `ABossEffect`

*   **클래스 개요:** `ABossEffect`는 보스 캐릭터와 관련된 시각 효과(파티클, 머티리얼 변화 등)를 관리하는 액터 클래스입니다. 이펙트의 활성화, 비활성화, 특정 소켓에 부착 등의 기능을 제공하여, 보스의 다양한 행동에 따른 시각적인 피드백을 제공합니다.

*   **목적과 책임:**
    *   보스 이펙트의 생성 및 소멸 관리.
    *   이펙트의 활성화 및 비활성화 제어.
    *   보스 캐릭터의 특정 소켓에 이펙트를 부착.
    *   현재 이펙트의 상태(활성화 여부, 태그) 정보 제공.

*   **주요 메서드 및 프로퍼티:**

    *   `ActivateEffect()`: 이펙트를 활성화합니다.
    *   `ActivateEffectAttachedToSocket(FName SocketName)`: 지정된 소켓에 이펙트를 부착하고 활성화합니다.  `SocketName`은 이펙트를 부착할 소켓의 이름입니다.
    *   `DeactivateEffect()`: 이펙트를 비활성화합니다.
    *   `IsActive()`: 이펙트가 활성화되어 있는지 여부를 반환합니다.
    *   `GetCurrentEffectTag()`: 현재 이펙트의 태그를 반환합니다. 이 태그는 이펙트의 종류나 상태를 식별하는 데 사용됩니다.
    *   `AttachToBoss()`: 이펙트를 보스 캐릭터에 부착합니다.
    *   `AttachToSocket(FName SocketName)`: 이펙트를 지정된 소켓에 부착합니다.
    *   `PlaceInWorld(FVector Location, FRotator Rotation)`: 이펙트를 월드 공간의 특정 위치와 회전에 배치합니다.

*   **클래스의 생명주기:**

    1.  **생성:** `UBossEffectManager`에 의해 필요에 따라 풀링되어 생성됩니다.
    2.  **초기화:** `BeginPlay()`에서 초기화됩니다.
    3.  **활성화/비활성화:** `ActivateEffect()` 및 `DeactivateEffect()`를 통해 활성화 및 비활성화됩니다.
    4.  **소멸:** `UBossEffectManager`에 의해 관리되며, 필요에 따라 풀로 반환되거나 소멸됩니다.

*   **사용 예제 및 코드:**

    ```c++
    // 헤더 파일 (BossEffect.h)
    #include "CoreMinimal.h"
    #include "GameFramework/Actor.h"
    #include "BossEffect.generated.h"

    UCLASS()
    class MYPROJECT_API ABossEffect : public AActor
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

    	UFUNCTION(BlueprintCallable, Category = "BossEffect")
    	void ActivateEffect();

    	UFUNCTION(BlueprintCallable, Category = "BossEffect")
    	void ActivateEffectAttachedToSocket(FName SocketName);

    	UFUNCTION(BlueprintCallable, Category = "BossEffect")
    	void DeactivateEffect();

    	UFUNCTION(BlueprintCallable, Category = "BossEffect")
    	bool IsActive() const;

    	UFUNCTION(BlueprintCallable, Category = "BossEffect")
    	FName GetCurrentEffectTag() const;

    private:
    	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
    	bool bIsActive;

    	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = "true"))
    	FName EffectTag;

    };

    // 소스 파일 (BossEffect.cpp)
    #include "BossEffect.h"

    ABossEffect::ABossEffect()
    {
    	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    	PrimaryActorTick.bCanEverTick = true;

    }

    void ABossEffect::BeginPlay()
    {
    	Super::BeginPlay();

    }

    void ABossEffect::Tick(float DeltaTime)
    {
    	Super::Tick(DeltaTime);

    }

    void ABossEffect::ActivateEffect()
    {
    	bIsActive = true;
    	// 이펙트 활성화 로직 (파티클 시스템 활성화, 머티리얼 변경 등)
    	UE_LOG(LogTemp, Warning, TEXT("Effect Activated!"));
    }

    void ABossEffect::ActivateEffectAttachedToSocket(FName SocketName)
    {
    	// 소켓에 부착하는 로직
    	bIsActive = true;
    	UE_LOG(LogTemp, Warning, TEXT("Effect Activated at Socket: %s!"), *SocketName.ToString());
    }

    void ABossEffect::DeactivateEffect()
    {
    	bIsActive = false;
    	// 이펙트 비활성화 로직 (파티클 시스템 비활성화, 머티리얼 복구 등)
    	UE_LOG(LogTemp, Warning, TEXT("Effect Deactivated!"));
    }

    bool ABossEffect::IsActive() const
    {
    	return bIsActive;
    }

    FName ABossEffect::GetCurrentEffectTag() const
    {
    	return EffectTag;
    }

    ```

    ```c++
    // UBossEffectManager에서 이펙트 사용 예제
    ABossEffect* Effect = EffectManager->GetEffectFromPool(EffectTag);
    if (Effect)
    {
        Effect->AttachToSocket(SocketName);
        Effect->ActivateEffectAttachedToSocket(SocketName);
    }
    ```

*   **성능 특성 및 최적화 포인트:**

    *   액터 생성 및 소멸은 비용이 많이 들기 때문에, `UBossEffectManager`를 통해 오브젝트 풀링을 사용하여 성능을 최적화합니다.
    *   `Tick()` 함수 내에서 불필요한 연산을 최소화합니다. 이펙트의 상태가 변경될 때만 업데이트하도록 구현합니다.
    *   파티클 시스템의 복잡도를 줄여 GPU 부하를 줄입니다.

*   **사용 시나리오 및 베스트 프랙티스:**

    *   보스의 공격 시, 특정 스킬 사용 시, 피격 시 등 다양한 상황에서 시각적인 피드백을 제공하는 데 사용됩니다.
    *   이펙트의 종류를 태그로 관리하여, 다양한 이펙트를 쉽게 관리하고 재사용할 수 있도록 합니다.
    *   `UBossEffectManager`를 통해 이펙트를 효율적으로 관리합니다.

*   **다른 클래스와의 상호작용:**

    *   `UBossEffectManager`: 이펙트의 풀링 및 관리를 담당합니다.
    *   `ACBoss`: 보스 캐릭터의 액터이며, 이펙트가 부착될 대상입니다.
    *   `UBossAnimInstance`: 애니메이션 이벤트에 따라 이펙트를 트리거합니다.

*   **설계 패턴:**

    *   **Object Pooling:** `UBossEffectManager`에서 이펙트를 풀링하여 액터 생성 및 소멸 비용을 줄입니다.

*   **확장성 및 유지보수성:**

    *   새로운 이펙트를 추가하기 쉽도록 설계되었습니다. 이펙트의 태그를 추가하고, `UBossEffectManager`에서 해당 태그에 대한 이펙트를 풀링하도록 설정하면 됩니다.
    *   코드의 가독성을 높이기 위해, 각 메서드의 역할을 명확하게 정의하고 주석을 충분히 작성합니다.

*   **실무 적용 가이드:**

    *   이펙트의 종류를 태그로 관리하고, 데이터 테이블을 사용하여 이펙트의 속성을 정의합니다.
    *   `UBossEffectManager`를 싱글톤으로 구현하여, 어디서든 쉽게 이펙트를 관리할 수 있도록 합니다.
    *   이펙트의 활성화 및 비활성화 로직을 `ABossEffect` 클래스 내부에 캡슐화하여, 외부 클래스에서 이펙트의 내부 동작을 알 필요가 없도록 합니다.

### 2. `ABossManager`

*   **클래스 개요:** `ABossManager`는 게임 월드 내의 보스 캐릭터를 생성, 관리, 초기화하는 역할을 담당하는 액터 클래스입니다. 보스 전투의 시작과 종료, 보스 캐릭터의 상태 초기화, UI 관리 등 보스 전투의 전반적인 흐름을 제어합니다.

*   **목적과 책임:**

    *   보스 캐릭터의 생성 및 소멸 관리.
    *   보스 캐릭터의 초기 상태 설정 및 초기화.
    *   보스 전투 시작 및 종료 시퀀스 제어.
    *   보스 관련 UI(HP 바 등) 관리.
    *   보스 전투 영역 관리 (문 개폐 등).
    *   보스 AI 초기화 및 재시작.

*   **주요 메서드 및 프로퍼티:**

    *   `ResetBossCompletely()`: 보스 캐릭터를 완전히 초기화합니다. 이는 보스 캐릭터의 HP, 상태, AI 등을 초기 상태로 되돌리는 것을 포함합니다.
    *   `OpenDoor()`: 보스 전투 영역의 문을 엽니다.
    *   `FindBossInWorld()`: 월드에서 보스 캐릭터를 찾습니다.
    *   `ResetAllBossComponents()`: 보스 캐릭터의 각 컴포넌트(HP, AI, 상태 등)를 초기화합니다.
    *   `ResetBossStateTree()`: 보스 AI의 StateTree를 완전히 재시작합니다.
    *   `OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)`: 플레이어가 보스 전투 영역에 진입했을 때 호출되는 함수입니다. 이 함수는 보스 전투를 시작하는 데 사용될 수 있습니다.
    *   `BossClass`: 스폰할 보스 캐릭터의 클래스입니다.

*   **클래스의 생명주기:**

    1.  **생성:** 게임 월드에 배치되거나, 게임 시작 시 스폰됩니다.
    2.  **초기화:** `BeginPlay()`에서 보스 캐릭터를 찾거나 스폰하고, 초기 상태를 설정합니다.
    3.  **전투 관리:** 플레이어의 진입을 감지하고, 보스 전투를 시작합니다. 보스 캐릭터의 상태 변화에 따라 UI를 업데이트하고, 필요에 따라 보스 캐릭터를 초기화합니다.
    4.  **종료:** 보스 캐릭터가 사망하거나, 전투가 종료되면 보스 관련 리소스를 정리합니다.

*   **사용 예제 및 코드:**

    ```c++
    // 헤더 파일 (BossManager.h)
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

    	UFUNCTION(BlueprintCallable, Category = "BossManager")
    	void ResetBossCompletely();

    	UFUNCTION(BlueprintCallable, Category = "BossManager")
    	void OpenDoor();

    private:
    	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boss")
    	TSubclassOf<class ACBoss> BossClass;

    	ACBoss* SpawnedBoss;

    	UFUNCTION()
    	void OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

    };

    // 소스 파일 (BossManager.cpp)
    #include "BossManager.h"
    #include "CBoss.h"
    #include "Components/BoxComponent.h"

    ABossManager::ABossManager()
    {
    	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    	PrimaryActorTick.bCanEverTick = true;

    	// Box 컴포넌트 생성 및 설정 (트리거 역할을 수행)
    	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
    	RootComponent = TriggerBox;
    	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    	TriggerBox->SetCollisionObjectType(ECC_WorldStatic);
    	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // Pawn과의 Overlap 이벤트 활성화
    	TriggerBox->SetBoxExtent(FVector(500.0f, 500.0f, 200.0f)); // Box 크기 설정

    }

    void ABossManager::BeginPlay()
    {
    	Super::BeginPlay();

    	// 보스 스폰
    	if (BossClass)
    	{
    		FActorSpawnParameters SpawnParams;
    		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    		SpawnedBoss = GetWorld()->SpawnActor<ACBoss>(BossClass, GetActorLocation(), GetActorRotation(), SpawnParams);
    	}

    	// Overlap 이벤트 바인딩
    	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABossManager::OnTriggerBoxOverlapBegin);

    }

    void ABossManager::Tick(float DeltaTime)
    {
    	Super::Tick(DeltaTime);

    }

    void ABossManager::ResetBossCompletely()
    {
    	if (SpawnedBoss)
    	{
    		SpawnedBoss->Destroy(); // 기존 보스 파괴

    		FActorSpawnParameters SpawnParams;
    		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    		SpawnedBoss = GetWorld()->SpawnActor<ACBoss>(BossClass, GetActorLocation(), GetActorRotation(), SpawnParams); // 새로운 보스 스폰
    	}
    }

    void ABossManager::OpenDoor()
    {
    	// 문 여는 로직
    	UE_LOG(LogTemp, Warning, TEXT("Door Opened!"));
    }

    void ABossManager::OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
    {
    	// 플레이어 감지 로직
    	UE_LOG(LogTemp, Warning, TEXT("Player Entered Boss Area!"));
    }
    ```

    ```c++
    // 보스 초기화 예제
    BossManager->ResetBossCompletely();
    ```

*   **성능 특성 및 최적화 포인트:**

    *   `Tick()` 함수 내에서 불필요한 연산을 최소화합니다. 보스 캐릭터의 상태가 변경될 때만 UI를 업데이트하도록 구현합니다.
    *   보스 캐릭터의 스폰 및 소멸은 비용이 많이 들기 때문에, 필요에 따라 오브젝트 풀링을 사용하여 성능을 최적화할 수 있습니다.

*   **사용 시나리오 및 베스트 프랙티스:**

    *   보스 전투 시작 및 종료 시퀀스를 제어하는 데 사용됩니다.
    *   보스 캐릭터의 상태 변화에 따라 UI를 업데이트하는 데 사용됩니다.
    *   보스 전투 영역을 관리하는 데 사용됩니다.

*   **다른 클래스와의 상호작용:**

    *   `ACBoss`: 보스 캐릭터의 액터입니다.
    *   `UBossStatusWidget`: 보스 캐릭터의 HP 바 등 UI를 관리합니다.
    *   `ACBossAIC`: 보스 캐릭터의 AI를 제어합니다.

*   **설계 패턴:**

    *   **Factory Pattern:** `ABossManager`는 `BossClass`를 사용하여 보스 캐릭터를 생성합니다.

*   **확장성 및 유지보수성:**

    *   새로운 보스 캐릭터를 추가하기 쉽도록 설계되었습니다. `BossClass`를 변경하고, 필요한 설정을 변경하면 됩니다.
    *   코드의 가독성을 높이기 위해, 각 메서드의 역할을 명확하게 정의하고 주석을 충분히 작성합니다.

*   **실무 적용 가이드:**

    *   보스 캐릭터의 스폰 위치, 초기 상태, UI 등을 데이터 테이블에 정의하고, `ABossManager`에서 해당 데이터를 로드하여 보스 캐릭터를 초기화합니다.
    *   보스 전투 시작 및 종료 시퀀스를 블루프린트 시퀀스로 구현하여, 쉽게 수정하고 확장할 수 있도록 합니다.
    *   보스 캐릭터의 상태 변화를 이벤트로 발생시켜, UI를 업데이트하고, 다른 시스템과 연동할 수 있도록 합니다.

### 3. `ABossProjectileActor`

*   **클래스 개요:** `ABossProjectileActor`는 보스 캐릭터가 발사하는 투사체의 기본 클래스입니다. 투사체의 발사, 소멸, 충돌 처리, 이펙트 재생 등의 기능을 제공합니다.

*   **목적과 책임:**

    *   투사체의 발사 및 이동 관리.
    *   투사체의 충돌 처리.
    *   투사체의 소멸 관리.
    *   투사체 관련 시각 효과 및 사운드 재생.

*   **주요 메서드 및 프로퍼티:**

    *   `FireProjectile(FVector Direction)`: 지정된 방향으로 투사체를 발사합니다.
    *   `FireProjectileToLocation(FVector TargetLocation)`: 지정된 위치로 투사체를 발사합니다.
    *   `PlaySpawnEffect()`: 투사체가 생성될 때 재생되는 이펙트를 재생합니다.
    *   `PlayDestroyEffect()`: 투사체가 소멸될 때 재생되는 이펙트를 재생합니다.
    *   `OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)`: 투사체가 다른 액터와 충돌했을 때 호출되는 함수입니다.
    *   `ProjectileMovementComponent`: 투사체의 이동을 담당하는 컴포넌트입니다.

*   **클래스의 생명주기:**

    1.  **생성:** `UBossProjectileComponent`에 의해 필요에 따라 풀링되어 생성됩니다.
    2.  **초기화:** `BeginPlay()`에서 초기화됩니다.
    3.  **발사:** `FireProjectile()` 또는 `FireProjectileToLocation()`를 통해 발사됩니다.
    4.  **이동:** `ProjectileMovementComponent`에 의해 이동합니다.
    5.  **충돌:** `OnProjectileHit()`에서 충돌 처리를 수행합니다.
    6.  **소멸:** 충돌하거나, 일정 시간이 지나면 소멸됩니다. `UBossProjectileComponent`에 의해 관리되며, 필요에 따라 풀로 반환되거나 소멸됩니다.

*   **사용 예제 및 코드:**

    ```c++
    // 헤더 파일 (BossProjectileActor.h)
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

    protected:
    	// Called when the game starts or when spawned
    	virtual void BeginPlay() override;

    	UFUNCTION()
    	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    public:
    	// Called every frame
    	virtual void Tick(float DeltaTime) override;

    	UFUNCTION(BlueprintCallable, Category = "Projectile")
    	void FireProjectile(FVector Direction);

    	UFUNCTION(BlueprintCallable, Category = "Projectile")
    	void FireProjectileToLocation(FVector TargetLocation);

    	UFUNCTION(BlueprintCallable, Category = "Projectile")
    	void PlaySpawnEffect();

    	UFUNCTION(BlueprintCallable, Category = "Projectile")
    	void PlayDestroyEffect();

    	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    	UProjectileMovementComponent* ProjectileMovementComponent;

    };

    // 소스 파일 (BossProjectileActor.cpp)
    #include "BossProjectileActor.h"
    #include "Components/SphereComponent.h"

    ABossProjectileActor::ABossProjectileActor()
    {
    	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    	PrimaryActorTick.bCanEverTick = true;

    	// Sphere 컴포넌트 생성 및 설정 (충돌 감지)
    	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    	RootComponent = SphereComponent;
    	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    	SphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
    	SphereComponent->SetCollisionResponseToAllChannels(ECR_Block);

    	// ProjectileMovementComponent 생성 및 설정 (투사체 이동)
    	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    	ProjectileMovementComponent->InitialSpeed = 3000.0f;
    	ProjectileMovementComponent->MaxSpeed = 3000.0f;
    	ProjectileMovementComponent->bRotationFollowsVelocity = true;
    	ProjectileMovementComponent->bShouldBounce = false;

    	// 충돌 이벤트 바인딩
    	SphereComponent->OnComponentHit.AddDynamic(this, &ABossProjectileActor::OnProjectileHit);

    }

    void ABossProjectileActor::BeginPlay()
    {
    	Super::BeginPlay();

    }

    void ABossProjectileActor::Tick(float DeltaTime)
    {
    	Super::Tick(DeltaTime);

    }

    void ABossProjectileActor::FireProjectile(FVector Direction)
    {
    	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
    	UE_LOG(LogTemp, Warning, TEXT("Projectile Fired!"));
    }

    void ABossProjectileActor::FireProjectileToLocation(FVector TargetLocation)
    {
    	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
    	FireProjectile(Direction);
    }

    void ABossProjectileActor::PlaySpawnEffect()
    {
    	// 스폰 이펙트 재생 로직
    	UE_LOG(LogTemp, Warning, TEXT("Spawn Effect Played!"));
    }

    void ABossProjectileActor::PlayDestroyEffect()
    {
    	// 소멸 이펙트 재생 로직
    	UE_LOG(LogTemp, Warning, TEXT("Destroy Effect Played!"));
    }

    void ABossProjectileActor::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
    {
    	// 충돌 처리 로직
    	UE_LOG(LogTemp, Warning, TEXT("Projectile Hit!"));
    	PlayDestroyEffect();
    	Destroy();
    }
    ```

    ```c++
    // UBossProjectileComponent에서 투사체 사용 예제
    ABossProjectileActor* Projectile = ProjectileComponent->GetProjectileFromPool();
    if (Projectile)
    {
        Projectile->SetActorLocation(SpawnLocation);
        Projectile->FireProjectileToLocation(TargetLocation);
    }
    ```

*   **성능 특성 및 최적화 포인트:**

    *   액터 생성 및 소멸은 비용이 많이 들기 때문에, `UBossProjectileComponent`를 통해 오브젝트 풀링을 사용하여 성능을 최적화합니다.
    *   `Tick()` 함수 내에서 불필요한 연산을 최소화합니다.
    *   충돌 감지 방식을 최적화합니다. 필요에 따라 채널 기반 충돌 감지 또는 오브젝트 타입 기반 충돌 감지를 사용합니다.
    *   투사체의 이동 거리를 제한하여, 불필요한 연산을 줄입니다.

*   **사용 시나리오 및 베스트 프랙티스:**

    *   보스 캐릭터의 원거리 공격에 사용됩니다.
    *   투사체의 종류에 따라 다양한 공격 패턴을 구현할 수 있습니다.
    *   `UBossProjectileComponent`를 통해 투사체를 효율적으로 관리합니다.

*   **다른 클래스와의 상호작용:**

    *   `UBossProjectileComponent`: 투사체의 풀링 및 관리를 담당합니다.
    *   `ACBoss`: 보스 캐릭터의 액터이며, 투사체를 발사하는 주체입니다.
    *   `UProjectileMovementComponent`: 투사체의 이동을 담당합니다.

*   **설계 패턴:**

    *   **Object Pooling:** `UBossProjectileComponent`에서 투사체를 풀링하여 액터 생성 및 소멸 비용을 줄입니다.

*   **확장성 및 유지보수성:**

    *   새로운 투사체를 추가하기 쉽도록 설계되었습니다. 투사체의 클래스를 생성하고, `UBossProjectileComponent`에서 해당 클래스의 투사체를 풀링하도록 설정하면 됩니다.
    *   코드의 가독성을 높이기 위해, 각 메서드의 역할을 명확하게 정의하고 주석을 충분히 작성합니다.

*   **실무 적용 가이드:**

    *   투사체의 속성(속도, 데미지, 이펙트 등)을 데이터 테이블에 정의하고, `ABossProjectileActor`에서 해당 데이터를 로드하여 투사체를 초기화합니다.
    *   투사체의 충돌 처리를 블루프린트에서 구현하여, 쉽게 수정하고 확장할 수 있도록 합니다.
    *   투사체의 발사 패턴을 데이터 테이블에 정의하고, `UBossProjectileComponent`에서 해당 데이터를 로드하여 투사체를 발사합니다.

### 4. `ABossProjectileOrb`

*   **클래스 개요:** `ABossProjectileOrb`는 `ABossProjectileActor`에서 파생된 클래스로, 특정한 궤적을 그리며 이동하거나, 특정 위치에 머물다가 폭발하는 등 특수한 동작을 하는 투사체를 구현하는 데 사용됩니다. 오브 형태의 투사체에 특화된 기능을 제공합니다.

*   **목적과 책임:**

    *   특수한 궤적을 그리며 이동하는 투사체 구현.
    *   특정 위치에 머물다가 폭발하는 투사체 구현.
    *   오브 형태의 투사체에 특화된 기능 제공 (예: 오브 소환, 파괴, 충돌 시 특수 효과).

*   **주요 메서드 및 프로퍼티:**

    *   `SpawnProjectile()`: 오브에서 다른 투사체를 스폰합니다.
    *   `DestroyOrb()`: 오브를 파괴합니다.
    *   `OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)`: 다른 액터와 겹쳤을 때 호출됩니다.
    *   `PlaySpawnSound()`: 오브가 스폰될 때 사운드를 재생합니다.
    *   `PlayReturnToPoolSound()`: 오브가 풀로 돌아갈 때 사운드를 재생합니다.
    *   `PlayCollisionSound()`: 오브가 충돌할 때 사운드를 재생합니다.
    *   `PlaySpawnEffect()`: 오브가 스폰될 때 이펙트를 재생합니다.
    *   `PlayReturnToPoolEffect()`: 오브가 풀로 돌아갈 때 이펙트를 재생합니다.
    *   `PlayCollisionEffect()`: 오브가 충돌할 때 이펙트를 재생합니다.
    *   `DestroyOrbWithDelay(float Delay)`: 지정된 시간 후에 오브를 파괴합니다.
    *   `ActivateOrb()`: 오브를 활성화합니다.

*   **클래스의 생명주기:**

    1.  **생성:** `UBossProjectileComponent`에 의해 필요에 따라 풀링되어 생성됩니다.
    2.  **초기화:** `BeginPlay()`에서 초기화됩니다.
    3.  **활성화:** `ActivateOrb()`를 통해 활성화됩니다.
    4.  **이동/대기:** 특수한 궤적을 그리며 이동하거나, 특정 위치에 머무릅니다.
    5.  **충돌/폭발:** 다른 액터와 충돌하거나, 특정 시간이 지나면 폭발합니다.
    6.  **소멸:** `DestroyOrb()`를 통해 파괴됩니다. `UBossProjectileComponent`에 의해 관리되며, 필요에 따라 풀로 반환되거나 소멸됩니다.

*   **사용 예제 및 코드:**

    ```c++
    // 헤더 파일 (BossProjectileOrb.h)
    #include "CoreMinimal.h"
    #include "BossProjectileActor.h"
    #include "BossProjectileOrb.generated.h"

    UCLASS()
    class MYPROJECT_API ABossProjectileOrb : public ABossProjectileActor
    {
    	GENERATED_BODY()

    public:
    	// Sets default values for this actor's properties
    	ABossProjectileOrb();

    protected:
    	// Called when the game starts or when spawned
    	virtual void BeginPlay() override;

    	UFUNCTION()
    	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    public:
    	// Called every frame
    	virtual void Tick(float DeltaTime) override;

    	UFUNCTION(BlueprintCallable, Category = "Orb")
    	void SpawnProjectile();

    	UFUNCTION(BlueprintCallable, Category = "Orb")
    	void DestroyOrb();

    	UFUNCTION(BlueprintCallable, Category = "Orb")
    	void PlaySpawnSound();

    	UFUNCTION(BlueprintCallable, Category = "Orb")
    	void PlayReturnToPoolSound();

    	UFUNCTION(BlueprintCallable, Category = "Orb")
    	void PlayCollisionSound();

    	UFUNCTION(BlueprintCallable, Category = "Orb")
    	void PlaySpawnEffect();

    	UFUNCTION(BlueprintCallable, Category = "Orb")
    	void PlayReturnToPoolEffect();

    	UFUNCTION(BlueprintCallable, Category = "Orb")
    	void PlayCollisionEffect();

    	UFUNCTION(BlueprintCallable, Category = "Orb")
    	void DestroyOrbWithDelay(float Delay);

    	UFUNCTION(BlueprintCallable, Category = "Orb")
    	void ActivateOrb();

    };

    // 소스 파일 (BossProjectileOrb.cpp)
    #include "BossProjectileOrb.h"
    #include "Components/SphereComponent.h"

    ABossProjectileOrb::ABossProjectileOrb()
    {
    	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    	PrimaryActorTick.bCanEverTick = true;

    	// Sphere 컴포넌트 설정 (기존 SphereComponent 재사용)
    	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossProjectileOrb::OnOverlap);

    }

    void ABossProjectileOrb::BeginPlay()
    {
    	Super::BeginPlay();

    }

    void ABossProjectileOrb::Tick(float DeltaTime)
    {
    	Super::Tick(DeltaTime);

    }

    void ABossProjectileOrb::SpawnProjectile()
    {
    	// 오브에서 투사체 스폰 로직
    	UE_LOG(LogTemp, Warning, TEXT("Projectile Spawned from Orb!"));
    }

    void ABossProjectileOrb::DestroyOrb()
    {
    	// 오브 파괴 로직
    	UE_LOG(LogTemp, Warning, TEXT("Orb Destroyed!"));
    	Destroy();
    }

    void ABossProjectileOrb::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
    {
    	// 충돌 처리 로직 (예: 폭발)
    	UE_LOG(LogTemp, Warning, TEXT("Orb Overlapped with something!"));
    }

    void ABossProjectileOrb::PlaySpawnSound()
    {
    	// 스폰 사운드 재생 로직
    	UE_LOG(LogTemp, Warning, TEXT("Spawn Sound Played!"));
    }

    void ABossProjectileOrb::PlayReturnToPoolSound()
    {
    	// 풀 반환 사운드 재생 로직
    	UE_LOG(LogTemp, Warning, TEXT("Return to Pool Sound Played!"));
    }

    void ABossProjectileOrb::PlayCollisionSound()
    {
    	// 충돌 사운드 재생 로직
    	UE_LOG(LogTemp, Warning, TEXT("Collision Sound Played!"));
    }

    void ABossProjectileOrb::PlaySpawnEffect()
    {
    	// 스폰 이펙트 재생 로직
    	UE_LOG(LogTemp, Warning, TEXT("Spawn Effect Played!"));
    }

    void ABossProjectileOrb::PlayReturnToPoolEffect()
    {
    	// 풀 반환 이펙트 재생 로직
    	UE_LOG(LogTemp, Warning, TEXT("Return to Pool Effect Played!"));
    }

    void ABossProjectileOrb::PlayCollisionEffect()
    {
    	// 충돌 이펙트 재생 로직
    	UE_LOG(LogTemp, Warning, TEXT("Collision Effect Played!"));
    }

    void ABossProjectileOrb::DestroyOrbWithDelay(float Delay)
    {
    	// 지연 파괴 로직
    	UE_LOG(LogTemp, Warning, TEXT("Orb will be Destroyed in %f seconds!"), Delay);
    }

    void ABossProjectileOrb::ActivateOrb()
    {
    	// 오브 활성화 로직
    	UE_LOG(LogTemp, Warning, TEXT("Orb Activated!"));
    }
    ```

    ```c++
    // UBossProjectileComponent에서 오브 사용 예제
    ABossProjectileOrb* Orb = ProjectileComponent->GetOrbFromPool();
    if (Orb)
    {
        Orb->SetActorLocation(SpawnLocation);
        Orb->

## 클래스별 상세 분석

### 1. `ABossEffect`

*   **클래스 개요:**
    *   `ABossEffect`는 보스에게 적용되는 시각 효과 및 기타 효과를 관리하는 추상 클래스입니다. 이 클래스는 이펙트의 활성화, 비활성화, 부착 위치 설정 등을 담당하며, 다른 클래스에서 상속받아 구체적인 이펙트 구현을 제공합니다.
    *   이 클래스는 `UBossEffectManager`에 의해 관리되며, `UBossEffectExecute`를 통해 실행됩니다.
    *   설계 철학은 이펙트의 재사용성을 높이고, 이펙트 관리의 중앙 집중화를 통해 효율적인 리소스 관리를 가능하게 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**
    *   `ActivateEffect()`: 이펙트를 활성화하는 메서드입니다. 구체적인 활성화 로직은 상속받는 클래스에서 구현합니다.
        ```c++
        virtual void ActivateEffect();
        ```
        *   사용 예제:
            ```c++
            // UBossEffectManager에서 이펙트 활성화
            ABossEffect* Effect = EffectManager->GetEffectFromPool(EffectTag);
            if (Effect)
            {
                Effect->ActivateEffect();
            }
            ```
    *   `DeactivateEffect()`: 이펙트를 비활성화하는 메서드입니다. 마찬가지로 상속받는 클래스에서 구체적인 비활성화 로직을 구현합니다.
        ```c++
        virtual void DeactivateEffect();
        ```
    *   `IsActive()`: 이펙트가 현재 활성화 상태인지 확인하는 메서드입니다.
        ```c++
        bool IsActive() const;
        ```
    *   `AttachToBoss()`: 이펙트를 보스 캐릭터에 부착하는 메서드입니다.
        ```c++
        void AttachToBoss(AActor* Boss);
        ```
    *   `AttachToSocket()`: 이펙트를 특정 소켓에 부착하는 메서드입니다.
        ```c++
        void AttachToSocket(AActor* Actor, FName SocketName);
        ```
    *   `PlaceInWorld()`: 이펙트를 월드에 특정 위치에 배치하는 메서드입니다.
        ```c++
        void PlaceInWorld(FVector Location, FRotator Rotation);
        ```
    *   `GetCurrentEffectTag()`: 현재 이펙트의 태그를 반환합니다. 이 태그는 이펙트 관리에 사용됩니다.
        ```c++
        FName GetCurrentEffectTag() const;
        ```
*   **생명주기 및 상태 관리:**
    *   `ABossEffect`는 풀링 시스템에 의해 관리됩니다. `UBossEffectManager`는 이펙트 풀을 생성하고, 필요할 때 풀에서 이펙트를 가져와 활성화하고, 사용이 끝나면 풀로 반환합니다.
    *   이펙트의 상태는 `IsActive()` 메서드를 통해 확인할 수 있습니다.
*   **성능 특성 및 최적화 포인트:**
    *   풀링 시스템을 사용하여 이펙트 생성 및 파괴 비용을 줄입니다.
    *   이펙트 활성화 및 비활성화 로직을 최적화하여 프레임 드랍을 최소화합니다.
    *   불필요한 Tick 함수 호출을 피합니다.
*   **사용 시나리오 및 베스트 프랙티스:**
    *   보스의 공격 시 발생하는 시각 효과, 피격 시 효과, 상태 변화에 따른 효과 등을 구현하는 데 사용됩니다.
    *   이펙트의 종류, 지속 시간, 부착 위치 등을 데이터 테이블에 정의하여 관리하는 것이 좋습니다.
*   **다른 클래스와의 상호작용:**
    *   `UBossEffectManager`: 이펙트 풀 관리 및 이펙트 실행을 담당합니다.
    *   `UBossEffectExecute`: 이펙트 실행을 위한 인터페이스를 제공합니다.
    *   `ACBoss`: 이펙트가 부착될 보스 캐릭터입니다.

### 2. `ABossManager`

*   **클래스 개요:**
    *   `ABossManager`는 게임 월드에 보스를 스폰하고 관리하는 역할을 담당합니다. 보스의 초기화, 상태 관리, 레벨 디자인과의 상호 작용 등을 처리합니다.
    *   이 클래스는 싱글톤 패턴을 사용하여 게임 내에서 유일한 인스턴스만 존재하도록 설계될 수 있습니다.
    *   설계 철학은 보스 관련 로직을 중앙 집중화하여 관리하고, 레벨 디자이너가 보스를 쉽게 배치하고 설정할 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**
    *   `ResetBossCompletely()`: 보스를 완전히 초기화하는 메서드입니다. 보스의 HP, 상태, 위치 등을 초기 상태로 되돌립니다.
        ```c++
        void ResetBossCompletely();
        ```
        *   사용 예제:
            ```c++
            // 보스 전투 시작 시 보스 초기화
            BossManager->ResetBossCompletely();
            ```
    *   `OpenDoor()`: 보스 방의 문을 여는 메서드입니다.
        ```c++
        void OpenDoor();
        ```
    *   `FindBossInWorld()`: 월드에서 보스 액터를 찾는 메서드입니다.
        ```c++
        ACBoss* FindBossInWorld();
        ```
    *   `ResetAllBossComponents()`: 보스의 모든 컴포넌트를 초기화하는 메서드입니다.
        ```c++
        void ResetAllBossComponents();
        ```
    *   `ResetBossStateTree()`: 보스의 StateTree를 초기화하는 메서드입니다.
        ```c++
        void ResetBossStateTree();
        ```
    *   `OnTriggerBoxOverlapBegin()`: 플레이어가 보스 방에 진입했을 때 호출되는 메서드입니다.
        ```c++
        UFUNCTION()
        void OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
        ```
*   **생명주기 및 상태 관리:**
    *   `ABossManager`는 게임 시작 시 생성되고, 게임 종료 시 파괴됩니다.
    *   보스의 상태는 `ResetBossCompletely()` 메서드를 통해 관리됩니다.
*   **성능 특성 및 최적화 포인트:**
    *   보스 스폰 로직을 최적화하여 게임 시작 시 로딩 시간을 줄입니다.
    *   불필요한 Tick 함수 호출을 피합니다.
*   **사용 시나리오 및 베스트 프랙티스:**
    *   보스 전투 시작 시 보스를 스폰하고 초기화합니다.
    *   플레이어가 보스 방에 진입했을 때 특정 이벤트를 발생시킵니다.
    *   보스 전투 종료 후 보스를 리스폰하거나 제거합니다.
*   **다른 클래스와의 상호작용:**
    *   `ACBoss`: 관리할 보스 액터입니다.
    *   `ABossProjectileComponent`: 보스의 투사체를 관리합니다.
    *   `UBossStatusWidget`: 보스의 상태를 표시하는 위젯입니다.

### 3. `ABossProjectileActor`

*   **클래스 개요:**
    *   `ABossProjectileActor`는 보스가 발사하는 투사체의 기본 클래스입니다. 투사체의 이동, 충돌, 이펙트 재생 등을 담당합니다.
    *   이 클래스는 풀링 시스템을 사용하여 투사체의 재사용성을 높입니다.
    *   설계 철학은 투사체 관련 로직을 캡슐화하고, 다양한 종류의 투사체를 쉽게 생성하고 관리할 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**
    *   `FireProjectile()`: 투사체를 발사하는 메서드입니다.
        ```c++
        void FireProjectile(FVector Direction, float Speed);
        ```
        *   사용 예제:
            ```c++
            // UBossProjectileComponent에서 투사체 발사
            ABossProjectileActor* Projectile = ProjectileComponent->GetProjectileFromPool();
            if (Projectile)
            {
                Projectile->SetActorLocation(SpawnLocation);
                Projectile->FireProjectile(FireDirection, ProjectileSpeed);
            }
            ```
    *   `FireProjectileToLocation()`: 특정 위치로 투사체를 발사하는 메서드입니다.
        ```c++
        void FireProjectileToLocation(FVector TargetLocation, float Speed);
        ```
    *   `PlaySpawnEffect()`: 투사체 생성 시 이펙트를 재생하는 메서드입니다.
        ```c++
        virtual void PlaySpawnEffect();
        ```
    *   `PlayDestroyEffect()`: 투사체 파괴 시 이펙트를 재생하는 메서드입니다.
        ```c++
        virtual void PlayDestroyEffect();
        ```
    *   `OnProjectileHit()`: 투사체가 다른 액터와 충돌했을 때 호출되는 메서드입니다.
        ```c++
        UFUNCTION()
        void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
        ```
*   **생명주기 및 상태 관리:**
    *   `ABossProjectileActor`는 풀링 시스템에 의해 관리됩니다.
    *   투사체의 상태는 활성화 여부로 관리됩니다.
*   **성능 특성 및 최적화 포인트:**
    *   풀링 시스템을 사용하여 투사체 생성 및 파괴 비용을 줄입니다.
    *   충돌 판정을 최적화하여 성능 저하를 방지합니다.
*   **사용 시나리오 및 베스트 프랙티스:**
    *   보스의 원거리 공격을 구현하는 데 사용됩니다.
    *   투사체의 종류, 속도, 데미지 등을 데이터 테이블에 정의하여 관리하는 것이 좋습니다.
*   **다른 클래스와의 상호작용:**
    *   `UBossProjectileComponent`: 투사체 풀 관리 및 투사체 발사를 담당합니다.
    *   `ACBoss`: 투사체를 발사하는 보스 캐릭터입니다.

### 4. `ABossProjectileOrb`

*   **클래스 개요:**
    *   `ABossProjectileOrb`는 `ABossProjectileActor`를 상속받아 구현된 특정 형태의 투사체입니다. 궤도 이동, 특수한 충돌 판정, 특정 이펙트 등을 가질 수 있습니다.
    *   이 클래스는 구체 형태의 투사체를 구현하는 데 특화되어 있습니다.
    *   설계 철학은 `ABossProjectileActor`의 기능을 확장하여 다양한 종류의 투사체를 쉽게 구현할 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**
    *   `SpawnProjectile()`: 오브를 스폰하는 메서드입니다.
        ```c++
        void SpawnProjectile();
        ```
    *   `DestroyOrb()`: 오브를 파괴하는 메서드입니다.
        ```c++
        void DestroyOrb();
        ```
    *   `OnOverlap()`: 오브가 다른 액터와 겹쳤을 때 호출되는 메서드입니다.
        ```c++
        UFUNCTION()
        void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
        ```
    *   `PlaySpawnSound()`: 오브 생성 시 사운드를 재생하는 메서드입니다.
        ```c++
        virtual void PlaySpawnSound();
        ```
    *   `PlayReturnToPoolSound()`: 오브가 풀로 반환될 때 사운드를 재생하는 메서드입니다.
        ```c++
        virtual void PlayReturnToPoolSound();
        ```
    *   `PlayCollisionSound()`: 오브가 충돌했을 때 사운드를 재생하는 메서드입니다.
        ```c++
        virtual void PlayCollisionSound();
        ```
    *   `PlaySpawnEffect()`: 오브 생성 시 이펙트를 재생하는 메서드입니다.
        ```c++
        virtual void PlaySpawnEffect();
        ```
    *   `PlayReturnToPoolEffect()`: 오브가 풀로 반환될 때 이펙트를 재생하는 메서드입니다.
        ```c++
        virtual void PlayReturnToPoolEffect();
        ```
    *   `PlayCollisionEffect()`: 오브가 충돌했을 때 이펙트를 재생하는 메서드입니다.
        ```c++
        virtual void PlayCollisionEffect();
        ```
    *   `DestroyOrbWithDelay()`: 지연 후 오브를 파괴하는 메서드입니다.
        ```c++
        void DestroyOrbWithDelay(float Delay);
        ```
    *   `ActivateOrb()`: 오브를 활성화하는 메서드입니다.
        ```c++
        void ActivateOrb();
        ```

*   **생명주기 및 상태 관리:**
    *   `ABossProjectileOrb`는 `ABossProjectileActor`와 마찬가지로 풀링 시스템에 의해 관리됩니다.
    *   오브의 상태는 활성화 여부로 관리됩니다.

*   **성능 특성 및 최적화 포인트:**
    *   `ABossProjectileActor`의 성능 특성 및 최적화 포인트를 상속받습니다.
    *   특수한 이동 로직이나 충돌 판정을 최적화하여 성능 저하를 방지합니다.

*   **사용 시나리오 및 베스트 프랙티스:**
    *   보스의 유도탄, 궤도탄 등을 구현하는 데 사용됩니다.
    *   오브의 이동 패턴, 속도, 데미지 등을 데이터 테이블에 정의하여 관리하는 것이 좋습니다.

*   **다른 클래스와의 상호작용:**
    *   `ABossProjectileActor`: 기본 투사체 기능을 상속받습니다.
    *   `UBossProjectileComponent`: 오브 풀 관리 및 오브 발사를 담당합니다.
    *   `ACBoss`: 오브를 발사하는 보스 캐릭터입니다.

### 5. `ACBoss`

*   **클래스 개요:**
    *   `ACBoss`는 보스 캐릭터의 기본 클래스입니다. 보스의 이동, 공격, 피격, 사망 등 기본적인 동작을 담당합니다.
    *   이 클래스는 `ACharacter` 클래스를 상속받아 구현됩니다.
    *   설계 철학은 보스 캐릭터의 기본 기능을 캡슐화하고, 다양한 종류의 보스 캐릭터를 쉽게 생성하고 관리할 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**
    *   `TakeDamage()`: 데미지를 받는 메서드입니다.
        ```c++
        virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
        ```
        *   사용 예제:
            ```c++
            // 플레이어의 공격으로 보스가 데미지를 받음
            Boss->TakeDamage(DamageAmount, DamageEvent, PlayerController, Player);
            ```
    *   `PlayHitMotion()`: 피격 모션을 재생하는 메서드입니다.
        ```c++
        void PlayHitMotion();
        ```
    *   `ShowBossStatusWidget()`: 보스 상태 위젯을 표시하는 메서드입니다.
        ```c++
        void ShowBossStatusWidget();
        ```
    *   `HPUpdate()`: HP를 업데이트하는 메서드입니다.
        ```c++
        void HPUpdate();
        ```
    *   `PlayBossBGM()`: 보스 배경 음악을 재생하는 메서드입니다.
        ```c++
        void PlayBossBGM();
        ```
    *   `StopBossBGM()`: 보스 배경 음악을 정지하는 메서드입니다.
        ```c++
        void StopBossBGM();
        ```
    *   `LowerBossBGMVolume()`: 보스 배경 음악 볼륨을 낮추는 메서드입니다.
        ```c++
        void LowerBossBGMVolume();
        ```

*   **생명주기 및 상태 관리:**
    *   `ACBoss`는 게임 월드에 스폰될 때 생성되고, 파괴될 때 소멸됩니다.
    *   보스의 상태는 HP, 상태 태그 등을 통해 관리됩니다.

*   **성능 특성 및 최적화 포인트:**
    *   애니메이션, AI, 이펙트 등을 최적화하여 성능 저하를 방지합니다.
    *   불필요한 Tick 함수 호출을 피합니다.

*   **사용 시나리오 및 베스트 프랙티스:**
    *   보스 캐릭터의 기본 동작을 구현하는 데 사용됩니다.
    *   보스의 공격 패턴, 이동 패턴, AI 등을 데이터 테이블에 정의하여 관리하는 것이 좋습니다.

*   **다른 클래스와의 상호작용:**
    *   `ACBossAIC`: 보스 AI 컨트롤러입니다.
    *   `UBossStatusWidget`: 보스 상태 위젯입니다.
    *   `ABossManager`: 보스 관리자입니다.
    *   `UBossMovementComponent`: 보스의 이동을 담당합니다.
    *   `UBossStatusComponent`: 보스의 스탯을 관리합니다.
    *   `UBossWeaponComponent`: 보스의 무기 시스템을 관리합니다.

### 6. `ACBossAIC`

*   **클래스 개요:**
    *   `ACBossAIC`는 보스 AI 컨트롤러 클래스입니다. 보스의 행동을 결정하고, StateTree를 통해 AI 로직을 관리합니다.
    *   이 클래스는 `AAIController` 클래스를 상속받아 구현됩니다.
    *   설계 철학은 보스 AI 로직을 캡슐화하고, StateTree를 통해 유연하고 확장 가능한 AI를 구현하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**
    *   `OnPossess()`: Pawn을 소유했을 때 호출되는 메서드입니다.
        ```c++
        virtual void OnPossess(APawn* InPawn) override;
        ```

*   **생명주기 및 상태 관리:**
    *   `ACBossAIC`는 `ACBoss`가 스폰될 때 함께 생성됩니다.
    *   보스의 행동은 StateTree에 의해 관리됩니다.

*   **성능 특성 및 최적화 포인트:**
    *   StateTree의 복잡도를 줄여 AI 연산 비용을 최소화합니다.
    *   불필요한 Tick 함수 호출을 피합니다.

*   **사용 시나리오 및 베스트 프랙티스:**
    *   보스의 공격 패턴, 이동 패턴, 상태 변화 등을 StateTree를 통해 정의합니다.
    *   StateTree의 조건을 최적화하여 AI 연산 비용을 줄입니다.

*   **다른 클래스와의 상호작용:**
    *   `ACBoss`: 제어할 보스 캐릭터입니다.
    *   StateTree: 보스 AI 로직을 정의합니다.
    *   `UCBossEnemyStateTreeEvaluator`: StateTree의 의사 결정을 위한 데이터를 수집합니다.

### 7. `ACBossWeapon`

*   **클래스 개요:**
    *   `ACBossWeapon`은 보스의 무기를 담당하는 클래스입니다. 무기의 장착, 해제, 충돌 판정 등을 관리합니다.
    *   이 클래스는 `AActor` 클래스를 상속받아 구현됩니다.
    *   설계 철학은 무기 관련 로직을 캡슐화하고, 다양한 종류의 무기를 쉽게 생성하고 관리할 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**
    *   `OnBossBeginEquip()`: 무기 장착 시작 시 호출되는 이벤트입니다.
        ```c++
        UFUNCTION(BlueprintNativeEvent)
        void OnBossBeginEquip();
        virtual void OnBossBeginEquip_Implementation();
        ```
    *   `OnBossUnequip()`: 무기 해제 시 호출되는 이벤트입니다.
        ```c++
        UFUNCTION(BlueprintNativeEvent)
        void OnBossUnequip();
        virtual void OnBossUnequip_Implementation();
        ```
    *   `OnBossCollisions()`: 무기의 충돌을 처리하는 메서드입니다.
        ```c++
        void OnBossCollisions(bool bOn);
        ```
    *   `StartCollisionAtSocket()`: 특정 소켓에서 충돌을 시작하는 메서드입니다.
        ```c++
        void StartCollisionAtSocket(FName SocketName);
        ```
    *   `EndCollisionToOwner()`: 소유자에게 충돌을 종료하는 메서드입니다.
        ```c++
        void EndCollisionToOwner();
        ```
    *   `BossAttachTo()`: 무기를 특정 액터에 부착하는 메서드입니다.
        ```c++
        void BossAttachTo(AActor* Target, FName SocketName);
        ```
    *   `OnBossComponentBeginOverlap()`: 컴포넌트 오버랩 시작 시 호출되는 메서드입니다.
        ```c++
        UFUNCTION()
        void OnBossComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
        ```
    *   `OnBossComponentEndOverlap()`: 컴포넌트 오버랩 종료 시 호출되는 메서드입니다.
        ```c++
        UFUNCTION()
        void OnBossComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
        ```

*   **생명주기 및 상태 관리:**
    *   `ACBossWeapon`는 `ACBoss`가 스폰될 때 함께 스폰되거나, 필요에 따라 동적으로 생성될 수 있습니다.
    *   무기의 상태는 장착 여부, 충돌 활성화 여부 등으로 관리됩니다.

*   **성능 특성 및 최적화 포인트:**
    *   충돌 판정을 최적화하여 성능 저하를 방지합니다.
    *   불필요한 Tick 함수 호출을 피합니다.

*   **사용 시나리오 및 베스트 프랙티스:**
    *   보스의 근접 공격, 원거리 공격 등을 구현하는 데 사용됩니다.
    *   무기의 종류, 데미지, 이펙트 등을 데이터 테이블에 정의하여 관리하는 것이 좋습니다.

*   **다른 클래스와의 상호작용:**
    *   `ACBoss`: 무기를 사용하는 보스 캐릭터입니다.
    *   `UCBossWeaponComponent`: 무기 시스템을 관리합니다.
    *   `UCBossDoAction`: 보스 액션을 실행합니다.

### 8. `AFlySpline`

*   **클래스 개요:**
    *   `AFlySpline`는 보스의 비행 경로를 정의하는 스플라인 액터입니다.
    *   이 클래스는 `AActor` 클래스를 상속받아 구현됩니다.
    *   설계 철학은 레벨 디자이너가 보스의 비행 경로를 쉽게 설정할 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**
    *   `OnConstruction()`: 액터가 생성될 때 호출되는 메서드입니다.
        ```c++
        virtual void OnConstruction(const FTransform& Transform) override;
        ```
    *   `BuildCylinderAndRims()`: 실린더와 림을 생성하는 메서드입니다.
        ```c++
        void BuildCylinderAndRims();
        ```
    *   `GetHorizontalSplines()`: 수평 스플라인을 가져오는 메서드입니다.
        ```c++
        TArray<USplineComponent*> GetHorizontalSplines();
        ```
    *   `GetSplineAtIndex()`: 특정 인덱스의 스플라인을 가져오는 메서드입니다.
        ```c++
        USplineComponent* GetSplineAtIndex(int32 Index);
        ```
    *   `BuildRim()`: 림을 생성하는 메서드입니다.
        ```c++
        void BuildRim(int32 Index);
        ```
    *   `CreateHorizontalSplines()`: 수평 스플라인을 생성하는 메서드입니다.
        ```c++
        void CreateHorizontalSplines();
        ```

*   **생명주기 및 상태 관리:**
    *   `AFlySpline`는 레벨 에디터에서 배치됩니다.
    *   스플라인의 경로는 레벨 에디터에서 수정할 수 있습니다.

*   **성능 특성 및 최적화 포인트:**
    *   스플라인의 복잡도를 줄여 성능 저하를 방지합니다.
    *   불필요한 Tick 함수 호출을 피합니다.

*   **사용 시나리오 및 베스트 프랙티스:**
    *   보스의 비행 경로를 정의하는 데 사용됩니다.
    *   스플라인의 경로를 부드럽게 만들어 자연스러운 비행 동작을 구현합니다.

*   **다른 클래스와의 상호작용:**
    *   `ACBoss`: 비행 경로를 따라 이동할 보스 캐릭터입니다.
    *   `UTask_FlySetLocation`: 보스의 호버링 위치를 설정하는 StateTree Task입니다.

### 9. `AGateOfBabylon`

*   **클래스 개요:**
    *   `AGateOfBabylon`는 바빌론의 문을 구현하는 액터 클래스입니다. 투사체 생성 및 발사, 문 활성화/비활성화 등을 담당합니다.
    *   이 클래스는 `AActor` 클래스를 상속받아 구현됩니다.
    *   설계 철학은 특수한 공격 패턴을 가진 보스를 구현하는 데 사용됩니다.

*   **핵심 메서드 및 프로퍼티 분석:**
    *   `ActivateGate()`: 문을 활성화하는 메서드입니다.
        ```c++
        void ActivateGate();
        ```
    *   `DeactivateGate()`: 문을 비활성화하는 메서드입니다.
        ```c++
        void DeactivateGate();
        ```
    *   `InitializeProjectilePool()`: 투사체 풀을 초기화하는 메서드입니다.
        ```c++
        void InitializeProjectilePool(int32 PoolSize);
        ```
    *   `GetProjectileFromPool()`: 투사체 풀에서 투사체를 가져오는 메서드입니다.
        ```c++
        AGateOfBabyonProjectile* GetProjectileFromPool();
        ```
    *   `SpawnProjectile()`: 투사체를 스폰하는 메서드입니다.
        ```c++
        void SpawnProjectile();
        ```
    *   `UpdateLookAtPlayer()`: 플레이어를 바라보도록 회전하는 메서드입니다.
        ```c++
        void UpdateLookAtPlayer();
        ```

*   **생명주기 및 상태 관리:**
    *   `AGateOfBabylon`는 레벨 에디터에서 배치됩니다.
    *   문의 상태는 활성화 여부로 관리됩니다.

*   **성능 특성 및 최적화 포인트:**
    *   투사체 풀링을 사용하여 성능을 최적화합니다.
    *   투사체의 이동 및 충돌 판정을 최적화합니다.

*   **사용 시나리오 및 베스트 프랙티스:**
    *   보스의 특수한 공격 패턴을 구현하는 데 사용됩니다.
    *   투사체의 종류, 속도, 데미지 등을 데이터 테이블에 정의하여 관리하는 것이 좋습니다.

*   **다른 클래스와의 상호작용:**
    *   `AGateOfBabyonProjectile`: 바빌론의 문에서 발사되는 투사체입니다.
    *   `ACBoss`: 바빌론의 문을 사용하는 보스 캐릭터입니다.

### 10. `AGateOfBabyonProjectile`

*   **클래스 개요:**
    *   `AGateOfBabyonProjectile`는 바빌론의 문에서 발사되는 투사체 클래스입니다.
    *   이 클래스는 `AActor` 클래스를 상속받아 구현됩니다.
    *   설계 철학은 특수한 이동 패턴을 가진 투사체를 쉽게 구현할 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**
    *   `ActivateProjectile()`: 투사체를 활성화하는 메서드입니다.
        ```c++
        void ActivateProjectile();
        ```
    *   `DeactivateProjectile()`: 투사체를 비활성화하는 메서드입니다.
        ```c++
        void DeactivateProjectile();
        ```
    *   `OnBeginOverlap()`: 오버랩 시작 시 호출되는 메서드입니다.
        ```c++
        UFUNCTION()
        void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
        ```
    *   `MoveToRandomLocationAroundPlayer()`: 플레이어 주변의 랜덤 위치로 이동하는 메서드입니다.
        ```c++
        void MoveToRandomLocationAroundPlayer();
        ```

*   **생명주기 및 상태 관리:**
    *   `AGateOfBabyonProjectile`는 `AGateOfBabylon`에 의해 풀링 시스템으로 관리됩니다.
    *   투사체의 상태는 활성화 여부로 관리됩니다.

*   **성능 특성 및 최적화 포인트:**
    *   투사체의 이동 및 충돌 판정을 최적화합니다.

*   **사용 시나리오 및 베스트 프랙티스:**
    *   바빌론의 문에서 발사되는 투사체의 이동 패턴을 구현하는 데 사용됩니다.

*   **다른 클래스와의 상호작용:**
    *   `AGateOfBabylon`: 투사체를 생성하고 관리하는 액터입니다.

### 11. `AHolySwordMagic`

*   **클래스 개요:**
    *   `AHolySwordMagic`는 성검 마법을 구현하는 액터 클래스입니다.
    *   이 클래스는 `AActor` 클래스를 상속받아 구현됩니다.
    *   설계 철학은 특수한 이펙트와 공격 패턴을 가진 마법 공격을 쉽게 구현할 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**
    *   `StartFirstNiagara()`: 첫 번째 나이아가라 이펙트를 시작하는 메서드입니다.
        ```c++
        void StartFirstNiagara();
        ```
    *   `StartSecondNiagara()`: 두 번째 나이아가라 이펙트를 시작하는 메서드입니다.
        ```c++
        void StartSecondNiagara();
        ```
    *   `EnableCollision()`: 충돌을 활성화하는 메서드입니다.
        ```c++
        void EnableCollision();
        ```
    *   `DisableCollision()`: 충돌을 비활성화하는 메서드입니다.
        ```c++
        void DisableCollision();
        ```
    *   `CheckNiagaraCompletion()`: 나이아가라 이펙트 완료 여부를 확인하는 메서드입니다.
        ```c++
        void CheckNiagaraCompletion();
        ```
    *   `ResetForPool()`: 풀링 시스템을 위해 리셋하는 메서드입니다.
        ```c++
        void ResetForPool();
        ```
    *   `OnOverlapBegin()`: 오버랩 시작 시 호출되는 메서드입니다.
        ```c++
        UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
        ```

*   **생명주기 및 상태 관리:**
    *   `AHolySwordMagic`는 풀링 시스템에 의해 관리됩니다.
    *   마법의 상태는 활성화 여부, 이펙트 재생 여부, 충돌 활성화 여부 등으로 관리됩니다.

*   **성능 특성 및 최적화 포인트:**
    *   나이아가라 이펙트의 복잡도를 줄여 성능 저하를 방지합니다.
    *   충돌 판정을 최적화합니다.

*   **사용 시나리오 및 베스트 프랙티스:**
    *   보스의 특수한 마법 공격을 구현하는 데 사용됩니다.

*   **다른 클래스와의 상호작용:**
    *   `UBossProjectileComponent`: 마법 풀 관리 및 마법 발사를 담당합니다.
    *   `ACBoss`: 마법을 사용하는 보스 캐릭터입니다.

### 12. `AProjectile_LightSpear`

*   **클래스 개요:**
    *   `AProjectile_LightSpear`는 번개 창 투사체를 구현하는 액터 클래스입니다.
    *   이 클래스는 `AActor` 클래스를 상속받아 구현됩니다.
    *   설계 철학은 빠른 속도와 강력한 데미지를 가진 투사체를 쉽게 구현할 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**
    *   `FireProjectile()`: 투사체를 발사하는 메서드입니다.
        ```c++
        void FireProjectile(FVector Direction, float Speed);
        ```
    *   `PlayDestroyEffect()`: 파괴 이펙트를 재생하는 메서드입니다.
        ```c++
        void PlayDestroyEffect();
        ```
    *   `OnProjectileHit()`: 투사체가 충돌했을 때 호출되는 메서드입니다.
        ```c++
        UFUNCTION()
        void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
        ```

*   **생명주기 및 상태 관리:**
    *   `AProjectile_LightSpear`는 풀링 시스템에 의해 관리될 수 있습니다.
    *   투사체의 상태는 활성화 여부로 관리됩니다.

*   **성능 특성 및 최적화 포인트:**
    *   투사체의 이동 및 충돌 판정을 최적화합니다.

*   **사용 시나리오 및 베스트 프랙티스:**
    *   보스의 빠른 속도와 강력한 데미지를 가진 투사체 공격을 구현하는 데 사용됩니다.

*   **다른 클래스와의 상호작용:**
    *   `UBossProjectileComponent`: 투사체 풀 관리 및 투사체 발사를 담당합니다.
    *   `ACBoss`: 투사체를 발사하는 보스 캐릭터입니다.

### 13. ~ 41. Editor Plugin 관련 클래스 및 구조체

*   **클래스 개요:**
    *   `FEditorPlugin_DataSyncModule`, `FEditorPlugin_DataSyncCommands`, `FEditorPlugin_DataSyncStyle` 등은 언리얼 엔진 에디터 플러그인 개발에 관련된 클래스들입니다. 이 플러그인은 게임 데이터 (GameplayTags, BossStats 등)를 외부 소스 (예: HTTP API)와 동기화하는 기능을 제공합니다.
    *   이 클래스들은 언리얼 엔진의 에디터 모듈 시스템을 활용하여 구현됩니다.
    *   설계 철학은 게임 데이터 관리의 효율성을 높이고, 개발 프로세스를 자동화하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**
    *   `FEditorPlugin_DataSyncModule::StartupModule()`: 플러그인 모듈이 시작될 때 호출되는 메서드입니다. UI를 생성하고, 명령을 등록하는 등의 초기화 작업을 수행합니다.
    *   `FEditorPlugin_DataSyncModule::ShutdownModule()`: 플러그인 모듈이 종료될 때 호출되는 메서드입니다. 리소스를 해제하고, UI를 제거하는 등의 정리 작업을 수행합니다.
    *   `FEditorPlugin_DataSyncModule::Sync

## 클래스별 상세 분석

### 1. `ABossEffect`

*   **클래스 개요:**
    *   `ABossEffect`는 보스에게 시각적인 효과를 제공하는 액터 클래스입니다. 파티클 시스템, 머티리얼 효과 등을 포함할 수 있으며, 보스의 공격, 피격, 상태 변화 등 다양한 상황에 따라 활성화/비활성화됩니다.
    *   이 클래스는 `UBossEffectManager`에 의해 관리되며, 오브젝트 풀링을 통해 성능을 최적화합니다.
    *   설계 철학은 이펙트의 재사용성을 높이고, 보스 전투의 시각적인 완성도를 높이는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**

    *   `ActivateEffect()`: 이펙트를 활성화합니다. 이펙트의 파티클 시스템을 시작하고, 머티리얼 파라미터를 변경하는 등의 작업을 수행합니다.

        ```c++
        void ABossEffect::ActivateEffect()
        {
            // 파티클 시스템 활성화
            if (ParticleSystemComponent)
            {
                ParticleSystemComponent->Activate(true);
            }

            // 머티리얼 파라미터 변경 (예시)
            if (MeshComponent && DynamicMaterialInstance)
            {
                DynamicMaterialInstance->SetScalarParameterValue(FName("Opacity"), 1.0f);
            }

            bIsActive = true;
        }
        ```

    *   `ActivateEffectAttachedToSocket()`: 이펙트를 특정 소켓에 부착하여 활성화합니다. 보스의 무기, 팔, 머리 등 특정 위치에서 이펙트가 발생해야 할 때 사용됩니다.

        ```c++
        void ABossEffect::ActivateEffectAttachedToSocket(USceneComponent* Parent, FName SocketName)
        {
            AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
            ActivateEffect();
        }
        ```

    *   `DeactivateEffect()`: 이펙트를 비활성화합니다. 파티클 시스템을 정지하고, 머티리얼 파라미터를 초기화하는 등의 작업을 수행합니다.

        ```c++
        void ABossEffect::DeactivateEffect()
        {
            // 파티클 시스템 비활성화
            if (ParticleSystemComponent)
            {
                ParticleSystemComponent->Deactivate();
            }

            // 머티리얼 파라미터 초기화 (예시)
            if (MeshComponent && DynamicMaterialInstance)
            {
                DynamicMaterialInstance->SetScalarParameterValue(FName("Opacity"), 0.0f);
            }

            bIsActive = false;
        }
        ```

    *   `IsActive()`: 이펙트가 현재 활성화되어 있는지 여부를 반환합니다.

    *   `AttachToBoss()`: 이펙트를 보스 액터에 부착합니다.

    *   `AttachToSocket()`: 이펙트를 보스 액터의 특정 소켓에 부착합니다.

    *   `PlaceInWorld()`: 이펙트를 월드 공간에 배치합니다.

*   **클래스의 생명주기와 상태 관리:**

    *   `ABossEffect`는 `UBossEffectManager`에 의해 생성되고 관리됩니다.
    *   `UBossEffectManager`는 오브젝트 풀을 사용하여 `ABossEffect` 인스턴스를 재활용합니다.
    *   이펙트의 상태는 `bIsActive` 변수로 관리됩니다.

*   **실제 사용 예제와 코드:**

    ```c++
    // 보스가 공격할 때 이펙트 활성화
    void ACBoss::Attack()
    {
        // ... 공격 로직 ...

        // 공격 이펙트 활성화
        if (BossEffectManager)
        {
            BossEffectManager->PlayEffectAttachedToSocket(FName("AttackEffect"), GetMesh(), FName("WeaponSocket"));
        }
    }

    // 보스가 피격당할 때 이펙트 활성화
    void ACBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
    {
        // ... 데미지 처리 로직 ...

        // 피격 이펙트 활성화
        if (BossEffectManager)
        {
            BossEffectManager->PlayEffectAtBossLocation(FName("HitEffect"), GetActorLocation());
        }
    }
    ```

*   **성능 특성과 최적화 포인트:**

    *   오브젝트 풀링을 통해 이펙트 생성 및 소멸 비용을 줄입니다.
    *   불필요한 Tick 함수 호출을 피합니다. 이펙트가 활성화되어 있는 동안에만 Tick 함수를 호출하도록 구현할 수 있습니다.
    *   파티클 시스템의 복잡도를 줄입니다. 필요한 파티클만 사용하고, 파티클 수를 제한합니다.
    *   머티리얼의 복잡도를 줄입니다. 간단한 머티리얼을 사용하고, 불필요한 연산을 피합니다.

*   **사용 시나리오와 베스트 프랙티스:**

    *   보스의 공격, 피격, 상태 변화 등 다양한 상황에 시각적인 피드백을 제공합니다.
    *   `UBossEffectManager`를 사용하여 이펙트를 중앙 집중적으로 관리합니다.
    *   오브젝트 풀링을 통해 성능을 최적화합니다.
    *   이펙트의 태그를 사용하여 이펙트를 쉽게 식별하고 관리합니다.

*   **다른 클래스와의 상호작용:**

    *   `UBossEffectManager`: 이펙트 풀 관리 및 이펙트 재생을 담당합니다.
    *   `ACBoss`: 이펙트를 사용하는 보스 캐릭터입니다.

### 2. `ABossManager`

*   **클래스 개요:**

    *   `ABossManager`는 보스 전투의 전체적인 흐름을 관리하는 액터 클래스입니다. 보스 스폰, 보스 초기화, 전투 시작/종료, 문 개방 등의 기능을 담당합니다.
    *   이 클래스는 싱글톤 패턴을 사용하여 게임 내에서 유일한 인스턴스만 존재하도록 할 수 있습니다.
    *   설계 철학은 보스 전투의 복잡성을 숨기고, 게임 디자이너가 쉽게 보스 전투를 구성할 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**

    *   `ResetBossCompletely()`: 보스를 완전히 초기화합니다. 보스의 HP, 상태, 위치 등을 초기화하고, StateTree를 재시작합니다.

        ```c++
        void ABossManager::ResetBossCompletely()
        {
            // 월드에서 보스 찾기
            ACBoss* Boss = FindBossInWorld();
            if (!Boss) return;

            // 보스 위치 초기화
            Boss->SetActorLocation(InitialBossLocation);

            // 보스 컴포넌트 초기화
            ResetAllBossComponents();

            // 보스 StateTree 초기화
            ResetBossStateTree();

            // 보스 HP 초기화 (예시)
            // Boss->HP = Boss->MaxHP;

            // 보스 상태 초기화 (예시)
            // Boss->SetState(EBossState::Idle);
        }
        ```

    *   `OpenDoor()`: 보스 전투가 종료된 후 문을 개방합니다.

    *   `FindBossInWorld()`: 월드에서 보스 액터를 찾습니다.

    *   `ResetAllBossComponents()`: 보스의 모든 컴포넌트를 초기화합니다.

    *   `ResetBossStateTree()`: 보스의 StateTree를 재시작합니다.

    *   `OnTriggerBoxOverlapBegin()`: 플레이어가 특정 영역에 진입했을 때 호출됩니다. 보스 전투를 시작하는 데 사용될 수 있습니다.

*   **클래스의 생명주기와 상태 관리:**

    *   `ABossManager`는 게임 시작 시 생성됩니다.
    *   보스 전투의 상태는 `ABossManager` 내부 변수로 관리될 수 있습니다. (예: `bIsBossFightActive`)

*   **실제 사용 예제와 코드:**

    ```c++
    // 플레이어가 특정 영역에 진입했을 때 보스 전투 시작
    void ABossManager::OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
    {
        if (OtherActor->IsA(APlayerCharacter::StaticClass()) && !bIsBossFightActive)
        {
            bIsBossFightActive = true;
            ResetBossCompletely();
            // ... 보스 전투 시작 로직 ...
        }
    }

    // 보스 전투 종료 후 문 개방
    void ABossManager::OnBossDeath()
    {
        bIsBossFightActive = false;
        OpenDoor();
    }
    ```

*   **성능 특성과 최적화 포인트:**

    *   불필요한 Tick 함수 호출을 피합니다.
    *   보스 액터 검색 시 캐싱을 활용합니다.
    *   보스 초기화 시 필요한 작업만 수행합니다.

*   **사용 시나리오와 베스트 프랙티스:**

    *   보스 스폰, 보스 초기화, 전투 시작/종료, 문 개방 등 보스 전투의 전체적인 흐름을 관리합니다.
    *   싱글톤 패턴을 사용하여 게임 내에서 유일한 인스턴스만 존재하도록 합니다.
    *   게임 디자이너가 쉽게 보스 전투를 구성할 수 있도록 인터페이스를 제공합니다.

*   **다른 클래스와의 상호작용:**

    *   `ACBoss`: 관리 대상인 보스 캐릭터입니다.
    *   `UBossStateTreeComponent`: 보스의 AI를 제어하는 StateTree 컴포넌트입니다.
    *   `AGate`: 보스 전투 종료 후 개방될 문 액터입니다.

### 3. `ABossProjectileActor`

*   **클래스 개요:**

    *   `ABossProjectileActor`는 보스가 발사하는 투사체의 기본 클래스입니다. 투사체의 이동, 충돌 판정, 이펙트 재생 등을 담당합니다.
    *   이 클래스는 오브젝트 풀링을 통해 성능을 최적화합니다.
    *   설계 철학은 투사체의 재사용성을 높이고, 다양한 투사체 행동을 쉽게 구현할 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**

    *   `FireProjectile()`: 투사체를 발사합니다. 투사체의 속도, 방향 등을 설정합니다.

        ```c++
        void ABossProjectileActor::FireProjectile(FVector Direction, float Speed)
        {
            // 투사체 이동 컴포넌트 활성화
            if (ProjectileMovementComponent)
            {
                ProjectileMovementComponent->Activate();
                ProjectileMovementComponent->Velocity = Direction * Speed;
            }

            // 스폰 이펙트 재생
            PlaySpawnEffect();
        }
        ```

    *   `FireProjectileToLocation()`: 특정 위치로 투사체를 발사합니다.

    *   `PlaySpawnEffect()`: 투사체 스폰 시 이펙트를 재생합니다.

    *   `PlayDestroyEffect()`: 투사체 소멸 시 이펙트를 재생합니다.

    *   `OnProjectileHit()`: 투사체가 충돌했을 때 호출됩니다. 데미지를 가하고, 소멸 이펙트를 재생하는 등의 작업을 수행합니다.

*   **클래스의 생명주기와 상태 관리:**

    *   `ABossProjectileActor`는 `UBossProjectileComponent`에 의해 생성되고 관리됩니다.
    *   `UBossProjectileComponent`는 오브젝트 풀을 사용하여 `ABossProjectileActor` 인스턴스를 재활용합니다.
    *   투사체의 상태는 활성화 여부로 관리됩니다.

*   **실제 사용 예제와 코드:**

    ```c++
    // 보스가 투사체를 발사할 때
    void ACBoss::Attack()
    {
        // ... 공격 로직 ...

        // 투사체 발사
        if (BossProjectileComponent)
        {
            FVector Direction = GetActorForwardVector();
            BossProjectileComponent->ShotProjectile(Direction, 1000.0f);
        }
    }

    // 투사체가 충돌했을 때
    void ABossProjectileActor::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
    {
        // ... 데미지 처리 로직 ...

        // 소멸 이펙트 재생
        PlayDestroyEffect();

        // 투사체 비활성화 및 풀로 반환
        DeactivateProjectile();
        UBossProjectileComponent* ProjectileComponent = Cast<UBossProjectileComponent>(GetComponentByClass(UBossProjectileComponent::StaticClass()));
        if(ProjectileComponent)
        {
            ProjectileComponent->ReturnProjectileToPool(this);
        }
    }
    ```

*   **성능 특성과 최적화 포인트:**

    *   오브젝트 풀링을 통해 투사체 생성 및 소멸 비용을 줄입니다.
    *   투사체의 이동 및 충돌 판정을 최적화합니다.
    *   불필요한 Tick 함수 호출을 피합니다. 투사체가 활성화되어 있는 동안에만 Tick 함수를 호출하도록 구현할 수 있습니다.

*   **사용 시나리오와 베스트 프랙티스:**

    *   보스의 원거리 공격을 구현하는 데 사용됩니다.
    *   `UBossProjectileComponent`를 사용하여 투사체를 중앙 집중적으로 관리합니다.
    *   오브젝트 풀링을 통해 성능을 최적화합니다.

*   **다른 클래스와의 상호작용:**

    *   `UBossProjectileComponent`: 투사체 풀 관리 및 투사체 발사를 담당합니다.
    *   `ACBoss`: 투사체를 발사하는 보스 캐릭터입니다.

### 4. `ABossProjectileOrb`

*   **클래스 개요:**

    *   `ABossProjectileOrb`는 `ABossProjectileActor`를 상속받아 구현된 특수한 투사체입니다. 궤적을 그리며 이동하거나, 특정 위치에 정지해 있다가 발사되는 등 복잡한 행동을 구현할 수 있습니다.
    *   이 클래스는 `ABossProjectileActor`의 기능을 확장하여 더욱 다양한 투사체 패턴을 만들 수 있도록 합니다.

*   **핵심 메서드 및 프로퍼티 분석:**

    *   `SpawnProjectile()`: 오브를 생성하는 함수입니다.

    *   `DestroyOrb()`: 오브를 소멸시키는 함수입니다.

    *   `OnOverlap()`: 오브가 다른 액터와 겹쳤을 때 호출되는 함수입니다.

    *   `PlaySpawnSound()`: 오브 생성 시 사운드를 재생합니다.

    *   `PlayReturnToPoolSound()`: 오브가 풀로 돌아갈 때 사운드를 재생합니다.

    *   `PlayCollisionSound()`: 오브가 충돌했을 때 사운드를 재생합니다.

    *   `PlaySpawnEffect()`: 오브 생성 시 이펙트를 재생합니다.

    *   `PlayReturnToPoolEffect()`: 오브가 풀로 돌아갈 때 이펙트를 재생합니다.

    *   `PlayCollisionEffect()`: 오브가 충돌했을 때 이펙트를 재생합니다.

    *   `DestroyOrbWithDelay()`: 오브를 지연시간 후 소멸시킵니다.

    *   `ActivateOrb()`: 오브를 활성화합니다.

*   **클래스의 생명주기와 상태 관리:**

    *   `ABossProjectileOrb`는 `UBossProjectileComponent`에 의해 생성되고 관리됩니다.
    *   `UBossProjectileComponent`는 오브젝트 풀을 사용하여 `ABossProjectileOrb` 인스턴스를 재활용합니다.
    *   투사체의 상태는 활성화 여부로 관리됩니다.

*   **실제 사용 예제와 코드:**

    ```c++
    // 보스가 오브를 발사할 때
    void ACBoss::Attack()
    {
        // ... 공격 로직 ...

        // 오브 발사
        if (BossProjectileComponent)
        {
            BossProjectileComponent->SpawnOrb();
        }
    }

    // 오브가 충돌했을 때
    void ABossProjectileOrb::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
    {
        // ... 데미지 처리 로직 ...

        // 충돌 이펙트 재생
        PlayCollisionEffect();

        // 오브 비활성화 및 풀로 반환
        DestroyOrb();
    }
    ```

*   **성능 특성과 최적화 포인트:**

    *   오브젝트 풀링을 통해 투사체 생성 및 소멸 비용을 줄입니다.
    *   투사체의 이동 및 충돌 판정을 최적화합니다.
    *   불필요한 Tick 함수 호출을 피합니다. 투사체가 활성화되어 있는 동안에만 Tick 함수를 호출하도록 구현할 수 있습니다.

*   **사용 시나리오와 베스트 프랙티스:**

    *   보스의 유도 미사일, 궤적을 그리는 투사체 등 복잡한 투사체 패턴을 구현하는 데 사용됩니다.
    *   `UBossProjectileComponent`를 사용하여 투사체를 중앙 집중적으로 관리합니다.
    *   오브젝트 풀링을 통해 성능을 최적화합니다.

*   **다른 클래스와의 상호작용:**

    *   `UBossProjectileComponent`: 투사체 풀 관리 및 투사체 발사를 담당합니다.
    *   `ACBoss`: 투사체를 발사하는 보스 캐릭터입니다.

### 5. `ACBoss`

*   **클래스 개요:**

    *   `ACBoss`는 보스 캐릭터의 기본 클래스입니다. 보스의 HP, 상태, AI, 공격 패턴 등을 관리합니다.
    *   이 클래스는 `ACharacter`를 상속받아 구현되며, 언리얼 엔진의 캐릭터 시스템을 활용합니다.
    *   설계 철학은 보스 캐릭터의 기본적인 기능을 제공하고, 다양한 보스 캐릭터를 쉽게 만들 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**

    *   `TakeDamage()`: 데미지를 처리합니다. 보스의 HP를 감소시키고, 피격 이펙트를 재생하는 등의 작업을 수행합니다.

        ```c++
        float ACBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
        {
            // ... 데미지 처리 로직 ...

            // HP 감소
            HP -= DamageAmount;
            HP = FMath::Max(HP, 0.0f);

            // HP 업데이트 UI
            HPUpdate();

            // 피격 모션 재생
            PlayHitMotion();

            // ... 기타 데미지 처리 로직 ...

            return DamageAmount;
        }
        ```

    *   `PlayHitMotion()`: 피격 모션을 재생합니다.

    *   `ShowBossStatusWidget()`: 보스 상태 위젯을 표시합니다.

    *   `HPUpdate()`: 보스의 HP를 업데이트합니다.

    *   `PlayBossBGM()`: 보스 배경 음악을 재생합니다.

    *   `StopBossBGM()`: 보스 배경 음악을 정지합니다.

    *   `LowerBossBGMVolume()`: 보스 배경 음악 볼륨을 낮춥니다.

*   **클래스의 생명주기와 상태 관리:**

    *   `ACBoss`는 게임 시작 시 스폰됩니다.
    *   보스의 상태는 `ACBoss` 내부 변수로 관리됩니다. (예: `EBossState State`)

*   **실제 사용 예제와 코드:**

    ```c++
    // 보스가 공격할 때
    void ACBoss::Attack()
    {
        // ... 공격 로직 ...

        // 애니메이션 몽타주 재생
        PlayAnimMontage(AttackMontage);

        // ... 기타 공격 로직 ...
    }

    // 보스가 사망했을 때
    void ACBoss::Die()
    {
        // ... 사망 처리 로직 ...

        // 애니메이션 몽타주 재생
        PlayAnimMontage(DeathMontage);

        // ... 기타 사망 처리 로직 ...
    }
    ```

*   **성능 특성과 최적화 포인트:**

    *   불필요한 Tick 함수 호출을 피합니다.
    *   AI 로직을 최적화합니다.
    *   애니메이션 몽타주 재생 시 메모리 사용량을 줄입니다.

*   **사용 시나리오와 베스트 프랙티스:**

    *   보스 캐릭터의 기본적인 기능을 제공합니다.
    *   다양한 보스 캐릭터를 쉽게 만들 수 있도록 인터페이스를 제공합니다.
    *   보스 AI를 StateTree로 구현하여 유연성을 높입니다.

*   **다른 클래스와의 상호작용:**

    *   `ACBossAIC`: 보스의 AI를 제어하는 AIController입니다.
    *   `UBossStateTreeComponent`: 보스의 AI를 제어하는 StateTree 컴포넌트입니다.
    *   `UBossStatusWidget`: 보스의 상태를 표시하는 위젯입니다.
    *   `UBossProjectileComponent`: 보스의 투사체를 관리하는 컴포넌트입니다.
    *   `UBossEffectManager`: 보스의 이펙트를 관리하는 컴포넌트입니다.

### 6. `ACBossAIC`

*   **클래스 개요:**

    *   `ACBossAIC`는 보스 캐릭터의 AI를 제어하는 AIController 클래스입니다. StateTree를 사용하여 보스의 행동을 정의합니다.
    *   이 클래스는 `AAIController`를 상속받아 구현되며, 언리얼 엔진의 AI 시스템을 활용합니다.
    *   설계 철학은 보스 AI의 복잡성을 숨기고, 게임 디자이너가 쉽게 보스 AI를 구성할 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**

    *   `OnPossess()`: Pawn을 소유했을 때 호출됩니다. StateTree를 시작하는 등의 초기화 작업을 수행합니다.

        ```c++
        void ACBossAIC::OnPossess(APawn* InPawn)
        {
            Super::OnPossess(InPawn);

            // StateTree 컴포넌트 가져오기
            UBossStateTreeComponent* StateTreeComponent = InPawn->FindComponentByClass<UBossStateTreeComponent>();
            if (StateTreeComponent)
            {
                // StateTree 시작
                StateTreeComponent->StartLogic();
            }
        }
        ```

*   **클래스의 생명주기와 상태 관리:**

    *   `ACBossAIC`는 보스 캐릭터가 스폰될 때 함께 스폰됩니다.
    *   보스의 AI 상태는 StateTree에서 관리됩니다.

*   **실제 사용 예제와 코드:**

    ```c++
    // StateTree에서 특정 행동을 실행하도록 지시
    void ACBossAIC::PerformAction(FName ActionName)
    {
        // ... 액션 실행 로직 ...
    }
    ```

*   **성능 특성과 최적화 포인트:**

    *   StateTree의 노드 수를 줄입니다.
    *   복잡한 계산은 C++ 코드에서 수행합니다.
    *   불필요한 Tick 함수 호출을 피합니다.

*   **사용 시나리오와 베스트 프랙티스:**

    *   보스 AI를 StateTree로 구현하여 유연성을 높입니다.
    *   게임 디자이너가 쉽게 보스 AI를 구성할 수 있도록 인터페이스를 제공합니다.

*   **다른 클래스와의 상호작용:**

    *   `ACBoss`: AI를 제어하는 보스 캐릭터입니다.
    *   `UBossStateTreeComponent`: 보스의 AI를 제어하는 StateTree 컴포넌트입니다.

### 7. `ACBossWeapon`

*   **클래스 개요:**

    *   `ACBossWeapon`은 보스 캐릭터가 사용하는 무기의 기본 클래스입니다. 무기의 콜리전, 데미지 처리, 이펙트 재생 등을 담당합니다.
    *   이 클래스는 `AActor`를 상속받아 구현되며, 언리얼 엔진의 액터 시스템을 활용합니다.
    *   설계 철학은 무기의 기본적인 기능을 제공하고, 다양한 무기를 쉽게 만들 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**

    *   `OnBossBeginEquip()`: 보스가 무기를 장착하기 시작할 때 호출됩니다.

    *   `OnBossUnequip()`: 보스가 무기를 해제하기 시작할 때 호출됩니다.

    *   `OnBossCollisions()`: 보스 콜리전이 시작될 때 호출됩니다.

    *   `OnSelectCollision()`: 특정 콜리전을 선택합니다.

    *   `OffBossCollisions()`: 보스 콜리전을 해제합니다.

    *   `BossAttachToCollision()`: 콜리전에 보스를 부착합니다.

    *   `StartCollisionAtSocket()`: 소켓에서 콜리전을 시작합니다.

    *   `EndCollisionToOwner()`: 소유자에게 콜리전을 종료합니다.

    *   `BossAttachTo()`: 보스를 부착합니다.

    *   `OnBossComponentBeginOverlap()`: 보스 컴포넌트 오버랩이 시작될 때 호출됩니다.

    *   `OnBossComponentEndOverlap()`: 보스 컴포넌트 오버랩이 종료될 때 호출됩니다.

*   **클래스의 생명주기와 상태 관리:**

    *   `ACBossWeapon`는 보스 캐릭터가 스폰될 때 함께 스폰되거나, 필요에 따라 동적으로 생성될 수 있습니다.
    *   무기의 상태는 `ACBossWeapon` 내부 변수로 관리됩니다. (예: `bIsEquipped`)

*   **실제 사용 예제와 코드:**

    ```c++
    // 무기가 충돌했을 때
    void ACBossWeapon::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
    {
        // ... 데미지 처리 로직 ...

        // 충돌 이펙트 재생
        PlayCollisionEffect();
    }
    ```

*   **성능 특성과 최적화 포인트:**

    *   콜리전 채널을 최적화합니다.
    *   불필요한 Tick 함수 호출을 피합니다.

*   **사용 시나리오와 베스트 프랙티스:**

    *   보스 캐릭터가 사용하는 무기의 기본적인 기능을 제공합니다.
    *   다양한 무기를 쉽게 만들 수 있도록 인터페이스를 제공합니다.

*   **다른 클래스와의 상호작용:**

    *   `ACBoss`: 무기를 사용하는 보스 캐릭터입니다.

### 8. `AFlySpline`

*   **클래스 개요:**

    *   `AFlySpline`은 보스가 비행할 때 사용하는 스플라인 액터입니다. 스플라인을 따라 보스가 이동하도록 합니다.
    *   이 클래스는 `AActor`를 상속받아 구현되며, 언리얼 엔진의 액터 시스템과 스플라인 컴포넌트를 활용합니다.
    *   설계 철학은 보스의 비행 경로를 쉽게 만들고, 수정할 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**

    *   `OnConstruction()`: 액터가 생성되거나 속성이 변경될 때 호출됩니다. 스플라인 메시를 생성하는 등의 작업을 수행합니다.

    *   `BuildCylinderAndRims()`: 실린더와 림을 생성합니다.

    *   `GetHorizontalSplines()`: 수평 스플라인을 가져옵니다.

    *   `GetSplineAtIndex()`: 특정 인덱스의 스플라인을 가져옵니다.

    *   `BuildRim()`: 림을 생성합니다.

    *   `CreateHorizontalSplines()`: 수평 스플라인을 생성합니다.

*   **클래스의 생명주기와 상태 관리:**

    *   `AFlySpline`은 레벨 에디터에서 배치됩니다.
    *   스플라인의 상태는 `AFlySpline` 내부 변수로 관리됩니다.

*   **실제 사용 예제와 코드:**

    ```c++
    // 보스가 스플라인을 따라 이동
    void ACBoss::Tick(float DeltaTime)
    {
        Super::Tick(DeltaTime);

        // ... 비행 로직 ...

        // 스플라인 위치 계산
        FVector SplineLocation = FlySpline->GetLocationAtTime(CurrentSplineTime);
        SetActorLocation(SplineLocation);

        // ... 기타 비행 로직 ...
    }
    ```

*   **성능 특성과 최적화 포인트:**

    *   스플라인 메시의 복잡도를 줄입니다.
    *   불필요한 Tick 함수 호출을 피합니다.

*   **사용 시나리오와 베스트 프랙티스:**

    *   보스의 비행 경로를 쉽게 만들고, 수정할 수 있도록 합니다.

*   **다른 클래스와의 상호작용:**

    *   `ACBoss`: 스플라인을 따라 이동하는 보스 캐릭터입니다.

### 9. `AGateOfBabylon`

*   **클래스 개요:**

    *   `AGateOfBabylon`은 보스가 사용하는 특수한 공격 패턴인 "바빌론의 문"을 구현하는 액터입니다.  플레이어 주변에 투사체를 소환하여 공격합니다.
    *   이 클래스는 오브젝트 풀링을 사용하여 투사체 생성 및 소멸 비용을 최적화합니다.
    *   설계 철학은 화려하고 위협적인 공격 패턴을 쉽게 구현할 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**

    *   `ActivateGate()`: 바빌론의 문 공격을 시작합니다. 투사체 풀에서 투사체를 가져와 플레이어 주변에 소환합니다.

    *   `DeactivateGate()`: 바빌론의 문 공격을 종료합니다. 소환된 투사체를 비활성화하고 풀로 반환합니다.

    *   `InitializeProjectilePool()`: 투사체 풀을 초기화합니다.

    *   `GetProjectileFromPool()`: 투사체 풀에서 사용 가능한 투사체를 가져옵니다.

    *   `SpawnProjectile()`: 투사체를 월드에 소환합니다.

    *   `UpdateLookAtPlayer()`: 투사체가 플레이어를 바라보도록 회전합니다.

*   **클래스의 생명주기와 상태 관리:**

    *   `AGateOfBabylon`은 보스 전투 중에 필요에 따라 생성되고 소멸됩니다.
    *   공격의 상태는 `AGateOfBabylon` 내부 변수로 관리됩니다. (예: `bIsActive`)

*   **실제 사용 예제와 코드:**

    ```c++
    // 보스가 바빌론의 문 공격을 시작할 때
    void ACBoss::Attack()
    {
        // ... 공격 로직 ...

        // 바빌론의 문 공격 시작
        if (GateOfBabylon)
        {
            GateOfBabylon->ActivateGate();
        }
    }

    // 바빌론의 문 투사체 소환
    void AGateOfBabylon::ActivateGate()
    {
        // ... 투사체 소환 로직 ...

        // 투사체 풀에서 투사체 가져오기
        AGateOfBabyonProjectile* Projectile = GetProjectileFromPool();
        if (Projectile)
        {
            // 투사체 위치 설정
            FVector SpawnLocation = GetActorLocation() + FVector(FMath::FRandRange(-500.0f, 500.0f), FMath::FRandRange(-500.0f, 500.0f), 0.0f);
            Projectile->SetActorLocation(SpawnLocation);

            // 투사체 활성화
            Projectile->ActivateProjectile();
        }
    }
    ```

*   **성능 특성과 최적화 포인트:**

    *   오브젝트 풀링을 통해 투사체 생성 및 소멸 비용을 줄입니다.
    *   투사체의 이동 및 충돌 판정을 최적화합니다.
    *   불필요한 Tick 함수 호출을 피합니다.

*   **사용 시나리오와 베스트 프랙티스:**

    *   보스의 화려하고 위협적인 공격 패턴을 구현하는 데 사용됩니다.
    *   오브젝트 풀링을 통해 성능을 최적화합니다.

*   **다른 클래스와의 상호작용:**

    *   `AGateOfBabyonProjectile`: 바빌론의 문 투사체 클래스입니다.
    *   `ACBoss`: 바빌론의 문 공격을 사용하는 보스 캐릭터입니다.

### 10. `AGateOfBabyonProjectile`

*   **클래스 개요:**

    *   `AGateOfBabyonProjectile`은 `AGateOfBabylon`에서 소환되는 투사체의 클래스입니다.  투사체의 이동, 충돌 판정, 이펙트 재생 등을 담당합니다.
    *   이 클래스는 `AActor`를 상속받아 구현되며, 언리얼 엔진의 액터 시스템을 활용합니다.
    *   설계 철학은 투사체의 재사용성을 높이고, 다양한 투사체 행동을 쉽게 구현할 수 있도록 하는 것입니다.

*   **핵심 메서드 및 프로퍼티 분석:**

    *   `ActivateProjectile()`: 투사체를 활성화합니다. 투사체의 속도, 방향 등을 설정합니다.

    *   `DeactivateProjectile()`: 투사체를 비활성화합니다.

    *   `OnBeginOverlap()`: 투사체가 다른 액터와 겹쳤을 때 호출됩니다. 데미지를 가하고, 소멸 이펙트를 재생하는 등의 작업을 수행합니다.

    *   `MoveToRandomLocationAroundPlayer()`: 플레이어 주변의 랜덤한 위치로 투사체를 이동합니다.

*   **클래스의 생명주기와 상태 관리:**

    *   `AGateOfBabyonProjectile`는 `AGateOfBabylon`에 의해 생성되고 관리됩니다.
    *   투사체의 상태는 활성화 여부로 관리됩니다.

*   **실제 사용 예제와 코드:**

    ```c++
    // 투사체가 활성화될 때
    void AGateOfBabyonProjectile::ActivateProjectile()
    {
        // ... 투사체 활성화 로직 ...

        // 투사체 이동
        MoveToRandomLocationAroundPlayer();
    }

    // 투사체가 충돌했을 때
    void AGateOfBabyonProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
    {
        // ... 데미지 처리 로직 ...

        // 투사체 비활성화
        DeactivateProjectile();
    }
    ```

*   **성능 특성과 최적화 포인트:**

    *   투사체의 이동 및 충돌 판정을 최적화합니다.
    *   불필요한 Tick 함수 호출을 피합니다.

*   **사용 시나리오와 베스트 프랙티스:**

    *   바빌론의 문 공격 패턴을 구현하는 데 사용됩니다.

*   **다른 클래스와의 상호작용:**

    *   `AGateOfBabylon`: 투사체를 소환하고 관리하는 액터입니다.

### 11. `AHolySwordMagic`

*   **클래

## 클래스 분석 상세 문서 (중간 부분)

이 문서는 언리얼 엔진 프로젝트 내의 보스 AI 및 전투 시스템과 관련된 클래스들을 상세히 분석합니다. 각 클래스의 역할, 기능, 상호 작용, 그리고 최적화 방안을 제시하여 프로젝트의 이해도를 높이고 효율적인 개발을 돕는 것을 목표로 합니다. 이전 섹션에서는 프로젝트의 개요와 핵심 클래스들의 기능 목록을 제공했습니다. 이제 각 클래스를 더욱 깊이 있게 분석하여 실제 사용 예제와 코드, 성능 특성, 확장성, 유지보수성, 그리고 실무 적용 가이드를 제공합니다.

### 1. 클래스 개요 (계속)

이 프로젝트는 복잡한 보스 전투 시스템을 구현하고 있으며, 다양한 클래스들이 유기적으로 연결되어 동작합니다. 크게 다음과 같은 역할 그룹으로 나눌 수 있습니다.

*   **보스 액터 및 AI:** `ACBoss`, `ACBossAIC`, `CBossMovementComponent`, `CBossStatusComponent`, `CBossTargetingComponent` 등은 보스의 기본적인 동작, AI, 이동, 스탯 관리, 타겟팅 등을 담당합니다. StateTree를 사용하여 복잡한 AI 로직을 구현하고 있습니다.
*   **무기 및 액션:** `ACBossWeapon`, `UCBossWeaponAsset`, `UCBossDoAction`, `UCBossDoAction_ComBo` 등은 보스의 무기 관리, 공격 액션, 콤보 액션 등을 처리합니다. 애니메이션 노티파이를 통해 액션과 애니메이션을 동기화합니다.
*   **이펙트 및 투사체:** `ABossEffect`, `ABossProjectileActor`, `ABossProjectileOrb`, `AGateOfBabylon`, `AGateOfBabyonProjectile`, `AHolySwordMagic`, `UAnimNotify_PlayEffect`, `UBossEffectManager`, `UBossProjectileComponent` 등은 보스의 다양한 이펙트, 투사체, 마법 공격 등을 구현합니다. 오브젝트 풀링을 사용하여 성능을 최적화합니다.
*   **애니메이션:** `UBossAnimInstance`, `UAnimNotify_*` 들은 보스의 애니메이션 로직을 처리하고, 애니메이션과 게임 로직을 연결하는 역할을 합니다.
*   **상태 관리:** `BossStateComponent.h`, `USTC_*`, `UTask_*` 들은 보스의 상태를 관리하고, StateTree에서 사용되는 조건과 태스크들을 제공합니다.
*   **UI:** `UBossStatusWidget`, `UDDTLoadingWidget`, `UDDTMainThemeWidget` 등은 보스의 상태를 표시하고, 게임 UI를 관리합니다.
*   **에디터 플러그인:** `FEditorPlugin_DataSyncModule`, `FEditorPlugin_DataSyncCommands`, `FEditorPlugin_DataSyncStyle` 등은 에디터 환경에서 데이터 동기화 및 관리를 위한 기능을 제공합니다.

이러한 클래스들은 서로 의존하며, 복잡한 상호 작용을 통해 보스 전투 시스템을 구성합니다. 예를 들어, `ACBossAIC`는 StateTree를 사용하여 보스의 행동을 결정하고, `CBossMovementComponent`를 통해 이동을 제어하며, `ACBossWeapon`을 사용하여 공격을 수행합니다. 공격 시에는 `UAnimNotify_*`를 통해 애니메이션과 동기화하고, `UBossProjectileComponent`를 통해 투사체를 발사하며, `ABossEffect`를 통해 시각 효과를 생성합니다.

### 2. 핵심 클래스 분석 (계속)

이 섹션에서는 몇 가지 핵심 클래스를 선정하여 더욱 상세하게 분석합니다.

#### 2.1. `ACBoss`

*   **목적과 책임:** `ACBoss`는 보스 캐릭터의 기본 클래스이며, 보스의 체력, 데미지 처리, UI 표시, 배경 음악 재생 등 기본적인 기능을 담당합니다.

*   **주요 메서드와 프로퍼티:**
    *   `TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)`: 데미지를 처리하고, 체력을 감소시키며, 필요한 UI 업데이트를 수행합니다.
    ```c++
    float ACBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
    {
        float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
        if (ActualDamage > 0.0f)
        {
            CurrentHP -= ActualDamage;
            HPUpdate(); // UI 업데이트
            if (CurrentHP <= 0.0f)
            {
                // 사망 처리
            }
        }
        return ActualDamage;
    }
    ```
    *   `BeginPlay()`: 게임 시작 시 호출되며, 보스의 초기 위치를 저장하고, UI를 표시하며, 배경 음악을 재생합니다.
    *   `PlayHitMotion()`: 피격 시 애니메이션을 재생합니다.
    *   `ShowBossStatusWidget()`: 보스 상태 위젯을 표시합니다.
    *   `HPUpdate()`: 보스 체력 UI를 업데이트합니다.
    *   `PlayBossBGM()`: 보스 배경 음악을 재생합니다.
    *   `StopBossBGM()`: 보스 배경 음악을 정지합니다.
    *   `LowerBossBGMVolume()`: 보스 배경 음악 볼륨을 감소시킵니다 (사망 시).

*   **클래스의 생명주기와 상태 관리:** `ACBoss`는 게임 시작 시 생성되고, 체력이 0이 되면 사망합니다. 상태는 체력, 활성화 여부 등으로 관리됩니다.

*   **실제 사용 예제와 코드:**
    ```c++
    // ACBoss.h
    UCLASS()
    class MYPROJECT_API ACBoss : public ACharacter
    {
        GENERATED_BODY()

    public:
        // Sets default values for this character's properties
        ACBoss();

    protected:
        // Called when the game starts or when spawned
        virtual void BeginPlay() override;

    public:
        // Called every frame
        virtual void Tick(float DeltaTime) override;

        virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

        UFUNCTION(BlueprintCallable)
        void PlayBossBGM();

        UFUNCTION(BlueprintCallable)
        void StopBossBGM();

        UFUNCTION(BlueprintCallable)
        void LowerBossBGMVolume();

    private:
        UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
        float CurrentHP;

        UFUNCTION()
        void HPUpdate();

        UFUNCTION()
        void ShowBossStatusWidget();
    };
    ```

*   **성능 특성과 최적화 포인트:** `ACBoss`는 `ACharacter`를 상속받으므로, 기본적인 캐릭터 이동 및 충돌 처리에 대한 성능 고려가 필요합니다. UI 업데이트는 빈번하게 호출될 수 있으므로, 최적화가 필요합니다.

*   **사용 시나리오와 베스트 프랙티스:** 보스 캐릭터의 기본적인 기능을 구현하고, 다른 컴포넌트들과 상호 작용하여 더욱 복잡한 행동을 구현합니다.

*   **다른 클래스와의 상호작용:**
    *   `ACBossAIC`: 보스의 AI를 제어합니다.
    *   `CBossStatusComponent`: 보스의 스탯을 관리합니다.
    *   `UBossStatusWidget`: 보스의 상태를 UI에 표시합니다.

#### 2.2. `ACBossAIC`

*   **목적과 책임:** `ACBossAIC`는 보스 AI 컨트롤러 클래스이며, StateTree를 사용하여 보스의 행동을 결정하고 제어합니다.

*   **주요 메서드와 프로퍼티:**
    *   `OnPossess(APawn* InPawn)`: Pawn을 소유할 때 호출되며, StateTree를 시작합니다.

*   **클래스의 생명주기와 상태 관리:** `ACBossAIC`는 보스 액터가 스폰될 때 생성되고, 보스가 사망하면 소멸됩니다. 상태는 StateTree에 의해 관리됩니다.

*   **실제 사용 예제와 코드:**
    ```c++
    // ACBossAIC.h
    UCLASS()
    class MYPROJECT_API ACBossAIC : public AAIController
    {
        GENERATED_BODY()

    public:
        ACBossAIC();

    protected:
        virtual void OnPossess(APawn* InPawn) override;
    };

    // ACBossAIC.cpp
    void ACBossAIC::OnPossess(APawn* InPawn)
    {
        Super::OnPossess(InPawn);

        // StateTree 시작 로직
        if (InPawn != nullptr)
        {
            // ... StateTree 시작 코드 ...
        }
    }
    ```

*   **성능 특성과 최적화 포인트:** StateTree의 복잡도가 높을수록 AI 연산에 대한 성능 부담이 증가할 수 있습니다. StateTree의 구조를 최적화하고, 불필요한 연산을 줄이는 것이 중요합니다.

*   **사용 시나리오와 베스트 프랙티스:** StateTree를 사용하여 복잡한 보스 AI를 구현하고, 다양한 조건과 태스크를 조합하여 다양한 행동 패턴을 생성합니다.

*   **다른 클래스와의 상호작용:**
    *   `ACBoss`: 보스 액터를 제어합니다.
    *   `CBossMovementComponent`: 보스의 이동을 제어합니다.
    *   `CBossWeaponComponent`: 보스의 공격을 제어합니다.
    *   `UTask_*`: StateTree에서 사용되는 태스크들을 실행합니다.
    *   `USTC_*`: StateTree에서 사용되는 조건들을 평가합니다.

#### 2.3. `UBossProjectileComponent`

*   **목적과 책임:** `UBossProjectileComponent`는 보스의 투사체 시스템을 관리하는 컴포넌트이며, 투사체 생성, 발사, 오브젝트 풀링 등을 담당합니다.

*   **주요 메서드와 프로퍼티:**
    *   `ShotProjectile()`: 투사체를 발사합니다.
    *   `SpawnOrb()`: 오브를 생성합니다.
    *   `ShotProjectileToLocation()`: 특정 위치로 투사체를 발사합니다.
    *   `DestroyOrb()`: 오브를 파괴합니다.
    *   `SpawnOrbContinuously()`: 오브를 연속적으로 생성합니다.
    *   `CancelOrbContinuousSpawning()`: 오브 연속 생성을 중단합니다.
    *   `SpawnProjectileContinuously()`: 투사체를 연속적으로 생성합니다.
    *   `CancelProjectileContinuousSpawning()`: 투사체 연속 생성을 중단합니다.
    *   `SpawnHolySwordMagicRepeatedly()`: 성검 마법을 반복적으로 생성합니다.
    *   `CancelHolySwordMagicSpawning()`: 성검 마법 생성을 중단합니다.
    *   `ResetProjectileSystem()`: 투사체 시스템을 초기화합니다.
    *   `GetBossProjectileFromPool()`: 오브젝트 풀에서 투사체를 가져옵니다.
    *   `ReturnGateToPool()`, `ReturnOrbToPool()`, `ReturnProjectileToPool()`, `ReturnHolySwordMagicToPool()`, `ReturnBossProjectileToPool()`: 사용 완료된 투사체를 오브젝트 풀에 반환합니다.

*   **클래스의 생명주기와 상태 관리:** `UBossProjectileComponent`는 보스 액터가 스폰될 때 생성되고, 보스가 사망하면 소멸됩니다. 투사체는 오브젝트 풀을 통해 관리되며, 활성화/비활성화 상태를 가집니다.

*   **실제 사용 예제와 코드:**
    ```c++
    // UBossProjectileComponent.h
    UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
    class MYPROJECT_API UBossProjectileComponent : public UActorComponent
    {
        GENERATED_BODY()

    public:
        UBossProjectileComponent();

    protected:
        virtual void BeginPlay() override;

    public:
        UFUNCTION(BlueprintCallable)
        void ShotProjectile();

        UFUNCTION(BlueprintCallable)
        void SpawnOrb();

    private:
        // 오브젝트 풀 관련 변수
        TArray<ABossProjectileActor*> ProjectilePool;

        ABossProjectileActor* GetProjectileFromPool();
        void ReturnProjectileToPool(ABossProjectileActor* Projectile);
    };

    // UBossProjectileComponent.cpp
    void UBossProjectileComponent::ShotProjectile()
    {
        ABossProjectileActor* Projectile = GetProjectileFromPool();
        if (Projectile)
        {
            // 투사체 발사 로직
            Projectile->FireProjectile();
        }
    }

    ABossProjectileActor* UBossProjectileComponent::GetProjectileFromPool()
    {
        if (ProjectilePool.Num() > 0)
        {
            ABossProjectileActor* Projectile = ProjectilePool.Pop();
            Projectile->SetActorHiddenInGame(false);
            Projectile->SetActorEnableCollision(true);
            return Projectile;
        }
        else
        {
            // 풀에 투사체가 없으면 새로 생성
            ABossProjectileActor* NewProjectile = GetWorld()->SpawnActor<ABossProjectileActor>(ABossProjectileActor::StaticClass());
            return NewProjectile;
        }
    }

    void UBossProjectileComponent::ReturnProjectileToPool(ABossProjectileActor* Projectile)
    {
        Projectile->SetActorHiddenInGame(true);
        Projectile->SetActorEnableCollision(false);
        ProjectilePool.Add(Projectile);
    }
    ```

*   **성능 특성과 최적화 포인트:** 오브젝트 풀링을 사용하여 투사체 생성 및 소멸에 대한 성능 부담을 줄입니다. 투사체의 이동 및 충돌 판정에 대한 최적화도 중요합니다.

*   **사용 시나리오와 베스트 프랙티스:** 보스의 다양한 투사체 공격을 구현하고, 오브젝트 풀링을 통해 성능을 최적화합니다.

*   **다른 클래스와의 상호작용:**
    *   `ACBoss`: 보스 액터에 부착되어 사용됩니다.
    *   `ABossProjectileActor`, `ABossProjectileOrb`, `AGateOfBabyonProjectile`, `AHolySwordMagic`: 다양한 투사체 액터를 생성하고 관리합니다.

### 3. 설계 패턴 분석

이 프로젝트에서 명확하게 드러나는 디자인 패턴은 다음과 같습니다.

*   **Object Pool Pattern:** `UBossProjectileComponent`와 관련된 투사체 클래스들 (`ABossProjectileActor`, `ABossProjectileOrb`, `AGateOfBabyonProjectile`, `AHolySwordMagic`)에서 오브젝트 풀 패턴이 사용됩니다. 이는 투사체의 잦은 생성 및 소멸로 인한 성능 저하를 방지하기 위해 미리 생성된 오브젝트들을 재사용하는 방식입니다.

    *   **장점:** 메모리 할당 및 해제 비용 감소, 가비지 컬렉션 부하 감소, 예측 가능한 성능.
    *   **단점:** 풀 크기 관리 필요, 활성화/비활성화 상태 관리 복잡도 증가.
    *   **대안:** 투사체 생성/소멸 빈도를 줄이는 방식 (예: 투사체 수명 증가), 비동기적인 오브젝트 생성/소멸.
    *   **효과성:** 투사체 기반 공격이 빈번한 보스 전투에서 성능 향상에 기여합니다.

*   **State Pattern (StateTree):** 보스의 AI는 StateTree를 통해 구현됩니다. StateTree는 상태 패턴의 확장된 형태로, 복잡한 상태 전이 로직을 시각적으로 표현하고 관리할 수 있도록 해줍니다.

    *   **장점:** 복잡한 AI 로직을 모듈화하고 관리하기 용이, 상태 전이 로직을 시각적으로 표현하여 이해도 향상, 새로운 상태 및 전이 조건 추가 용이.
    *   **단점:** StateTree 구조가 복잡해질수록 유지보수 어려움 증가, 디버깅 어려움 증가, 런타임 성능 저하 가능성.
    *   **대안:** Finite State Machine (FSM), Behavior Tree.
    *   **효과성:** 복잡한 보스 AI를 효율적으로 구현하고 관리할 수 있도록 해줍니다.

### 4. 성능 및 최적화

각 클래스의 성능 특성과 최적화 포인트는 위에서 개별 클래스 분석 시 언급되었습니다. 전체적인 성능 최적화를 위해서는 다음과 같은 사항을 고려해야 합니다.

*   **오브젝트 풀링:** 투사체, 이펙트 등 잦은 생성/소멸이 발생하는 오브젝트에 대해 오브젝트 풀링을 적극적으로 활용합니다.
*   **애니메이션 최적화:** 불필요한 애니메이션 연산을 줄이고, 애니메이션 Notify를 효율적으로 사용합니다.
*   **StateTree 최적화:** StateTree의 구조를 단순화하고, 불필요한 조건 및 태스크 실행을 줄입니다.
*   **UI 최적화:** UI 업데이트 빈도를 줄이고, 불필요한 위젯을 제거합니다.
*   **병렬 처리:** AI 연산, 물리 연산 등 병렬 처리가 가능한 부분을 활용하여 멀티 코어 CPU 활용도를 높입니다.
*   **프로파일링:** 언리얼 엔진의 프로파일링 도구를 사용하여 병목 지점을 파악하고, 최적화합니다.

### 5. 확장성 및 유지보수성

각 클래스는 모듈화되어 설계되었으며, StateTree, 오브젝트 풀링 등 디자인 패턴을 활용하여 확장성 및 유지보수성을 높였습니다. 하지만 다음과 같은 사항을 고려하여 더욱 개선할 수 있습니다.

*   **코드 가독성:** 코드 주석을 상세하게 작성하고, 명확한 변수 및 함수 이름을 사용합니다.
*   **테스트 가능성:** 각 클래스에 대한 유닛 테스트를 작성하여 코드의 안정성을 확보합니다.
*   **리팩토링:** 코드 중복을 줄이고, 디자인 패턴을 더욱 적극적으로 활용하여 코드의 품질을 향상시킵니다.
*   **설정 파일:** 하드 코딩된 값들을 설정 파일로 분리하여 유연성을 높입니다.
*   **Blueprint 연동:** C++ 코드를 Blueprint와 연동하여 게임 디자이너가 쉽게 콘텐츠를 수정하고 추가할 수 있도록 합니다.

### 6. 실무 적용 가이드

이 프로젝트는 복잡한 보스 전투 시스템을 구현하고 있으므로, 실제 프로젝트에 적용하기 위해서는 다음과 같은 사항을 고려해야 합니다.

*   **요구 사항 분석:** 프로젝트의 요구 사항을 명확하게 분석하고, 필요한 기능 및 성능 목표를 설정합니다.
*   **설계:** 시스템의 전체적인 구조를 설계하고, 각 클래스의 역할 및 상호 작용을 정의합니다.
*   **구현:** 설계에 따라 각 클래스를 구현하고, 유닛 테스트를 통해 코드의 안정성을 확보합니다.
*   **통합:** 각 클래스를 통합하고, 전체 시스템의 동작을 테스트합니다.
*   **최적화:** 성능 프로파일링을 통해 병목 지점을 파악하고, 최적화합니다.
*   **반복:** 요구 사항 변경에 따라 설계, 구현, 테스트, 최적화 과정을 반복합니다.

이러한 과정을 통해 이 프로젝트의 코드를 실제 게임 프로젝트에 성공적으로 적용할 수 있습니다.

(다음 섹션에서는 남은 클래스들에 대한 상세 분석과 더불어, 각 클래스 간의 상호작용, 그리고 전체 시스템의 아키텍처에 대한 심층적인 분석을 제공할 예정입니다.)


## 상세 클래스 분석

이제 제공된 데이터를 바탕으로 각 클래스의 기능과 역할을 중심으로 상세한 분석을 진행하겠습니다. 각 클래스별로 독립적인 섹션을 구성하여 분석의 집중도를 높이고, 필요한 경우 실제 코드 예제와 사용법을 포함하여 이해도를 높이겠습니다.

### 1. `ABossEffect`

*   **클래스 개요:** `ABossEffect`는 보스에게 적용되는 시각 효과(Visual Effect), 사운드 효과(Sound Effect) 등 다양한 이펙트를 관리하는 액터 클래스입니다. 이 클래스는 이펙트의 활성화, 비활성화, 특정 위치에 부착, 활성화 여부 확인 등의 기능을 제공하며, 오브젝트 풀링 시스템과 연동되어 효율적인 이펙트 관리를 가능하게 합니다.

*   **핵심 클래스 분석:**

    *   **목적 및 책임:** `ABossEffect`의 주요 목적은 보스 전투를 더욱 실감나고 역동적으로 만들어주는 다양한 이펙트를 손쉽게 제어하고 관리하는 것입니다. 이펙트의 생명주기를 관리하고, 필요한 시점에 활성화/비활성화하며, 보스의 특정 소켓에 부착하는 등의 역할을 수행합니다.
    *   **주요 메서드 및 프로퍼티:**
        *   `ActivateEffect()`: 이펙트를 활성화합니다. 파티클 시스템, 사운드, 머티리얼 변경 등 다양한 이펙트 관련 로직을 포함할 수 있습니다.
        *   `ActivateEffectAttachedToSocket()`: 이펙트를 보스의 특정 소켓에 부착하여 활성화합니다. 무기 장착 시 이펙트, 공격 시 이펙트 등에 유용하게 사용될 수 있습니다.
        *   `DeactivateEffect()`: 이펙트를 비활성화합니다. 활성화된 파티클 시스템을 정지시키거나, 사운드를 멈추는 등의 작업을 수행합니다.
        *   `IsActive()`: 현재 이펙트가 활성화되어 있는지 여부를 반환합니다. 이펙트의 상태를 확인하여 로직 분기에 활용할 수 있습니다.
        *   `GetCurrentEffectTag()`: 현재 이펙트의 태그를 반환합니다. 이펙트의 종류를 식별하고 관리하는 데 사용됩니다.
        *   `AttachToBoss()`: 이펙트를 보스 액터에 부착합니다.
        *   `AttachToSocket()`: 이펙트를 보스의 특정 소켓에 부착합니다.
        *   `PlaceInWorld()`: 이펙트를 월드에 특정 위치에 배치합니다.
    *   **생명주기 및 상태 관리:** `ABossEffect`는 액터의 일반적인 생명주기를 따릅니다. `BeginPlay()`에서 초기화 작업을 수행하고, 필요에 따라 `ActivateEffect()`와 `DeactivateEffect()`를 통해 활성화/비활성화 상태를 변경합니다. 오브젝트 풀링 시스템을 사용하는 경우, `ReturnEffectToPool()` 등의 메서드를 통해 풀로 반환될 수 있습니다.
    *   **실제 사용 예제 및 코드:**

        ```c++
        // 이펙트 활성화 예제
        void UMyBossComponent::PlayAttackEffect()
        {
            if (BossEffect)
            {
                BossEffect->ActivateEffect();
            }
        }

        // 소켓에 부착하여 이펙트 활성화 예제
        void UMyBossComponent::EquipWeaponEffect()
        {
            if (BossEffect)
            {
                BossEffect->ActivateEffectAttachedToSocket(TEXT("WeaponSocket"));
            }
        }
        ```

    *   **성능 특성 및 최적화 포인트:** `ABossEffect`는 특히 파티클 시스템과 사운드 재생에 많은 리소스를 소모할 수 있습니다. 따라서, 오브젝트 풀링을 통해 액터 생성/소멸 비용을 줄이고, 파티클 시스템의 LOD(Level of Detail) 설정을 최적화하여 성능을 향상시킬 수 있습니다. 또한, 불필요한 Tick 함수 호출을 최소화하는 것이 중요합니다.
    *   **사용 시나리오 및 베스트 프랙티스:**
        *   **공격 이펙트:** 보스가 공격할 때 발생하는 시각 효과 및 사운드 효과를 표현합니다.
        *   **피격 이펙트:** 보스가 공격에 맞았을 때 발생하는 시각 효과 및 사운드 효과를 표현합니다.
        *   **상태 변화 이펙트:** 보스의 상태가 변경될 때(예: 분노, 그로기) 발생하는 시각 효과 및 사운드 효과를 표현합니다.
        *   **무기 장착 이펙트:** 보스가 무기를 장착하거나 해제할 때 발생하는 시각 효과 및 사운드 효과를 표현합니다.
    *   **다른 클래스와의 상호작용:** `ABossEffect`는 주로 `ABossManager`, `ACBoss`, `UBossEffectManager`, `UBossEffectExecute` 등의 클래스와 상호작용합니다. `ABossManager`는 이펙트를 생성하고 관리하며, `ACBoss`는 이펙트를 요청하고, `UBossEffectManager`는 이펙트 풀링을 관리하고, `UBossEffectExecute`는 애니메이션 노티파이를 통해 이펙트를 실행합니다.

*   **설계 패턴 분석:** 오브젝트 풀링 패턴이 사용되어 이펙트 액터의 생성 및 소멸 비용을 줄이고 성능을 최적화합니다.

*   **성능 및 최적화:** 오브젝트 풀링, 파티클 시스템 LOD 최적화, 불필요한 Tick 함수 호출 최소화 등을 통해 성능을 향상시킬 수 있습니다.

*   **확장성 및 유지보수성:** 이펙트의 종류를 추가하거나 수정하기 쉽도록 설계되었으며, 데이터 테이블을 활용하여 이펙트 관련 데이터를 관리하여 확장성과 유지보수성을 높입니다.

*   **실무 적용 가이드:** 실제 프로젝트에서는 이펙트의 종류와 복잡도를 고려하여 오브젝트 풀링의 크기를 적절하게 설정해야 합니다. 또한, 이펙트의 성능을 지속적으로 모니터링하고 최적화해야 합니다.

### 2. `ABossManager`

*   **클래스 개요:** `ABossManager`는 보스 전투 시스템의 핵심 관리자 역할을 수행하는 액터 클래스입니다. 보스의 스폰, 초기화, 상태 관리, 전투 시작/종료 등을 총괄하며, 다른 클래스들과의 상호작용을 조율합니다. 싱글톤 패턴을 적용하여 게임 내에 단 하나의 `ABossManager` 인스턴스만 존재하도록 하는 것이 일반적입니다.

*   **핵심 클래스 분석:**

    *   **목적 및 책임:** `ABossManager`의 주요 목적은 보스 전투의 전체적인 흐름을 제어하고, 필요한 리소스를 관리하며, 다른 시스템과의 연동을 담당하는 것입니다. 보스의 생명주기를 관리하고, 전투 시작/종료 조건을 판단하며, UI 업데이트, 사운드 재생 등 다양한 기능을 수행합니다.
    *   **주요 메서드 및 프로퍼티:**
        *   `ResetBossCompletely()`: 보스를 완전 초기화합니다. 보스의 HP, 상태, 위치 등을 초기 상태로 되돌리고, 전투를 다시 시작할 수 있도록 준비합니다.
        *   `OpenDoor()`: 보스 전투 시작 시 문을 열어 플레이어가 보스 방으로 진입할 수 있도록 합니다.
        *   `FindBossInWorld()`: 월드 내에 존재하는 보스 액터를 찾습니다.
        *   `ResetAllBossComponents()`: 보스 액터의 모든 컴포넌트를 초기화합니다.
        *   `ResetBossStateTree()`: 보스의 StateTree를 완전히 재시작합니다.
        *   `OnTriggerBoxOverlapBegin()`: 플레이어가 특정 트리거 박스에 진입했을 때 호출되는 함수입니다. 이벤트를 통해 보스 전투를 시작하는 데 사용될 수 있습니다.
    *   **생명주기 및 상태 관리:** `ABossManager`는 게임 시작 시 생성되어 보스 전투가 종료될 때까지 존재합니다. `BeginPlay()`에서 초기화 작업을 수행하고, 전투 시작/종료 시점에 따라 상태를 변경합니다.
    *   **실제 사용 예제 및 코드:**

        ```c++
        // 보스 초기화 예제
        void AMyGameMode::StartBossBattle()
        {
            if (ABossManager* BossManager = GetWorld()->SpawnActor<ABossManager>(ABossManager::StaticClass()))
            {
                BossManager->ResetBossCompletely();
            }
        }

        // 보스 전투 시작 트리거 예제
        void ABossManager::OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
        {
            if (OtherActor->IsA(APlayerCharacter::StaticClass()))
            {
                // 플레이어가 트리거 박스에 진입했을 때 보스 전투 시작
                StartBossBattle();
            }
        }
        ```

    *   **성능 특성 및 최적화 포인트:** `ABossManager`는 보스 전투의 핵심 로직을 수행하므로, 성능에 미치는 영향이 클 수 있습니다. 따라서, 불필요한 Tick 함수 호출을 최소화하고, 효율적인 데이터 구조를 사용하여 성능을 향상시킬 수 있습니다. 또한, 멀티스레딩을 활용하여 병렬 처리를 수행하는 것도 고려해볼 수 있습니다.
    *   **사용 시나리오 및 베스트 프랙티스:**
        *   **보스 스폰 및 초기화:** 게임 시작 시 또는 특정 조건이 만족되었을 때 보스를 스폰하고 초기화합니다.
        *   **전투 시작/종료:** 플레이어가 보스 방에 진입하거나 보스를 처치했을 때 전투를 시작하거나 종료합니다.
        *   **상태 관리:** 보스의 HP, 상태, 페이즈 등을 관리하고, UI 업데이트, 사운드 재생 등 관련 기능을 수행합니다.
        *   **다른 시스템과의 연동:** 플레이어, UI, 사운드, AI 등 다른 시스템과의 연동을 담당합니다.
    *   **다른 클래스와의 상호작용:** `ABossManager`는 `ACBoss`, `UBossStatusWidget`, `UBossProjectileComponent`, `UBossEffectManager` 등 다양한 클래스와 상호작용합니다. `ACBoss`는 보스 액터 자체를 나타내며, `UBossStatusWidget`은 보스의 HP, 상태 등을 표시하는 UI 위젯입니다. `UBossProjectileComponent`는 보스의 투사체를 관리하고, `UBossEffectManager`는 보스의 이펙트를 관리합니다.

*   **설계 패턴 분석:** 싱글톤 패턴을 적용하여 게임 내에 단 하나의 `ABossManager` 인스턴스만 존재하도록 합니다. 옵저버 패턴을 활용하여 보스의 상태 변화를 다른 시스템에 알릴 수 있습니다.

*   **성능 및 최적화:** 불필요한 Tick 함수 호출 최소화, 효율적인 데이터 구조 사용, 멀티스레딩 활용 등을 통해 성능을 향상시킬 수 있습니다.

*   **확장성 및 유지보수성:** 보스 전투 관련 로직을 모듈화하고, 데이터 테이블을 활용하여 보스 관련 데이터를 관리하여 확장성과 유지보수성을 높입니다.

*   **실무 적용 가이드:** 실제 프로젝트에서는 보스 전투의 복잡도를 고려하여 `ABossManager`의 기능을 적절하게 분리하고, 다른 시스템과의 연동을 효율적으로 관리해야 합니다.

### 3. `ABossProjectileActor`

*   **클래스 개요:** `ABossProjectileActor`는 보스가 사용하는 투사체의 기본 클래스입니다. 투사체의 발사, 이동, 충돌 처리, 소멸 등의 기능을 제공하며, 오브젝트 풀링 시스템과 연동되어 효율적인 투사체 관리를 가능하게 합니다.

*   **핵심 클래스 분석:**

    *   **목적 및 책임:** `ABossProjectileActor`의 주요 목적은 보스가 사용하는 다양한 투사체를 생성하고 관리하는 것입니다. 투사체의 발사 방향, 속도, 데미지 등을 설정하고, 충돌 시 적에게 데미지를 입히거나 특정 효과를 발생시키는 등의 역할을 수행합니다.
    *   **주요 메서드 및 프로퍼티:**
        *   `FireProjectile()`: 투사체를 발사합니다. 발사 방향, 속도, 데미지 등을 설정할 수 있습니다.
        *   `FireProjectileToLocation()`: 특정 위치로 투사체를 발사합니다.
        *   `PlaySpawnEffect()`: 투사체가 생성될 때 발생하는 이펙트를 재생합니다.
        *   `PlayDestroyEffect()`: 투사체가 소멸될 때 발생하는 이펙트를 재생합니다.
        *   `OnProjectileHit()`: 투사체가 다른 액터와 충돌했을 때 호출되는 함수입니다. 충돌 대상에게 데미지를 입히거나 특정 효과를 발생시키는 등의 작업을 수행합니다.
    *   **생명주기 및 상태 관리:** `ABossProjectileActor`는 오브젝트 풀링 시스템에 의해 관리됩니다. 풀에서 가져와 발사되고, 충돌 또는 일정 시간이 지나면 풀로 반환됩니다.
    *   **실제 사용 예제 및 코드:**

        ```c++
        // 투사체 발사 예제
        void ABoss::Fire()
        {
            if (ABossProjectileActor* Projectile = GetWorld()->SpawnActor<ABossProjectileActor>(ProjectileClass))
            {
                Projectile->FireProjectile(GetActorForwardVector(), 1000.0f);
            }
        }

        // 투사체 충돌 처리 예제
        void ABossProjectileActor::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
        {
            if (OtherActor->IsA(APlayerCharacter::StaticClass()))
            {
                // 플레이어에게 데미지 입히기
                UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, GetOwnerController(), this, DamageTypeClass);
                // 소멸 이펙트 재생
                PlayDestroyEffect();
                // 풀로 반환
                ReturnToPool();
            }
        }
        ```

    *   **성능 특성 및 최적화 포인트:** 투사체의 수가 많아지면 성능에 큰 영향을 미칠 수 있습니다. 따라서, 오브젝트 풀링을 통해 액터 생성/소멸 비용을 줄이고, 불필요한 Tick 함수 호출을 최소화하며, 투사체의 LOD 설정을 최적화하여 성능을 향상시킬 수 있습니다. 또한, 충돌 검사를 최적화하는 것도 중요합니다.
    *   **사용 시나리오 및 베스트 프랙티스:**
        *   **원거리 공격:** 보스가 원거리에서 플레이어를 공격할 때 사용합니다.
        *   **특수 공격:** 보스가 특정 패턴의 공격을 할 때 사용합니다.
        *   **범위 공격:** 보스가 넓은 범위에 데미지를 입히는 공격을 할 때 사용합니다.
    *   **다른 클래스와의 상호작용:** `ABossProjectileActor`는 `ACBoss`, `UBossProjectileComponent`, `APlayerCharacter` 등의 클래스와 상호작용합니다. `ACBoss`는 투사체를 발사하고, `UBossProjectileComponent`는 투사체 풀링을 관리하며, `APlayerCharacter`는 투사체에 맞아 데미지를 입습니다.

*   **설계 패턴 분석:** 오브젝트 풀링 패턴을 사용하여 투사체 액터의 생성 및 소멸 비용을 줄이고 성능을 최적화합니다.

*   **성능 및 최적화:** 오브젝트 풀링, 불필요한 Tick 함수 호출 최소화, 투사체 LOD 최적화, 충돌 검사 최적화 등을 통해 성능을 향상시킬 수 있습니다.

*   **확장성 및 유지보수성:** 투사체의 종류를 추가하거나 수정하기 쉽도록 설계되었으며, 데이터 테이블을 활용하여 투사체 관련 데이터를 관리하여 확장성과 유지보수성을 높입니다.

*   **실무 적용 가이드:** 실제 프로젝트에서는 투사체의 종류와 복잡도를 고려하여 오브젝트 풀링의 크기를 적절하게 설정해야 합니다. 또한, 투사체의 성능을 지속적으로 모니터링하고 최적화해야 합니다.

### 4. `ABossProjectileOrb`

*   **클래스 개요:** `ABossProjectileOrb`는 `ABossProjectileActor`의 파생 클래스로, 구체 형태의 투사체를 나타냅니다. 특정 위치에 생성되어 궤적을 따라 이동하거나, 특정 대상을 추적하는 등의 기능을 수행할 수 있습니다. 오브젝트 풀링 시스템과 연동되어 효율적인 투사체 관리를 가능하게 합니다.

*   **핵심 클래스 분석:**

    *   **목적 및 책임:** `ABossProjectileOrb`의 주요 목적은 보스가 사용하는 구체 형태의 투사체를 생성하고 관리하는 것입니다. 투사체의 궤적, 속도, 데미지 등을 설정하고, 충돌 시 적에게 데미지를 입히거나 특정 효과를 발생시키는 등의 역할을 수행합니다.
    *   **주요 메서드 및 프로퍼티:**
        *   `SpawnProjectile()`: 투사체를 생성합니다.
        *   `DestroyOrb()`: 투사체를 소멸시킵니다.
        *   `OnOverlap()`: 투사체가 다른 액터와 겹쳤을 때 호출되는 함수입니다.
        *   `PlaySpawnSound()`: 투사체가 생성될 때 발생하는 사운드를 재생합니다.
        *   `PlayReturnToPoolSound()`: 투사체가 풀로 반환될 때 발생하는 사운드를 재생합니다.
        *   `PlayCollisionSound()`: 투사체가 다른 액터와 충돌했을 때 발생하는 사운드를 재생합니다.
        *   `PlaySpawnEffect()`: 투사체가 생성될 때 발생하는 이펙트를 재생합니다.
        *   `PlayReturnToPoolEffect()`: 투사체가 풀로 반환될 때 발생하는 이펙트를 재생합니다.
        *   `PlayCollisionEffect()`: 투사체가 다른 액터와 충돌했을 때 발생하는 이펙트를 재생합니다.
        *   `DestroyOrbWithDelay()`: 특정 시간 이후에 투사체를 소멸시킵니다.
        *   `ActivateOrb()`: 투사체를 활성화합니다.
    *   **생명주기 및 상태 관리:** `ABossProjectileOrb`는 오브젝트 풀링 시스템에 의해 관리됩니다. 풀에서 가져와 생성되고, 충돌 또는 일정 시간이 지나면 풀로 반환됩니다.
    *   **실제 사용 예제 및 코드:**

        ```c++
        // 투사체 생성 예제
        void ABoss::SpawnOrb()
        {
            if (ABossProjectileOrb* Orb = GetWorld()->SpawnActor<ABossProjectileOrb>(OrbClass))
            {
                Orb->ActivateOrb(GetActorLocation(), GetActorForwardVector(), 500.0f);
            }
        }

        // 투사체 충돌 처리 예제
        void ABossProjectileOrb::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
        {
            if (OtherActor->IsA(APlayerCharacter::StaticClass()))
            {
                // 플레이어에게 데미지 입히기
                UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, GetOwnerController(), this, DamageTypeClass);
                // 소멸 이펙트 재생
                PlayCollisionEffect();
                // 풀로 반환
                DestroyOrb();
            }
        }
        ```

    *   **성능 특성 및 최적화 포인트:** `ABossProjectileOrb`는 `ABossProjectileActor`와 마찬가지로 투사체의 수가 많아지면 성능에 큰 영향을 미칠 수 있습니다. 따라서, 오브젝트 풀링을 통해 액터 생성/소멸 비용을 줄이고, 불필요한 Tick 함수 호출을 최소화하며, 투사체의 LOD 설정을 최적화하여 성능을 향상시킬 수 있습니다. 또한, 궤적 계산 및 충돌 검사를 최적화하는 것도 중요합니다.
    *   **사용 시나리오 및 베스트 프랙티스:**
        *   **유도 미사일:** 플레이어를 추적하는 투사체를 구현할 때 사용합니다.
        *   **범위 공격:** 특정 범위 내에 있는 적에게 데미지를 입히는 투사체를 구현할 때 사용합니다.
        *   **장판 공격:** 특정 위치에 지속적으로 데미지를 입히는 투사체를 구현할 때 사용합니다.
    *   **다른 클래스와의 상호작용:** `ABossProjectileOrb`는 `ACBoss`, `UBossProjectileComponent`, `APlayerCharacter` 등의 클래스와 상호작용합니다. `ACBoss`는 투사체를 생성하고, `UBossProjectileComponent`는 투사체 풀링을 관리하며, `APlayerCharacter`는 투사체에 맞아 데미지를 입습니다.

*   **설계 패턴 분석:** 오브젝트 풀링 패턴을 사용하여 투사체 액터의 생성 및 소멸 비용을 줄이고 성능을 최적화합니다.

*   **성능 및 최적화:** 오브젝트 풀링, 불필요한 Tick 함수 호출 최소화, 투사체 LOD 최적화, 궤적 계산 및 충돌 검사 최적화 등을 통해 성능을 향상시킬 수 있습니다.

*   **확장성 및 유지보수성:** 투사체의 종류를 추가하거나 수정하기 쉽도록 설계되었으며, 데이터 테이블을 활용하여 투사체 관련 데이터를 관리하여 확장성과 유지보수성을 높입니다.

*   **실무 적용 가이드:** 실제 프로젝트에서는 투사체의 종류와 복잡도를 고려하여 오브젝트 풀링의 크기를 적절하게 설정해야 합니다. 또한, 투사체의 성능을 지속적으로 모니터링하고 최적화해야 합니다.

### 5. `ACBoss`

*   **클래스 개요:** `ACBoss`는 보스 캐릭터의 핵심 클래스이며, `ACharacter` 클래스를 상속받아 구현됩니다. 보스의 외형, 애니메이션, 스탯, AI, 공격 패턴 등 보스 캐릭터의 모든 것을 정의하고 관리합니다.

*   **핵심 클래스 분석:**

    *   **목적 및 책임:** `ACBoss`의 주요 목적은 게임 내에서 보스 캐릭터를 표현하고 제어하는 것입니다. 보스의 움직임, 공격, 피격, 사망 등 다양한 상황에 대한 로직을 처리하고, 다른 시스템과의 연동을 담당합니다.
    *   **주요 메서드 및 프로퍼티:**
        *   `TakeDamage()`: 보스가 데미지를 입었을 때 호출되는 함수입니다. 데미지 양을 계산하고, HP를 감소시키며, 피격 애니메이션을 재생하는 등의 작업을 수행합니다.
        *   `PlayHitMotion()`: 보스가 피격 당했을 때 재생되는 애니메이션을 재생합니다.
        *   `ShowBossStatusWidget()`: 보스의 HP, 상태 등을 표시하는 UI 위젯을 표시합니다.
        *   `HPUpdate()`: 보스의 HP를 업데이트하고 UI에 반영합니다.
        *   `PlayBossBGM()`: 보스 전투 배경 음악을 재생합니다.
        *   `StopBossBGM()`: 보스 전투 배경 음악을 정지합니다.
        *   `LowerBossBGMVolume()`: 보스 사망 시 배경 음악 볼륨을 감소시킵니다.
    *   **생명주기 및 상태 관리:** `ACBoss`는 게임 시작 시 스폰되어 보스가 사망할 때까지 존재합니다. `BeginPlay()`에서 초기화 작업을 수행하고, `TakeDamage()`를 통해 HP를 관리하며, 상태 변화에 따라 AI를 제어합니다.
    *   **실제 사용 예제 및 코드:**

        ```c++
        // 데미지 처리 예제
        float ACBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
        {
            float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
            if (ActualDamage > 0.0f)
            {
                CurrentHP -= ActualDamage;
                HPUpdate();
                PlayHitMotion();

                if (CurrentHP <= 0.0f)
                {
                    // 사망 처리
                    Die();
                }
            }
            return ActualDamage;
        }

        // 보스 사망 처리 예제
        void ACBoss::Die()
        {
            // 사망 애니메이션 재생
            PlayAnimMontage(DeathMontage);
            // AI 정지
            GetController()->StopMovement();
            // 콜리전 비활성화
            GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            // 사망 이펙트 재생
            PlayDeathEffect();
            // 배경 음악 볼륨 감소
            LowerBossBGMVolume();
            // 일정 시간 후 소멸
            Destroy();
        }
        ```

    *   **성능 특성 및 최적화 포인트:** `ACBoss`는 보스 캐릭터의 모든 로직을 처리하므로, 성능에 미치는 영향이 클 수 있습니다. 따라서, 불필요한 Tick 함수 호출을 최소화하고, 애니메이션 최적화, AI 최적화 등을 통해 성능을 향상시킬 수 있습니다.
    *   **사용 시나리오 및 베스트 프랙티스:**
        *   **보스 캐릭터 구현:** 게임 내에서 보스 캐릭터를 표현하고 제어합니다.
        *   **전투 로직 처리:** 보스의 움직임, 공격, 피격, 사망 등 다양한 상황에 대한 로직을 처리합니다.
        *   **다른 시스템과의 연동:** 플레이어, UI, 사운드, AI 등 다른 시스템과의 연동을 담당합니다.
    *   **다른 클래스와의 상호작용:** `ACBoss`는 `ABossManager`, `ACBossAIC`, `UBossStatusWidget`, `UBossProjectileComponent`, `UBossEffectManager` 등 다양한 클래스와 상호작용합니다. `ABossManager`는 보스를 생성하고 관리하며, `ACBossAIC`는 보스의 AI를 제어합니다. `UBossStatusWidget`은 보스의 HP, 상태 등을 표시하는 UI 위젯이고, `UBossProjectileComponent`는 보스의 투사체를 관리하며, `UBossEffectManager`는 보스의 이펙트를 관리합니다.

*   **설계 패턴 분석:** State 패턴을 활용하여 보스의 상태 변화를 관리하고, 이에 따라 AI를 제어할 수 있습니다. 옵저버 패턴을 활용하여 보스의 상태 변화를 다른 시스템에 알릴 수 있습니다.

*   **성능 및 최적화:** 불필요한 Tick 함수 호출 최소화, 애니메이션 최적화, AI 최적화 등을 통해 성능을 향상시킬 수 있습니다.

*   **확장성 및 유지보수성:** 보스 캐릭터 관련 로직을 모듈화하고, 데이터 테이블을 활용하여 보스 관련 데이터를 관리하여 확장성과 유지보수성을 높입니다.

*   **실무 적용 가이드:** 실제 프로젝트에서는 보스 캐릭터의 복잡도를 고려하여 `ACBoss`의 기능을 적절하게 분리하고, 다른 시스템과의 연동을 효율적으로 관리해야 합니다.

### 6. `ACBossAIC`

*   **클래스 개요:** `ACBossAIC`는 보스 캐릭터의 AI를 제어하는 AIController 클래스입니다. StateTree를 활용하여 보스의 행동 패턴을 정의하고, 상황에 따라 적절한 행동을 선택하도록 합니다.

*   **핵심 클래스 분석:**

    *   **목적 및 책임:** `ACBossAIC`의 주요 목적은 보스 캐릭터의 AI를 제어하고, 게임 내에서 지능적인 행동을 수행하도록 하는 것입니다. StateTree를 통해 보스의 행동 패턴을 정의하고, 상황에 따라 적절한 행동을 선택하도록 합니다.
    *   **주요 메서드 및 프로퍼티:**
        *   `OnPossess()`: AIController가 Pawn을 소유했을 때 호출되는 함수입니다. StateTree를 시작하고, AI 관련 초기화를 수행합니다.
    *   **생명주기 및 상태 관리:** `ACBossAIC`는 `ACBoss`가 스폰될 때 함께 생성되어 보스가 사망할 때까지 존재합니다. `OnPossess()`에서 초기화 작업을 수행하고, StateTree를 통해 보스의 행동 패턴을 제어합니다.
    *   **실제 사용 예제 및 코드:**

        ```c++
        // Pawn 소유 시 호출되는 함수
        void ACBossAIC::OnPossess(APawn* InPawn)
        {
            Super::OnPossess(InPawn);

            if (InPawn && BossStateTree)
            {
                // StateTree 시작
                RunBehaviorTree(BossStateTree);
            }
        }
        ```

    *   **성능 특성 및 최적화 포인트:** StateTree의 복잡도가 높아지면 AI 연산에 많은 리소스를 소모할 수 있습니다. 따라서, StateTree를 최적화하고, 불필요한 연산을 최소화하여 성능을 향상시킬 수 있습니다. 또한, 비동기 태스크를 활용하여 AI 연산을 백그라운드에서 처리하는 것도 고려해볼 수 있습니다.
    *   **사용 시나리오 및 베스트 프랙티스:**
        *   **보스 AI 제어:** StateTree를 활용하여 보스의 행동 패턴을 정의하고, 상황에 따라 적절한 행동을 선택하도록 합니다.
        *   **전투 로직 처리:** 보스의 공격 패턴, 회피 패턴, 이동 패턴 등을 정의하고, 플레이어의 행동에 따라 AI를 변경합니다.
    *   **다른 클래스와의 상호작용:** `ACBossAIC`는 `ACBoss`, `UBossEnemyStateTreeEvaluator`, StateTree Task 클래스 등과 상호작용합니다. `ACBoss`는 AIController를 소유하고, `UBossEnemyStateTreeEvaluator`는 StateTree의 의사 결정을 위한 데이터를 제공하며, StateTree Task 클래스는 실제 행동을 수행합니다.

*   **설계 패턴 분석:** StateTree를 사용하여 보스의 행동 패턴을 정의하고, 상황에 따라 적절한 행동을 선택하도록 합니다.

*   **성능 및 최적화:** StateTree 최적화, 불필요한 연산 최소화, 비동기 태스크 활용 등을 통해 성능을 향상시킬 수 있습니다.

*   **확장성 및 유지보수성:** StateTree를 모듈화하고, 데이터 테이블을 활용하여 AI 관련 데이터를 관리하여 확장성과 유지보수성을 높입니다.

*   **실무 적용 가이드:** 실제 프로젝트에서는 보스 AI의 복잡도를 고려하여 StateTree를 적절하게 설계하고, 성능을 지속적으로 모니터링하고 최적화해야 합니다.

### 7. `ACBossWeapon`

*   **클래스 개요:** `ACBossWeapon`는 보스 캐릭터가 사용하는 무기를 나타내는 액터 클래스입니다. 무기의 외형, 콜리전, 공격 로직 등을 정의하고 관리합니다.

*   **핵심 클래스 분석:**

    *   **목적 및 책임:** `ACBossWeapon`의 주요 목적은 보스 캐릭터가 사용하는 무기를 표현하고, 공격 로직을 처리하는 것입니다. 무기의 콜리전을 감지하고, 데미지를 입히며, 이펙트 재생 등의 작업을 수행합니다.
    *   **주요 메서드 및 프로퍼티:**
        *   `OnBossBeginEquip()`: 보스가 무기를 장착하기 시작할 때 호출되는 이벤트입니다.
        *   `OnBossUnequip()`: 보스가 무기를 해제할 때 호출되는 이벤트입니다.
        *   `OnBossCollisions()`: 무기의 콜리전이 활성화되었을 때 호출되는 함수입니다.
        *   `OnSelectCollision()`: 특정 콜리전을 선택합니다.
        *   `OffBossCollisions()`: 무기의 콜리전을 비활성화합니다.
        *   `BossAttachToCollision()`: 무기를 콜리전에 부착합니다.
        *   `StartCollisionAtSocket()`: 특정 소켓에서 콜리전을 시작합니다.
        *   `EndCollisionToOwner()`: 소유자에게 콜리전을 종료합니다.
        *   `BossAttachTo()`: 무기를 보스에 부착합니다.
        *   `OnBossComponentBeginOverlap()`: 무기의 컴포넌트가 다른 액터와 겹치기 시작했을 때 호출되는 함수입니다.
        *   `OnBossComponentEndOverlap()`: 무기의 컴포넌트가 다른 액터와 겹치기 끝났을 때 호출되는 함수입니다.
    *   **생명주기 및 상태 관리:** `ACBossWeapon`는 `ACBoss`가 스폰될 때 함께 생성되거나, 필요에 따라 동적으로 생성될 수 있습니다. 무기가 장착되면 보스에 부착되고, 공격 시 콜리전이 활성화되어 데미지를 입힙니다.
    *   **실제 사용 예제 및 코드:**

        ```c++
        // 무기 장착 예제
        void ACBoss::EquipWeapon(ACBossWeapon* NewWeapon)
        {
            if (CurrentWeapon)
            {
                CurrentWeapon->Destroy();
            }

            CurrentWeapon = NewWeapon;
            if (CurrentWeapon)
            {
                CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("WeaponSocket"));
                CurrentWeapon->OnBossBeginEquip();
            }
        }

        // 콜리전 시작 예제
        void ACBossWeapon::OnBossComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
        {
            if (OtherActor->IsA(APlayerCharacter::StaticClass()))
            {
                // 플레이어에게 데미지 입히기
                UGameplayStatics::ApplyDamage(OtherActor, DamageAmount, GetOwnerController(), this, DamageTypeClass);
            }
        }
        ```

    *   **성능 특성 및 최적화 포인트:** 무기의 콜리전 검사는 성능에 많은 영향을 미칠 수 있습니다. 따라서, 콜리전 채널을 적절하게 설정하고, 불필요한 콜리전 검사를 최소화하여 성능을 향상시킬 수 있습니다. 또한, 멀티스레딩을 활용하여 콜리전 검사를 백그라운드에서 처리하는 것도 고려해볼 수 있습니다.
    *   **사용 시나리오 및 베스트 프랙티스:**
        *   **보스 무기 표현:** 게임 내에서 보스 캐릭터가 사용하는 무기를 표현합니다.
        *   **공격 로직 처리:** 무기의 콜리전을 감지하고, 데미지를 입히며, 이펙트 재생 등의 작업을 수행합니다.
    *   **다른 클래스와의 상호작용:** `ACBossWeapon`는 `ACBoss`, `APlayerCharacter` 등과 상호작용합니다. `ACBoss`는 무기를 장착하고, `APlayerCharacter`는 무기에 맞아 데미지를 입습니다.

*   **설계 패턴 분석:** 컴포넌트 패턴을 활용하여 무기의 기능을 모듈화하고, 필요에 따라 기능을 추가하거나 수정할 수 있습니다.

*   **성능 및 최적화:** 콜리전 채널 최적화, 불필요한 콜리전 검사 최소화, 멀티스레딩 활용 등을 통해 성능을 향상시킬 수 있습니다.

*   **확장성 및 유지보수성:** 무기의 종류를 추가하거나 수정하기 쉽도록 설계되었으며, 데이터 테이블을 활용하여 무기 관련 데이터를 관리하여 확장성과 유지보수성을 높입니다.

*   **실무 적용 가이드:** 실제 프로젝트에서는 무기의 종류와 복잡도를 고려하여 콜리전 설정을 적절하게 하고, 성능을 지속적으로 모니터링하고 최적화해야 합니다.

### 8. `AFlySpline`

*   **클래스 개요:** `AFlySpline`는 보스가 비행할 때 사용하는 스플라인 액터 클래스입니다. 스플라인을 따라 보스의 이동 경로를 정의하고, 비행 관련 로직을 처리합니다.

*   **핵심 클래스 분석:**

    *   **목적 및 책임:** `AFlySpline

## 클래스별 상세 분석

### 1. `ABossEffect`

*   **클래스 개요:**
    *   `ABossEffect`는 보스 캐릭터가 사용하는 시각 효과(Visual Effects, VFX)를 관리하고 제어하는 액터 클래스입니다. 보스의 다양한 공격, 스킬, 상태 변화에 따른 이펙트를 활성화/비활성화하고, 특정 소켓에 부착하여 위치를 동기화하는 기능을 제공합니다. 이 클래스는 이펙트의 풀링(Pooling)을 지원하여 런타임에 이펙트 생성/소멸에 따른 성능 저하를 최소화합니다.
    *   `ABossEffect`는 `ABossEffectManager`에 의해 관리되며, `ABoss`의 상태 변화 및 공격 로직에 따라 활성화됩니다.
    *   설계 철학은 이펙트의 재사용성을 극대화하고, 보스 액터의 복잡성을 줄이는 데 있습니다. 컴포넌트 패턴을 활용하여 이펙트 관련 기능을 모듈화하고, 데이터 테이블을 통해 이펙트 설정을 관리하여 확장성과 유지보수성을 높입니다.

*   **핵심 클래스 분석:**
    *   **목적 및 책임:**
        *   이펙트 활성화 및 비활성화
        *   이펙트를 보스 캐릭터 또는 특정 소켓에 부착
        *   이펙트의 활성화 상태 관리
        *   현재 이펙트의 태그 반환
    *   **주요 메서드와 프로퍼티 상세 분석:**
        *   `ActivateEffect()`: 이펙트를 활성화합니다. 파티클 시스템, 사운드, 라이트 등 다양한 컴포넌트를 활성화하여 시각 효과를 표시합니다.
        *   `ActivateEffectAttachedToSocket()`: 이펙트를 특정 소켓에 부착하여 활성화합니다. 소켓 이름과 트랜스폼 규칙을 인자로 받아 이펙트의 위치와 회전을 보스 캐릭터의 움직임에 동기화합니다.
        *   `DeactivateEffect()`: 이펙트를 비활성화합니다. 활성화된 파티클 시스템, 사운드, 라이트 등을 중지하고 메모리 풀로 반환합니다 (풀링을 사용하는 경우).
        *   `IsActive()`: 이펙트가 현재 활성화되어 있는지 여부를 반환합니다.
        *   `GetCurrentEffectTag()`: 현재 이펙트의 태그를 반환합니다. 이 태그는 이펙트를 식별하고 관리하는 데 사용됩니다.
        *   `AttachToBoss()`: 이펙트를 보스 캐릭터에 부착합니다.
        *   `AttachToSocket()`: 이펙트를 보스 캐릭터의 특정 소켓에 부착합니다.
        *   `PlaceInWorld()`: 이펙트를 월드 공간에 배치합니다.
    *   **클래스의 생명주기와 상태 관리:**
        *   `BeginPlay()`: 액터가 생성된 후 게임 시작 시 호출됩니다. 이펙트 풀을 초기화하고, 필요한 데이터를 로드합니다.
        *   `Tick()`: 매 프레임마다 호출됩니다. 이펙트의 위치, 회전, 활성화 상태 등을 업데이트합니다.
        *   이펙트는 활성화/비활성화 상태를 가지며, `IsActive()` 메서드를 통해 상태를 확인할 수 있습니다.
    *   **실제 사용 예제와 코드:**

```cpp
// 이펙트 활성화 (보스 위치에)
ABossEffect* Effect = BossEffectManager->PlayEffect(EffectTag, GetActorLocation(), GetActorRotation());

// 이펙트 활성화 (소켓에 부착)
ABossEffect* Effect = BossEffectManager->PlayEffectAttachedToSocket(EffectTag, Mesh, SocketName);

// 이펙트 비활성화
Effect->DeactivateEffect();
```

    *   **성능 특성과 최적화 포인트:**
        *   이펙트 풀링: 런타임에 이펙트 생성/소멸을 최소화하여 성능을 향상시킵니다.
        *   파티클 시스템 최적화: 파티클 수, 수명, 스폰 레이트 등을 조절하여 성능을 향상시킵니다.
        *   가시성 컬링: 카메라 시야 밖에 있는 이펙트는 업데이트를 중지하여 성능을 향상시킵니다.
    *   **사용 시나리오와 베스트 프랙티스:**
        *   보스 공격 이펙트: 보스의 강력한 공격 시 시각적인 효과를 강조합니다.
        *   보스 상태 변화 이펙트: 보스의 체력이 낮아지거나 특정 상태에 진입했을 때 시각적인 변화를 나타냅니다.
        *   피격 이펙트: 플레이어가 보스에게 공격을 당했을 때 피격 효과를 나타냅니다.
    *   **다른 클래스와의 상호작용:**
        *   `ABossEffectManager`: 이펙트를 생성, 관리, 풀링합니다.
        *   `ABoss`: 보스 캐릭터의 상태 변화 및 공격 로직에 따라 이펙트를 활성화/비활성화합니다.
        *   `APlayerCharacter`: 플레이어가 보스에게 공격을 당했을 때 피격 효과를 나타냅니다.

*   **설계 패턴 분석:**
    *   컴포넌트 패턴: 이펙트 관련 기능을 모듈화하고, 필요에 따라 기능을 추가하거나 수정할 수 있습니다.
    *   객체 풀 패턴: 이펙트의 재사용성을 높이고, 런타임 성능을 향상시킵니다.
*   **성능 및 최적화:**
    *   이펙트 풀링을 통해 메모리 할당 및 해제를 최소화합니다.
    *   파티클 시스템의 파라미터를 조절하여 GPU 부하를 줄입니다.
    *   가시성 컬링을 통해 불필요한 이펙트 업데이트를 방지합니다.
*   **확장성 및 유지보수성:**
    *   데이터 테이블을 활용하여 이펙트 설정을 관리합니다.
    *   컴포넌트 패턴을 통해 기능을 모듈화하여 확장성을 높입니다.
*   **실무 적용 가이드:**
    *   이펙트의 종류와 복잡도를 고려하여 풀 크기를 적절하게 설정합니다.
    *   성능을 지속적으로 모니터링하고, 필요에 따라 파티클 시스템 파라미터를 조절합니다.
    *   가시성 컬링을 적극적으로 활용하여 불필요한 이펙트 업데이트를 방지합니다.

### 2. `ABossManager`

*   **클래스 개요:**
    *   `ABossManager`는 게임 내 보스 캐릭터의 생성, 관리, 초기화, 상태 제어 등 전반적인 보스 관련 로직을 담당하는 액터 클래스입니다. 보스 스폰 위치, 보스 종류, 보스 상태 초기화, 보스 AI 제어 등을 담당하며, 게임 전체의 보스 관련 흐름을 관리합니다.
    *   `ABossManager`는 게임 모드(GameMode) 또는 레벨 블루프린트에서 인스턴스화되어 사용되며, 싱글톤 패턴을 적용하여 게임 내에서 유일한 인스턴스만 존재하도록 설계할 수 있습니다.
    *   설계 철학은 보스 관련 로직을 중앙 집중화하여 관리하고, 게임의 흐름에 따라 보스 상태를 제어하는 데 있습니다.

*   **핵심 클래스 분석:**
    *   **목적 및 책임:**
        *   보스 캐릭터 생성 및 관리
        *   보스 상태 초기화 (체력, AI, 장비 등)
        *   보스 AI 제어 (StateTree 리스타트 등)
        *   보스 관련 이벤트 처리 (콜리전 감지, 문 열기 등)
    *   **주요 메서드와 프로퍼티 상세 분석:**
        *   `ResetBossCompletely()`: 보스 캐릭터의 모든 상태를 초기화합니다. 체력, AI, 장비, 이펙트 등을 초기 상태로 되돌립니다.
        *   `OpenDoor()`: 보스 전투 시작 시 문을 열어 플레이어가 보스 방으로 진입할 수 있도록 합니다.
        *   `FindBossInWorld()`: 현재 월드에 존재하는 보스 캐릭터를 찾습니다.
        *   `ResetAllBossComponents()`: 보스 캐릭터의 모든 컴포넌트를 초기화합니다.
        *   `ResetBossStateTree()`: 보스 AI의 StateTree를 완전히 리스타트합니다.
        *   `OnTriggerBoxOverlapBegin()`: 플레이어가 특정 영역에 진입했을 때 호출됩니다. 보스 전투 시작을 알리는 트리거 역할을 합니다.
    *   **클래스의 생명주기와 상태 관리:**
        *   `BeginPlay()`: 액터가 생성된 후 게임 시작 시 호출됩니다. 보스 캐릭터를 스폰하고 초기화합니다.
        *   `Tick()`: 매 프레임마다 호출됩니다. 보스 캐릭터의 상태를 업데이트하고, AI를 제어합니다.
        *   보스 매니저는 보스 캐릭터의 생명주기를 관리하며, 보스 캐릭터의 상태 변화에 따라 적절한 액션을 수행합니다.
    *   **실제 사용 예제와 코드:**

```cpp
// 보스 완전 초기화
BossManager->ResetBossCompletely();

// 문 열기
BossManager->OpenDoor();

// 월드에서 보스 찾기
ACBoss* Boss = BossManager->FindBossInWorld();

// StateTree 리스타트
BossManager->ResetBossStateTree();
```

    *   **성능 특성과 최적화 포인트:**
        *   보스 캐릭터 스폰 시 풀링을 사용하여 성능을 향상시킵니다.
        *   불필요한 Tick 함수 호출을 최소화합니다.
        *   보스 AI의 StateTree를 최적화하여 CPU 부하를 줄입니다.
    *   **사용 시나리오와 베스트 프랙티스:**
        *   보스 전투 시작 시 보스 캐릭터를 스폰하고 초기화합니다.
        *   플레이어가 보스 방에 진입했을 때 보스 AI를 활성화합니다.
        *   보스 캐릭터가 사망했을 때 보스 보상을 지급하고, 게임 클리어를 처리합니다.
    *   **다른 클래스와의 상호작용:**
        *   `ACBoss`: 보스 캐릭터 클래스입니다. `ABossManager`는 `ACBoss`를 생성하고 관리합니다.
        *   `ACBossAIC`: 보스 AI 컨트롤러 클래스입니다. `ABossManager`는 `ACBossAIC`를 통해 보스 AI를 제어합니다.
        *   `APlayerCharacter`: 플레이어 캐릭터 클래스입니다. `ABossManager`는 플레이어의 위치를 추적하고, 보스 AI에 정보를 제공합니다.

*   **설계 패턴 분석:**
    *   싱글톤 패턴: 게임 내에서 유일한 `ABossManager` 인스턴스만 존재하도록 합니다.
    *   팩토리 패턴: 보스 캐릭터를 생성하는 로직을 캡슐화합니다.
*   **성능 및 최적화:**
    *   보스 캐릭터 스폰 시 풀링을 사용하여 메모리 할당 및 해제를 최소화합니다.
    *   불필요한 Tick 함수 호출을 최소화합니다.
    *   보스 AI의 StateTree를 최적화하여 CPU 부하를 줄입니다.
*   **확장성 및 유지보수성:**
    *   데이터 테이블을 활용하여 보스 캐릭터의 스탯, AI, 장비 등을 관리합니다.
    *   컴포넌트 패턴을 통해 보스 캐릭터의 기능을 모듈화하여 확장성을 높입니다.
*   **실무 적용 가이드:**
    *   보스 캐릭터의 종류와 복잡도를 고려하여 풀 크기를 적절하게 설정합니다.
    *   성능을 지속적으로 모니터링하고, 필요에 따라 보스 AI의 StateTree를 최적화합니다.
    *   보스 캐릭터의 스탯, AI, 장비 등을 데이터 테이블을 통해 관리하여 유지보수성을 높입니다.

### 3. `ABossProjectileActor`

*   **클래스 개요:**
    *   `ABossProjectileActor`는 보스 캐릭터가 발사하는 투사체의 기본 클래스입니다. 투사체의 이동, 충돌, 소멸 로직을 담당하며, 이펙트 재생, 데미지 처리 등 투사체 관련 기능을 제공합니다.
    *   `ABossProjectileActor`는 다양한 종류의 투사체를 생성하기 위한 기반 클래스로 사용되며, 상속을 통해 투사체의 외형, 이동 방식, 효과 등을 커스터마이즈할 수 있습니다.
    *   설계 철학은 투사체의 공통적인 기능을 추상화하고, 재사용성을 높이는 데 있습니다.

*   **핵심 클래스 분석:**
    *   **목적 및 책임:**
        *   투사체 이동 및 충돌 처리
        *   투사체 소멸 처리
        *   투사체 관련 이펙트 재생
        *   투사체 데미지 처리
    *   **주요 메서드와 프로퍼티 상세 분석:**
        *   `FireProjectile()`: 투사체를 발사합니다. 초기 속도, 방향, 가속도 등을 설정합니다.
        *   `FireProjectileToLocation()`: 특정 위치로 투사체를 발사합니다.
        *   `PlaySpawnEffect()`: 투사체 생성 시 이펙트를 재생합니다.
        *   `PlayDestroyEffect()`: 투사체 소멸 시 이펙트를 재생합니다.
        *   `OnProjectileHit()`: 투사체가 다른 액터와 충돌했을 때 호출됩니다. 데미지 처리, 이펙트 재생 등을 수행합니다.
    *   **클래스의 생명주기와 상태 관리:**
        *   `BeginPlay()`: 액터가 생성된 후 게임 시작 시 호출됩니다. 투사체의 초기 상태를 설정합니다.
        *   `Tick()`: 매 프레임마다 호출됩니다. 투사체의 위치, 속도, 수명 등을 업데이트합니다.
        *   투사체는 생성, 이동, 충돌, 소멸 상태를 가지며, 각 상태에 따라 적절한 로직을 수행합니다.
    *   **실제 사용 예제와 코드:**

```cpp
// 투사체 발사
ABossProjectileActor* Projectile = GetWorld()->SpawnActor<ABossProjectileActor>(ProjectileClass, GetActorLocation(), GetActorRotation());
Projectile->FireProjectile(InitialSpeed, Direction, Acceleration);

// 충돌 처리
void ABossProjectileActor::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    // 데미지 처리
    UGameplayStatics::ApplyPointDamage(OtherActor, DamageAmount, NormalImpulse, Hit, GetOwnerController(), this, DamageType);

    // 이펙트 재생
    PlayDestroyEffect();

    // 투사체 소멸
    Destroy();
}
```

    *   **성능 특성과 최적화 포인트:**
        *   투사체 풀링: 런타임에 투사체 생성/소멸을 최소화하여 성능을 향상시킵니다.
        *   콜리전 채널 최적화: 불필요한 콜리전 검사를 최소화하여 성능을 향상시킵니다.
        *   투사체 수명 제한: 투사체가 일정 시간 동안 생존하도록 하여 메모리 누수를 방지합니다.
    *   **사용 시나리오와 베스트 프랙티스:**
        *   보스 원거리 공격: 보스가 플레이어에게 원거리 공격을 할 때 사용합니다.
        *   보스 스킬: 보스가 특정 스킬을 사용할 때 투사체를 발사합니다.
    *   **다른 클래스와의 상호작용:**
        *   `ACBoss`: 보스 캐릭터 클래스입니다. `ACBoss`는 `ABossProjectileActor`를 생성하고 발사합니다.
        *   `APlayerCharacter`: 플레이어 캐릭터 클래스입니다. `ABossProjectileActor`는 플레이어에게 데미지를 입힙니다.

*   **설계 패턴 분석:**
    *   템플릿 메서드 패턴: 투사체의 공통적인 로직을 추상화하고, 상속을 통해 세부 동작을 커스터마이즈할 수 있도록 합니다.
    *   객체 풀 패턴: 투사체의 재사용성을 높이고, 런타임 성능을 향상시킵니다.
*   **성능 및 최적화:**
    *   투사체 풀링을 통해 메모리 할당 및 해제를 최소화합니다.
    *   콜리전 채널을 최적화하여 불필요한 콜리전 검사를 방지합니다.
    *   투사체의 수명을 제한하여 메모리 누수를 방지합니다.
*   **확장성 및 유지보수성:**
    *   상속을 통해 다양한 종류의 투사체를 쉽게 추가할 수 있습니다.
    *   데이터 테이블을 활용하여 투사체의 스탯, 이펙트 등을 관리합니다.
*   **실무 적용 가이드:**
    *   투사체의 종류와 복잡도를 고려하여 풀 크기를 적절하게 설정합니다.
    *   콜리전 채널을 최적화하여 불필요한 콜리전 검사를 방지합니다.
    *   투사체의 스탯, 이펙트 등을 데이터 테이블을 통해 관리하여 유지보수성을 높입니다.

### 4. `ABossProjectileOrb`

*   **클래스 개요:**
    *   `ABossProjectileOrb`는 `ABossProjectileActor`를 상속받아 구현된 특정 형태의 투사체입니다. 궤적을 그리며 이동하거나, 특정 위치에 정지하여 지속적인 효과를 발휘하는 등 특수한 동작을 수행하는 데 사용됩니다.
    *   이 클래스는 오브 형태의 투사체에 특화된 로직을 추가적으로 구현하며, `ABossProjectileActor`의 기본적인 투사체 기능을 확장합니다.
    *   설계 철학은 특정 형태의 투사체에 특화된 기능을 제공하고, 코드 재사용성을 높이는 데 있습니다.

*   **핵심 클래스 분석:**
    *   **목적 및 책임:**
        *   오브 형태의 투사체 이동 및 충돌 처리
        *   오브 생성, 소멸 시 사운드 및 이펙트 재생
        *   오브 충돌 시 사운드 및 이펙트 재생
        *   오브 활성화 및 비활성화
    *   **주요 메서드와 프로퍼티 상세 분석:**
        *   `SpawnProjectile()`: 오브를 생성합니다.
        *   `DestroyOrb()`: 오브를 소멸합니다.
        *   `OnOverlap()`: 오브가 다른 액터와 겹쳤을 때 호출됩니다.
        *   `PlaySpawnSound()`: 오브 생성 시 사운드를 재생합니다.
        *   `PlayReturnToPoolSound()`: 오브가 풀로 반환될 때 사운드를 재생합니다.
        *   `PlayCollisionSound()`: 오브가 충돌했을 때 사운드를 재생합니다.
        *   `PlaySpawnEffect()`: 오브 생성 시 이펙트를 재생합니다.
        *   `PlayReturnToPoolEffect()`: 오브가 풀로 반환될 때 이펙트를 재생합니다.
        *   `PlayCollisionEffect()`: 오브가 충돌했을 때 이펙트를 재생합니다.
        *   `ActivateOrb()`: 오브를 활성화합니다.
    *   **클래스의 생명주기와 상태 관리:**
        *   `BeginPlay()`: 액터가 생성된 후 게임 시작 시 호출됩니다. 오브의 초기 상태를 설정합니다.
        *   `Tick()`: 매 프레임마다 호출됩니다. 오브의 위치, 속도, 수명 등을 업데이트합니다.
        *   오브는 생성, 이동, 충돌, 소멸, 활성화, 비활성화 상태를 가지며, 각 상태에 따라 적절한 로직을 수행합니다.
    *   **실제 사용 예제와 코드:**

```cpp
// 오브 생성
ABossProjectileOrb* Orb = GetWorld()->SpawnActor<ABossProjectileOrb>(OrbClass, GetActorLocation(), GetActorRotation());
Orb->ActivateOrb();

// 오브 충돌 처리
void ABossProjectileOrb::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    // 데미지 처리
    UGameplayStatics::ApplyPointDamage(OtherActor, DamageAmount, SweepResult.ImpactPoint, SweepResult, GetOwnerController(), this, DamageType);

    // 이펙트 재생
    PlayCollisionEffect();

    // 사운드 재생
    PlayCollisionSound();

    // 오브 소멸
    DestroyOrb();
}
```

    *   **성능 특성과 최적화 포인트:**
        *   투사체 풀링: 런타임에 오브 생성/소멸을 최소화하여 성능을 향상시킵니다.
        *   콜리전 채널 최적화: 불필요한 콜리전 검사를 최소화하여 성능을 향상시킵니다.
        *   오브 수명 제한: 오브가 일정 시간 동안 생존하도록 하여 메모리 누수를 방지합니다.
    *   **사용 시나리오와 베스트 프랙티스:**
        *   보스 원거리 공격: 보스가 플레이어에게 원거리 공격을 할 때 사용합니다.
        *   보스 스킬: 보스가 특정 스킬을 사용할 때 오브를 발사합니다.
    *   **다른 클래스와의 상호작용:**
        *   `ACBoss`: 보스 캐릭터 클래스입니다. `ACBoss`는 `ABossProjectileOrb`를 생성하고 발사합니다.
        *   `APlayerCharacter`: 플레이어 캐릭터 클래스입니다. `ABossProjectileOrb`는 플레이어에게 데미지를 입힙니다.

*   **설계 패턴 분석:**
    *   상속: `ABossProjectileActor`의 기능을 상속받아 오브 형태의 투사체에 특화된 기능을 추가합니다.
    *   객체 풀 패턴: 오브의 재사용성을 높이고, 런타임 성능을 향상시킵니다.
*   **성능 및 최적화:**
    *   투사체 풀링을 통해 메모리 할당 및 해제를 최소화합니다.
    *   콜리전 채널을 최적화하여 불필요한 콜리전 검사를 방지합니다.
    *   오브의 수명을 제한하여 메모리 누수를 방지합니다.
*   **확장성 및 유지보수성:**
    *   상속을 통해 다양한 종류의 오브를 쉽게 추가할 수 있습니다.
    *   데이터 테이블을 활용하여 오브의 스탯, 이펙트, 사운드 등을 관리합니다.
*   **실무 적용 가이드:**
    *   오브의 종류와 복잡도를 고려하여 풀 크기를 적절하게 설정합니다.
    *   콜리전 채널을 최적화하여 불필요한 콜리전 검사를 방지합니다.
    *   오브의 스탯, 이펙트, 사운드 등을 데이터 테이블을 통해 관리하여 유지보수성을 높입니다.

### 5. `ACBoss`

*   **클래스 개요:**
    *   `ACBoss`는 게임 내 보스 캐릭터의 핵심 로직을 담당하는 캐릭터 클래스입니다. 보스의 이동, 공격, 피격, 사망, 상태 관리 등 보스 캐릭터의 전반적인 동작을 제어합니다.
    *   `ACBoss`는 `ACharacter` 클래스를 상속받아 구현되며, 언리얼 엔진의 캐릭터 이동 시스템, 애니메이션 시스템, 콜리전 시스템 등을 활용합니다.
    *   설계 철학은 보스 캐릭터의 동작을 중앙 집중화하여 관리하고, 게임의 흐름에 따라 보스 상태를 제어하는 데 있습니다.

*   **핵심 클래스 분석:**
    *   **목적 및 책임:**
        *   보스 캐릭터 이동 및 회전
        *   보스 캐릭터 공격 및 스킬 사용
        *   보스 캐릭터 피격 및 데미지 처리
        *   보스 캐릭터 사망 처리
        *   보스 캐릭터 상태 관리 (체력, AI, 장비 등)
    *   **주요 메서드와 프로퍼티 상세 분석:**
        *   `TakeDamage()`: 데미지를 처리합니다. 데미지 양, 데미지 타입, 공격자 등을 인자로 받아 체력을 감소시키고, 피격 이펙트를 재생합니다.
        *   `BeginPlay()`: 액터가 생성된 후 게임 시작 시 호출됩니다. 보스 캐릭터의 초기 상태를 설정합니다.
        *   `PlayHitMotion()`: 피격 모션을 재생합니다.
        *   `ShowBossStatusWidget()`: 보스 상태 위젯을 표시합니다.
        *   `HPUpdate()`: 체력 UI를 업데이트합니다.
        *   `RestartUI()`: UI를 리스타트합니다.
        *   `PlayBossBGM()`: 보스 배경음악을 재생합니다.
        *   `StopBossBGM()`: 보스 배경음악을 정지합니다.
        *   `LowerBossBGMVolume()`: 보스 배경음악 볼륨을 감소시킵니다 (죽음 시).
    *   **클래스의 생명주기와 상태 관리:**
        *   `BeginPlay()`: 액터가 생성된 후 게임 시작 시 호출됩니다. 보스 캐릭터의 초기 상태를 설정합니다.
        *   `Tick()`: 매 프레임마다 호출됩니다. 보스 캐릭터의 위치, 회전, 애니메이션, AI 등을 업데이트합니다.
        *   보스 캐릭터는 생성, 이동, 공격, 피격, 사망 상태를 가지며, 각 상태에 따라 적절한 로직을 수행합니다.
    *   **실제 사용 예제와 코드:**

```cpp
// 데미지 처리
float ACBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    // 체력 감소
    CurrentHealth -= DamageAmount;

    // 피격 이펙트 재생
    PlayHitMotion();

    // 체력 UI 업데이트
    HPUpdate();

    // 사망 처리
    if (CurrentHealth <= 0)
    {
        Die();
    }

    return DamageAmount;
}

// 사망 처리
void ACBoss::Die()
{
    // AI 정지
    GetController()->StopMovement();

    // 애니메이션 재생
    PlayDeathAnimation();

    // 콜리전 비활성화
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    // 보상 지급
    GiveReward();

    // 게임 클리어 처리
    GameClear();
}
```

    *   **성능 특성과 최적화 포인트:**
        *   불필요한 Tick 함수 호출을 최소화합니다.
        *   애니메이션 최적화: 애니메이션 LOD, 본 최적화 등을 통해 성능을 향상시킵니다.
        *   콜리전 채널 최적화: 불필요한 콜리전 검사를 최소화하여 성능을 향상시킵니다.
    *   **사용 시나리오와 베스트 프랙티스:**
        *   보스 전투: 플레이어가 보스 캐릭터와 전투를 벌일 때 사용합니다.
        *   보스 이벤트: 보스 캐릭터가 특정 이벤트를 발생시킬 때 사용합니다.
    *   **다른 클래스와의 상호작용:**
        *   `ACBossAIC`: 보스 AI 컨트롤러 클래스입니다. `ACBoss`는 `ACBossAIC`를 통해 보스 AI를 제어합니다.
        *   `APlayerCharacter`: 플레이어 캐릭터 클래스입니다. `ACBoss`는 플레이어에게 데미지를 입히고, 플레이어의 위치를 추적합니다.
        *   `ABossManager`: 보스 매니저 클래스입니다. `ABossManager`는 `ACBoss`를 생성하고 관리합니다.

*   **설계 패턴 분석:**
    *   상태 패턴: 보스 캐릭터의 상태를 관리하고, 상태 변화에 따라 적절한 동작을 수행합니다.
    *   옵저버 패턴: 보스 캐릭터의 상태 변화를 다른 클래스에 알립니다.
*   **성능 및 최적화:**
    *   불필요한 Tick 함수 호출을 최소화합니다.
    *   애니메이션 LOD, 본 최적화 등을 통해 애니메이션 성능을 향상시킵니다.
    *   콜리전 채널을 최적화하여 불필요한 콜리전 검사를 방지합니다.
*   **확장성 및 유지보수성:**
    *   상속을 통해 다양한 종류의 보스 캐릭터를 쉽게 추가할 수 있습니다.
    *   데이터 테이블을 활용하여 보스 캐릭터의 스탯, AI, 장비 등을 관리합니다.
    *   컴포넌트 패턴을 통해 보스 캐릭터의 기능을 모듈화하여 확장성을 높입니다.
*   **실무 적용 가이드:**
    *   보스 캐릭터의 종류와 복잡도를 고려하여 애니메이션, 콜리전 등을 최적화합니다.
    *   데이터 테이블을 활용하여 보스 캐릭터의 스탯, AI, 장비 등을 관리합니다.
    *   컴포넌트 패턴을 통해 보스 캐릭터의 기능을 모듈화하여 확장성을 높입니다.

### 6. `ACBossAIC`

*   **클래스 개요:**
    *   `ACBossAIC`는 보스 캐릭터의 인공지능(AI)을 담당하는 AIController 클래스입니다. 보스의 행동 패턴, 의사 결정, 경로 탐색 등을 제어하며, StateTree를 사용하여 복잡한 AI 로직을 구현합니다.
    *   `ACBossAIC`는 `AAIController` 클래스를 상속받아 구현되며, 언리얼 엔진의 AI 시스템을 활용합니다.
    *   설계 철학은 보스 캐릭터의 AI 로직을 중앙 집중화하여 관리하고, StateTree를 통해 유연하고 확장 가능한 AI를 구현하는 데 있습니다.

*   **핵심 클래스 분석:**
    *   **목적 및 책임:**
        *   보스 캐릭터 행동 패턴 정의
        *   보스 캐릭터 의사 결정 (공격, 이동, 회피 등)
        *   보스 캐릭터 경로 탐색
        *   StateTree를 통한 AI 로직 관리
    *   **주요 메서드와 프로퍼티 상세 분석:**
        *   `OnPossess()`: Pawn을 소유했을 때 호출됩니다. AI를 초기화하고, StateTree를 실행합니다.
    *   **클래스의 생명주기와 상태 관리:**
        *   `BeginPlay()`: 액터가 생성된 후 게임 시작 시 호출됩니다. AI의 초기 상태를 설정합니다.
        *   `OnPossess()`: Pawn을 소유했을 때 호출됩니다. AI를 초기화하고, StateTree를 실행합니다.
        *   AI는 StateTree에 정의된 상태에 따라 행동하며, 상태 변화에 따라 적절한 로직을 수행합니다.
    *   **실제 사용 예제와 코드:**

```cpp
// Pawn 소유 시 호출
void ACBossAIC::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // StateTree 실행
    RunBehaviorTree(BehaviorTreeAsset);
}
```

    *   **성능 특성과 최적화 포인트:**
        *   StateTree 최적화: 불필요한 브랜치, 조건 검사 등을 최소화하여 성능을 향상시킵니다.
        *   경로 탐색 최적화: 경로 탐색 빈도를 줄이고, 비용이 낮은 알고리즘을 사용합니다.
        *   AI 업데이트 빈도 조절: AI 업데이트 빈도를 줄여 CPU 부하를 줄입니다.
    *   **사용 시나리오와 베스트 프랙티스:**
        *   보스 전투: 플레이어가 보스 캐릭터와 전투를 벌일 때 AI를 사용합니다.
        *   보스 이벤트: 보스 캐릭터가 특정 이벤트를 발생시킬 때 AI를 사용합니다.
    *   **다른 클래스와의 상호작용:**
        *   `ACBoss`: 보스 캐릭터 클래스입니다. `ACBossAIC`는 `ACBoss`의 AI를 제어합니다.
        *   `APlayerCharacter`: 플레이어 캐릭터 클래스입니다. `ACBossAIC`는 플레이어의 위치를 추적하고, AI에 정보를 제공합니다.

*   **설계 패턴 분석:**
    *   State 패턴: StateTree를 사용하여 AI의 상태를 관리하고, 상태 변화에 따라 적절한 행동을 수행합니다.
    *   전략 패턴: AI의 행동 전략을 캡슐화하고, 런타임에 전략을 변경할 수 있도록 합니다.
*   **성능 및 최적화:**
    *   StateTree를 최적화하여 불필요한 브랜치, 조건 검사 등을 최소화합니다.
    *   경로 탐색 빈도를 줄이고, 비용이 낮은 알고리즘을 사용합니다.
    *   AI 업데이트 빈도를 조절하여 CPU 부하를 줄입니다.
*   **확장성 및 유지보수성:**
    *   StateTree를 사용하여 AI 로직을 모듈화하고, 쉽게 수정하고 확장할 수 있도록 합니다.
    *   데이터 테이블을 활용하여 AI의 파라미터를 관리합니다.
*   **실무 적용 가이드:**
    *   StateTree를 설계할 때 AI의 행동 패턴을 명확하게 정의하고, 불필요한 복잡성을 줄입니다.
    *   AI의 파라미터를 데이터 테이블을 통해 관리하여 쉽게 조정할 수 있도록 합니다.
    *   AI 성능을 지속적으로 모니터링하고, 필요에 따라 StateTree를 최적화합니다.

### 7. `ACBossWeapon`

*   **클래스 개요:**
    *   `ACBossWeapon`은 보스 캐릭터가 사용하는 무기를 표현하고, 무기의 공격 로직, 콜리전 감지, 데미지 처리 등을 담당하는 액터 클래스입니다.
    *   `ACBossWeapon`은 보스 캐릭터에 부착되어 사용되며, 무기의 종류에 따라 다양한 공격 방식과 효과를 제공합니다.
    *   설계 철학은 무기의 기능을 모듈화하고, 보스 캐릭터의 공격 로직을 유연하게 관리하는 데 있습니다.

*   **핵심 클래스 분석:**
    *   **목적 및 책임:**
        *   무기의 콜리전 감지 및 데미지 처리
        *   무기의 공격 애니메이션 재생
        *   무기의 특수 효과 (이펙트, 사운드 등) 재생
        *   무기의 상태 관리 (장착, 해제 등)
    *   **주요 메서드와 프로퍼티 상세 분석:**
        *   `OnBossBeginEquip()`: 보스가 무기를 장착하기 시작할 때 호출됩니다.
        *   `OnBossUnequip()`: 보스가 무기를 해제할 때 호출됩니다.
        *   `OnBossCollisions()`: 무기의 콜리전을 활성화합니다.
        *   `OffBossCollisions()`: 무기의 콜리전을 비활성화합니다.
        *   `BossAttachToCollision()`: 무기를 콜리전에 부착합니다.
        *   `StartCollisionAtSocket()`: 특정 소켓에서 콜리전을 시작합니다.
        *   `EndCollisionToOwner()`: 오너에게 콜리전을 종료합니다.
        *   `BossAttachTo()`: 무기를 특정 액터에 부착합니다.
        *   `OnBossComponentBeginOverlap()`: 무기가 다른 액터와 겹치기 시작할 때 호출됩니다.
        *   `OnBossComponentEndOverlap()`: 무기가 다른 액터와 겹치기 끝날 때 호출됩니다.
    *   **클래스의 생명주기와 상태 관리:**
        *   `BeginPlay()`: 액터가 생성된 후 게임 시작 시 호출됩니다. 무기의 초기 상태를 설정합니다.
        *   `Tick()`: 매 프레임마다 호출됩니다. 무기의 위치, 회전, 콜리전 등을 업데이트합니다.
        *   무기는 장착, 해제, 공격 상태를 가지며

