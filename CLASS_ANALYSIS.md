## C++ 클래스 설계 및 분석 문서

### 1. 클래스 개요

이 문서는 언리얼 엔진 4(UE4) 기반의 게임 프로젝트, 특히 보스 몬스터와 관련된 시스템의 클래스 구조를 상세하게 분석합니다.  전체적인 역할은 보스 몬스터의 행동, 능력, 이펙트, AI, 애니메이션, 그리고 게임 내 상호작용을 관리하는 것입니다.

**클래스 간 관계 및 의존성:**

*   **`ABossManager`**: 보스 생성 및 초기화, 전체 상태 관리의 중심 역할을 합니다. `ACBoss`, `ACBossAIC`, `UBossProjectileComponent`, `UBossEffectManager`, `CBossMovementComponent`, `CBossStatusComponent` 등의 클래스와 강하게 결합되어 있습니다.  `ABossManager`는 이들을 생성, 초기화, 관리하며, 보스의 전체적인 생명주기를 제어합니다.
*   **`ACBoss`**: 보스 캐릭터 자체를 나타냅니다.  데미지 처리, 애니메이션 재생, UI 업데이트 등을 담당합니다. `UBossAnimInstance`, `ACBossWeapon`, `UBossStatusWidget` 등과 연관됩니다.
*   **`ACBossAIC`**: 보스의 AI를 담당하며, `StateTree`를 사용하여 행동을 결정합니다.  `UCBossEnemyStateTreeEvaluator`와 `UTask_*` 클래스들과 협력하여 보스의 행동을 제어합니다.
*   **`UBossProjectileComponent`**: 보스의 투사체 발사 시스템을 관리합니다. `ABossProjectileActor`, `ABossProjectileOrb`, `AGateOfBabylon`, `AHolySwordMagic`, `AProjectile_LightSpear` 등 다양한 투사체를 풀링하여 관리하고 발사합니다.
*   **`UBossEffectManager`**: 보스의 시각 효과 및 사운드 효과를 관리합니다. `ABossEffect` 클래스를 풀링하여 관리하고, 필요에 따라 활성화/비활성화합니다.
*   **`CBossMovementComponent`**: 보스의 이동 로직을 담당합니다. 플레이어 추적, 회피, 궤도 이동 등을 수행합니다.
*   **`CBossStatusComponent`**: 보스의 스탯(HP, AP 등)을 관리하고, 상태 변화(그로기, 사망 등)를 감지합니다.
*   **`ACBossWeapon`**: 보스의 무기를 나타내며, 공격 판정 및 콜리전 처리를 담당합니다. `UAnimNotify_BossWeaponCollision`과 함께 사용되어 애니메이션 기반의 공격 타이밍을 제어합니다.
*   **`UAnimNotify_*`**: 애니메이션 노티파이를 통해 특정 시점에 이벤트(이펙트 재생, 콜리전 활성화 등)를 발생시킵니다.  `ACBoss`, `ACBossWeapon`, `UBossEffectManager`, `UBossProjectileComponent` 등 다양한 클래스와 연동됩니다.
*   **`UTask_*`**: `StateTree`에서 사용되는 태스크 클래스들입니다. 보스의 특정 행동(추적, 공격, 회피 등)을 수행합니다.
*   **`EditorPlugin_*`**: 언리얼 엔진 에디터에서 보스 데이터를 관리하고 동기화하는 플러그인 관련 클래스입니다.

**설계 철학과 아키텍처 패턴:**

