# 에너미 스테이터스 시스템 (Enemy Status System)

## 개요
이 시스템은 모든 에너미들이 공통적으로 갖게 되는 스테이터스(공격력, 체력)를 관리하고, 플레이어의 공격을 받았을 때 체력이 감소하며, 체력이 0 이하일 경우 사망하게 만드는 기능을 제공합니다.

## 주요 컴포넌트

### 1. CEnemyStatusComponent
- **위치**: `Public/ODH/ODH_Enemy/Component/CEnemyStatusComponent.h`
- **기능**: 에너미의 기본 스테이터스(체력, 공격력, 방어력)를 관리
- **주요 속성**:
  - `MaxHealth`: 최대 체력 (기본값: 100.0f)
  - `CurrentHealth`: 현재 체력
  - `AttackPower`: 공격력 (기본값: 20.0f)
  - `DefensePower`: 방어력 (기본값: 5.0f)
  - `bIsDead`: 사망 상태

### 2. IDamageable 인터페이스
- **위치**: `Public/ODH/ODH_Enemy/Interface/IDamageable.h`
- **기능**: 데미지를 받을 수 있는 객체임을 나타내는 인터페이스
- **주요 함수**:
  - `TakeDamage(float DamageAmount)`: 데미지를 받는 함수
  - `CanTakeDamage()`: 데미지를 받을 수 있는지 확인
  - `IsDead()`: 사망했는지 확인

### 3. CEnemyDamageComponent
- **위치**: `Public/ODH/ODH_Enemy/Component/CEnemyDamageComponent.h`
- **기능**: 플레이어의 무기가 에너미에게 데미지를 줄 수 있도록 하는 컴포넌트
- **주요 기능**:
  - 단일 타겟에게 데미지 주기
  - 범위 내의 모든 데미지 가능한 액터들에게 데미지 주기

## 구현된 에너미 클래스들

### 1. CFlyingSkull
- 스테이터스 컴포넌트와 IDamageable 인터페이스 구현
- 사망 시 "Flying Skull Died!" 메시지 출력

### 2. CSkeletonBodyEnemy
- 스테이터스 컴포넌트와 IDamageable 인터페이스 구현
- 사망 시 "Skeleton Body Enemy Died!" 메시지 출력

### 3. CSperatedEnemy
- 스테이터스 컴포넌트와 IDamageable 인터페이스 구현
- 사망 시 "Separated Enemy Died!" 메시지 출력

## 사용법

### 1. 에너미에 스테이터스 컴포넌트 추가
```cpp
// 헤더 파일에서
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
UCEnemyStatusComponent* StatusComponent;

// 생성자에서
StatusComponent = CreateDefaultSubobject<UCEnemyStatusComponent>(TEXT("StatusComponent"));
```

### 2. IDamageable 인터페이스 구현
```cpp
// 헤더 파일에서
class AYourEnemyClass : public ACharacter, public IDamageable

// 구현 파일에서
void AYourEnemyClass::TakeDamage_Implementation(float DamageAmount)
{
    if (StatusComponent)
    {
        StatusComponent->TakeDamage(DamageAmount);
    }
}

bool AYourEnemyClass::CanTakeDamage_Implementation() const
{
    if (StatusComponent)
    {
        return !StatusComponent->IsDead();
    }
    return false;
}

bool AYourEnemyClass::IsDead_Implementation() const
{
    if (StatusComponent)
    {
        return StatusComponent->IsDead();
    }
    return false;
}
```

### 3. 사망 이벤트 처리
```cpp
// BeginPlay에서
if (StatusComponent)
{
    StatusComponent->OnDeath.AddDynamic(this, &AYourEnemyClass::OnDeath);
}

// 사망 처리 함수
void AYourEnemyClass::OnDeath()
{
    // 사망 시 처리 로직
    // 예: 사망 애니메이션 재생, 효과 재생, 액터 제거 등
}
```

### 4. 플레이어 무기에 데미지 컴포넌트 추가
```cpp
// 헤더 파일에서
UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
UCEnemyDamageComponent* DamageComponent;

// 생성자에서
DamageComponent = CreateDefaultSubobject<UCEnemyDamageComponent>(TEXT("DamageComponent"));

// 공격 시 데미지 주기
void AYourWeapon::Attack()
{
    // 범위 내의 에너미들에게 데미지 주기
    DamageComponent->DealDamageInRadius(GetActorLocation(), 150.0f);
    
    // 또는 특정 에너미에게 데미지 주기
    // DamageComponent->DealDamage(TargetEnemy);
}
```

## 이벤트 시스템

### CEnemyStatusComponent 이벤트
- `OnHealthChanged`: 체력이 변경될 때 발생
- `OnMaxHealthChanged`: 최대 체력이 변경될 때 발생
- `OnDamageReceived`: 데미지를 받았을 때 발생
- `OnDeath`: 사망했을 때 발생

### 이벤트 바인딩 예시
```cpp
// 블루프린트에서 이벤트를 바인딩하여 UI 업데이트, 효과 재생 등 가능
StatusComponent->OnHealthChanged.AddDynamic(this, &AYourEnemyClass::UpdateHealthUI);
StatusComponent->OnDeath.AddDynamic(this, &AYourEnemyClass::PlayDeathEffect);
```

## 디버그 기능

### 화면 디버그 메시지
- 데미지를 받았을 때: "Enemy took X.X damage! Current Health: X.X"
- 사망했을 때: "Enemy Died!"
- 공격했을 때: "Dealt X.X damage to [EnemyName]"

### 시각적 디버그
- 데미지 범위: 빨간색 구체 (범위 공격 시)

## 커스터마이징

### 스테이터스 값 조정
- 블루프린트에서 `CEnemyStatusComponent`의 속성들을 직접 조정 가능
- 코드에서 `SetMaxHealth()`, `SetAttackPower()`, `SetDefensePower()` 함수 사용

### 데미지 값 조정
- `CEnemyDamageComponent`의 `DamageAmount` 속성 조정
- 블루프린트에서 실시간으로 값 변경 가능

## 주의사항

1. **컴포넌트 순서**: 스테이터스 컴포넌트는 다른 컴포넌트보다 먼저 생성되어야 합니다.
2. **인터페이스 구현**: `_Implementation` 접미사를 사용하여 인터페이스 함수를 구현해야 합니다.
3. **이벤트 바인딩**: `BeginPlay()`에서 이벤트 바인딩을 수행해야 합니다.
4. **메모리 관리**: 사망 시 액터를 제거하는 로직을 적절히 구현해야 합니다.

## 향후 확장 가능성

- 레벨 시스템
- 경험치 시스템
- 아이템 드롭 시스템
- 상태 이상 시스템 (독, 기절 등)
- 팀 시스템
- AI 행동 수정 (사망 시 행동 변경)


