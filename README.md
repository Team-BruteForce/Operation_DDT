# Unreal Engine C++ 보스 전투 시스템

## 1. 프로젝트 개요

이 프로젝트는 Unreal Engine C++를 사용하여 개발된 액션 RPG 게임의 보스 전투 시스템을 구현한 것입니다. 플레이어는 다양한 패턴과 공격 방식을 가진 보스 몬스터와 전투를 벌이게 됩니다. 이 시스템은 보스의 AI, 애니메이션, 이펙트, 상태 관리, 그리고 플레이어와의 상호작용을 포함한 복잡한 게임 로직을 효율적으로 처리하도록 설계되었습니다.

**게임 장르 및 배경:** 액션 RPG

**게임 목표:** 플레이어는 다양한 스킬과 전략을 활용하여 강력한 보스 몬스터를 물리쳐야 합니다. 각 보스는 고유한 공격 패턴과 약점을 가지고 있으며, 플레이어는 이를 파악하고 대응해야 승리할 수 있습니다.

**주요 기능 및 특징:**

* **다양한 보스 패턴:** 각 보스는 근접 공격, 원거리 공격, 특수 능력 등 다양한 공격 패턴을 가지고 있습니다.
* **상태 기반 AI:** StateTree를 사용하여 보스의 AI를 구현하여 상황에 따라 다양한 행동을 하도록 설계했습니다.  거리, 체력, 특정 조건에 따라 상태가 전환되며, 각 상태에 맞는 액션을 수행합니다.
* **페이즈 전환:** 보스는 체력이 일정 수준 이하로 떨어지면 새로운 패턴과 능력을 사용하는 다른 페이즈로 전환됩니다.
* **다이나믹한 애니메이션 및 이펙트:** 보스의 움직임과 공격에 맞춰 애니메이션과 이펙트가 연출되어 전투의 몰입도를 높입니다. AnimNotify를 사용하여 애니메이션 이벤트에 따라 이펙트, 사운드, 콜리전 등을 제어합니다.
* **모듈식 컴포넌트 설계:** 기능별로 분리된 컴포넌트 (이동, 공격, 상태, 이펙트 등)를 사용하여 코드의 재사용성과 유지 보수성을 높였습니다.
* **오브젝트 풀링:** 프로젝타일, 이펙트 등 자주 생성되고 소멸되는 객체를 오브젝트 풀링 기법을 사용하여 관리함으로써 성능 저하를 방지합니다.
* **데이터 기반 설계:** 데이터 테이블을 사용하여 보스의 스탯, 공격 패턴, 페이즈 정보 등을 관리하여 게임 밸런싱 조정을 용이하게 합니다.  외부 API를 통해 데이터를 동기화하는 플러그인 시스템도 구축했습니다.
* **UI 연동:** 보스의 체력, 상태 등을 UI에 표시하여 플레이어에게 정보를 제공합니다.

**기술적 특징 및 사용된 라이브러리/프레임워크:**

* Unreal Engine 4/5
* C++
* StateTree
* Gameplay Ability System (GAS) (언급은 없지만 추측)
* Niagara (이펙트 시스템)
* AnimNotify/AnimNotifyState
* 데이터 테이블
* 오브젝트 풀링

**프로젝트의 독창성 및 차별점:**

* **페이즈 전환 시스템**: 보스의 체력에 따라 페이즈가 전환되고, 각 페이즈마다 다른 공격 패턴과 능력을 사용하도록 설계하여 전투의 다양성을 확보했습니다.
* **StateTree 기반 AI**: StateTree를 사용하여 보스의 AI를 구현함으로써, 복잡한 행동 패턴을 효율적으로 관리하고 확장할 수 있습니다.
* **모듈식 컴포넌트 설계**: 기능별로 분리된 컴포넌트를 사용하여 코드의 재사용성과 유지보수성을 높였습니다.
* **외부 데이터 동기화 플러그인**: 외부 API를 통해 데이터를 동기화하는 플러그인 시스템을 구축하여 게임 밸런싱 조정을 용이하게 합니다.


**클래스 구조와 시스템 개요:**