*   **컴포넌트 기반 설계:** 보스 캐릭터의 기능을 여러 컴포넌트로 분리하여 재사용성과 유지보수성을 높입니다.  `ACBoss`는 핵심 캐릭터 로직을 담당하고, 나머지 기능은 `UBossProjectileComponent`, `UBossEffectManager`, `CBossMovementComponent` 등과 같은 컴포넌트에서 처리합니다.
*   **객체 풀링:** `UBossProjectileComponent`, `UBossEffectManager` 등에서 투사체와 이펙트를 객체 풀링하여 생성/소멸 비용을 줄이고 성능을 최적화합니다.
*   **StateTree:** 보스의 AI를 `StateTree`를 사용하여 구현합니다. `StateTree`는 계층적인 상태 머신을 통해 복잡한 AI 행동을 쉽게 설계하고 관리할 수 있도록 합니다.
*   **애니메이션 노티파이:** 애니메이션 노티파이를 사용하여 애니메이션 재생 중 특정 시점에 이벤트(공격 판정, 이펙트 재생 등)를 발생시킵니다.  이를 통해 애니메이션과 게임 로직을 긴밀하게 연동할 수 있습니다.
*   **데이터 테이블:** 보스 스탯, 이펙트 정보, 투사체 정보 등을 데이터 테이블에 저장하여 관리합니다.  데이터 테이블을 사용하면 게임 로직을 변경하지 않고도 보스의 능력치를 쉽게 조정할 수 있습니다.
*   **에디터 플러그인:** `EditorPlugin_*` 클래스들은 언리얼 엔진 에디터에서 보스 데이터를 관리하고 외부 데이터 소스(예: 스프레드시트)와 동기화하는 기능을 제공합니다.

### 2. 핵심 클래스 분석

#### 2.1. `ABossManager`

*   **목적과 책임:**
    *   보스 몬스터의 생성, 초기화, 상태 관리의 중심 역할을 합니다.
    *   게임 월드에 보스를 스폰하고, 보스의 생명주기를 관리합니다.
    *   보스의 상태를 초기화하고, 필요에 따라 리셋합니다.
    *   보스 전투 시작/종료 시 이벤트(문 열기/닫기 등)를 처리합니다.

*   **주요 메서드와 프로퍼티 상세 분석:**

    *   **`ABossManager()`**: 생성자. 보스 스폰에 필요한 기본 설정을 초기화합니다.
    *   **`Tick(float DeltaTime)`**: 매 프레임 호출됩니다. 보스의 상태를 업데이트하고, 필요한 로직을 실행합니다.
    *   **`ResetBossCompletely()`**: 보스를 완전히 초기화합니다.  보스의 모든 컴포넌트를 리셋하고, `StateTree`를 재시작합니다.  이 함수는 보스 전투를 다시 시작하거나, 보스를 새로운 페이즈로 전환할 때 사용됩니다.
    *   **`OpenDoor()`**: 보스 전투 시작 시 문을 열어 플레이어가 보스 방으로 들어갈 수 있도록 합니다.
    *   **`FindBossInWorld()`**: 게임 월드에서 보스 캐릭터를 찾습니다.  이 함수는 보스 캐릭터가 이미 스폰되어 있는지 확인하는 데 사용됩니다.
    *   **`ResetAllBossComponents()`**: 보스의 모든 컴포넌트를 초기화합니다.  이 함수는 `ResetBossCompletely()`에서 호출되어 보스의 모든 기능을 초기 상태로 되돌립니다.
    *   **`ResetBossStateTree()`**: 보스의 `StateTree`를 재시작합니다.  이 함수는 보스의 AI를 초기 상태로 되돌립니다.
    *   **`BeginPlay()`**: 게임 시작 시 호출됩니다. 보스 스폰 위치를 찾고, 보스를 스폰합니다.
    *   **`OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)`**: 트리거 박스와 플레이어가 겹칠 때 호출됩니다. 보스 전투를 시작하는 데 사용될 수 있습니다.

*   **클래스의 생명주기와 상태 관리:**

    *   `ABossManager`는 게임 시작 시 생성되고, 레벨이 언로드될 때 소멸됩니다.
    *   보스의 상태는 `StateTree`와 `CBossStatusComponent`를 통해 관리됩니다.
    *   `ResetBossCompletely()` 함수를 통해 보스의 상태를 초기화할 수 있습니다.

