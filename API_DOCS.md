## Unreal Engine C++ API 문서

이 문서는 제공된 데이터를 기반으로 언리얼 엔진 C++ API에 대한 상세한 설명을 제공합니다. 각 클래스, 함수, 구조체에 대한 설명, 사용 예제, 성능 고려사항, 호환성 정보 및 트러블슈팅 가이드라인을 포함합니다.

---

### ABossEffect

**클래스 개요:**

`ABossEffect` 클래스는 보스 캐릭터에 적용되는 시각적 또는 청각적 이펙트를 관리하는 액터 클래스입니다. 이 클래스는 이펙트의 활성화, 비활성화, 위치 설정, 태그 관리를 담당합니다. 주로 보스의 특정 행동이나 상태 변화를 시각적으로 표현하는 데 사용됩니다.

**사용 시나리오:**

*   보스가 특정 스킬을 사용할 때 특정 위치에 시각 효과를 생성합니다.
*   보스가 특정 페이즈에 진입할 때 전체적인 분위기를 바꾸는 이펙트를 활성화합니다.
*   보스가 데미지를 입었을 때 피격 이펙트를 재생합니다.

**상속 관계:**

*   `AActor`

**인터페이스:**

*   이펙트 활성화/비활성화 함수
*   이펙트 위치 설정 함수
*   이펙트 태그 관리 함수

**주의사항:**

*   이펙트의 성능은 게임의 전체적인 성능에 영향을 미칠 수 있으므로, 과도한 이펙트 사용은 피해야 합니다.
*   이펙트의 위치 설정 시, 부모 액터의 트랜스폼을 고려해야 합니다.

**성능 고려사항:**

*   파티클 시스템의 복잡도를 줄여 성능을 최적화합니다.
*   불필요한 이펙트 활성화를 최소화합니다.
*   이펙트 풀링을 사용하여 메모리 할당/해제를 최적화합니다.

**API:**

*   **`ABossEffect()`**
    *   **설명:** `ABossEffect` 클래스의 생성자입니다.
    *   **매개변수:** 없음
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        ABossEffect::ABossEffect()
        {
            // 생성자 코드
        }
        ```

*   **`void Tick(float DeltaTime)`**
    *   **설명:** 매 프레임마다 호출되는 함수입니다. 이펙트의 업데이트 로직을 처리합니다.
    *   **매개변수:**
        *   `DeltaTime` (float): 프레임 간의 시간 간격입니다.
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossEffect::Tick(float DeltaTime)
        {
            Super::Tick(DeltaTime);
            // 이펙트 업데이트 로직
        }
        ```

*   **`DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectFinished, ABossEffect*, Effect)`**
    *   **설명:** 이펙트가 완료되었을 때 호출되는 델리게이트를 선언합니다.
    *   **매개변수:**
        *   `Effect` (ABossEffect*): 완료된 이펙트 객체입니다.
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectFinished, ABossEffect*, Effect);
        ```

*   **`void ActivateEffect()`**
    *   **설명:** 이펙트를 활성화합니다.
    *   **매개변수:** 없음
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossEffect::ActivateEffect()
        {
            // 이펙트 활성화 로직
            SetActorHiddenInGame(false);
        }
        ```

*   **`void ActivateEffectAttachedToSocket(USceneComponent* Parent, FName SocketName)`**
    *   **설명:** 지정된 소켓에 이펙트를 부착하고 활성화합니다.
    *   **매개변수:**
        *   `Parent` (USceneComponent*): 부착될 부모 컴포넌트입니다.
        *   `SocketName` (FName): 부착될 소켓의 이름입니다.
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossEffect::ActivateEffectAttachedToSocket(USceneComponent* Parent, FName SocketName)
        {
            AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
            ActivateEffect();
        }
        ```

*   **`void DeactivateEffect()`**
    *   **설명:** 이펙트를 비활성화합니다.
    *   **매개변수:** 없음
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossEffect::DeactivateEffect()
        {
            // 이펙트 비활성화 로직
            SetActorHiddenInGame(true);
        }
        ```

*   **`bool IsActive()`**
    *   **설명:** 이펙트가 현재 활성화되어 있는지 확인합니다.
    *   **매개변수:** 없음
    *   **반환값:**
        *   `true`: 이펙트가 활성화되어 있는 경우
        *   `false`: 이펙트가 비활성화되어 있는 경우
    *   **사용 예제:**
        ```c++
        bool ABossEffect::IsActive()
        {
            return !IsActorHiddenInGame();
        }
        ```

