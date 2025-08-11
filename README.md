# Operation_DDT 프로젝트 소개

이 프로젝트는 언리얼 엔진으로 개발된 게임 프로젝트로, 주요 기능들을 클래스별로 구현하여 효율적인 구조를 갖추고 있습니다. 아래에 각 클래스와 주요 함수에 대한 설명을 제공합니다.

## 핵심 클래스

### 1. `ACBoss` (보스 캐릭터)

* **설명:** 게임 내 보스 캐릭터를 나타내는 클래스입니다.

* **주요 함수:**

    * `ACBoss()`: 보스 객체 생성자입니다.
    * `BeginPlay()`: 게임 시작 또는 보스 스폰 시 호출되는 함수입니다. 초기화 로직을 구현하는데 사용됩니다.

        ```cpp
        void ACBoss::BeginPlay()
        {
            Super::BeginPlay();
            // 초기화 로직
        }
        ```

### 2. `ACBossAIC` (보스 AI 컨트롤러)

* **설명:** 보스 캐릭터의 AI를 담당하는 컨트롤러 클래스입니다.

* **주요 함수:**

    * `OnPossess(APawn* InPawn)`: AI 컨트롤러가 Pawn을 소유할 때 호출됩니다.  소유한 Pawn에 대한 제어를 시작하는 로직을 구현합니다.

        ```cpp
        void ACBossAIC::OnPossess(APawn* InPawn)
        {
            Super::OnPossess(InPawn);
            // Pawn 제어 로직
        }
        ```

### 3. `UCBossEnemyStateTreeEvaluator` (보스 상태 트리 평가자)

* **설명:** 보스 AI의 상태 트리를 평가하고 의사 결정에 필요한 데이터를 제공하는 클래스입니다.

* **주요 함수:**

    * `Tick(float DeltaTime)`: 매 프레임 호출되며, 상태 트리 업데이트 로직을 담당합니다.
    * `Get_Decision_Data()`: 의사 결정에 필요한 데이터를 수집하고 반환합니다.

### 4. `UCEnemyAnimInstance` (적 캐릭터 애니메이션 인스턴스)

* **설명:** 적 캐릭터의 애니메이션을 관리하는 클래스입니다.


### 5. `USTC_IsBossDistanceGreaterThan` (보스와의 거리 조건 - 초과)

* **설명:** 보스와 플레이어 간의 거리가 특정 값보다 큰지 확인하는 상태 트리 조건입니다.

* **주요 함수:**

    * `TestCondition()`: 조건을 테스트하고 결과를 반환합니다.

### 6. `USTC_IsBossDistanceLessThan` (보스와의 거리 조건 - 미만)

* **설명:** 보스와 플레이어 간의 거리가 특정 값보다 작은지 확인하는 상태 트리 조건입니다.

* **주요 함수:**

    * `TestCondition()`: 조건을 테스트하고 결과를 반환합니다.

## 유틸리티 클래스

### 1. `CHelpers` (헬퍼 함수)

* **설명:** 다양한 유틸리티 함수를 제공하는 클래스입니다.  컴포넌트 생성, 에셋 로딩, 액터 찾기 등의 기능을 제공합니다.  자세한 함수 설명은 코드를 참조하세요.

### 2. `CLog` (로그 출력)

* **설명:** 로그 출력을 위한 클래스입니다. 다양한 로그 레벨 및 출력 형식을 지원합니다.


## 프로젝트 설정 및 기타 파일

* `Operration_DDT`, `Operration_DDTEditorTarget`, `Operration_DDTTarget`: 게임 모듈, 에디터 타겟, 게임 타겟 설정 파일입니다.
* `.cpp`, `.h`, `.cs`: C++, C#, 헤더 파일들입니다. 각 클래스의 구현 및 선언을 담고 있습니다.
* `convert_docs.py`: Doxygen 문서를 변환하는 Python 스크립트입니다.


## 추가 설명

본 README는 Doxygen에서 추출된 데이터를 기반으로 작성되었으며,  추후 프로젝트 진행에 따라 업데이트될 예정입니다. 더 자세한 내용은 프로젝트의 소스 코드를 참조하시기 바랍니다.


This project is licensed under the [License Name] license.  (라이선스 명시)