*   **실제 사용 예제와 코드:**

    ```c++
    // BossManager.h
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

        // 보스를 완전히 초기화합니다.
        UFUNCTION(BlueprintCallable, Category = "Boss")
        void ResetBossCompletely();

        // 문을 엽니다.
        UFUNCTION(BlueprintCallable, Category = "Boss")
        void OpenDoor();

    private:
        // 월드에서 보스를 찾습니다.
        AActor* FindBossInWorld();

        // 모든 보스 컴포넌트를 초기화합니다.
        void ResetAllBossComponents();

        // 보스 StateTree를 초기화합니다.
        void ResetBossStateTree();

        // 트리거 박스 오버랩 시작 이벤트
        UFUNCTION()
        void OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

    };

    // BossManager.cpp
    #include "BossManager.h"
    #include "CBoss.h"
    #include "Components/BoxComponent.h"

    ABossManager::ABossManager()
    {
        // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
        PrimaryActorTick.bCanEverTick = true;

        // 트리거 박스 생성
        TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
        RootComponent = TriggerBox;
        TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
        TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABossManager::OnTriggerBoxOverlapBegin);
    }

    void ABossManager::BeginPlay()
    {
        Super::BeginPlay();

        // 보스 찾기
        BossActor = FindBossInWorld();
    }

    void ABossManager::Tick(float DeltaTime)
    {
        Super::Tick(DeltaTime);
    }

    void ABossManager::ResetBossCompletely()
    {
        if (BossActor)
        {
            ResetAllBossComponents();
            ResetBossStateTree();
        }
    }

    void ABossManager::OpenDoor()
    {
        // 문 열기 로직 구현
        UE_LOG(LogTemp, Warning, TEXT("Door Opened!"));
    }

    AActor* ABossManager::FindBossInWorld()
    {
        // 월드에서 특정 태그를 가진 액터 찾기
        TArray<AActor*> FoundActors;
        UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Boss"), FoundActors);

        if (FoundActors.Num() > 0)
        {
            return FoundActors[0];
        }

        return nullptr;
    }

    void ABossManager::ResetAllBossComponents()
    {
        // 보스 컴포넌트 초기화 로직 구현
        UE_LOG(LogTemp, Warning, TEXT("All Boss Components Reset!"));
    }

    void ABossManager::ResetBossStateTree()
    {
        // 보스 StateTree 초기화 로직 구현
        UE_LOG(LogTemp, Warning, TEXT("Boss StateTree Reset!"));
    }

    void ABossManager::OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
    {
        // 플레이어 감지 로직 구현
        UE_LOG(LogTemp, Warning, TEXT("Player Entered the Boss Area!"));
    }
    ```

*   **성능 특성과 최적화 포인트:**

    *   `ABossManager`는 게임 월드에 존재하는 동안 계속 Tick() 함수를 호출하므로, Tick() 함수 내의 로직을 최적화해야 합니다.
    *   `FindBossInWorld()` 함수는 게임 월드의 모든 액터를 검색하므로, 성능에 영향을 줄 수 있습니다.  보스 캐릭터를 직접 참조하거나, 이벤트 시스템을 사용하여 보스 캐릭터를 찾는 것이 좋습니다.
    *   `ResetAllBossComponents()` 함수는 보스의 모든 컴포넌트를 초기화하므로, 초기화 로직을 최적화해야 합니다.

*   **사용 시나리오와 베스트 프랙티스:**

    *   `ABossManager`는 레벨에 하나만 존재해야 합니다.
    *   `ABossManager`는 보스 캐릭터의 스폰 위치를 관리하고, 보스 전투 시작/종료 시 이벤트를 처리하는 데 사용됩니다.
    *   `ABossManager`는 보스 캐릭터의 상태를 초기화하고, 필요에 따라 리셋하는 데 사용됩니다.
    *   `ABossManager`는 보스 캐릭터의 AI를 관리하는 데 사용됩니다.

*   **다른 클래스와의 상호작용:**

    *   `ACBoss`: 보스 캐릭터 자체를 관리합니다.
    *   `ACBossAIC`: 보스의 AI를 관리합니다.
    *   `UBossProjectileComponent`: 보스의 투사체 발사 시스템을 관리합니다.
    *   `UBossEffectManager`: 보스의 시각 효과 및 사운드 효과를 관리합니다.
    *   `CBossMovementComponent`: 보스의 이동 로직을 관리합니다.
    *   `CBossStatusComponent`: 보스의 스탯을 관리합니다.