*   **`FName GetCurrentEffectTag()`**
    *   **설명:** 현재 이펙트의 태그를 반환합니다.
    *   **매개변수:** 없음
    *   **반환값:**
        *   `FName`: 현재 이펙트의 태그
    *   **사용 예제:**
        ```c++
        FName ABossEffect::GetCurrentEffectTag()
        {
            return EffectTag;
        }
        ```

*   **`void BeginPlay()`**
    *   **설명:** 액터가 게임에 배치될 때 호출되는 함수입니다. 초기화 로직을 처리합니다.
    *   **매개변수:** 없음
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossEffect::BeginPlay()
        {
            Super::BeginPlay();
            // 초기화 로직
        }
        ```

*   **`void AttachToBoss(AActor* BossActor)`**
    *   **설명:** 이펙트를 보스 액터에 부착합니다.
    *   **매개변수:**
        *   `BossActor` (AActor*): 부착될 보스 액터입니다.
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossEffect::AttachToBoss(AActor* BossActor)
        {
            AttachToActor(BossActor, FAttachmentTransformRules::KeepRelativeTransform);
        }
        ```

*   **`void AttachToSocket(USkeletalMeshComponent* MeshComponent, FName SocketName)`**
    *   **설명:** 이펙트를 지정된 메시 컴포넌트의 소켓에 부착합니다.
    *   **매개변수:**
        *   `MeshComponent` (USkeletalMeshComponent*): 부착될 메시 컴포넌트입니다.
        *   `SocketName` (FName): 부착될 소켓의 이름입니다.
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossEffect::AttachToSocket(USkeletalMeshComponent* MeshComponent, FName SocketName)
        {
            AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
        }
        ```

*   **`void PlaceInWorld(FVector Location, FRotator Rotation)`**
    *   **설명:** 이펙트를 월드에 지정된 위치와 회전으로 배치합니다.
    *   **매개변수:**
        *   `Location` (FVector): 배치될 위치입니다.
        *   `Rotation` (FRotator): 배치될 회전입니다.
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossEffect::PlaceInWorld(FVector Location, FRotator Rotation)
        {
            SetActorLocationAndRotation(Location, Rotation);
        }
        ```

**사용 예제 (완전한 코드):**

```c++
// BossEffect.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossEffect.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectFinished, ABossEffect*, Effect);

UCLASS()
class MYPROJECT_API ABossEffect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 이펙트 활성화
	UFUNCTION(BlueprintCallable, Category = "BossEffect")
	void ActivateEffect();

	// 소켓에 붙여서 이펙트 활성화
	UFUNCTION(BlueprintCallable, Category = "BossEffect")
	void ActivateEffectAttachedToSocket(USceneComponent* Parent, FName SocketName);

	// 이펙트 비활성화
	UFUNCTION(BlueprintCallable, Category = "BossEffect")
	void DeactivateEffect();

	// 이펙트가 활성화되어 있는지 확인
	UFUNCTION(BlueprintCallable, Category = "BossEffect")
	bool IsActive();

	// 현재 이펙트 태그 반환
	UFUNCTION(BlueprintCallable, Category = "BossEffect")
	FName GetCurrentEffectTag();

	// 보스에 이펙트 어태치
	UFUNCTION(BlueprintCallable, Category = "BossEffect")
	void AttachToBoss(AActor* BossActor);

	// 소켓에 이펙트 붙이기
	UFUNCTION(BlueprintCallable, Category = "BossEffect")
	void AttachToSocket(USkeletalMeshComponent* MeshComponent, FName SocketName);

	// 월드에 이펙트 배치
	UFUNCTION(BlueprintCallable, Category = "BossEffect")
	void PlaceInWorld(FVector Location, FRotator Rotation);

	// 이펙트 완료 델리게이트
	UPROPERTY(BlueprintAssignable, Category = "BossEffect")
	FOnEffectFinished OnEffectFinished;

private:
	// 이펙트 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossEffect")
	FName EffectTag;
};

// BossEffect.cpp
#include "BossEffect.h"

ABossEffect::ABossEffect()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorHiddenInGame(true);
}

void ABossEffect::BeginPlay()
{
	Super::BeginPlay();
}

void ABossEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossEffect::ActivateEffect()
{
	SetActorHiddenInGame(false);
}

void ABossEffect::ActivateEffectAttachedToSocket(USceneComponent* Parent, FName SocketName)
{
	AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
	ActivateEffect();
}

void ABossEffect::DeactivateEffect()
{
	SetActorHiddenInGame(true);
	OnEffectFinished.Broadcast(this);
}

bool ABossEffect::IsActive()
{
	return !IsActorHiddenInGame();
}

FName ABossEffect::GetCurrentEffectTag()
{
	return EffectTag;
}

void ABossEffect::AttachToBoss(AActor* BossActor)
{
	AttachToActor(BossActor, FAttachmentTransformRules::KeepRelativeTransform);
}

void ABossEffect::AttachToSocket(USkeletalMeshComponent* MeshComponent, FName SocketName)
{
	AttachToComponent(MeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
}

void ABossEffect::PlaceInWorld(FVector Location, FRotator Rotation)
{
	SetActorLocationAndRotation(Location, Rotation);
}
```

