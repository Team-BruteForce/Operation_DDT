## 보스 AI 및 전투 시스템 구현 가이드

본 가이드는 언리얼 엔진 5 기반의 보스 AI 및 전투 시스템 구현에 대한 상세한 단계를 제공합니다. 제공된 문서 데이터를 기반으로 실제 개발자가 따라 할 수 있도록 구성되었으며, 각 기능별 핵심 클래스 구현, 시스템 통합, 그리고 코드 예제를 포함합니다.

**가이드 구성:**

1.  **프로젝트 설정**
    *   개발 환경 구성
    *   프로젝트 구조 설정
    *   의존성 관리
    *   빌드 설정
2.  **핵심 클래스 구현**
    *   클래스 설계 및 구현
    *   주요 메서드 구현
    *   프로퍼티 설정
    *   초기화 및 정리
    *   실제 사용 예제
3.  **시스템 통합**
    *   클래스 간 연동
    *   데이터 플로우 구현
    *   이벤트 시스템 구성
    *   상태 관리 구현

---

### 1. 프로젝트 설정

#### 1.1 개발 환경 구성

*   **언리얼 엔진 설치:** 언리얼 엔진 5 (또는 사용하려는 버전)를 언리얼 엔진 공식 웹사이트에서 다운로드하여 설치합니다.
*   **Visual Studio 설치:** C++ 코드를 컴파일하고 디버깅하기 위해 Visual Studio (또는 다른 지원되는 IDE)를 설치합니다. 언리얼 엔진과 호환되는 버전인지 확인하십시오.
*   **언리얼 엔진 프로젝트 생성:** 언리얼 에디터를 실행하고 새로운 C++ 기반 프로젝트를 생성합니다. "Third Person" 템플릿을 선택하여 기본적인 캐릭터 이동 기능을 포함할 수 있습니다.

#### 1.2 프로젝트 구조 설정

프로젝트 폴더 구조를 다음과 같이 구성합니다.

```
MyProject/
├── Content/
│   ├── Boss/
│   │   ├── AI/
│   │   ├── Animation/
│   │   ├── Effects/
│   │   ├── Projectiles/
│   │   ├── Components/
│   │   ├── Data/
│   │   └── ...
│   ├── UI/
│   └── ...
├── Source/
│   ├── MyProject/
│   │   ├── Public/
│   │   │   ├── Boss/
│   │   │   │   ├── AI/
│   │   │   │   ├── Components/
│   │   │   │   └── ...
│   │   ├── Private/
│   │   │   ├── Boss/
│   │   │   │   ├── AI/
│   │   │   │   ├── Components/
│   │   │   │   └── ...
│   │   ├── MyProject.Build.cs
│   │   └── MyProject.h
│   └── ...
└── ...
```

#### 1.3 의존성 관리

`MyProject.Build.cs` 파일을 열고 필요한 모듈을 추가합니다.

```csharp
using UnrealBuildTool;

public class MyProject : ModuleRules
{
    public MyProject(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "AIModule", // AI 모듈 추가
            "GameplayTasks", // GameplayTasks 모듈 추가
            "NavigationSystem", // Navigation 모듈 추가
            "StateTreeModule", // StateTree 모듈 추가
            "UMG", // UMG 모듈 추가
            "Niagara" // Niagara 모듈 추가
        });

        PrivateDependencyModuleNames.AddRange(new string[] {  });
    }
}
```

#### 1.4 빌드 설정

*   **C++ 코드 컴파일:** 언리얼 에디터에서 "File" -> "Refresh Visual Studio Project"를 선택하여 Visual Studio 프로젝트 파일을 업데이트합니다. Visual Studio에서 프로젝트를 열고 "Development Editor" 구성으로 빌드합니다.
*   **에디터 재시작:** 빌드가 완료되면 언리얼 에디터를 재시작합니다.

---

### 2. 핵심 클래스 구현

#### 2.1 `ABossEffect` 클래스 구현

**설명:** 이펙트 활성화 및 관리

*   **헤더 파일 (`BossEffect.h`)**

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "NiagaraComponent.h"
#include "BossEffect.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBossEffectFinished, ABossEffect*, Effect);