#### 2.2. `ACBoss`

*   **목적과 책임:**
    *   보스 캐릭터의 기본적인 속성, 동작, 그리고 게임플레이 로직을 정의합니다.
    *   데미지 처리, 애니메이션 재생, UI 업데이트, 배경음악 재생 등을 담당합니다.
    *   `UBossAnimInstance`와 함께 애니메이션을 제어합니다.
    *   `ACBossWeapon`을 통해 공격 판정을 처리합니다.
    *   `UBossStatusWidget`을 통해 보스 상태 UI를 업데이트합니다.

*   **주요 메서드와 프로퍼티 상세 분석:**

    *   **`ACBoss()`**: 생성자. 보스 캐릭터의 기본 설정을 초기화합니다.
    *   **`Tick(float DeltaTime)`**: 매 프레임 호출됩니다. 보스 캐릭터의 상태를 업데이트하고, 필요한 로직을 실행합니다.
    *   **`TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)`**: 데미지를 처리합니다.  데미지 종류에 따라 다른 로직을 실행할 수 있습니다.
    *   **`BeginPlay()`**: 게임 시작 시 호출됩니다. 보스 캐릭터의 초기 위치를 저장하고, UI를 표시합니다.
    *   **`PlayHitMotion()`**: 피격 시 애니메이션을 재생합니다.
    *   **`ShowBossStatusWidget()`**: 보스 상태 UI를 표시합니다.
    *   **`HPUpdate()`**: 보스 HP를 업데이트하고, UI를 업데이트합니다.
    *   **`RestartUI()`**: UI를 재시작합니다.
    *   **`PlayBossBGM()`**: 보스 배경음악을 재생합니다.
    *   **`StopBossBGM()`**: 보스 배경음악을 정지합니다.
    *   **`LowerBossBGMVolume()`**: 보스 배경음악 볼륨을 감소시킵니다(사망 시).

*   **클래스의 생명주기와 상태 관리:**

    *   `ACBoss`는 `ABossManager`에 의해 스폰되고, HP가 0이 되면 소멸됩니다.
    *   보스의 상태는 `CBossStatusComponent`를 통해 관리됩니다.
    *   `TakeDamage()` 함수를 통해 보스의 HP를 감소시킬 수 있습니다.