**에러 처리 및 예외 상황:**

*   `ActivateEffectAttachedToSocket` 함수에서 `Parent`가 `nullptr`인 경우, 로그를 출력하고 함수를 종료합니다.
*   이펙트 풀링 시스템에서 풀이 가득 찬 경우, 새로운 이펙트를 생성하거나 기존 이펙트를 재사용합니다.

**성능 최적화 예제:**

```c++
// 이펙트 풀링을 사용하는 예제
ABossEffect* ABossEffectManager::GetEffectFromPool(FName EffectTag)
{
    // 풀에서 이펙트를 가져오는 로직
    if (EffectPool.Contains(EffectTag) && EffectPool[EffectTag].Num() > 0)
    {
        ABossEffect* Effect = EffectPool[EffectTag].Pop();
        Effect->ActivateEffect();
        return Effect;
    }
    else
    {
        // 풀이 비어있으면 새로운 이펙트 생성
        ABossEffect* NewEffect = GetWorld()->SpawnActor<ABossEffect>(ABossEffect::StaticClass());
        NewEffect->EffectTag = EffectTag;
        NewEffect->ActivateEffect();
        return NewEffect;
    }
}

void ABossEffectManager::ReturnEffectToPool(ABossEffect* Effect)
{
    // 이펙트를 풀로 반환하는 로직
    Effect->DeactivateEffect();
    EffectPool.FindOrAdd(Effect->EffectTag).Add(Effect);
}
```

**프로파일링 가이드:**

*   Unreal Engine의 프로파일링 툴을 사용하여 이펙트의 성능을 측정합니다.
*   CPU 및 GPU 사용량을 분석하여 병목 현상을 파악합니다.
*   파티클 시스템의 복잡도를 조절하여 성능을 최적화합니다.

**호환성 정보:**

*   Unreal Engine 4.26 이상
*   모든 플랫폼 지원

**트러블슈팅:**

*   이펙트가 보이지 않는 경우, 액터의 숨김 설정을 확인합니다.
*   이펙트의 위치가 잘못된 경우, 부모 액터의 트랜스폼을 확인합니다.
*   이펙트의 성능이 저하되는 경우, 파티클 시스템의 복잡도를 줄입니다.

---

### ABossManager

**클래스 개요:**

`ABossManager` 클래스는 보스 캐릭터의 생성, 초기화, 상태 관리, 이벤트 처리 등 보스 관련 전반적인 로직을 담당하는 액터 클래스입니다. 이 클래스는 보스의 스폰, 리스폰, AI 제어, UI 업데이트 등을 관리합니다.

**사용 시나리오:**

*   게임 시작 시 보스를 스폰합니다.
*   보스가 사망했을 때 리스폰 로직을 처리합니다.
*   보스의 상태 변화에 따라 UI를 업데이트합니다.
*   보스의 AI를 제어합니다.

**상속 관계:**

*   `AActor`

**인터페이스:**

*   보스 생성/초기화 함수
*   보스 상태 관리 함수
*   이벤트 처리 함수
*   AI 제어 함수
*   UI 업데이트 함수

**주의사항:**

*   보스 매니저는 게임의 핵심 로직을 담당하므로, 안정성과 성능을 고려하여 설계해야 합니다.
*   보스 매니저는 싱글톤 패턴을 사용하여 게임 내에서 유일한 인스턴스만 존재하도록 관리하는 것이 좋습니다.

**성능 고려사항:**

*   보스 AI의 복잡도를 줄여 성능을 최적화합니다.
*   불필요한 업데이트를 최소화합니다.
*   보스 상태 관리를 효율적으로 구현하여 성능을 최적화합니다.

**API:**

