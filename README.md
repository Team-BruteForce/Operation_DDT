# 언리얼 엔진 C++ 보스 전투 시스템

![Boss Battle](https://via.placeholder.com/150/0000FF/808080?text=Boss+Image)

## 1. 프로젝트 개요

이 프로젝트는 언리얼 엔진 5를 사용하여 개발된 액션 RPG 게임의 **보스 전투 시스템**을 구현합니다. 플레이어는 다양한 공격 패턴과 페이즈 변화를 가진 보스와 전투를 벌이게 됩니다. 이 시스템은 StateTree 기반 AI, 객체 풀링, 다양한 애니메이션 노티파이, 그리고 커스텀 무기 및 이펙트 시스템을 활용하여 역동적이고 몰입감 있는 전투 경험을 제공합니다. 또한, 에디터 플러그인을 통해 게임플레이 태그 및 보스 스탯 데이터를 외부 API와 동기화하여 데이터 관리의 효율성을 높였습니다.

**주요 기능:**

* 페이즈 변화에 따른 다양한 공격 패턴
* StateTree 기반의 정교한 보스 AI
* 다양한 무기와 마법 공격 (바빌론의 문, 성검 마법, 번개 창 등)
* 화려한 이펙트와 애니메이션
* 객체 풀링을 통한 성능 최적화
* 외부 API 데이터 동기화 기능

**기술적 특징:**

* 언리얼 엔진 5 및 C++ 사용
* StateTree 기반 AI 구현
* 객체 풀링 시스템 구현
* 커스텀 애니메이션 노티파이 시스템
* 데이터 테이블 및 외부 API 연동
* 에디터 플러그인 개발


## 2. 아키텍처 분석

![Architecture Diagram](https://via.placeholder.com/800/FFFF00/000?text=Architecture+Diagram)

**클래스 관계 및 설계 패턴:**

* **`ACBoss`**: 보스 캐릭터의 핵심 클래스. 체력, 데미지 처리, 애니메이션 등을 담당합니다.
* **`ACBossAIC`**: StateTree를 사용하여 보스 AI를 제어합니다. `UCBossEnemyStateTreeEvaluator`를 통해 게임 상태 정보를 StateTree에 전달합니다.
* **`UBossAnimInstance`**: 보스 애니메이션을 관리하고, 다양한 애니메이션 노티파이를 통해 게임 로직과 연동합니다.
* **`UBossProjectileComponent`**: 보스의 투사체 공격을 관리하며, 객체 풀링을 통해 성능을 최적화합니다.
* **`UBossEffectManager`**: 이펙트 생성 및 관리, 객체 풀링을 담당합니다.
* **`UCBossDoAction`**: 보스의 공격 액션을 정의하고 실행합니다.
* **`CBossMovementComponent`**: 보스의 이동 로직을 처리합니다. StateTree Task와 연동하여 복잡한 이동 패턴을 구현합니다.
* **`EditorPlugin_DataSyncModule`**: 외부 API와 데이터 동기화를 담당하는 에디터 플러그인입니다.

**시스템 구조:**

보스 AI는 StateTree를 기반으로 동작하며, 다양한 조건(`USTC_*` 클래스)과 태스크(`UTask_*` 클래스)를 통해 복잡한 행동 패턴을 구현합니다. 애니메이션 노티파이는 애니메이션 이벤트에 따라 특정 게임 로직을 실행하는데 사용됩니다. 예를 들어, `UAnimNotify_SpawnLightningSpear`는 번개 창 공격 애니메이션 재생 시점에 `UBossProjectileComponent`의 `ShotProjectile` 함수를 호출하여 투사체를 생성합니다.


## 3. 핵심 시스템 설명

### 3.1. StateTree 기반 AI

`ACBossAIC`와 `UCBossEnemyStateTreeEvaluator`를 통해 StateTree를 사용하여 보스 AI를 구현했습니다. 다양한 조건 클래스(`USTC_*`)를 통해 보스와 플레이어의 거리, 보스의 현재 상태, 확률 등을 판단하고, 그에 따라 적절한 태스크 클래스(`UTask_*`)를 실행합니다. 이를 통해 페이즈 변화, 공격 패턴 전환, 이동 등 복잡한 AI 로직을 효율적으로 관리할 수 있습니다.

### 3.2. 투사체 시스템 및 객체 풀링

`UBossProjectileComponent`는 보스의 투사체 공격을 관리합니다. `ABossProjectileActor`, `ABossProjectileOrb`, `AProjectile_LightSpear` 등 다양한 투사체 클래스를 사용하며, 객체 풀링을 통해 투사체 생성 및 소멸에 따른 성능 저하를 방지합니다.


## 4. API 문서

### 4.1. `UBossProjectileComponent::ShotProjectile`

```cpp
void UBossProjectileComponent::ShotProjectile(FVector Direction);
```

**설명:** 지정된 방향으로 투사체를 발사합니다.

**매개변수:**

* `Direction`: 투사체 발사 방향 벡터.

**예제:**

```cpp
// 플레이어 위치를 향해 투사체 발사
FVector Direction = (GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
ShotProjectile(Direction);
```


## 5. 기술적 하이라이트

* **StateTree의 효율적인 활용:** 다양한 조건과 태스크를 조합하여 복잡한 보스 AI를 구현했습니다.
* **객체 풀링을 통한 성능 최적화:** 투사체 및 이펙트 관리에 객체 풀링을 적용하여 성능 저하를 최소화했습니다.
* **애니메이션 노티파이 시스템 활용:** 애니메이션 이벤트와 게임 로직을 유연하게 연동했습니다.
* **외부 API 데이터 동기화:** 에디터 플러그인을 통해 게임 데이터 관리의 효율성을 높였습니다.


## 6. 설치 및 실행 가이드

1. 언리얼 엔진 5 프로젝트에 플러그인 폴더를 추가하고, 플러그인 파일들을 복사합니다.
2. 언리얼 엔진 에디터를 실행하고, 플러그인을 활성화합니다.
3. 프로젝트를 빌드하고 실행합니다.


## 7. 개발자 노트

* **StateTree 설계:** StateTree를 사용하여 보스 AI의 유지보수성과 확장성을 높였습니다. 향후 더욱 다양한 공격 패턴과 행동 로직을 추가할 예정입니다.
* **객체 풀링:** 객체 풀링을 통해 성능을 최적화했지만, 풀 크기 관리에 대한 추가적인 개선이 필요할 수 있습니다.
* **데이터 동기화:** 현재는 GameplayTag와 BossStats 데이터만 동기화하지만, 추후 다른 데이터 타입으로 확장할 계획입니다.


이 README 파일은 프로젝트의 핵심적인 부분들을 요약하여 설명하고 있습니다. 자세한 내용은 프로젝트 소스 코드 및 관련 문서를 참조하시기 바랍니다.  🚀