*   **실제 사용 예제와 코드:**

    ```c++
    // CBoss.h
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

    protected:
        // Called when the game starts or when spawned
        virtual void BeginPlay() override;

    public:
        // Called every frame
        virtual void Tick(float DeltaTime) override;

        // 데미지 처리 함수
        virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

        // 피격 모션 재생
        UFUNCTION(BlueprintCallable, Category = "Boss")
        void PlayHitMotion();

        // 보스 상태 위젯 표시
        UFUNCTION(BlueprintCallable, Category = "Boss")
        void ShowBossStatusWidget();

        // HP 업데이트
        UFUNCTION(BlueprintCallable, Category = "Boss")
        void HPUpdate();

        // UI 재시작
        UFUNCTION(BlueprintCallable, Category = "Boss")
        void RestartUI();

        // 보스 배경음악 재생
        UFUNCTION(BlueprintCallable, Category = "Boss")
        void PlayBossBGM();

        // 보스 배경음악 정지
        UFUNCTION(BlueprintCallable, Category = "Boss")
        void StopBossBGM();

        // 보스 배경음악 볼륨 감소 (죽음 시)
        UFUNCTION(BlueprintCallable, Category = "Boss")
        void LowerBossBGMVolume();

    };

    // CBoss.cpp
    #include "CBoss.h"
    #include "Components/SkeletalMeshComponent.h"
    #include "Components/CapsuleComponent.h"
    #include "GameFramework/CharacterMovementComponent.h"
    #include "Kismet/GameplayStatics.h"
    #include "UBossStatusWidget.h"

    ACBoss::ACBoss()
    {
        // Set size for collision capsule
        GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

        // Set our turn rates for input
        BaseTurnRate = 45.f;
        BaseLookUpRate = 45.f;

        // Don't rotate when the controller rotates. Let that just affect the camera.
        bUseControllerRotationPitch = false;
        bUseControllerRotationYaw = false;
        bUseControllerRotationRoll = false;

        // Configure character movement
        GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
        GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
        GetCharacterMovement()->JumpZVelocity = 600.f;
        GetCharacterMovement()->AirControl = 0.2f;
    }

    void ACBoss::BeginPlay()
    {
        Super::BeginPlay();

        // 초기 위치 저장
        InitialLocation = GetActorLocation();

        // 보스 상태 위젯 표시
        ShowBossStatusWidget();

        // 보스 배경음악 재생
        PlayBossBGM();
    }

    void ACBoss::Tick(float DeltaTime)
    {
        Super::Tick(DeltaTime);
    }

    float ACBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
    {
        // 데미지 처리 로직 구현
        UE_LOG(LogTemp, Warning, TEXT("Boss Took Damage: %f"), DamageAmount);

        // HP 감소
        // ...

        // HP 업데이트
        HPUpdate();

        // 피격 모션 재생
        PlayHitMotion();

        return DamageAmount;
    }

    void ACBoss::PlayHitMotion()
    {
        // 피격 모션 재생 로직 구현
        UE_LOG(LogTemp, Warning, TEXT("Boss Played Hit Motion"));
    }

    void ACBoss::ShowBossStatusWidget()
    {
        // 보스 상태 위젯 표시 로직 구현
        UE_LOG(LogTemp, Warning, TEXT("Boss Status Widget Shown"));

        // 위젯 생성 및 화면에 추가
        if (BossStatusWidgetClass)
        {
            BossStatusWidget = CreateWidget<UBossStatusWidget>(GetWorld(), BossStatusWidgetClass);
            if (BossStatusWidget)
            {
                BossStatusWidget->AddToViewport();
            }
        }
    }

    void ACBoss::HPUpdate()
    {
        // HP 업데이트 로직 구현
        UE_LOG(LogTemp, Warning, TEXT("Boss HP Updated"));

        // 위젯에 HP 업데이트
        if (BossStatusWidget)
        {
            BossStatusWidget->UpdateBossHP();
        }
    }

    void ACBoss::RestartUI()
    {
        // UI 재시작 로직 구현
        UE_LOG(LogTemp, Warning, TEXT("Boss UI Restarted"));
    }

    void ACBoss::PlayBossBGM()
    {
        // 보스 배경음악 재생 로직 구현
        UE_LOG(LogTemp, Warning, TEXT("Boss BGM Played"));
    }

    void ACBoss::StopBossBGM()
    {
        // 보스 배경음악 정지 로직 구현
        UE_LOG(LogTemp, Warning, TEXT("Boss BGM Stopped"));
    }

    void ACBoss::LowerBossBGMVolume()
    {
        // 보스 배경음악 볼륨 감소 로직 구현
        UE_LOG(LogTemp, Warning, TEXT("Boss BGM Volume Lowered"));
    }
    ```

*   **성능 특성과 최적화 포인트:**

    *   `ACBoss`는 게임 월드에 존재하는 동안 계속 Tick() 함수를 호출하므로, Tick() 함수 내의 로직을 최적화해야 합니다.
    *   `TakeDamage()` 함수는 데미지 처리 로직을 수행하므로, 데미지 계산 및 상태 업데이트 로직을 최적화해야 합니다.
    *   `ShowBossStatusWidget()` 함수는 UI를 생성하고 표시하므로, UI 생성 비용을 줄이기 위해 UI를 미리 생성해두거나, 객체 풀링을 사용하는 것이 좋습니다.

*   **사용 시나리오와 베스트 프랙티스:**

    *   `ACBoss`는 `ABossManager`에 의해 스폰되고, 게임 월드에 하나만 존재해야 합니다.
    *   `ACBoss`는 보스 캐릭터의 기본적인 속성, 동작, 그리고 게임플레이 로직을 정의하는 데 사용됩니다.
    *   `ACBoss`는 데미지 처리, 애니메이션 재생, UI 업데이트, 배경음악 재생 등을 담당합니다.