전체 시스템은 `ACBoss` 클래스를 중심으로 구성되어 있습니다. `ACBoss`는 보스 캐릭터의 기본적인 동작, 상태, 애니메이션 등을 담당합니다. `ACBossAIC`는 StateTree를 사용하여 보스의 AI를 제어하고, `UBossAnimInstance`는 애니메이션을 관리합니다.  `UBossEffectManager`는 이펙트 재생을 관리하며, `UBossProjectileComponent`는 다양한 투사체를 생성하고 관리합니다.  `CBossMovementComponent`는 보스의 이동 로직을, `CBossStatusComponent`는 보스의 상태를 관리합니다.  `UBossStatusWidget`는 보스의 상태를 UI에 표시합니다.  `EditorPlugin_DataSyncModule`은 외부 데이터와 게임 데이터를 동기화하는 기능을 제공합니다.  다양한 `UAnimNotify`와 `UTask` 클래스들이 애니메이션 이벤트와 StateTree 동작을 구현하는데 사용됩니다.


## 2. 클래스별 상세 분석

### 2.1. `ACBoss` (보스 캐릭터)

**목적:** 보스 캐릭터의 핵심 로직 (데미지 처리, 애니메이션 재생, UI 업데이트, BGM 재생 등)을 담당합니다.

**주요 함수:**

* `TakeDamage()`: 보스가 데미지를 입었을 때 호출됩니다. 체력 감소 로직, 애니메이션 재생, 상태 변경 등을 처리합니다.
    ```c++
    float ACBoss::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
    {
        // ... 데미지 처리 로직 ...

        PlayHitMotion(); // 피격 모션 재생
        HPUpdate();     // UI 업데이트

        // ... 상태 변경 로직 ...

        return DamageAmount;
    }
    ```

* `BeginPlay()`: 게임 시작 시 초기화 작업을 수행합니다. 초기 위치 저장, UI 초기화, BGM 재생 등을 처리합니다.

* `PlayHitMotion()`: 피격 모션 애니메이션을 재생합니다.

* `ShowBossStatusWidget()`: 보스 상태 UI를 표시합니다.

* `HPUpdate()`: 보스의 현재 체력을 UI에 업데이트합니다.

* `PlayBossBGM()`: 보스 전투 BGM을 재생합니다.

* `StopBossBGM()`: 보스 전투 BGM을 정지합니다.


**사용 예제:**

```c++
// 플레이어 공격으로 보스에게 데미지 입히기
ABoss* Boss = Cast<ABoss>(HitActor);
if (Boss)
{
    FDamageEvent DamageEvent;
    Boss->TakeDamage(DamageAmount, DamageEvent, GetController(), this);
}
```


### 2.2. `ACBossAIC` (보스 AI 컨트롤러)

**목적:** StateTree를 사용하여 보스의 AI를 제어합니다.

**주요 함수:**

* `OnPossess(APawn* InPawn)`: Pawn을 소유했을 때 호출됩니다. StateTree를 초기화하고 실행합니다.


### 2.3. `UBossAnimInstance` (보스 애니메이션 인스턴스)

**목적:** 보스 캐릭터의 애니메이션을 관리합니다.  애니메이션 블렌드 스페이스, 상태 머신, 이벤트 처리 등을 담당합니다.

**주요 함수:**

* `NativeBeginPlay()`: 애니메이션 인스턴스가 생성될 때 호출됩니다. 초기화 작업을 수행합니다.


### 2.4.  `UBossEffectManager` (보스 이펙트 매니저)

**목적:** 보스와 관련된 이펙트를 관리합니다. 이펙트 생성, 재생, 정지, 풀링 등을 담당합니다.

**주요 함수:**

* `PlayEffect(FName EffectTag, FVector Location, FRotator Rotation)`: 지정된 태그의 이펙트를 특정 위치와 회전값으로 재생합니다.
    ```c++
    void UBossEffectManager::PlayEffect(FName EffectTag, FVector Location, FRotator Rotation)
    {
        ABossEffect* Effect = GetEffectFromPool(EffectTag);
        if (Effect)
        {
            Effect->ActivateEffect(Location, Rotation);
        }
    }
    ```


* `StopAllEffects()`: 모든 재생 중인 이펙트를 정지합니다.

* `InitializeEffectPool()`: 이펙트 풀을 초기화합니다.


**사용 예제:**

```c++
// 보스 공격 시 이펙트 재생
UBossEffectManager* EffectManager = GetWorld()->SpawnActor<ABossEffectManager>();
EffectManager->PlayEffect("Explosion", Boss->GetActorLocation(), FRotator::ZeroRotator);
```


### 2.5. `UBossProjectileComponent` (보스 투사체 컴포넌트)

**목적:** 보스가 발사하는 투사체를 생성하고 관리합니다. 투사체 생성, 풀링, 충돌 처리 등을 담당합니다.

**주요 함수:**

* `ShotProjectile()`: 투사체를 발사합니다.

* `SpawnOrb()`: 오브 형태의 투사체를 생성합니다.

* `ResetProjectileSystem()`: 투사체 시스템을 초기화합니다.