UCLASS(Blueprintable, BlueprintType)
class MYPROJECT_API ABossEffect : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Root component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComponent;

	// Niagara particle system component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* NiagaraComponent;

	// Effect tag
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	FGameplayTag EffectTag;

	// Is the effect currently active?
	UPROPERTY(BlueprintReadOnly, Category = "Effect", meta = (AllowPrivateAccess = "true"))
	bool bIsActive;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Delegate for when the effect finishes
	UPROPERTY(BlueprintAssignable, Category = "Effect")
	FOnBossEffectFinished OnEffectFinished;

	// Activate the effect
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ActivateEffect();

	// Activate the effect attached to a socket
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void ActivateEffectAttachedToSocket(USceneComponent* Parent, FName SocketName);

	// Deactivate the effect
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void DeactivateEffect();

	// Check if the effect is active
	UFUNCTION(BlueprintCallable, Category = "Effect", BlueprintPure)
	bool IsActive() const { return bIsActive; }

	// Get the current effect tag
	UFUNCTION(BlueprintCallable, Category = "Effect", BlueprintPure)
	FGameplayTag GetCurrentEffectTag() const { return EffectTag; }

	// Attach the effect to the boss
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void AttachToBoss(AActor* BossActor, FName SocketName = NAME_None);

	// Attach the effect to a socket
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void AttachToSocket(USceneComponent* Parent, FName SocketName);

	// Place the effect in the world
	UFUNCTION(BlueprintCallable, Category = "Effect")
	void PlaceInWorld(FVector Location, FRotator Rotation);
};
```

*   **소스 파일 (`BossEffect.cpp`)**

```c++
#include "BossEffect.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ABossEffect::ABossEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create root component
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SetRootComponent(RootComponent);

	// Create Niagara component
	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(RootComponent);

	bIsActive = false;
}

// Called when the game starts or when spawned
void ABossEffect::BeginPlay()
{
	Super::BeginPlay();

	// Deactivate the effect on start
	DeactivateEffect();
}

// Called every frame
void ABossEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossEffect::ActivateEffect()
{
	if (NiagaraComponent)
	{
		NiagaraComponent->Activate();
		bIsActive = true;
	}
}

void ABossEffect::ActivateEffectAttachedToSocket(USceneComponent* Parent, FName SocketName)
{
	if (NiagaraComponent && Parent)
	{
		AttachToSocket(Parent, SocketName);
		ActivateEffect();
	}
}

void ABossEffect::DeactivateEffect()
{
	if (NiagaraComponent)
	{
		NiagaraComponent->Deactivate();
		bIsActive = false;
		OnEffectFinished.Broadcast(this);
	}
}

void ABossEffect::AttachToBoss(AActor* BossActor, FName SocketName)
{
	if (BossActor)
	{
		USceneComponent* BossRoot = BossActor->GetRootComponent();
		if (BossRoot)
		{
			AttachToSocket(BossRoot, SocketName);
		}
	}
}

void ABossEffect::AttachToSocket(USceneComponent* Parent, FName SocketName)
{
	if (Parent)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		AttachToComponent(Parent, AttachmentRules, SocketName);
	}
}

void ABossEffect::PlaceInWorld(FVector Location, FRotator Rotation)
{
	SetActorLocation(Location);
	SetActorRotation(Rotation);
}
```

**설명:**

*   `RootComponent`: 이펙트의 위치 및 회전을 정의하는 루트 컴포넌트입니다.
*   `NiagaraComponent`: Niagara 파티클 시스템을 관리하는 컴포넌트입니다.
*   `EffectTag`: 이펙트를 식별하는 데 사용되는 GameplayTag입니다.
*   `bIsActive`: 이펙트가 활성화되었는지 여부를 나타내는 불리언 변수입니다.
*   `OnEffectFinished`: 이펙트가 완료되었을 때 호출되는 델리게이트입니다.
*   `ActivateEffect()`: 이펙트를 활성화합니다. Niagara 파티클 시스템을 시작하고 `bIsActive`를 true로 설정합니다.
*   `ActivateEffectAttachedToSocket()`: 지정된 소켓에 이펙트를 부착하고 활성화합니다.
*   `DeactivateEffect()`: 이펙트를 비활성화합니다. Niagara 파티클 시스템을 중지하고 `bIsActive`를 false로 설정합니다.
*   `AttachToBoss()`: 이펙트를 보스 액터의 지정된 소켓에 부착합니다.
*   `AttachToSocket()`: 이펙트를 지정된 컴포넌트의 소켓에 부착합니다.
*   `PlaceInWorld()`: 이펙트를 지정된 위치와 회전으로 월드에 배치합니다.

#### 2.2 `ABossManager` 클래스 구현

**설명:** 보스 스폰 및 관리

*   **헤더 파일 (`BossManager.h`)**

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "AIController.h"
#include "StateTreeComponent.h"
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

	// Trigger box for boss encounter
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* TriggerBox;

	// Boss class to spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
	TSubclassOf<APawn> BossClass;

	// Spawned boss instance
	UPROPERTY(BlueprintReadOnly, Category = "Boss", meta = (AllowPrivateAccess = "true"))
	APawn* SpawnedBoss;

	// AI Controller
	UPROPERTY(BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	AAIController* BossAIController;

	// State Tree Component
	UPROPERTY(BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UStateTreeComponent* BossStateTreeComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Reset the boss completely
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void ResetBossCompletely();

	// Open the door (example function)
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void OpenDoor();

	// Find the boss in the world
	UFUNCTION(BlueprintCallable, Category = "Boss")
	APawn* FindBossInWorld();

protected:
	// Reset all boss components
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void ResetAllBossComponents();

	// Reset the boss state tree
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void ResetBossStateTree();

	// Called when the trigger box is overlapped
	UFUNCTION()
	void OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
```