*   **다른 클래스와의 상호작용:**

    *   `UBossAnimInstance`: 보스 캐릭터의 애니메이션을 제어합니다.
    *   `ACBossWeapon`: 보스 캐릭터의 공격 판정을 처리합니다.
    *   `UBossStatusWidget`: 보스 캐릭터의 상태 UI를 업데이트합니다.
    *   `CBossStatusComponent`: 보스 캐릭터의 스탯을 관리합니다.
    *   `ACBossAIC`: 보스 캐릭터의 AI를 관리합니다.

#### 2.3. `ACBossAIC`

*   **목적과 책임:**
    *   보스 캐릭터의 인공지능(AI)을 담당합니다.
    *   `StateTree`를 사용하여 보스의 행동을 결정합니다.
    *   `UCBossEnemyStateTreeEvaluator`와 `UTask_*` 클래스들과 협력하여 보스의 행동을 제어합니다.

*   **주요 메서드와 프로퍼티 상세 분석:**

    *   **`ACBossAIC()`**: 생성자. AI 컨트롤러의 기본 설정을 초기화합니다.
    *   **`OnPossess(APawn* InPawn)`**: Pawn을 소유할 때 호출됩니다.  `StateTree`를 시작하고, 필요한 데이터를 초기화합니다.

*   **클래스의 생명주기와 상태 관리:**

    *   `ACBossAIC`는 `ACBoss` 캐릭터가 스폰될 때 함께 생성됩니다.
    *   AI의 상태는 `StateTree`를 통해 관리됩니다.

*   **실제 사용 예제와 코드:**

    ```c++
    // CBossAIC.h
    #include "CoreMinimal.h"
    #include "AIController.h"
    #include "CBossAIC.generated.h"

    UCLASS()
    class MYPROJECT_API ACBossAIC : public AAIController
    {
        GENERATED_BODY()

    public:
        // Sets default values for this actor's properties
        ACBossAIC();

    protected:
        // Called when the game starts or when spawned
        virtual void BeginPlay() override;

    public:
        // Called when the AI possesses a pawn
        virtual void OnPossess(APawn* InPawn) override;

    };

    // CBossAIC.cpp
    #include "CBossAIC.h"
    #include "BehaviorTree/BlackboardComponent.h"
    #include "BehaviorTree/BehaviorTreeComponent.h"
    #include "BehaviorTree/BehaviorTree.h"
    #include "CBoss.h"

    ACBossAIC::ACBossAIC()
    {
        // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
        PrimaryActorTick.bCanEverTick = true;

        // Blackboard와 BehaviorTree 컴포넌트 생성
        BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
        BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
    }

    void ACBossAIC::BeginPlay()
    {
        Super::BeginPlay();
    }

    void ACBossAIC::OnPossess(APawn* InPawn)
    {
        Super::OnPossess(InPawn);

        // 소유한 Pawn이 보스 캐릭터인지 확인
        ACBoss* BossPawn = Cast<ACBoss>(InPawn);
        if (BossPawn)
        {
            // 보스 캐릭터의 BehaviorTree를 가져옴
            UBehaviorTree* BehaviorTree = BossPawn->GetBehaviorTree();

            if (BehaviorTree)
            {
                // Blackboard 초기화
                if (BlackboardComponent)
                {
                    BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
                }

                // BehaviorTree 실행
                BehaviorTreeComponent->StartTree(*BehaviorTree);
            }
        }
    }
    ```

*   **성능 특성과 최적화 포인트:**

    *   `ACBossAIC`는 AI 로직을 수행하므로, AI 연산 비용을 줄이기 위해 `StateTree`를 최적화해야 합니다.
    *   `StateTree`의 조건 검사 및 태스크 실행 빈도를 줄이는 것이 좋습니다.
    *   복잡한 AI 연산은 비동기적으로 처리하거나, LOD(Level of Detail)를 사용하여 AI 연산 비용을 줄일 수 있습니다.