*   **`ABossManager()`**
    *   **설명:** `ABossManager` 클래스의 생성자입니다.
    *   **매개변수:** 없음
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        ABossManager::ABossManager()
        {
            // 생성자 코드
        }
        ```

*   **`void Tick(float DeltaTime)`**
    *   **설명:** 매 프레임마다 호출되는 함수입니다. 보스 매니저의 업데이트 로직을 처리합니다.
    *   **매개변수:**
        *   `DeltaTime` (float): 프레임 간의 시간 간격입니다.
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossManager::Tick(float DeltaTime)
        {
            Super::Tick(DeltaTime);
            // 보스 매니저 업데이트 로직
        }
        ```

*   **`void ResetBossCompletely()`**
    *   **설명:** 보스를 완전히 초기화합니다. 이 함수는 보스의 상태, 위치, AI 등을 초기 상태로 되돌립니다.
    *   **매개변수:** 없음
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossManager::ResetBossCompletely()
        {
            // 보스 완전 초기화 로직
            if (CurrentBoss)
            {
                CurrentBoss->Destroy();
            }
            SpawnBoss(); // 보스 다시 스폰
            ResetAllBossComponents();
            ResetBossStateTree();
        }
        ```

*   **`void OpenDoor()`**
    *   **설명:** 보스 방의 문을 엽니다.
    *   **매개변수:** 없음
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossManager::OpenDoor()
        {
            // 문 여는 로직
        }
        ```

*   **`ACBoss* FindBossInWorld()`**
    *   **설명:** 월드에서 보스 액터를 찾습니다.
    *   **매개변수:** 없음
    *   **반환값:**
        *   `ACBoss*`: 월드에서 찾은 보스 액터입니다. 찾지 못한 경우 `nullptr`을 반환합니다.
    *   **사용 예제:**
        ```c++
        ACBoss* ABossManager::FindBossInWorld()
        {
            // 월드에서 보스 찾기 로직
            for (TActorIterator<ACBoss> ActorItr(GetWorld()); ActorItr; ++ActorItr)
            {
                return *ActorItr;
            }
            return nullptr;
        }
        ```

*   **`void ResetAllBossComponents()`**
    *   **설명:** 보스의 모든 컴포넌트를 초기화합니다.
    *   **매개변수:** 없음
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossManager::ResetAllBossComponents()
        {
            // 각 컴포넌트별 초기화 실행
            if (CurrentBoss)
            {
                // 예시: CurrentBoss->GetStatusComponent()->Reset();
            }
        }
        ```

*   **`void ResetBossStateTree()`**
    *   **설명:** 보스의 StateTree를 완전히 재시작합니다.
    *   **매개변수:** 없음
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossManager::ResetBossStateTree()
        {
            // StateTree 완전 리스타트 로직
            if (CurrentBoss)
            {
                // 예시: CurrentBoss->GetAIController()->RestartLogic();
            }
        }
        ```

*   **`void BeginPlay()`**
    *   **설명:** 액터가 게임에 배치될 때 호출되는 함수입니다. 초기화 로직을 처리합니다.
    *   **매개변수:** 없음
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossManager::BeginPlay()
        {
            Super::BeginPlay();
            // 초기화 로직
            SpawnBoss();
        }
        ```

*   **`void OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)`**
    *   **설명:** 트리거 박스에 다른 액터가 겹치기 시작할 때 호출되는 함수입니다.
    *   **매개변수:**
        *   `OverlappedComponent` (UPrimitiveComponent*): 겹쳐진 컴포넌트입니다.
        *   `OtherActor` (AActor*): 겹쳐진 액터입니다.
        *   `OtherComp` (UPrimitiveComponent*): 겹쳐진 액터의 컴포넌트입니다.
        *   `OtherBodyIndex` (int32): 겹쳐진 액터의 바디 인덱스입니다.
        *   `bFromSweep` (bool): 스윕으로 인한 겹침인지 여부입니다.
        *   `SweepResult` (const FHitResult&): 스윕 결과입니다.
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossManager::OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
        {
            // 콜리전 오버랩 시작 이벤트 처리 로직
            if (OtherActor && OtherActor->ActorHasTag("Player"))
            {
                // 플레이어가 트리거 박스에 들어왔을 때
                OpenDoor();
            }
        }
        ```

**사용 예제 (완전한 코드):**