*   **소스 파일 (`BossManager.cpp`)**

```c++
#include "BossManager.h"
#include "Components/BoxComponent.h"
#include "AIController.h"
#include "StateTreeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossManager::ABossManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create trigger box
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	SetRootComponent(TriggerBox);
	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ABossManager::OnTriggerBoxOverlapBegin);
}

// Called when the game starts or when spawned
void ABossManager::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABossManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossManager::ResetBossCompletely()
{
	// Find and destroy the existing boss
	APawn* ExistingBoss = FindBossInWorld();
	if (ExistingBoss)
	{
		ExistingBoss->Destroy();
	}

	// Spawn a new boss
	if (BossClass)
	{
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		SpawnedBoss = GetWorld()->SpawnActor<APawn>(BossClass, SpawnLocation, SpawnRotation);

		// Get AI Controller
		BossAIController = Cast<AAIController>(SpawnedBoss->GetController());
		if (BossAIController)
		{
			BossStateTreeComponent = BossAIController->FindComponentByClass<UStateTreeComponent>();
		}
	}

	// Reset all components and state tree
	ResetAllBossComponents();
	ResetBossStateTree();
}

void ABossManager::OpenDoor()
{
	// Example function to open a door
}

APawn* ABossManager::FindBossInWorld()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawn::StaticClass(), FoundActors);

	for (AActor* Actor : FoundActors)
	{
		APawn* Pawn = Cast<APawn>(Actor);
		if (Pawn && Pawn->IsA(BossClass))
		{
			return Pawn;
		}
	}

	return nullptr;
}

void ABossManager::ResetAllBossComponents()
{
	// Find all components that need to be reset and call their reset functions
	if (SpawnedBoss)
	{
		TArray<UActorComponent*> Components;
		SpawnedBoss->GetComponents(Components);

		for (UActorComponent* Component : Components)
		{
			// Example: Reset a custom component
			// UMyCustomComponent* CustomComponent = Cast<UMyCustomComponent>(Component);
			// if (CustomComponent)
			// {
			// 	CustomComponent->ResetComponent();
			// }
		}
	}
}

void ABossManager::ResetBossStateTree()
{
	if (BossStateTreeComponent)
	{
		BossStateTreeComponent->Reset();
		BossStateTreeComponent->Start();
	}
}

void ABossManager::OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Example: Start the boss encounter when the player enters the trigger box
	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag(FName("Player")))
	{
		ResetBossCompletely();
	}
}
```

**설명:**

*   `TriggerBox`: 플레이어가 보스 영역에 진입했는지 감지하는 데 사용되는 박스 컴포넌트입니다.
*   `BossClass`: 스폰할 보스 폰 클래스입니다.
*   `SpawnedBoss`: 스폰된 보스 폰의 인스턴스입니다.
*   `BossAIController`: 보스의 AI 컨트롤러입니다.
*   `BossStateTreeComponent`: 보스의 StateTree 컴포넌트입니다.
*   `ResetBossCompletely()`: 보스를 완전 초기화합니다. 기존 보스를 파괴하고 새 보스를 스폰합니다.
*   `OpenDoor()`: 문을 여는 예제 함수입니다.
*   `FindBossInWorld()`: 월드에서 보스 폰을 찾습니다.
*   `ResetAllBossComponents()`: 보스의 모든 컴포넌트를 초기화합니다.
*   `ResetBossStateTree()`: 보스의 StateTree를 초기화합니다.
*   `OnTriggerBoxOverlapBegin()`: 트리거 박스에 겹침이 시작될 때 호출됩니다. 플레이어가 트리거 박스에 들어가면 보스 전투를 시작합니다.

#### 2.3 `ABossProjectileActor` 클래스 구현

**설명:** 보스 투사체 액터

*   **헤더 파일 (`BossProjectileActor.h`)**

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "BossProjectileActor.generated.h"