*   **사용 시나리오와 베스트 프랙티스:**

    *   `ACBossAIC`는 `ACBoss` 캐릭터에 부착되어 보스의 AI를 제어합니다.
    *   `ACBossAIC`는 `StateTree`를 사용하여 보스의 행동을 결정합니다.
    *   `ACBossAIC`는 `UCBossEnemyStateTreeEvaluator`와 `UTask_*` 클래스들과 협력하여 보스의 행동을 제어합니다.

*   **다른 클래스와의 상호작용:**

    *   `ACBoss`: 보스 캐릭터 자체를 관리합니다.
    *   `UCBossEnemyStateTreeEvaluator`: `StateTree`의 조건을 평가합니다.
    *   `UTask_*`: `StateTree`에서 사용되는 태스크 클래스들입니다.

#### 2.4. `UBossProjectileComponent`

*   **목적과 책임:**
    *   보스 몬스터의 투사체 시스템을 관리합니다.
    *   투사체 객체 풀을 관리하여 투사체 생성 및 소멸 비용을 줄입니다.
    *   다양한 투사체(일반 투사체, 오브, 바빌론의 문, 성검 마법, 번개 창)를 발사합니다.
    *   투사체 발사 위치, 방향, 속도 등을 제어합니다.

*   **주요 메서드와 프로퍼티 상세 분석:**

    *   **`UBossProjectileComponent()`**: 생성자. 투사체 시스템의 기본 설정을 초기화합니다.
    *   **`ShotProjectile()`**: 일반 투사체를 발사합니다.
    *   **`SpawnOrb()`**: 오브를 생성합니다.
    *   **`ShotProjectileToLocation()`**: 특정 위치로 투사체를 발사합니다.
    *   **`DestroyOrb()`**: 오브를 소멸시킵니다.
    *   **`SpawnOrbContinuously()`**: 오브를 연속적으로 생성합니다.
    *   **`CancelOrbContinuousSpawning()`**: 오브 연속 생성
        을 취소합니다.
    *   **`SpawnProjectileContinuously()`**: 투사체를 연속적으로 생성합니다.
    *   **`CancelProjectileContinuousSpawning()`**: 투사체 연속 생성을 취소합니다.
    *   **`SpawnHolySwordMagicRepeatedly()`**: 성검 마법을 반복적으로 생성합니다.
    *   **`CancelHolySwordMagicSpawning()`**: 성검 마법 생성을 취소합니다.
    *   **`ResetProjectileSystem()`**: 투사체 시스템을 완전히 초기화합니다.
    *   **`GetBossProjectileFromPool()`**: 풀에서 투사체를 가져옵니다.
    *   **`ReturnGateToPool()`**: 바빌론의 문을 풀로 반환합니다.
    *   **`ReturnOrbToPool()`**: 오브를 풀로 반환합니다.
    *   **`ReturnProjectileToPool()`**: 투사체를 풀로 반환합니다.
    *   **`ReturnHolySwordMagicToPool()`**: 성검 마법을 풀로 반환합니다.
    *   **`ReturnBossProjectileToPool()`**: 보스 투사체를 풀로 반환합니다.
    *   **`SetRectangleRange()`**: 사각형 범위를 설정합니다.
    *   **`ToggleRectangleRange()`**: 사각형 범위를 토글합니다.
    *   **`TestRectangleRange()`**: 사각형 범위를 테스트합니다.
    *   **`SpawnMagicCirclesAtCirclePositions()`**: 원 위치에 마법진을 생성합니다.
    *   **`SpawnHolySwordMagicAtLocation()`**: 특정 위치에 성검 마법을 생성합니다.
    *   **`SpawnHolySwordMagicAtCurrentPlayerLocation()`**: 현재 플레이어 위치에 성검 마법을 생성합니다.
    *   **`SpawnSingleOrb()`**: 단일 오브를 생성합니다.
    *   **`SpawnSingleProjectile()`**: 단일 투사체를 생성합니다.
    *   **`InitializeObjectPool()`**: 객체 풀을 초기화합니다.
    *   **`GetGateFromPool()`**: 풀에서 바빌론의 문을 가져옵니다.
    *   **`GetOrbFromPool()`**: 풀에서 오브를 가져옵니다.
    *   **`GetProjectileFromPool()`**: 풀에서 투사체를 가져옵니다.
    *   **`GetHolySwordMagicFromPool()`**: 풀에서 성검 마법을 가져옵니다.
    *   **`DrawRectangleRangeDebug()`**: 사각형 범위 디버그를 그립니다.
    *   **`BeginPlay()`**: 게임 시작 시 호출됩니다. 투사체 시스템을 초기화합니다.
    *   **`TickComponent()`**: 매 프레임 호출됩니다. 투사체 시스템을 업데이트합니다.

