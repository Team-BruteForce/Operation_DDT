# Operation_DDT 프로젝트 소개

이 프로젝트는 Unreal Engine에서 StateTree 기반 AI를 활용한 적 캐릭터의 순찰 기능을 구현하는 것을 목표로 합니다. `AAEnemy` 클래스를 중심으로 적 캐릭터의 이동 및 행동 로직이 구현되어 있으며, `UCPatrolTask`를 통해 순찰 동작을 정의합니다.  다양한 헬퍼 클래스와 인터페이스를 통해 코드의 재사용성과 확장성을 높였습니다.

## 주요 클래스 설명

### `AAEnemy`

캐릭터의 실제 이동을 처리하는 커스텀 무브먼트 컴포넌트입니다.

```cpp
UCLASS()
class AAEnemy : public ACharacter
{
    GENERATED_BODY()
public:
    AAEnemy();

    UFUNCTION(BlueprintCallable)
    AAPatrolPath* GetPatrolPath() const;

protected:
    virtual void BeginPlay() override;

    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// ... (생략)
};
```

- `AAEnemy()`: AAEnemy의 생성자입니다.
- `GetPatrolPath()`: 적이 순찰할 경로(`AAPatrolPath`)를 반환합니다.
- `BeginPlay()`: 게임 시작 시 초기화 로직을 수행합니다.
- `SetupPlayerInputComponent()`: 입력 컴포넌트를 설정합니다.


### `AAPatrolPath`

적 캐릭터의 순찰 경로를 정의하는 클래스입니다.


### `ACEnemyController`

StateTree AI 로직을 담당하는 컴포넌트입니다.

```cpp
UCLASS()
class ACEnemyController : public AAIController
{
    GENERATED_BODY()
public:
    ACEnemyController();

protected:
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable)
    void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result);

    virtual void OnPossess(APawn* InPawn) override;

// ... (생략)
};
```

- `ACEnemyController()`: ACEnemyController의 생성자입니다.
- `BeginPlay()`: 게임 시작 시 초기화 로직을 수행합니다.
- `OnMoveCompleted()`: 이동이 완료되었을 때 호출되는 콜백 함수입니다.
- `OnPossess()`: 폰에 빙의하고 필요한 컴포넌트들을 연결하는 초기화 작업을 수행합니다.

### `CHelpers`

다양한 유틸리티 함수를 제공하는 헬퍼 클래스입니다.  `CreateComponent`, `GetAsset`, `FindActor` 등의 함수를 통해 개발 편의성을 높입니다.

### `CLog`

로그 출력을 위한 클래스입니다. 다양한 `Log` 및 `Print` 함수를 제공합니다.

### `UCPatrolTask`

StateTree에서 사용되는 순찰 태스크입니다.

```cpp
UCLASS()
class UCPatrolTask : public UBTTaskNode
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, Category = "Patrol")
    AActor* Enemy;  // 순찰을 수행할 적 캐릭터

protected:
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void EnterState(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
    virtual void Tick(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

// ... (생략)
};
```

- `Enemy`: 이 태스크가 제어할 적 액터(Enemy)입니다. StateTree 에디터에서 Context Actor로 바인딩해야 합니다.
- `EnterState()`: 태스크가 시작될 때 한 번 호출되어 순찰 행동을 개시합니다.
- `Tick()`: 태스크가 활성화된 동안 매 틱 호출됩니다. 이동 완료 여부 등을 확인할 수 있습니다.


## 인터페이스 설명

### `IEnemyInterface` & `UEnemyInterface`

적 캐릭터 관련 기능을 위한 인터페이스입니다. `GetPatrolPath` 함수를 통해 순찰 경로 정보를 얻을 수 있습니다.

### `IIAIController` & `UIAIController`

AI 컨트롤러 관련 기능을 위한 인터페이스입니다. `Patrol` 함수를 통해 순찰 동작을 실행할 수 있습니다.



## 추가 설명

이 README 파일은 Doxygen에서 추출된 데이터를 기반으로 작성되었으며, 추가적인 정보는 프로젝트의 소스 코드를 참조하시기 바랍니다.  본 프로젝트는 계속해서 개발 중이며, 기능 개선 및 버그 수정이 이루어질 예정입니다.


## 라이선스

[라이선스 정보를 입력하세요]
