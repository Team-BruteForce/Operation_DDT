# 에너미 공격 시스템 (Enemy Attack System)

## 개요
이 시스템은 에너미들이 근접 공격과 원거리 공격을 수행할 수 있도록 하는 기능을 제공합니다. 근접 공격은 콜리전 기반으로, 원거리 공격은 프로젝타일 기반으로 구현되어 있습니다. **모든 공격의 데미지는 에너미의 StatusComponent에서 설정한 공격력(AttackPower)을 사용합니다.**

## 주요 컴포넌트

### 1. CEnemyMeleeAttackComponent
- **위치**: `Public/ODH/ODH_Enemy/Component/CEnemyMeleeAttackComponent.h`
- **기능**: 에너미의 근접 공격을 처리하는 컴포넌트
- **주요 속성**:
  - `AttackCooldown`: 공격 쿨다운 시간 (기본값: 1.0f)
  - `bIsAttackActive`: 공격 활성화 상태
  - `MeleeAttackCollision`: 근접 공격용 콜리전
  - **데미지**: StatusComponent의 `AttackPower` 사용

### 2. UCEnemyProjectileComp (확장)
- **위치**: `Public/ODH/Component/CEnemyProjectileComp.h`
- **기능**: 에너미의 원거리 공격을 처리하는 컴포넌트 (기존 확장)
- **주요 속성**:
  - `ProjectileSpeed`: 프로젝타일 속도 (기본값: 1200.0f)
  - `MaxLifeSeconds`: 프로젝타일 수명 (기본값: 5.0f)
  - `OnProjectileHit`: 프로젝타일 히트 이벤트
  - **데미지**: StatusComponent의 `AttackPower` 사용

## 구현된 에너미 클래스들

### 1. CFlyingSkull (공격형)
- **체력**: 80 HP
- **공격력**: 35
- **방어력**: 3
- **특징**: 체력이 낮지만 공격력이 높음 (유리한 유리)
- `PlayMeleeAttack()`: 근접 공격 실행
- `PlayRangedAttack()`: 원거리 공격 실행

### 2. CSkeletonBodyEnemy (밸런스형)
- **체력**: 150 HP
- **공격력**: 25
- **방어력**: 8
- **특징**: 체력과 공격력이 균형잡힌 일반형
- `PlayMeleeAttack()`: 근접 공격 실행
- `PlayRangedAttack()`: 원거리 공격 실행

### 3. CSperatedEnemy (탱커형)
- **체력**: 200 HP
- **공격력**: 15
- **방어력**: 12
- **특징**: 체력이 매우 높고 공격력이 낮음 (탱커 역할)
- `PlayMeleeAttack()`: 근접 공격 실행
- `PlayRangedAttack()`: 원거리 공격 실행

## 데미지 시스템

### 통합된 데미지 관리
- **모든 공격 데미지**: `CEnemyStatusComponent`의 `AttackPower` 값을 사용
- **데미지 계산**: 각 공격 컴포넌트가 자동으로 StatusComponent에서 공격력을 가져옴
- **일관성**: 모든 에너미의 공격력이 중앙에서 관리되어 밸런싱이 용이

### 에너미별 스테이터스 설정
각 에너미는 `BeginPlay()`에서 개별적으로 스테이터스를 설정합니다:

```cpp
// CFlyingSkull - 공격형
StatusComponent->SetMaxHealth(80.0f);
StatusComponent->SetCurrentHealth(80.0f);
StatusComponent->SetAttackPower(35.0f);
StatusComponent->SetDefensePower(3.0f);

// CSkeletonBodyEnemy - 밸런스형
StatusComponent->SetMaxHealth(150.0f);
StatusComponent->SetCurrentHealth(150.0f);
StatusComponent->SetAttackPower(25.0f);
StatusComponent->SetDefensePower(8.0f);

// CSperatedEnemy - 탱커형
StatusComponent->SetMaxHealth(200.0f);
StatusComponent->SetCurrentHealth(200.0f);
StatusComponent->SetAttackPower(15.0f);
StatusComponent->SetDefensePower(12.0f);
```