UCLASS()
class MYPROJECT_API ABossProjectileActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossProjectileActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Static mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	// Projectile movement component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	// Spawn effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* SpawnEffect;

	// Destroy effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* DestroyEffect;

	// Collision component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Fire the projectile
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void FireProjectile(FVector Direction, float Speed);

	// Fire the projectile to a specific location
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void FireProjectileToLocation(FVector TargetLocation, float Speed);

	// Play the spawn effect
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void PlaySpawnEffect();

	// Play the destroy effect
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void PlayDestroyEffect();

	// Called when the projectile hits something
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
```

*   **소스 파일 (`BossProjectileActor.cpp`)**

```c++
#include "BossProjectileActor.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossProjectileActor::ABossProjectileActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create static mesh component
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	SetRootComponent(ProjectileMesh);

	// Create projectile movement component
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 1000.0f;
	ProjectileMovement->MaxSpeed = 2000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	// Create collision component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetSphereRadius(25.0f);
	CollisionComponent->OnComponentHit.AddDynamic(this, &ABossProjectileActor::OnProjectileHit);
}

// Called when the game starts or when spawned
void ABossProjectileActor::BeginPlay()
{
	Super::BeginPlay();

	// Play spawn effect
	PlaySpawnEffect();
}

// Called every frame
void ABossProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossProjectileActor::FireProjectile(FVector Direction, float Speed)
{
	if (ProjectileMovement)
	{
		ProjectileMovement->Velocity = Direction * Speed;
	}
}

void ABossProjectileActor::FireProjectileToLocation(FVector TargetLocation, float Speed)
{
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	FireProjectile(Direction, Speed);
}

void ABossProjectileActor::PlaySpawnEffect()
{
	if (SpawnEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, GetActorTransform());
	}
}

void ABossProjectileActor::PlayDestroyEffect()
{
	if (DestroyEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DestroyEffect, GetActorTransform());
	}
}

void ABossProjectileActor::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Play destroy effect
	PlayDestroyEffect();

	// Destroy the projectile
	Destroy();
}
```

**설명:**

*   `ProjectileMesh`: 투사체의 시각적 표현을 위한 스태틱 메시 컴포넌트입니다.
*   `ProjectileMovement`: 투사체의 움직임을 제어하는 투사체 이동 컴포넌트입니다.
*   `SpawnEffect`: 투사체가 스폰될 때 재생되는 파티클 시스템입니다.
*   `DestroyEffect`: 투사체가 파괴될 때 재생되는 파티클 시스템입니다.
*   `CollisionComponent`: 투사체의 충돌을 감지하는 스피어 컴포넌트입니다.
*   `FireProjectile()`: 지정된 방향과 속도로 투사체를 발사합니다.
*   `FireProjectileToLocation()`: 지정된 위치로 투사체를 발사합니다.
*   `PlaySpawnEffect()`: 스폰 이펙트를 재생합니다.
*   `PlayDestroyEffect()`: 파괴 이펙트를 재생합니다.
*   `OnProjectileHit()`: 투사체가 무언가에 부딪혔을 때 호출됩니다. 파괴 이펙트를 재생하고 투사체를 파괴합니다.

#### 2.4 `ABossProjectileOrb` 클래스 구현

**설명:** 보스 투사체 오브

*   **헤더 파일 (`BossProjectileOrb.h`)**

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "BossProjectileOrb.generated.h"

UCLASS()
class MYPROJECT_API ABossProjectileOrb : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossProjectileOrb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Static mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* OrbMesh;

	// Sphere collision component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

	// Spawn effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* SpawnEffect;

	// Return to pool effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* ReturnToPoolEffect;

	// Collision effect
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Effects")
	UParticleSystem* CollisionEffect;

	// Spawn sound
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	USoundBase* SpawnSound;

	// Return to pool sound
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	USoundBase* ReturnToPoolSound;

	// Collision sound
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sounds")
	USoundBase* CollisionSound;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Spawn a projectile
	UFUNCTION(BlueprintCallable, Category = "Orb")
	void SpawnProjectile();

	// Destroy the orb
	UFUNCTION(BlueprintCallable, Category = "Orb")
	void DestroyOrb();

	// Called when the orb overlaps with something
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Play the spawn sound
	UFUNCTION(BlueprintCallable, Category = "Sounds")
	void PlaySpawnSound();

	// Play the return to pool sound
	UFUNCTION(BlueprintCallable, Category = "Sounds")
	void PlayReturnToPoolSound();

	// Play the collision sound
	UFUNCTION(BlueprintCallable, Category = "Sounds")
	void PlayCollisionSound();

	// Play the spawn effect
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void PlaySpawnEffect();

	// Play the return to pool effect
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void PlayReturnToPoolEffect();

	// Play the collision effect
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void PlayCollisionEffect();

	// Destroy the orb with a delay
	UFUNCTION(BlueprintCallable, Category = "Orb")
	void DestroyOrbWithDelay(float Delay);

	// Activate the orb
	UFUNCTION(BlueprintCallable, Category = "Orb")
	void ActivateOrb();
};
```

