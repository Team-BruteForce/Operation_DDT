# C++ 보스 AI 프로젝트

이 프로젝트는 StateTree를 사용하여 보스 캐릭터의 AI를 구현합니다. 다양한 조건에 따라 보스의 행동을 제어하고, 애니메이션과의 연동을 통해 생동감 있는 전투를 연출합니다. 거리, 공격 범위 등을 고려한 정교한 AI 로직을 통해 흥미로운 게임플레이를 제공합니다.


## 주요 클래스

### `ACBoss`

보스 캐릭터의 핵심 클래스입니다. 공격 애니메이션 몽타주 맵을 관리하고, 게임 시작 시 초기화 작업을 수행합니다.

```cpp
// ACBoss.h
```

* **`ACBoss()` (생성자):**  객체 생성 및 초기화를 담당합니다.
* **`AttackTest()`:** 공격 애니메이션을 재생하는 테스트 함수입니다. 디버깅 및 애니메이션 확인에 유용합니다.
* **`BeginPlay()`:** 게임 시작 또는 스폰 시 호출됩니다. 필요한 초기 설정을 수행합니다.


### `ACBossAIC`

보스 캐릭터의 AI 컨트롤러 클래스입니다. Pawn을 소유하게 되면 AI 로직이 시작됩니다.

```cpp
// ACBossAIC.h
```

* **`OnPossess(APawn* InPawn)`:** Pawn 소유 시 호출됩니다.  보스 AI의 시작점입니다.


### `UCBossEndAttack`

보스 공격 종료 애니메이션 노티파이 클래스입니다.

```cpp
// UCBossEndAttack.h
```

* **`GetNotifyName_Implementation()`:**
* **`Notify()`:**


### `UCBossEnemyStateTreeEvaluator`

AI 컨트롤러의 소유자이며, StateTree의 핵심 로직을 담당합니다. 매 프레임마다 상태를 평가하고 적절한 행동을 결정합니다.

```cpp
// UCBossEnemyStateTreeEvaluator.h
```

* **`Tick()`:** 매 프레임 호출됩니다. StateTree의 상태를 업데이트하고, 조건에 따라 상태 전환을 수행합니다.
* **`TreeStart()`:** StateTree 시작 시 호출됩니다.
* **`Get_Decision_Data()`:** 의사결정에 필요한 데이터를 수집합니다.  예를 들어 플레이어와의 거리, 현재 상태 등을 가져올 수 있습니다.
* **`EvaluateDistanceState()`:**  플레이어와의 거리에 따라 StateTree의 태그를 설정합니다. 이를 통해 거리에 따른 다양한 행동을 구현할 수 있습니다.


### `UCEnemyAnimInstance`

적 캐릭터의 애니메이션 인스턴스 클래스입니다.

```cpp
// UCEnemyAnimInstance.h
```


## StateTree 조건 클래스

다음 클래스들은 StateTree에서 사용되는 조건들을 정의합니다. 각 클래스는 `TestCondition` 함수를 통해 조건의 참/거짓을 반환합니다.

* **`USTC_IsBossActionInProgress`**: 보스가 현재 공격 중인지 확인합니다.
* **`USTC_IsBossDistanceGreaterThan`**: 보스와 플레이어 사이의 거리가 특정 값보다 큰지 확인합니다.
* **`USTC_IsBossDistanceLessThan`**: 보스와 플레이어 사이의 거리가 특정 값보다 작은지 확인합니다.
* **`USTC_IsBossInDashRange`**: 보스가 돌진 공격 범위 안에 있는지 확인합니다.
* **`USTC_IsBossInMeleeRange`**: 보스가 근접 공격 범위 안에 있는지 확인합니다.
* **`USTC_IsBossInRangedRange`**: 보스가 원거리 공격 범위 안에 있는지 확인합니다.


## StateTree 태스크 클래스

다음 클래스들은 StateTree에서 실행되는 태스크들을 정의합니다.

* **`UTask_Action`**: 특정 액션을 실행하는 태스크입니다.
* **`UTask_Log`**: 로그를 출력하는 태스크입니다. 디버깅에 유용합니다.
* **`UTask_PlayMontage`**: 애니메이션 몽타주를 재생하는 태스크입니다.
* **`UTask_RotateTowardsPlayer`**: 보스를 플레이어 방향으로 회전시키는 태스크입니다.
* **`UTask_SwitchState`**: StateTree의 상태를 전환하는 태스크입니다.


## 추가 설명

* `.cpp` 파일들은 각 클래스의 구현을 담고 있습니다.
* `.h` 파일들은 각 클래스의 선언을 담고 있습니다.
* `BossStateComponent`는 보스의 상태를 관리하는 컴포넌트입니다.
* `CBossMovementComponent`는 보스의 이동을 제어하는 컴포넌트입니다.


이 README 파일은 프로젝트의 전반적인 구조를 이해하는 데 도움이 되도록 작성되었습니다.  더 자세한 내용은 각 클래스의 소스 코드를 참조하십시오.  궁금한 점이나 제안 사항이 있으면 언제든지 이슈를 남겨주세요.
