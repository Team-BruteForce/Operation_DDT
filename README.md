# 보스 AI 및 데이터 동기화 플러그인

이 프로젝트는 언리얼 엔진에서 보스 캐릭터의 AI와 애니메이션을 구현하고, 에디터 플러그인을 통해 게임 데이터를 동기화하는 기능을 제공합니다. StateTree 기반 AI를 사용하여 보스의 행동 패턴을 정의하고, 데이터 동기화 플러그인을 통해 Gameplay Tags 및 보스 스탯을 효율적으로 관리할 수 있습니다.

## 주요 기능

* **StateTree 기반 보스 AI:** 복잡한 보스 행동 패턴을 StateTree를 사용하여 구현하고 관리합니다.
* **데이터 동기화 플러그인:** Gameplay Tags와 보스 스탯 데이터를 외부 API와 동기화하여 효율적인 데이터 관리를 지원합니다.
* **다양한 보스 액션 및 애니메이션:** 공격, 이동, 상태 전환 등 다양한 액션과 애니메이션을 구현하여 보스 캐릭터에 생동감을 부여합니다.
* **모듈화된 코드 구조:** 각 기능별로 클래스와 컴포넌트를 분리하여 코드의 가독성과 유지보수성을 향상시킵니다.


## 클래스 및 주요 함수 설명

### 1. 보스 캐릭터 (`ACBoss`)

* **설명:** 보스 캐릭터를 나타내는 클래스입니다. 공격 애니메이션 몽타주 맵을 관리합니다.

* **주요 함수:**

```cpp
ACBoss(); // 보스 캐릭터 생성자
void AttackTest(); // 공격 애니메이션을 재생하는 테스트 함수
void BeginPlay() override; // 게임 시작 시 호출되는 함수
```

### 2. 보스 AI 컨트롤러 (`ACBossAIC`)

* **설명:** StateTree 기반 AI를 구현하는 컴포넌트입니다.

* **주요 함수:**

```cpp
ACBossAIC(); // 생성자
void OnPossess(APawn* InPawn) override; // Pawn 소유 시 호출되는 함수
```

### 3. 보스 무기 (`ACBossWeapon`)

* **설명:** 보스가 사용하는 무기를 나타내는 클래스입니다. (자세한 설명 필요)

* **주요 함수:**  다양한 무기 관련 함수들이 존재하지만, Doxygen 문서에 상세 설명이 부족합니다.  추후 README 업데이트 시 추가 설명을 제공할 예정입니다.

### 4. 데이터 동기화 플러그인 (`FEditorPlugin_DataSyncModule`)

* **설명:** Gameplay Tags 및 보스 스탯 데이터를 동기화하는 플러그인 모듈입니다.

* **주요 함수:**

```cpp
void StartupModule() override; // 모듈 시작 시 호출되는 함수
void ShutdownModule() override; // 모듈 종료 시 호출되는 함수
void SyncGameplayTags(); // GameplayTags 동기화 함수
void SyncBossStats();  // 보스 스탯 동기화 함수
void MakeAPIRequest(const FString& URL); // HTTP API 요청 함수
void OnDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful); // HTTP 응답 처리 함수
// ... 기타 함수들 (데이터 테이블 업데이트, UI 관련 함수 등)
```

### 5. StateTree 조건 및 태스크

다양한 StateTree 조건(`USTC_*`) 및 태스크(`UTask_*`) 클래스가 제공되어 보스 AI의 행동 패턴을 정의합니다. 각 클래스는 `TestCondition` 함수 또는 `EnterState`, `Tick`, `ExitState` 함수를 통해 보스의 상태를 확인하고 행동을 제어합니다. 예를 들어, `UTask_BossChase`는 보스가 플레이어를 추적하는 행동을 구현합니다.

```cpp
// UTask_BossChase 예시
void Tick(float DeltaTime) override; // 태스크 실행 중 매 프레임 호출
void EnterState() override; // 태스크 시작 시 호출
void ExitState() override; // 태스크 종료 시 호출
```

### 6. 기타 중요 클래스

* `UCBossDoAction`: 보스의 액션을 정의하고 실행하는 클래스입니다.
* `UCEnemyAnimInstance`: 적 캐릭터의 애니메이션을 관리하는 클래스입니다.
* `CBossEnemyStateTreeEvaluator`: StateTree의 매 프레임 실행 로직을 담당하는 클래스입니다.


## 향후 개선 사항

* Doxygen 문서 보완: `ACBossWeapon` 등 설명이 부족한 클래스 및 함수에 대한 자세한 설명 추가.
* 추가 기능 개발: 더욱 다양한 보스 행동 패턴 및 데이터 동기화 기능 추가.
* 성능 최적화: StateTree 및 데이터 동기화 기능의 성능 최적화.


## 기여 방법

프로젝트에 기여하고 싶으신 분들은 이슈를 등록하거나 풀 리퀘스트를 보내주세요.


This README provides a basic overview of the project.  Further documentation will be added as the project evolves.  Please check back for updates.