*   **소스 파일 (`BossProjectileOrb.cpp`)**

```c++
#include "BossProjectileOrb.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABossProjectileOrb::ABossProjectileOrb()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create static mesh component
	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbMesh"));
	SetRootComponent(OrbMesh);

	// Create sphere collision component
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetSphereRadius(50.0f);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossProjectileOrb::OnOverlap);
}

// Called when the game starts or when spawned
void ABossProjectileOrb::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABossProjectileOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABossProjectileOrb::SpawnProjectile()
{
	// Logic to spawn a projectile
}

void ABossProjectileOrb::DestroyOrb()
{
	// Logic to destroy the orb
	Destroy();
}

void ABossProjectileOrb::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Logic to handle overlap events
}

void ABossProjectileOrb::PlaySpawnSound()
{
	if (SpawnSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation());
	}
}

void ABossProjectileOrb::PlayReturnToPoolSound()
{
	if (ReturnToPoolSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ReturnToPoolSound, GetActorLocation());
	}
}

void ABossProjectileOrb::PlayCollisionSound()
{
	if (CollisionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CollisionSound, GetActorLocation());
	}
}

void ABossProjectileOrb::PlaySpawnEffect()
{
	if (SpawnEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, GetActorTransform());
	}
}

void ABossProjectileOrb::PlayReturnToPoolEffect()
{
	if (ReturnToPoolEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ReturnToPoolEffect, GetActorTransform());
	}
}

void ABossProjectileOrb::PlayCollisionEffect()
{
	if (CollisionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollisionEffect, GetActorTransform());
	}
}

void ABossProjectileOrb::DestroyOrbWithDelay(float Delay)
{
	FTimerHandle DestroyTimerHandle;
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ABossProjectileOrb::DestroyOrb, Delay, false);
}

void ABossProjectileOrb::ActivateOrb()
{
	// Logic to activate the orb
}
```

**설명:**

*   `OrbMesh`: 오브의 시각적 표현을 위한 스태틱 메시 컴포넌트입니다.
*   `CollisionComponent`: 오브의 충돌을 감지하는 스피어 컴포넌트입니다.
*   `SpawnEffect`: 오브가 스폰될 때 재생되는 파티클 시스템입니다.
*   `ReturnToPoolEffect`: 오브가 풀로 돌아갈 때 재생되는 파티클 시스템입니다.
*   `CollisionEffect`: 오브가 충돌할 때 재생되는 파티클 시스템입니다.
*   `SpawnSound`: 오브가 스폰될 때 재생되는 사운드입니다.
*   `ReturnToPoolSound`: 오브가 풀로 돌아갈 때 재생되는 사운드입니다.
*   `CollisionSound`: 오브가 충돌할 때 재생되는 사운드입니다.
*   `SpawnProjectile()`: 투사체를 스폰하는 로직입니다.
*   `DestroyOrb()`: 오브를 파괴하는 로직입니다.
*   `OnOverlap()`: 오브가 무언가와 겹칠 때 호출됩니다.
*   `PlaySpawnSound()`: 스폰 사운드를 재생합니다.
*   `PlayReturnToPoolSound()`: 풀로 돌아가는 사운드를 재생합니다.
*   `PlayCollisionSound()`: 충돌 사운드를 재생합니다.
*   `PlaySpawnEffect()`: 스폰 이펙트를 재생합니다.
*   `PlayReturnToPoolEffect()`: 풀로 돌아가는 이펙트를 재생합니다.
*   `PlayCollisionEffect()`: 충돌 이펙트를 재생합니다.
*   `DestroyOrbWithDelay()`: 지정된 지연 시간 후에 오브를 파괴합니다.
*   `ActivateOrb()`: 오브를 활성화하는 로직입니다.

#### 2.5 `ACBoss` 클래스 구현

**설명:** 보스 캐릭터

*   **헤더 파일 (`CBoss.h`)**

```c++
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "CBoss.generated.h"

UCLASS()
class MYPROJECT_API ACBoss : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACBoss();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Initial location of the boss
	UPROPERTY(BlueprintReadOnly, Category = "Boss", meta = (AllowPrivateAccess = "true"))
	FVector InitialLocation;

	// Boss status widget component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* BossStatusWidgetComponent