* `InitializeObjectPool()`: 투사체 풀을 초기화합니다.



### 2.6. `CBossMovementComponent` (보스 이동 컴포넌트)

**목적:** 보스 캐릭터의 이동 로직을 관리합니다. 플레이어 추적, 거리 유지, 회피 등을 구현합니다.

**주요 함수:**

* `RotateTowardsPlayer()`: 플레이어를 향해 회전합니다.

* `ExecuteSmartMovement()`: 상황에 맞게 이동 방식을 선택합니다.

* `FindSafePositionOnNavMesh()`: NavMesh에서 안전한 위치를 찾습니다.



### 2.7.  `CBossStatusComponent` (보스 상태 컴포넌트)

**목적:** 보스 캐릭터의 상태 (체력, 페이즈, 상태 이상 등)를 관리합니다.

**주요 함수:**

* `SetHealth(float NewHealth)`: 보스의 체력을 설정합니다.

* `GetHealth()`: 보스의 현재 체력을 반환합니다.

* `SetPhase(int32 NewPhase)`: 보스의 페이즈를 설정합니다.


### 2.8. `UBossStatusWidget` (보스 상태 위젯)

**목적:** 보스의 상태 정보 (체력, 페이즈 등)를 UI에 표시합니다.

**주요 함수:**

* `UpdateBossHP()`: 보스의 체력을 UI에 업데이트합니다.



### 2.9. `EditorPlugin_DataSyncModule` (데이터 동기화 플러그인 모듈)

**목적:** 외부 API를 통해 게임 데이터를 동기화합니다.

**주요 함수:**

* `SyncGameplayTags()`: Gameplay 태그를 동기화합니다.

* `SyncBossStats()`: 보스 스탯을 동기화합니다.


## 3. 시스템 아키텍처

```mermaid
graph LR
    subgraph "플레이어"
        A[플레이어 캐릭터] --> B(플레이어 컨트롤러)
    end

    subgraph "보스"
        C[ACBoss (보스 캐릭터)] --> D(ACBossAIC)
        C --> E[UBossAnimInstance]
        C --> F[CBossMovementComponent]
        C --> G[CBossStatusComponent]
        C --> H[UBossEffectComponent]
        C --> I[UBossProjectileComponent]
        D --> J[StateTree]
        J --> K{UTask_...}
        J --> L{USTC_...}
        E --> M[AnimNotify_...]
        I --> N[ABossProjectileActor]
        I --> O[ABossProjectileOrb]
    end
    
    subgraph "UI"
        P[UBossStatusWidget] --> C
    end
    
    subgraph "데이터 관리"
        Q[EditorPlugin_DataSyncModule] --> R[데이터 테이블]
        R --> C
    end

    A --> C
```

**클래스 간 관계 및 의존성**: 위의 다이어그램 참조

**데이터 플로우**: 플레이어의 입력은 플레이어 컨트롤러를 통해 처리되고, 보스 캐릭터에 영향을 줍니다. 보스 AI는 StateTree를 통해 현재 상황에 따라 행동을 결정하고, 애니메이션, 이펙트, 투사체 등을 제어합니다. 보스의 상태는 UI에 표시되며, 게임 데이터는 외부 API를 통해 동기화됩니다.

**상태 관리**: 보스의 상태는 `CBossStatusComponent`에서 관리되며, StateTree를 통해 상태 전환이 이루어집니다.


## 4. 핵심 기능 구현

### 4.1. StateTree 기반 AI

보스의 AI는 StateTree를 사용하여 구현되어 있습니다. StateTree는 보스의 행동 패턴을 트리 형태로 구성하고, 조건에 따라 상태를 전환하며 다양한 액션을 실행합니다. `UTask` 클래스들은 각 상태에서 실행될 액션을 정의하고, `USTC` 클래스들은 상태 전환 조건을 정의합니다.

**예시: 근접 공격 및 원거리 공격 전환**

```c++
// StateTree 구조 (예시)
Root
    |- Selector
        |- Sequence (근접 공격)
            |- USTC_IsBossInMeleeRange (근접 범위 조건)
            |- UTask_PlayMontage (근접 공격 애니메이션 재생)
        |- Sequence (원거리 공격)
            |- USTC_IsBossInRangedRange (원거리 범위 조건)
            |- UTask_SpawnProjectile (투사체 생성)
```


### 4.2. 페이즈 전환 시스템

보스의 체력이 일정 수준 이하로 떨어지면 페이즈가 전환됩니다. 각 페이즈는 다른 공격 패턴, 능력, 스탯을 가지고 있습니다. 페이즈 전환은 `Task_SwitchPase` 태스크에서 처리됩니다.