### 데미지 설정 방법
```cpp
// StatusComponent에서 공격력 설정
StatusComponent->AttackPower = 25.0f;

// 또는 블루프린트에서 직접 설정
// StatusComponent > AttackPower = 25.0
```

## 사용법

### 1. 근접 공격 사용
```cpp
// 블루프린트에서 호출하거나 코드에서 직접 호출
void ACFlyingSkull::PlayMeleeAttack()
{
    if (MeleeAttackComponent)
    {
        MeleeAttackComponent->ActivateMeleeAttack();
    }
}

// 애니메이션 노티파이에서 공격 활성화/비활성화
UFUNCTION()
void OnMeleeAttackStart();  // 공격 시작 시 콜리전 활성화

UFUNCTION()
void OnMeleeAttackEnd();    // 공격 종료 시 콜리전 비활성화
```

### 2. 원거리 공격 사용
```cpp
// 블루프린트에서 호출하거나 코드에서 직접 호출
void ACFlyingSkull::PlayRangedAttack()
{
    if (ProjectileComponent)
    {
        // 프로젝타일 발사 로직
        // 예: 프로젝타일 액터 스폰 및 초기화
    }
}
```

### 3. 공격 이벤트 처리
```cpp
// BeginPlay에서 이벤트 바인딩
void ACFlyingSkull::BeginPlay()
{
    Super::BeginPlay();
    
    // 근접 공격 히트 이벤트
    if (MeleeAttackComponent)
    {
        MeleeAttackComponent->OnMeleeAttackHit.AddDynamic(this, &ACFlyingSkull::OnMeleeAttackHit);
    }
    
    // 원거리 공격 히트 이벤트
    if (ProjectileComponent)
    {
        ProjectileComponent->OnProjectileHit.AddDynamic(this, &ACFlyingSkull::OnProjectileHit);
    }
}

// 히트 처리 함수들
void ACFlyingSkull::OnMeleeAttackHit(AActor* HitActor)
{
    // 근접 공격이 플레이어에게 히트했을 때의 처리
    if (HitActor && HitActor->IsA<APawn>())
    {
        // 플레이어에게 데미지를 주는 로직
        // HitActor->TakeDamage(MeleeAttackComponent->GetMeleeDamage());
    }
}

void ACFlyingSkull::OnProjectileHit(AActor* HitActor)
{
    // 프로젝타일이 플레이어에게 히트했을 때의 처리
    if (HitActor && HitActor->IsA<APawn>())
    {
        // 플레이어에게 데미지를 주는 로직
        // HitActor->TakeDamage(ProjectileComponent->GetProjectileDamage());
    }
}
```

## 이벤트 시스템

### CEnemyMeleeAttackComponent 이벤트
- `OnMeleeAttackHit`: 근접 공격이 플레이어에게 히트했을 때 발생

### UCEnemyProjectileComp 이벤트
- `OnProjectileHit`: 프로젝타일이 플레이어에게 히트했을 때 발생

## 디버그 기능

### 화면 디버그 메시지
- 에너미 스폰: "[EnemyType] Spawned - Health: X, Attack: Y"
- 근접 공격 활성화: "Melee Attack Activated!"
- 근접 공격 비활성화: "Melee Attack Deactivated!"
- 근접 공격 히트: "Melee Attack Hit: [ActorName] with X.X damage!"
- 프로젝타일 히트: "Projectile Hit: [ActorName] with X.X damage!"
- 공격 실행: "[EnemyType] Melee/Ranged Attack!"

### 시각적 디버그
- 근접 공격 범위: 빨간색 박스 (공격 활성화 시)
- 프로젝타일 궤적: 기존 프로젝타일 시스템 활용

## 커스터마이징

