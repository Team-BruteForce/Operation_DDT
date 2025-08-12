# Operation_DDT 프로젝트

이 프로젝트는 언리얼 엔진 기반으로 개발된 것으로 추정되며, 보스 캐릭터의 AI 및 애니메이션을 구현하는 데 초점을 맞추고 있습니다.  다양한 클래스와 함수를 통해 보스의 행동, 애니메이션, 그리고 게임 로직을 정의합니다.

## 핵심 클래스

### `ACBoss`

* **설명:** 공격 애니메이션 몽타주 맵을 관리하는 보스 캐릭터 클래스입니다.
* **주요 함수:**
    * `ACBoss()`: 보스 객체 생성자입니다.
    * `AttackTest()`: 공격 애니메이션을 재생하는 테스트 함수입니다.
    * `BeginPlay()`: 게임 시작 또는 보스 스폰 시 호출되는 초기화 함수입니다.

```cpp
// ACBoss::AttackTest() 예시
void ACBoss::AttackTest()
{
    // 공격 애니메이션 재생 로직
}
```

### `ACBossAIC`

* **설명:** 보스 캐릭터의 AI를 담당하는 컨트롤러 클래스입니다.
* **주요 함수:**
    * `OnPossess(APawn* InPawn)`: AI 컨트롤러가 Pawn을 소유할 때 호출됩니다.  이 함수 내에서 AI 로직 초기화를 수행할 수 있습니다.

```cpp
// ACBossAIC::OnPossess() 예시
void ACBossAIC::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);
    // AI 초기화 로직
}
```

### `UCEnemyAnimInstance`

* **설명:** 적 캐릭터, 특히 보스의 애니메이션을 관리하는 애니메이션 인스턴스 클래스입니다.


### `UCBossEnemyStateTreeEvaluator`

* **설명:** AI 컨트롤러의 소유자로서, 보스의 상태를 평가하고 행동 결정에 필요한 데이터를 제공합니다.
* **주요 함수:**
    * `Tick()`: 매 프레임 호출되어 보스의 상태를 업데이트합니다.
    * `Get_Decision_Data()`: 의사 결정에 필요한 데이터를 수집하고 반환합니다.

### `USTC_IsBossDistanceGreaterThan` & `USTC_IsBossDistanceLessThan`

* **설명:** 각각 보스와 플레이어 사이의 거리가 특정 값보다 큰지, 작은지를 판단하는 StateTree Condition 클래스입니다.
* **주요 함수:**
    * `TestCondition()`: 조건을 테스트하고 결과를 반환합니다.

### `UTask_Log`

* **설명:** 특정 상태 진입 시 로그를 출력하는 Task 클래스입니다. 디버깅에 유용합니다.
* **주요 함수:**
    * `EnterState()`: 상태 진입 시 호출됩니다.
    * `Tick()`: 상태 실행 중 매 프레임 호출됩니다.

### `UTask_PlayMontage`

* **설명:** 몽타주 애니메이션을 재생하는 Task 클래스입니다.
* **주요 함수:**
    * `EnterState()`: 상태 진입 시 호출됩니다.
    * `Tick()`: 상태 실행 중 매 프레임 호출됩니다.


## 유틸리티 클래스

### `CHelpers`

* **설명:** 다양한 유틸리티 함수를 제공하는 헬퍼 클래스입니다. 컴포넌트 생성, 액터 검색, 이펙트 재생 등의 기능을 포함합니다.

### `CLog`

* **설명:** 로그 출력을 위한 유틸리티 클래스입니다. 다양한 로그 레벨 및 출력 형식을 지원합니다.


## 프로젝트 설정 및 기타 파일

`Operration_DDT`, `Operration_DDTEditorTarget`, `Operration_DDTTarget`, `.cpp`, `.h`, `.cs` 파일들은 프로젝트 설정, 빌드, 엔진 연동 등에 관련된 파일들입니다. 자세한 내용은 각 파일의 코드를 참조하십시오.


## 추가 설명

이 README는 제공된 Doxygen 데이터를 기반으로 작성되었으며, 프로젝트의 전체적인 구조를 이해하는 데 도움을 주기 위한 것입니다.  더 자세한 내용은 프로젝트의 소스 코드와 관련 문서를 참조하십시오.  코드 예시는 이해를 돕기 위한 것이며, 실제 구현과 다를 수 있습니다.
