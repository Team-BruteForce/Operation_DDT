# 언리얼 엔진 보스 AI 프로젝트

이 프로젝트는 언리얼 엔진에서 보스 캐릭터의 AI와 애니메이션을 구현하는 것을 목표로 합니다. StateTree를 활용한 AI 로직과 다양한 애니메이션, 그리고 에디터 플러그인을 통해 보스 캐릭터를 효과적으로 제어하고 데이터를 관리할 수 있도록 설계되었습니다.

## 주요 기능

* StateTree 기반 AI: 보스의 행동 패턴을 StateTree를 사용하여 구현하여 유연하고 확장 가능한 AI 시스템을 제공합니다.
* 다양한 공격 애니메이션: 근접 공격, 원거리 공격, 대쉬 공격 등 다양한 공격 애니메이션을 지원합니다.
* 데이터 동기화 플러그인: 언리얼 에디터 플러그인을 통해 게임플레이 태그 등의 데이터를 외부와 동기화할 수 있습니다.
* 유연한 무기 및 장비 시스템: 보스가 다양한 무기와 장비를 사용하고 교체할 수 있도록 설계되었습니다.

## 클래스 및 함수 설명

### 핵심 클래스

* **`ACBoss`**: 보스 캐릭터의 핵심 로직을 담당하는 클래스입니다. 공격 애니메이션을 관리하고, 게임 시작 시 초기화 작업을 수행합니다.

```cpp
// ACBoss 주요 함수
void AttackTest(); // 공격 애니메이션 재생 테스트 함수
void BeginPlay(); // 게임 시작 또는 스폰 시 호출되는 함수
```

* **`ACBossAIC`**: 보스 캐릭터의 AI를 제어하는 컨트롤러 클래스입니다. StateTree를 사용하여 보스의 행동을 결정합니다.

```cpp
// ACBossAIC 주요 함수
void OnPossess(APawn* InPawn); // Pawn 소유 시 호출되는 함수
```

* **`UCEnemyAnimInstance`**: 적 캐릭터의 애니메이션 인스턴스 클래스입니다. 애니메이션 블렌딩 및 상태 머신 관리를 담당합니다.

### AI 관련 클래스

* **`UCBossEnemyStateTreeEvaluator`**: AI 컨트롤러 소유자로서, StateTree의 평가를 담당합니다. 매 프레임마다 상태를 업데이트하고 의사 결정에 필요한 데이터를 제공합니다.

```cpp
// UCBossEnemyStateTreeEvaluator 주요 함수
void Tick(float DeltaTime); // 매 프레임 호출되는 틱 함수
void TreeStart(); // StateTree 시작 시 호출되는 함수
FBossBTDecisionData Get_Decision_Data(); // 의사결정에 필요한 데이터를 수집하는 함수
void EvaluateDistanceState(); // 거리에 따른 태그를 설정하는 함수
```

* **`USTC_IsBoss*` 시리즈**: StateTree에서 사용되는 조건들을 정의하는 클래스입니다. 각 클래스는 특정 조건을 검사하는 `TestCondition` 함수를 포함합니다. 예를 들어 `USTC_IsBossInMeleeRange`는 보스가 근접 공격 범위 내에 있는지 확인합니다.

```cpp
// USTC_* 주요 함수
bool TestCondition(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory); // 조건을 테스트하는 함수
```

* **`UTask_*` 시리즈**: StateTree에서 사용되는 태스크들을 정의하는 클래스입니다. 각 클래스는 특정 행동을 수행하는 `EnterState` 및 `Tick` 함수를 포함합니다. 예를 들어 `UTask_PlayMontage`는 몽타주 애니메이션을 재생합니다.

```cpp
// UTask_PlayMontage 주요 함수
void EnterState(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory); // 상태 진입 시 호출되는 함수
void Tick(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds); // 상태 실행 중 매 프레임 호출되는 함수
```


### 무기 및 장비 관련 클래스

* **`ACBossWeapon`**: 보스가 사용하는 무기를 나타내는 클래스입니다. 무기 장착, 해제, 충돌 처리 등의 기능을 제공합니다.

* **`UCBossEquipment`**: 보스가 장착할 수 있는 장비를 나타내는 클래스입니다.

* **`UCBossWeaponAsset`**:  무기 및 장비와 관련된 에셋을 관리하는 클래스입니다.

* **`UCBossDoAction`**: 보스의 행동을 정의하는 클래스입니다. 공격, 이동 등 다양한 행동을 구현할 수 있습니다.

### 에디터 플러그인 관련 클래스

* **`FEditorPlugin_DataSyncModule`**: 데이터 동기화 플러그인의 핵심 모듈입니다. 플러그인 시작, 종료, 버튼 클릭 이벤트 처리 등을 담당합니다.

* **`FEditorPlugin_DataSyncCommands`**: 데이터 동기화 플러그인의 명령어를 정의하는 클래스입니다.

* **`FEditorPlugin_DataSyncStyle`**: 데이터 동기화 플러그인의 스타일을 정의하는 클래스입니다.


## 향후 개발 계획

* 더욱 다양한 보스 행동 패턴 추가
* 성능 최적화
* 에디터 플러그인 기능 확장


이 README 파일은 프로젝트의 전반적인 구조와 주요 클래스에 대한 설명을 제공합니다. 각 클래스의 상세한 구현은 소스 코드를 참조하십시오.  궁금한 점이나 제안 사항이 있으면 언제든지 이슈를 등록해주세요.