### 에너미별 스테이터스 커스터마이징
```cpp
// 새로운 에너미 클래스에서 스테이터스 설정
void ANewEnemy::BeginPlay()
{
    Super::BeginPlay();
    
    if (StatusComponent)
    {
        // 원하는 스테이터스 설정
        StatusComponent->SetMaxHealth(120.0f);
        StatusComponent->SetCurrentHealth(120.0f);
        StatusComponent->SetAttackPower(30.0f);
        StatusComponent->SetDefensePower(6.0f);
    }
}
```

### 공격력 설정 (StatusComponent)
```cpp
// StatusComponent에서 공격력 조정
StatusComponent->AttackPower = 30.0f;

// 또는 블루프린트에서
// StatusComponent > AttackPower = 30.0
```

### 근접 공격 설정
```cpp
// 쿨다운 조정
MeleeAttackComponent->AttackCooldown = 0.5f;

// 콜리전 크기 조정 (블루프린트에서)
MeleeAttackComponent->MeleeAttackCollision->SetBoxExtent(FVector(100.0f, 100.0f, 50.0f));
```

### 원거리 공격 설정
```cpp
// 속도 조정
ProjectileComponent->ProjectileSpeed = 1500.0f;

// 수명 조정
ProjectileComponent->MaxLifeSeconds = 3.0f;
```

## 애니메이션 연동

### 애니메이션 노티파이 활용
```cpp
// 애니메이션 블루프린트에서 노티파이 추가
UFUNCTION()
void OnMeleeAttackStart()
{
    if (MeleeAttackComponent)
    {
        MeleeAttackComponent->ActivateMeleeAttack();
    }
}

UFUNCTION()
void OnMeleeAttackEnd()
{
    if (MeleeAttackComponent)
    {
        MeleeAttackComponent->DeactivateMeleeAttack();
    }
}
```

## 주의사항

1. **데미지 통합**: 모든 공격 데미지는 StatusComponent의 AttackPower를 사용하므로, 개별 공격 컴포넌트에서 데미지를 설정할 수 없습니다.
2. **스테이터스 설정**: 각 에너미는 `BeginPlay()`에서 개별적으로 스테이터스를 설정해야 합니다.
3. **콜리전 설정**: 근접 공격 콜리전은 기본적으로 비활성화되어 있으며, 공격 시에만 활성화됩니다.
4. **쿨다운 관리**: 근접 공격은 쿨다운 시스템이 내장되어 있어 연속 공격을 방지합니다.
5. **이벤트 바인딩**: `BeginPlay()`에서 이벤트 바인딩을 수행해야 합니다.
6. **프로젝타일 관리**: 기존의 프로젝타일 풀링 시스템을 활용하여 성능을 최적화합니다.

## 장점

### 중앙화된 데미지 관리
- **일관성**: 모든 공격이 동일한 공격력 값을 사용
- **밸런싱**: StatusComponent에서만 공격력을 조정하면 모든 공격에 반영
- **유지보수**: 데미지 관련 코드가 한 곳에 집중되어 관리 용이

### 에너미별 차별화
- **역할 분담**: 각 에너미가 고유한 역할을 가짐 (공격형, 밸런스형, 탱커형)
- **전략적 게임플레이**: 플레이어가 에너미 타입에 따라 다른 전략 사용 가능
- **확장성**: 새로운 에너미 타입 추가 시에도 쉽게 스테이터스 설정 가능

### 확장성
- 새로운 공격 타입 추가 시에도 StatusComponent의 공격력 사용 가능
- 각 에너미별로 다른 공격력 설정 가능
- 레벨업 시스템과 연동 가능

## 향후 확장 가능성

- 다양한 공격 패턴 (콤보 공격, 특수 공격 등)
- 공격 효과 시스템 (파티클, 사운드 등)
- 공격 애니메이션 시스템
- AI 기반 공격 패턴 선택
- 공격 우선순위 시스템
- 레벨별 공격력 증가 시스템
- 에너미별 고유 능력 시스템
- 스테이터스 버프/디버프 시스템