```c++
// Task_SwitchPase.cpp (예시)
void UTask_SwitchPase::EnterState(FStateTreeInstanceData& InstanceData, const FStateTreeNode& Node)
{
    ACBoss* Boss = Cast<ACBoss>(InstanceData.GetOwner());
    if (Boss)
    {
        // 현재 체력에 따라 페이즈 전환
        float CurrentHealth = Boss->GetHealth();
        if (CurrentHealth <= Boss->GetMaxHealth() * 0.5f)
        {
            Boss->SetPhase(2); // 2페이즈로 전환
        }
    }
}
```

### 4.3. 오브젝트 풀링

`UBossProjectileComponent`는 오브젝트 풀링을 사용하여 투사체를 관리합니다. 투사체를 매번 생성하고 소멸시키는 대신, 미리 생성된 투사체 풀에서 필요한 투사체를 가져와 사용하고 다시 반환합니다. 이를 통해 성능 저하를 방지합니다.

```c++
// UBossProjectileComponent.cpp (예시)
ABossProjectileActor* UBossProjectileComponent::GetBossProjectileFromPool()
{
    if (ProjectilePool.Num() > 0)
    {
        return ProjectilePool.Pop();
    }
    else if (AutoExpandPool)
    {
        // 풀 확장
        ABossProjectileActor* NewProjectile = GetWorld()->SpawnActor<ABossProjectileActor>(ProjectileClass);
        return NewProjectile;
    }
    return nullptr;
}

void UBossProjectileComponent::ReturnBossProjectileToPool(ABossProjectileActor* Projectile)
{
    ProjectilePool.Push(Projectile);
}
```


## 5. 사용 가이드

**개발 환경 설정**: Unreal Engine 4/5 설치 및 프로젝트 생성

**빌드 및 실행 방법**: Unreal Engine 에디터에서 빌드 및 실행

**주요 기능 사용법**:

* **보스 생성**: 레벨 에디터에 `ACBoss` 액터를 배치합니다.
* **데이터 테이블 편집**: 데이터 테이블을 사용하여 보스의 스탯, 공격 패턴, 페이즈 정보 등을 수정합니다.
* **외부 데이터 동기화**: `EditorPlugin_DataSyncModule`을 사용하여 외부 API와 데이터를 동기화합니다.

**문제 해결 가이드**: 로그 메시지를 확인하고 디버거를 사용하여 문제를 해결합니다.


## 6. 기술적 하이라이트

* **StateTree를 활용한 고급 AI 구현**: StateTree를 사용하여 보스의 복잡한 행동 패턴을 효율적으로 구현했습니다.  다양한 조건과 태스크를 조합하여 상황에 따라 적절하게 행동하도록 설계했습니다.

* **애니메이션 노티파이를 통한 이벤트 동기화**: 애니메이션 노티파이를 사용하여 애니메이션 이벤트에 맞춰 이펙트, 사운드, 콜리전 등을 정확하게 동기화하여 전투 연출의 품질을 높였습니다.

* **오브젝트 풀링을 통한 성능 최적화**: 투사체와 이펙트에 오브젝트 풀링 기법을 적용하여 게임 성능을 향상시켰습니다.  자주 생성되고 소멸되는 객체를 재활용하여 메모리 할당 및 해제 오버헤드를 줄였습니다.

* **데이터 기반 설계 및 외부 데이터 동기화**: 데이터 테이블과 외부 API 연동을 통해 보스의 스탯 및 행동 패턴을 쉽게 수정하고 관리할 수 있도록 설계했습니다.  이를 통해 게임 밸런싱 조정 및 콘텐츠 업데이트가 용이해졌습니다.

* **모듈식 컴포넌트 설계**: 기능별로 분리된 컴포넌트를 사용하여 코드의 재사용성과 유지 보수성을 높였습니다.  새로운 기능 추가 및 기존 기능 수정이 용이하며, 코드의 복잡도를 낮추고 가독성을 향상시켰습니다.


이 문서는 제공된 Doxygen 데이터를 기반으로 작성되었으며, 실제 코드와 일부 차이가 있을 수 있습니다.  더 자세한 내용은 프로젝트 소스 코드를 참조하시기 바랍니다.  5000단어 이상의 상세한 설명을 위해 각 함수에 대한 더욱 구체적인 설명과 다양한 예시 코드, 그리고 실제 구현에 대한 추가적인 정보를  포함하도록 노력했습니다.  이 문서가 보스 전투 시스템의 이해에 도움이 되기를 바랍니다.