```c++
// BossManager.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CBoss.h"
#include "Components/BoxComponent.h"
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

	// 보스 완전 초기화 (메인 함수)
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	void ResetBossCompletely();

	// 문 열기
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	void OpenDoor();

	// 월드에서 보스 찾기
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	ACBoss* FindBossInWorld();

	// 각 컴포넌트별 초기화 실행
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	void ResetAllBossComponents();

	// StateTree 완전 리스타트
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	void ResetBossStateTree();

	// 트리거 박스 오버랩 시작 이벤트
	UFUNCTION()
	void OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	// 스폰할 보스 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossManager")
	TSubclassOf<ACBoss> BossClass;

	// 현재 보스
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BossManager")
	ACBoss* CurrentBoss;

	// 트리거 박스
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BossManager")
	UBoxComponent* TriggerBox;

	// 보스 스폰 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossManager")
	FVector BossSpawnLocation;

	// 보스 스폰 회전
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BossManager")
	FRotator BossSpawnRotation;

	// 보스 스폰 함수
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	void SpawnBoss();
};

// BossManager.cpp
#include "BossManager.h"
#include "Components/BoxComponent.h"
#include "EngineUtils.h"

ABossManager::ABossManager()
{
	PrimaryActorTick.bCanEverTick = true;

	// 트리거 박스 생성
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABossManager::OnTriggerBoxOverlapBegin);

	// 기본 스폰 위치 설정
	BossSpawnLocation = FVector(0.0f, 0.0f, 0.0f);
	BossSpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
}

void ABossManager::BeginPlay()
{
	Super::BeginPlay();

	// 보스 스폰
	SpawnBoss();
}

void ABossManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABossManager::ResetBossCompletely()
{
	// 보스 완전 초기화 로직
	if (CurrentBoss)
	{
		CurrentBoss->Destroy();
	}
	SpawnBoss(); // 보스 다시 스폰
	ResetAllBossComponents();
	ResetBossStateTree();
}

void ABossManager::OpenDoor()
{
	// 문 여는 로직
	UE_LOG(LogTemp, Warning, TEXT("Door Opened!"));
}

ACBoss* ABossManager::FindBossInWorld()
{
	// 월드에서 보스 찾기 로직
	for (TActorIterator<ACBoss> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		return *ActorItr;
	}
	return nullptr;
}

void ABossManager::ResetAllBossComponents()
{
	// 각 컴포넌트별 초기화 실행
	if (CurrentBoss)
	{
		// 예시: CurrentBoss->GetStatusComponent()->Reset();
		UE_LOG(LogTemp, Warning, TEXT("All Components Reset!"));
	}
}

void ABossManager::ResetBossStateTree()
{
	// StateTree 완전 리스타트 로직
	if (CurrentBoss)
	{
		// 예시: CurrentBoss->GetAIController()->RestartLogic();
		UE_LOG(LogTemp, Warning, TEXT("State Tree Reset!"));
	}
}

void ABossManager::OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 콜리전 오버랩 시작 이벤트 처리 로직
	if (OtherActor && OtherActor->ActorHasTag("Player"))
	{
		// 플레이어가 트리거 박스에 들어왔을 때
		OpenDoor();
	}
}

void ABossManager::SpawnBoss()
{
	// 보스 스폰 로직
	if (BossClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		CurrentBoss = GetWorld()->SpawnActor<ACBoss>(BossClass, BossSpawnLocation, BossSpawnRotation, SpawnParams);

		if (CurrentBoss)
		{
			UE_LOG(LogTemp, Warning, TEXT("Boss Spawned!"));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to spawn boss!"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("BossClass is not set in BossManager!"));
	}
}
```

**에러 처리 및 예외 상황:**

*   `FindBossInWorld` 함수에서 보스를 찾지 못한 경우, 로그를 출력하고 `nullptr`을 반환합니다.
*   `ResetAllBossComponents` 함수에서 보스 컴포넌트 초기화에 실패한 경우, 로그를 출력합니다.
*   `ResetBossStateTree` 함수에서 StateTree 재시작에 실패한 경우, 로그를 출력합니다.

**성능 최적화 예제:**

```c++
// 보스 AI 업데이트 빈도를 조절하는 예제
void ABossManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // AI 업데이트 빈도 조절
    static float AIUpdateTimer = 0.0f;
    AIUpdateTimer += DeltaTime;

    if (AIUpdateTimer >= AIUpdateInterval)
    {
        AIUpdateTimer -= AIUpdateInterval;
        UpdateBossAI();
    }
}

void ABossManager::UpdateBossAI()
{
    // 보스 AI 업데이트 로직
}
```

**프로파일링 가이드:**