*   **클래스의 생명주기와 상태 관리:**

    *   `UBossProjectileComponent`는 `ACBoss` 캐릭터가 스폰될 때 함께 생성됩니다.
    *   투사체는 객체 풀에서 관리됩니다.
    *   투사체는 발사될 때 활성화되고, 소멸될 때 풀로 반환됩니다.

*   **실제 사용 예제와 코드:**

    ```c++
    // BossProjectileComponent.h
    #include "CoreMinimal.h"
    #include "Components/ActorComponent.h"
    #include "BossProjectileComponent.generated.h"

    UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
    class MYPROJECT_API UBossProjectileComponent : public UActorComponent
    {
        GENERATED_BODY()

    public:
        // Sets default values for this component's properties
        UBossProjectileComponent();

    protected:
        // Called when the game starts
        virtual void BeginPlay() override;

    public:
        // Called every frame
        virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

        // 투사체 발사
        UFUNCTION(BlueprintCallable, Category = "Projectile")
        void ShotProjectile();

        // 오브 생성
        UFUNCTION(BlueprintCallable, Category = "Projectile")
        void SpawnOrb();

    private:
        // 투사체 풀
        TArray<AActor*> ProjectilePool;

        // 풀에서 투사체 가져오기
        AActor* GetProjectileFromPool();

        // 투사체를 풀로 반환
        void ReturnProjectileToPool(AActor* Projectile);

        // 객체 풀 초기화
        void InitializeObjectPool();
    };

    // BossProjectileComponent.cpp
    #include "BossProjectileComponent.h"
    #include "GameFramework/Actor.h"

    // Sets default values for this component's properties
    UBossProjectileComponent::UBossProjectileComponent()
    {
        // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
        // off to improve performance if you don't need them.
        PrimaryComponentTick.bCanEverTick = true;

        // ...
    }

    // Called when the game starts
    void UBossProjectileComponent::BeginPlay()
    {
        Super::BeginPlay();

        // 객체 풀 초기화
        InitializeObjectPool();
    }

    // Called every frame
    void UBossProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
    {
        Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

        // ...
    }

    // 투사체 발사
    void UBossProjectileComponent::ShotProjectile()
    {
        // 풀에서 투사체 가져오기
        AActor* Projectile = GetProjectileFromPool();

        if (Projectile)
        {
            // 투사체 발사 로직 구현
            UE_LOG(LogTemp, Warning, TEXT("Projectile Shot!"));
        }
    }

    // 오브 생성
    void UBossProjectileComponent::SpawnOrb()
    {
        // 오브 생성 로직 구현
        UE_LOG(LogTemp, Warning, TEXT("Orb Spawned!"));
    }

    // 풀에서 투사체 가져오기
    AActor* UBossProjectileComponent::GetProjectileFromPool()
    {
        // 풀에서 사용 가능한 투사체 찾기
        for (AActor* Projectile : ProjectilePool)
        {
            if (!Projectile->IsActive())
            {
                Projectile->SetActorHiddenInGame(false);
                Projectile->SetActorEnableCollision(true);
                Projectile->SetActive(true);
                return Projectile;
            }
        }

        // 풀에 사용 가능한 투사체가 없으면 null 반환
        return nullptr;
    }

    // 투사체를 풀로 반환
    void UBossProjectileComponent::ReturnProjectileToPool(AActor* Projectile)
    {
        // 투사체 비활성화
        Projectile->SetActorHiddenIn