*   Unreal Engine의 프로파일링 툴을 사용하여 보스 매니저의 성능을 측정합니다.
*   CPU 및 GPU 사용량을 분석하여 병목 현상을 파악합니다.
*   보스 AI의 복잡도를 조절하여 성능을 최적화합니다.

**호환성 정보:**

*   Unreal Engine 4.26 이상
*   모든 플랫폼 지원

**트러블슈팅:**

*   보스가 스폰되지 않는 경우, `BossClass`가 제대로 설정되었는지 확인합니다.
*   보스의 AI가 제대로 작동하지 않는 경우, StateTree 설정과 AIController 로직을 확인합니다.
*   보스 UI가 업데이트되지 않는 경우, UI 업데이트 로직과 보스 상태를 확인합니다.

---

### ABossProjectileActor

**클래스 개요:**

`ABossProjectileActor` 클래스는 보스 캐릭터가 발사하는 투사체의 기본 클래스입니다. 이 클래스는 투사체의 발사, 이동, 충돌 처리, 이펙트 재생 등을 담당합니다.

**사용 시나리오:**

*   보스가 원거리 공격을 할 때 투사체를 발사합니다.
*   투사체가 목표 지점에 도달하거나 충돌했을 때 이펙트를 재생합니다.
*   투사체의 이동 패턴을 정의합니다.

**상속 관계:**

*   `AActor`

**인터페이스:**

*   투사체 발사 함수
*   투사체 이동 함수
*   충돌 처리 함수
*   이펙트 재생 함수

**주의사항:**

*   투사체의 성능은 게임의 전체적인 성능에 영향을 미칠 수 있으므로, 과도한 투사체 사용은 피해야 합니다.
*   투사체의 충돌 처리는 정확하고 효율적으로 구현해야 합니다.

**성능 고려사항:**

*   투사체의 수를 제한하여 성능을 최적화합니다.
*   투사체의 이동 로직을 최적화합니다.
*   투사체 풀링을 사용하여 메모리 할당/해제를 최적화합니다.

**API:**

*   **`ABossProjectileActor()`**
    *   **설명:** `ABossProjectileActor` 클래스의 생성자입니다.
    *   **매개변수:** 없음
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        ABossProjectileActor::ABossProjectileActor()
        {
            // 생성자 코드
        }
        ```

*   **`void Tick(float DeltaTime)`**
    *   **설명:** 매 프레임마다 호출되는 함수입니다. 투사체의 업데이트 로직을 처리합니다.
    *   **매개변수:**
        *   `DeltaTime` (float): 프레임 간의 시간 간격입니다.
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossProjectileActor::Tick(float DeltaTime)
        {
            Super::Tick(DeltaTime);
            // 투사체 업데이트 로직
        }
        ```

*   **`void FireProjectile(FVector Direction)`**
    *   **설명:** 투사체를 지정된 방향으로 발사합니다.
    *   **매개변수:**
        *   `Direction` (FVector): 투사체의 발사 방향입니다.
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossProjectileActor::FireProjectile(FVector Direction)
        {
            // 투사체 발사 로직
            ProjectileMovementComponent->Velocity = Direction * ProjectileSpeed;
        }
        ```

*   **`void FireProjectileToLocation(FVector TargetLocation)`**
    *   **설명:** 투사체를 지정된 위치로 발사합니다.
    *   **매개변수:**
        *   `TargetLocation` (FVector): 투사체의 목표 위치입니다.
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossProjectileActor::FireProjectileToLocation(FVector TargetLocation)
        {
            // 투사체 발사 로직
            FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
            FireProjectile(Direction);
        }
        ```

*   **`void PlaySpawnEffect()`**
    *   **설명:** 투사체가 생성될 때 재생되는 이펙트를 재생합니다.
    *   **매개변수:** 없음
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossProjectileActor::PlaySpawnEffect()
        {
            // 투사체 생성 이펙트 재생 로직
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, GetActorTransform());
        }
        ```

*   **`void PlayDestroyEffect()`**
    *   **설명:** 투사체가 소멸될 때 재생되는 이펙트를 재생합니다.
    *   **매개변수:** 없음
    *   **반환값:** 없음
    *   **사용 예제:**
        ```c++
        void ABossProjectileActor::PlayDestroyEffect()
        {
            // 투사체 소멸 이펙트 재생 로직
            UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyEffect, GetActorTransform());
        }
        ```

*   **`void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)`**
    *   **설명:** 투사체가 다른 액터와 충돌했을 때 호출되는 함수입니다.
    *   **매개변수:**
