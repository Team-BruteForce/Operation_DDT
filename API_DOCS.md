```c++
// Copyright (c) 2024 DDT Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABossEffect.generated.h"

/**
 * @brief ABossEffect 클래스
 *
 * 보스 이펙트 액터는 보스 전투 중 시각적 효과를 담당하는 핵심 클래스입니다.
 * 이펙트 활성화, 비활성화, 특정 소켓에 부착, 월드에 배치 등 다양한 기능을 제공합니다.
 *
 * **주요 기능:**
 *  - 이펙트 활성화 및 비활성화
 *  - 특정 소켓에 이펙트 부착
 *  - 월드 좌표에 이펙트 배치
 *  - 이펙트 활성화 상태 확인
 *  - 현재 이펙트 태그 반환
 *
 * **사용 시나리오:**
 *  - 보스의 공격 시 이펙트 활성화
 *  - 보스의 특정 상태 변화 시 이펙트 변경
 *  - 보스의 피격 시 피격 이펙트 활성화
 *
 * **상속 관계:**
 *  - AActor
 *
 * **인터페이스:**
 *  - IInterface (필요한 경우 추가)
 *
 * **주의사항:**
 *  - 이펙트 풀링을 사용하여 성능을 최적화해야 합니다.
 *  - 이펙트 활성화/비활성화 시 불필요한 메모리 할당을 최소화해야 합니다.
 *
 * **성능 고려사항:**
 *  - 이펙트의 복잡도는 성능에 큰 영향을 미칩니다. 간단한 이펙트를 사용하는 것이 좋습니다.
 *  - 파티클 시스템의 파티클 수를 제한하여 성능을 최적화해야 합니다.
 *  - 불필요한 트랜스루슨시 사용을 최소화해야 합니다.
 *
 * **호환성 정보:**
 *  - Unreal Engine 5.0 이상
 *  - 모든 플랫폼 지원 (테스트 필요)
 *  - C++17 이상
 *
 * **트러블슈팅:**
 *  - 이펙트가 보이지 않는 경우, 이펙트의 위치와 회전을 확인하십시오.
 *  - 이펙트 성능이 저하되는 경우, 파티클 시스템의 설정을 조정하십시오.
 *  - 메모리 누수가 발생하는 경우, 이펙트 풀링을 확인하십시오.
 */
UCLASS(Blueprintable, BlueprintType)
class DDTPROJECT_API ABossEffect : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief ABossEffect 생성자
	 *
	 * 액터의 기본 속성을 초기화합니다.
	 *
	 * @param ObjectInitializer 객체 초기화자
	 */
	ABossEffect();

	/**
	 * @brief Tick 함수
	 *
	 * 매 프레임마다 호출됩니다.
	 *
	 * @param DeltaTime 프레임 간 시간 간격
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief 이펙트 활성화 델리게이트
	 *
	 * 이펙트가 활성화될 때 호출되는 델리게이트입니다.
	 *
	 * @param EffectTag 활성화된 이펙트의 태그
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectActivated, FName, EffectTag);

	/**
	 * @brief 이펙트 비활성화 델리게이트
	 *
	 * 이펙트가 비활성화될 때 호출되는 델리게이트입니다.
	 *
	 * @param EffectTag 비활성화된 이펙트의 태그
	 */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectDeactivated, FName, EffectTag);

	/**
	 * @brief 이펙트 활성화 이벤트
	 *
	 * 이펙트가 활성화될 때 브로드캐스트되는 이벤트입니다.
	 */
	UPROPERTY(BlueprintAssignable, Category = "BossEffect")
	FOnEffectActivated OnEffectActivated;

	/**
	 * @brief 이펙트 비활성화 이벤트
	 *
	 * 이펙트가 비활성화될 때 브로드캐스트되는 이벤트입니다.
	 */
	UPROPERTY(BlueprintAssignable, Category = "BossEffect")
	FOnEffectDeactivated OnEffectDeactivated;

	/**
	 * @brief 이펙트 활성화
	 *
	 * 이펙트를 활성화합니다.
	 *
	 * @param EffectTag 활성화할 이펙트의 태그
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossEffect")
	void ActivateEffect(FName EffectTag);

	/**
	 * @brief 소켓에 붙여서 이펙트 활성화
	 *
	 * 지정된 소켓에 이펙트를 부착하고 활성화합니다.
	 *
	 * @param EffectTag 활성화할 이펙트의 태그
	 * @param SocketName 부착할 소켓의 이름
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossEffect")
	void ActivateEffectAttachedToSocket(FName EffectTag, FName SocketName);

	/**
	 * @brief 이펙트 비활성화
	 *
	 * 이펙트를 비활성화합니다.
	 *
	 * @param EffectTag 비활성화할 이펙트의 태그
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossEffect")
	void DeactivateEffect(FName EffectTag);

	/**
	 * @brief 이펙트가 활성화되어 있는지 확인
	 *
	 * 지정된 이펙트가 현재 활성화되어 있는지 확인합니다.
	 *
	 * @param EffectTag 확인할 이펙트의 태그
	 * @return bool 활성화 여부 (true: 활성화됨, false: 비활성화됨)
	 */
	UFUNCTION(BlueprintCallable, Category = "BossEffect", meta = (DisplayName = "Is Effect Active"))
	bool IsActive(FName EffectTag) const;

	/**
	 * @brief 현재 이펙트 태그 반환
	 *
	 * 현재 활성화된 이펙트의 태그를 반환합니다.
	 *
	 * @return FName 현재 이펙트 태그
	 */
	UFUNCTION(BlueprintCallable, Category = "BossEffect")
	FName GetCurrentEffectTag() const;

protected:
	/**
	 * @brief BeginPlay 함수
	 *
	 * 액터가 생성된 후 게임이 시작될 때 호출됩니다.
	 *
	 * @return void
	 */
	virtual void BeginPlay() override;

private:

	/**
	 * @brief 이펙트를 보스에 어태치
	 *
	 * 이펙트를 보스 액터에 부착합니다.
	 *
	 * @param Boss 보스 액터
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossEffect")
	void AttachToBoss(AActor* Boss);

	/**
	 * @brief 소켓에 이펙트 붙이기
	 *
	 * 지정된 소켓에 이펙트를 부착합니다.
	 *
	 * @param SocketName 부착할 소켓의 이름
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossEffect")
	void AttachToSocket(FName SocketName);

	/**
	 * @brief 이펙트를 월드에 배치
	 *
	 * 지정된 위치에 이펙트를 배치합니다.
	 *
	 * @param Location 배치할 위치
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossEffect")
	void PlaceInWorld(FVector Location);

	/**
	 * @brief 현재 활성화된 이펙트의 태그를 저장합니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BossEffect", meta = (AllowPrivateAccess = "true"))
	FName CurrentEffectTag;

	/**
	 * @brief 보스 액터에 부착될 컴포넌트 (예: 파티클 시스템)를 저장합니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BossEffect", meta = (AllowPrivateAccess = "true"))
	class UParticleSystemComponent* EffectComponent; // 예시: 파티클 시스템

	/**
	 * @brief 이펙트 데이터 테이블에서 로드된 이펙트 데이터.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BossEffect", meta = (AllowPrivateAccess = "true"))
	class UDataTable* EffectDataTable;

	/**
	 * @brief 이펙트 풀.
	 */
	TArray<ABossEffect*> EffectPool;

	/**
	 * @brief 최대 풀 크기.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BossEffect", meta = (AllowPrivateAccess = "true"))
	int32 MaxPoolSize = 10;

	/**
	 * @brief 자동 풀 확장 여부.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BossEffect", meta = (AllowPrivateAccess = "true"))
	bool bAutoExpandPool = true;

	/**
	 * @brief 이펙트가 활성화되어 있는지 여부를 저장합니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BossEffect", meta = (AllowPrivateAccess = "true"))
	bool bIsActive = false;

	/**
	 * @brief 이펙트 지속 시간.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BossEffect", meta = (AllowPrivateAccess = "true"))
	float EffectDuration = 5.0f;

	/**
	 * @brief 이펙트가 부착될 소켓 이름.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BossEffect", meta = (AllowPrivateAccess = "true"))
	FName SocketName;
};

```

다음은 `ABossManager` 클래스에 대한 API 문서 작성입니다.
```c++
// Copyright (c) 2024 DDT Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "ABossManager.generated.h"

/**
 * @brief ABossManager 클래스
 *
 * 보스 매니저 액터는 보스 스폰, 초기화, 상태 관리 등 보스 전투 전반을 관리하는 핵심 클래스입니다.
 * 월드에 보스를 스폰하고, 보스의 상태를 초기화하며, 특정 트리거에 반응하여 문을 여는 등의 기능을 제공합니다.
 *
 * **주요 기능:**
 *  - 보스 스폰 및 초기화
 *  - 보스 상태 관리 (StateTree)
 *  - 특정 트리거에 반응하여 문 열기
 *  - 월드 내 보스 찾기
 *
 * **사용 시나리오:**
 *  - 게임 시작 시 보스 스폰
 *  - 보스 전투 시작 시 문 열기
 *  - 보스 사망 시 보스 초기화
 *
 * **상속 관계:**
 *  - AActor
 *
 * **인터페이스:**
 *  - IInterface (필요한 경우 추가)
 *
 * **주의사항:**
 *  - 보스 스폰 로직은 레벨 디자인에 따라 달라질 수 있습니다.
 *  - StateTree는 보스의 행동 패턴을 정의하는 데 사용됩니다.
 *  - 메모리 누수를 방지하기 위해 보스 초기화 시 모든 컴포넌트를 적절히 해제해야 합니다.
 *
 * **성능 고려사항:**
 *  - 보스 스폰 시 과도한 리소스 로딩을 방지해야 합니다.
 *  - StateTree의 복잡도는 성능에 영향을 미칩니다. 간단하고 효율적인 StateTree를 사용하는 것이 좋습니다.
 *  - 불필요한 컴포넌트 업데이트를 최소화해야 합니다.
 *
 * **호환성 정보:**
 *  - Unreal Engine 5.0 이상
 *  - 모든 플랫폼 지원 (테스트 필요)
 *  - C++17 이상
 *
 * **트러블슈팅:**
 *  - 보스가 스폰되지 않는 경우, 스폰 위치와 보스 클래스를 확인하십시오.
 *  - 보스가 예상대로 행동하지 않는 경우, StateTree를 디버깅하십시오.
 *  - 메모리 누수가 발생하는 경우, 보스 초기화 로직을 확인하십시오.
 */
UCLASS(Blueprintable, BlueprintType)
class DDTPROJECT_API ABossManager : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief ABossManager 생성자
	 *
	 * 보스 매니저 액터의 기본 속성을 초기화합니다.
	 *
	 * @param ObjectInitializer 객체 초기화자
	 */
	ABossManager();

	/**
	 * @brief Tick 함수
	 *
	 * 매 프레임마다 호출됩니다.
	 *
	 * @param DeltaTime 프레임 간 시간 간격
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief 보스 완전 초기화 (메인 함수)
	 *
	 * 보스를 완전하게 초기화합니다. 보스의 HP, 상태, 위치 등을 초기 상태로 되돌립니다.
	 * 이 함수는 보스 전투가 종료되거나 재시작될 때 호출됩니다.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	void ResetBossCompletely();

	/**
	 * @brief 문 열기
	 *
	 * 보스 전투 시작 시 문을 엽니다.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	void OpenDoor();

	/**
	 * @brief 월드에서 보스 찾기
	 *
	 * 월드에서 보스 액터를 찾습니다.
	 *
	 * @return ACBoss* 찾은 보스 액터 (nullptr: 찾지 못함)
	 */
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	ACBoss* FindBossInWorld();

protected:

	/**
	 * @brief BeginPlay 함수
	 *
	 * 액터가 생성된 후 게임이 시작될 때 호출됩니다.
	 *
	 * @return void
	 */
	virtual void BeginPlay() override;

private:

	/**
	 * @brief 콜리전 오버랩 시작 이벤트
	 *
	 * 트리거 박스와 다른 액터가 겹치기 시작할 때 호출됩니다.
	 *
	 * @param OverlappedComponent 오버랩된 컴포넌트
	 * @param OtherActor 오버랩된 다른 액터
	 * @param OtherComp 오버랩된 다른 컴포넌트
	 * @param OtherBodyIndex 기타 바디 인덱스
	 * @param bFromSweep 스윕에서 왔는지 여부
	 * @param SweepResult 스윕 결과
	 * @return void
	 */
	UFUNCTION()
	void OnTriggerBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * @brief 각 컴포넌트별 초기화 실행
	 *
	 * 보스의 각 컴포넌트를 초기화합니다. (예: HP, 상태, 무기)
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	void ResetAllBossComponents();

	/**
	 * @brief StateTree 완전 리스타트
	 *
	 * 보스의 StateTree를 완전히 리스타트합니다.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossManager")
	void ResetBossStateTree();

	/**
	 * @brief 스폰할 보스 클래스
	 *
	 * 스폰할 보스 액터의 클래스입니다.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BossManager")
	TSubclassOf<ACBoss> BossClass;

	/**
	 * @brief 보스 스폰 위치
	 *
	 * 보스 액터가 스폰될 위치입니다.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BossManager")
	FVector BossSpawnLocation;

	/**
	 * @brief 보스 액터
	 *
	 * 현재 월드에 스폰된 보스 액터입니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BossManager")
	ACBoss* SpawnedBoss;

	/**
	 * @brief 문 액터
	 *
	 * 보스 전투 시작 시 열릴 문 액터입니다.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BossManager")
	AActor* DoorActor;

	/**
	 * @brief 트리거 박스
	 *
	 * 플레이어가 이 박스에 들어가면 보스 전투가 시작됩니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BossManager", meta=(AllowPrivateAccess = "true"))
	UBoxComponent* TriggerBox;

	/**
	 * @brief 보스 전투 시작 여부
	 *
	 * 보스 전투가 시작되었는지 여부를 나타냅니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BossManager")
	bool bBossFightStarted = false;

	/**
	 * @brief 보스 초기 HP
	 *
	 * 보스의 초기 HP 값입니다.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BossManager")
	float InitialBossHP = 1000.0f;

	/**
	 * @brief 보스 AI 컨트롤러 클래스
	 *
	 * 보스 AI 컨트롤러의 클래스입니다.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BossManager")
	TSubclassOf<ACBossAIC> BossAIControllerClass;

	/**
	 * @brief 보스 AI 컨트롤러
	 *
	 * 현재 월드에 스폰된 보스 AI 컨트롤러입니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "BossManager")
	ACBossAIC* SpawnedBossAIController;
};
```

이어서 `ABossProjectileActor` 클래스에 대한 API 문서를 작성하겠습니다.
```c++
// Copyright (c) 2024 DDT Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "ABossProjectileActor.generated.h"

/**
 * @brief ABossProjectileActor 클래스
 *
 * 보스 프로젝타일 액터는 보스가 발사하는 투사체를 구현하는 기본 클래스입니다.
 * 투사체의 발사, 이동, 충돌 처리, 이펙트 재생 등의 기능을 제공합니다.
 *
 * **주요 기능:**
 *  - 투사체 발사 (FireProjectile, FireProjectileToLocation)
 *  - 투사체 이동
 *  - 투사체 충돌 처리 (OnProjectileHit)
 *  - 스폰 및 파괴 이펙트 재생 (PlaySpawnEffect, PlayDestroyEffect)
 *
 * **사용 시나리오:**
 *  - 보스의 원거리 공격 패턴 구현
 *  - 특정 위치로 투사체 발사
 *  - 투사체 충돌 시 데미지 적용 및 이펙트 재생
 *
 * **상속 관계:**
 *  - AActor
 *
 * **인터페이스:**
 *  - IInterface (필요한 경우 추가)
 *
 * **주의사항:**
 *  - 투사체의 이동 속도, 데미지, 이펙트는 데이터 테이블 또는 에셋으로 관리하는 것이 좋습니다.
 *  - 투사체 풀링을 사용하여 성능을 최적화해야 합니다.
 *  - 투사체의 충돌 처리는 정확하고 효율적으로 구현해야 합니다.
 *
 * **성능 고려사항:**
 *  - 투사체의 복잡도는 성능에 큰 영향을 미칩니다. 간단한 메시와 이펙트를 사용하는 것이 좋습니다.
 *  - 투사체의 이동 거리를 제한하여 불필요한 연산을 줄여야 합니다.
 *  - 투사체 풀링을 통해 메모리 할당 및 해제 비용을 줄여야 합니다.
 *
 * **호환성 정보:**
 *  - Unreal Engine 5.0 이상
 *  - 모든 플랫폼 지원 (테스트 필요)
 *  - C++17 이상
 *
 * **트러블슈팅:**
 *  - 투사체가 발사되지 않는 경우, 발사 위치와 방향, 이동 컴포넌트 설정을 확인하십시오.
 *  - 투사체가 예상대로 이동하지 않는 경우, 이동 컴포넌트의 설정을 조정하십시오.
 *  - 투사체 충돌이 제대로 처리되지 않는 경우, 충돌 컴포넌트의 설정을 확인하십시오.
 */
UCLASS(Blueprintable, BlueprintType)
class DDTPROJECT_API ABossProjectileActor : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief ABossProjectileActor 생성자
	 *
	 * 투사체 액터의 기본 속성을 초기화합니다.
	 *
	 * @param ObjectInitializer 객체 초기화자
	 */
	ABossProjectileActor();

	/**
	 * @brief Tick 함수
	 *
	 * 매 프레임마다 호출됩니다.
	 *
	 * @param DeltaTime 프레임 간 시간 간격
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief 투사체 발사
	 *
	 * 지정된 방향으로 투사체를 발사합니다.
	 *
	 * @param Direction 발사 방향 (단위 벡터)
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossProjectile")
	void FireProjectile(FVector Direction);

	/**
	 * @brief 특정 위치로 투사체 발사
	 *
	 * 지정된 위치로 투사체를 발사합니다.
	 *
	 * @param TargetLocation 목표 위치
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossProjectile")
	void FireProjectileToLocation(FVector TargetLocation);

	/**
	 * @brief 스폰 이펙트 재생
	 *
	 * 투사체가 스폰될 때 재생되는 이펙트를 재생합니다.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossProjectile")
	void PlaySpawnEffect();

	/**
	 * @brief 파괴 이펙트 재생
	 *
	 * 투사체가 파괴될 때 재생되는 이펙트를 재생합니다.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossProjectile")
	void PlayDestroyEffect();

protected:
	/**
	 * @brief BeginPlay 함수
	 *
	 * 액터가 생성된 후 게임이 시작될 때 호출됩니다.
	 *
	 * @return void
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief 투사체 충돌 이벤트 처리
	 *
	 * 투사체가 다른 액터와 충돌했을 때 호출됩니다.
	 *
	 * @param HitActor 충돌된 액터
	 * @param HitComponent 충돌된 컴포넌트
	 * @return void
	 */
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
	/**
	 * @brief 투사체 이동 컴포넌트
	 *
	 * 투사체의 이동을 담당하는 컴포넌트입니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	/**
	 * @brief 투사체 메시 컴포넌트
	 *
	 * 투사체의 시각적 표현을 담당하는 메시 컴포넌트입니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	/**
	 * @brief 충돌 컴포넌트
	 *
	 * 투사체의 충돌을 감지하는 컴포넌트입니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

	/**
	 * @brief 투사체 데미지
	 *
	 * 투사체가 충돌 시 가하는 데미지입니다.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float Damage = 10.0f;

	/**
	 * @brief 스폰 이펙트
	 *
	 * 투사체가 스폰될 때 재생되는 파티클 시스템입니다.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	UParticleSystem* SpawnEffect;

	/**
	 * @brief 파괴 이펙트
	 *
	 * 투사체가 파괴될 때 재생되는 파티클 시스템입니다.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	UParticleSystem* DestroyEffect;

	/**
	 * @brief 투사체 수명
	 *
	 * 투사체가 존재할 수 있는 최대 시간입니다.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float LifeSpan = 5.0f;

	/**
	 * @brief 투사체 속도
	 *
	 * 투사체의 이동 속도입니다.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float InitialSpeed = 1000.0f;
};
```

다음 클래스인 `ABossProjectileOrb`에 대한 API 문서를 작성해주세요.
```c++
// Copyright (c) 2024 DDT Company. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "ABossProjectileOrb.generated.h"

/**
 * @brief ABossProjectileOrb 클래스
 *
 * 보스 프로젝타일 오브 액터는 보스가 발사하는 구체 형태의 투사체를 구현합니다.
 * 특정 위치에 오브를 스폰하고, 오브가 플레이어에게 접근하거나 일정 시간이 지나면 파괴되는 로직을 포함합니다.
 * 오브는 스폰, 충돌, 회수 시 각각 다른 사운드와 이펙트를 재생합니다.
 *
 * **주요 기능:**
 *  - 오브 스폰 (SpawnProjectile)
 *  - 오브 파괴 (DestroyOrb)
 *  - 플레이어와의 충돌 감지 (OnOverlap)
 *  - 스폰, 충돌, 회수 사운드 및 이펙트 재생 (PlaySpawnSound, PlayCollisionSound, PlayReturnToPoolSound, PlaySpawnEffect, PlayCollisionEffect, PlayReturnToPoolEffect)
 *  - 오브 활성화 및 비활성화 (ActivateOrb)
 *
 * **사용 시나리오:**
 *  - 보스의 특정 패턴 공격 시 오브 스폰
 *  - 플레이어에게 접근하는 오브를 피하는 게임 플레이
 *  - 오브 충돌 시 데미지 적용 및 이펙트 재생
 *
 * **상속 관계:**
 *  - AActor
 *
 * **인터페이스:**
 *  - IInterface (필요한 경우 추가)
 *
 * **주의사항:**
 *  - 오브의 이동 속도, 데미지, 사운드, 이펙트는 데이터 테이블 또는 에셋으로 관리하는 것이 좋습니다.
 *  - 오브 풀링을 사용하여 성능을 최적화해야 합니다.
 *  - 오브의 충돌 처리는 정확하고 효율적으로 구현해야 합니다.
 *
 * **성능 고려사항:**
 *  - 오브의 복잡도는 성능에 큰 영향을 미칩니다. 간단한 메시와 이펙트를 사용하는 것이 좋습니다.
 *  - 오브의 이동 거리를 제한하여 불필요한 연산을 줄여야 합니다.
 *  - 오브 풀링을 통해 메모리 할당 및 해제 비용을 줄여야 합니다.
 *  - 사운드 재생 시 과도한 동시 재생을 피해야 합니다.
 *
 * **호환성 정보:**
 *  - Unreal Engine 5.0 이상
 *  - 모든 플랫폼 지원 (테스트 필요)
 *  - C++17 이상
 *
 * **트러블슈팅:**
 *  - 오브가 스폰되지 않는 경우, 스폰 위치와 오브 클래스를 확인하십시오.
 *  - 오브가 예상대로 이동하지 않는 경우, 이동 로직을 확인하십시오.
 *  - 오브 충돌이 제대로 처리되지 않는 경우, 충돌 컴포넌트의 설정을 확인하십시오.
 *  - 사운드가 재생되지 않는 경우, 오디오 컴포넌트 설정을 확인하십시오.
 */
UCLASS(Blueprintable, BlueprintType)
class DDTPROJECT_API ABossProjectileOrb : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief ABossProjectileOrb 생성자
	 *
	 * 오브 액터의 기본 속성을 초기화합니다.
	 *
	 * @param ObjectInitializer 객체 초기화자
	 */
	ABossProjectileOrb();

	/**
	 * @brief Tick 함수
	 *
	 * 매 프레임마다 호출됩니다.
	 *
	 * @param DeltaTime 프레임 간 시간 간격
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief 투사체 스폰
	 *
	 * 투사체를 스폰합니다.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void SpawnProjectile();

	/**
	 * @brief 오브 파괴
	 *
	 * 오브를 파괴합니다.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void DestroyOrb();

protected:
	/**
	 * @brief BeginPlay 함수
	 *
	 * 액터가 생성된 후 게임이 시작될 때 호출됩니다.
	 *
	 * @return void
	 */
	virtual void BeginPlay() override;

private:

	/**
	 * @brief 오버랩 이벤트 처리
	 *
	 * 다른 액터와 오버랩되었을 때 호출됩니다.
	 *
	 * @param OverlappedComponent 오버랩된 컴포넌트
	 * @param OtherActor 오버랩된 다른 액터
	 * @param OtherComp 오버랩된 다른 컴포넌트
	 * @param OtherBodyIndex 기타 바디 인덱스
	 * @param bFromSweep 스윕에서 왔는지 여부
	 * @param SweepResult 스윕 결과
	 * @return void
	 */
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * @brief 스폰 사운드 재생
	 *
	 * 오브가 스폰될 때 재생되는 사운드를 재생합니다.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void PlaySpawnSound();

	/**
	 * @brief 풀로 반환 사운드 재생
	 *
	 * 오브가 풀로 반환될 때 재생되는 사운드를 재생합니다.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void PlayReturnToPoolSound();

	/**
	 * @brief 충돌 사운드 재생
	 *
	 * 오브가 충돌했을 때 재생되는 사운드를 재생합니다.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void PlayCollisionSound();

	/**
	 * @brief 스폰 이펙트 재생
	 *
	 * 오브가 스폰될 때 재생되는 이펙트를 재생합니다.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void PlaySpawnEffect();

	/**
	 * @brief 풀로 반환 이펙트 재생
	 *
	 * 오브가 풀로 반환될 때 재생되는 이펙트를 재생합니다.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void PlayReturnToPoolEffect();

	/**
	 * @brief 충돌 이펙트 재생
	 *
	 * 오브가 충돌했을 때 재생되는 이펙트를 재생합니다.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void PlayCollisionEffect();

	/**
	 * @brief 지연 후 오브 파괴
	 *
	 * 지정된 시간 후에 오브를 파괴합니다.
	 *
	 * @param Delay 파괴 지연 시간 (초)
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void DestroyOrbWithDelay(float Delay);

	/**
	 * @brief 오브 활성화
	 *
	 * 오브를 활성화합니다.
	 *
	 * @return void
	 */
	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void ActivateOrb();

	/**
	 * @brief 오브 메시 컴포넌트
	 *
	 * 오브의 시각적 표현을 담당하는 메시 컴포넌트입니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* OrbMesh;

	/**
	 * @brief 충돌 컴포넌트
	 *
	 * 오브의 충돌을 감지하는 컴포넌트입니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

	/**
	 * @brief 스폰 사운드
	 *
	 * 오브가 스폰될 때 재생되는 사운드입니다.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	USoundBase* SpawnSound;

	/**
	 * @brief 풀로 반환 사운드
	 *
	 * 오브가 풀로 반환될 때 재생되는 사운드입니다.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	USoundBase* ReturnToPoolSound;

	/**
	 * @brief 충돌

## ABossProjectileOrb 상세 API 문서

**클래스 개요:**

`ABossProjectileOrb` 클래스는 보스 몬스터가 사용하는 투사체 오브젝트를 나타냅니다. 이 오브젝트는 다양한 형태로 스폰되어 플레이어를 공격하며, 특정 조건에 따라 파괴되거나 풀(Pool)로 반환되어 재사용될 수 있습니다. 오브의 외형, 충돌 설정, 사운드, 이펙트 등을 제어하며, 보스의 공격 패턴을 다양하게 만드는 데 중요한 역할을 합니다.

**상속 관계:**

*   `AActor`
    *   `ABossProjectileOrb`

**주요 기능:**

*   오브 활성화 및 비활성화
*   오브 파괴 (즉시 또는 지연 후)
*   오브 충돌 처리
*   스폰, 풀 반환, 충돌 시 사운드 및 이펙트 재생
*   오브 풀링 시스템을 통한 메모리 관리

**사용 시나리오:**

*   보스 몬스터의 원거리 공격 패턴 구현
*   특정 조건을 만족했을 때 오브를 스폰하여 플레이어에게 위협을 가함
*   오브젝트 풀링을 통해 오브젝트 생성 및 파괴에 따른 성능 저하 방지

**주의사항:**

*   오브의 스폰 및 파괴 로직은 보스 AI와 긴밀하게 연동되어야 합니다.
*   오브젝트 풀링 시스템을 올바르게 사용하지 않으면 메모리 누수가 발생할 수 있습니다.
*   충돌 설정은 게임 디자인에 따라 신중하게 결정해야 합니다.

**성능 고려사항:**

*   오브젝트 풀링을 통해 오브젝트 생성 및 파괴 비용을 최소화합니다.
*   불필요한 Tick 함수 호출을 줄여 성능을 최적화합니다.
*   충돌 검사는 필요한 경우에만 활성화하여 성능을 향상시킵니다.
*   이펙트 및 사운드 재생 시 과도한 리소스 사용을 피합니다.

### 함수 상세 설명

#### `ABossProjectileOrb()`

```cpp
ABossProjectileOrb();
```

**기능:** `ABossProjectileOrb` 클래스의 생성자입니다.

**설명:** 오브젝트의 기본 속성을 초기화합니다.

**매개변수:** 없음

**반환값:** 없음

**사용 예제:**

```cpp
ABossProjectileOrb::ABossProjectileOrb()
{
	// 컴포넌트 초기화
	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbMesh"));
	RootComponent = OrbMesh;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));

	// 이벤트 바인딩
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossProjectileOrb::OnOverlap);

	// 변수 초기화
	bReplicates = true;
	SetReplicatingMovement(true);
}
```

**성능:** 생성자이므로 초기화 단계에서만 실행됩니다. 성능에 큰 영향을 미치지 않습니다.

#### `virtual void Tick(float DeltaTime) override`

```cpp
virtual void Tick(float DeltaTime) override;
```

**기능:** 매 프레임마다 호출되는 함수입니다.

**설명:** 오브의 움직임, 상태 업데이트 등을 처리합니다.

**매개변수:**

*   `DeltaTime`: 프레임 간의 시간 간격입니다.

**반환값:** 없음

**사용 예제:**

```cpp
void ABossProjectileOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 오브의 움직임 로직 (예: 특정 방향으로 이동)
	FVector NewLocation = GetActorLocation() + (MovementDirection * MovementSpeed * DeltaTime);
	SetActorLocation(NewLocation);
}
```

**성능:** Tick 함수는 매 프레임마다 실행되므로 성능에 영향을 미칠 수 있습니다. 불필요한 연산을 최소화하고, 필요한 경우 Tick 함수를 비활성화하는 것이 좋습니다.

#### `UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb") void SpawnProjectile()`

```cpp
UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
void SpawnProjectile();
```

**기능:** 투사체를 스폰합니다.

**설명:**  오브젝트 풀에서 투사체를 가져와 월드에 배치합니다. 스폰 위치, 방향, 속도 등은 설정에 따라 달라질 수 있습니다.

**매개변수:** 없음

**반환값:** 없음

**사용 예제:**

```cpp
void ABossProjectileOrb::SpawnProjectile()
{
	// 투사체 스폰 로직
	if (ProjectileClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// 월드에 투사체 스폰
		ABossProjectileActor* Projectile = GetWorld()->SpawnActor<ABossProjectileActor>(ProjectileClass, GetActorLocation(), GetActorRotation(), SpawnParams);

		if (Projectile)
		{
			// 투사체 발사 (예: 특정 방향으로)
			Projectile->FireProjectile(GetActorForwardVector());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass is not set!"));
	}
}
```

**성능:** 투사체 스폰은 오브젝트 생성 및 초기화 비용이 발생하므로, 오브젝트 풀링을 사용하여 성능을 최적화하는 것이 좋습니다.

#### `UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb") void DestroyOrb()`

```cpp
UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
void DestroyOrb();
```

**기능:** 오브를 즉시 파괴합니다.

**설명:** 오브를 월드에서 제거하고, 오브젝트 풀로 반환합니다 (풀링 시스템을 사용하는 경우).

**매개변수:** 없음

**반환값:** 없음

**사용 예제:**

```cpp
void ABossProjectileOrb::DestroyOrb()
{
	// 파괴 이펙트 재생
	PlayDestroyEffect();

	// 파괴 사운드 재생
	PlayCollisionSound();

	// 오브젝트 풀로 반환
	PlayReturnToPoolSound();

	// 오브젝트 비활성화 및 풀로 반환
	DeactivateOrb();
}
```

**성능:** 오브젝트 파괴는 메모리 해제 비용이 발생하므로, 오브젝트 풀링을 사용하여 성능을 최적화하는 것이 좋습니다.

#### `UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb") void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)`

```cpp
UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
```

**기능:** 오브가 다른 액터와 겹쳤을 때 호출되는 함수입니다.

**설명:** 충돌 처리 로직을 구현합니다. 예를 들어, 플레이어와 충돌했을 때 데미지를 주거나, 특정 이펙트를 재생할 수 있습니다.

**매개변수:**

*   `OverlappedComponent`: 충돌한 컴포넌트입니다 (이 오브의 컴포넌트).
*   `OtherActor`: 충돌한 다른 액터입니다.
*   `OtherComp`: 충돌한 다른 액터의 컴포넌트입니다.
*   `OtherBodyIndex`: 충돌한 다른 액터의 바디 인덱스입니다.
*   `bFromSweep`: 스윕 충돌인지 여부입니다.
*   `SweepResult`: 스윕 충돌 결과입니다.

**반환값:** 없음

**사용 예제:**

```cpp
void ABossProjectileOrb::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어와 충돌했을 때
	if (OtherActor != this && OtherActor->IsA(ACharacter::StaticClass()))
	{
		// 데미지 적용
		UGameplayStatics::ApplyPointDamage(OtherActor, DamageAmount, (SweepResult.TraceStart - SweepResult.TraceEnd), SweepResult, GetInstigatorController(), this, DamageType);

		// 충돌 이펙트 재생
		PlayCollisionEffect();

		// 오브 파괴
		DestroyOrb();
	}
}
```

**성능:** 충돌 검사는 성능에 영향을 미칠 수 있습니다. 불필요한 충돌 검사를 피하고, 충돌 채널 및 반응 설정을 최적화하는 것이 좋습니다.

#### `UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb") void PlaySpawnSound()`

```cpp
UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
void PlaySpawnSound();
```

**기능:** 오브가 스폰될 때 사운드를 재생합니다.

**설명:** `SpawnSound` 변수에 지정된 사운드 에셋을 재생합니다.

**매개변수:** 없음

**반환값:** 없음

**사용 예제:**

```cpp
void ABossProjectileOrb::PlaySpawnSound()
{
	// 스폰 사운드 재생
	if (SpawnSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation());
	}
}
```

**성능:** 사운드 재생은 CPU 및 메모리 리소스를 사용합니다. 과도한 사운드 재생을 피하고, 사운드 에셋을 최적화하는 것이 좋습니다.

#### `UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb") void PlayReturnToPoolSound()`

```cpp
UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
void PlayReturnToPoolSound();
```

**기능:** 오브가 풀로 반환될 때 사운드를 재생합니다.

**설명:** `ReturnToPoolSound` 변수에 지정된 사운드 에셋을 재생합니다.

**매개변수:** 없음

**반환값:** 없음

**사용 예제:**

```cpp
void ABossProjectileOrb::PlayReturnToPoolSound()
{
	// 풀 반환 사운드 재생
	if (ReturnToPoolSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ReturnToPoolSound, GetActorLocation());
	}
}
```

**성능:** 사운드 재생은 CPU 및 메모리 리소스를 사용합니다. 과도한 사운드 재생을 피하고, 사운드 에셋을 최적화하는 것이 좋습니다.

#### `UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb") void PlayCollisionSound()`

```cpp
UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
void PlayCollisionSound();
```

**기능:** 오브가 다른 액터와 충돌했을 때 사운드를 재생합니다.

**설명:** (충돌 사운드 에셋 변수가 있는 경우) 해당 사운드 에셋을 재생합니다.

**매개변수:** 없음

**반환값:** 없음

**사용 예제:**

```cpp
void ABossProjectileOrb::PlayCollisionSound()
{
	// (충돌 사운드 에셋 변수가 있는 경우) 충돌 사운드 재생
	if (CollisionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CollisionSound, GetActorLocation());
	}
}
```

**성능:** 사운드 재생은 CPU 및 메모리 리소스를 사용합니다. 과도한 사운드 재생을 피하고, 사운드 에셋을 최적화하는 것이 좋습니다.

#### `UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb") void PlaySpawnEffect()`

```cpp
UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
void PlaySpawnEffect();
```

**기능:** 오브가 스폰될 때 이펙트를 재생합니다.

**설명:** (스폰 이펙트 템플릿 변수가 있는 경우) 해당 이펙트를 재생합니다.

**매개변수:** 없음

**반환값:** 없음

**사용 예제:**

```cpp
void ABossProjectileOrb::PlaySpawnEffect()
{
	// (스폰 이펙트 템플릿 변수가 있는 경우) 스폰 이펙트 재생
	if (SpawnEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, GetActorTransform());
	}
}
```

**성능:** 이펙트 재생은 GPU 리소스를 사용합니다. 과도한 이펙트 재생을 피하고, 이펙트 퀄리티를 조정하는 것이 좋습니다.

#### `UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb") void PlayReturnToPoolEffect()`

```cpp
UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
void PlayReturnToPoolEffect();
```

**기능:** 오브가 풀로 반환될 때 이펙트를 재생합니다.

**설명:** (풀 반환 이펙트 템플릿 변수가 있는 경우) 해당 이펙트를 재생합니다.

**매개변수:** 없음

**반환값:** 없음

**사용 예제:**

```cpp
void ABossProjectileOrb::PlayReturnToPoolEffect()
{
	// (풀 반환 이펙트 템플릿 변수가 있는 경우) 풀 반환 이펙트 재생
	if (ReturnToPoolEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ReturnToPoolEffect, GetActorTransform());
	}
}
```

**성능:** 이펙트 재생은 GPU 리소스를 사용합니다. 과도한 이펙트 재생을 피하고, 이펙트 퀄리티를 조정하는 것이 좋습니다.

#### `UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb") void PlayCollisionEffect()`

```cpp
UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
void PlayCollisionEffect();
```

**기능:** 오브가 다른 액터와 충돌했을 때 이펙트를 재생합니다.

**설명:** (충돌 이펙트 템플릿 변수가 있는 경우) 해당 이펙트를 재생합니다.

**매개변수:** 없음

**반환값:** 없음

**사용 예제:**

```cpp
void ABossProjectileOrb::PlayCollisionEffect()
{
	// (충돌 이펙트 템플릿 변수가 있는 경우) 충돌 이펙트 재생
	if (CollisionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollisionEffect, GetActorTransform());
	}
}
```

**성능:** 이펙트 재생은 GPU 리소스를 사용합니다. 과도한 이펙트 재생을 피하고, 이펙트 퀄리티를 조정하는 것이 좋습니다.

#### `UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb") void DestroyOrbWithDelay(float Delay)`

```cpp
UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
void DestroyOrbWithDelay(float Delay);
```

**기능:** 지정된 시간 후에 오브를 파괴합니다.

**설명:** `Delay` 매개변수로 지정된 시간(초) 후에 `DestroyOrb()` 함수를 호출합니다.

**매개변수:**

*   `Delay`: 파괴까지의 지연 시간(초)입니다.

**반환값:** 없음

**사용 예제:**

```cpp
void ABossProjectileOrb::DestroyOrbWithDelay(float Delay)
{
	// 지정된 시간 후에 오브 파괴
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossProjectileOrb::DestroyOrb, Delay, false);
}
```

**성능:** 타이머 설정은 약간의 오버헤드를 발생시킬 수 있습니다.

#### `UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb") void ActivateOrb()`

```cpp
UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
void ActivateOrb();
```

**기능:** 오브를 활성화합니다.

**설명:** 오브의 메시 컴포넌트와 충돌 컴포넌트를 활성화하고, 필요한 경우 추가적인 초기화 작업을 수행합니다. 오브젝트 풀에서 가져온 오브를 재사용할 때 유용합니다.

**매개변수:** 없음

**반환값:** 없음

**사용 예제:**

```cpp
void ABossProjectileOrb::ActivateOrb()
{
	// 메시 컴포넌트 활성화
	if (OrbMesh)
	{
		OrbMesh->SetVisibility(true);
		OrbMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	// 충돌 컴포넌트 활성화
	if (CollisionComponent)
	{
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	// 스폰 사운드 재생
	PlaySpawnSound();

	// 스폰 이펙트 재생
	PlaySpawnEffect();

	// 초기 위치 및 방향 설정 (필요한 경우)
	// ...
}
```

**성능:** 컴포넌트 활성화는 비교적 저렴한 연산입니다.

#### `virtual void BeginPlay() override`

```cpp
virtual void BeginPlay() override;
```

**기능:** 액터가 게임에 스폰될 때 호출되는 함수입니다.

**설명:** 오브젝트 초기화 및 이벤트 바인딩 등의 작업을 수행합니다.

**매개변수:** 없음

**반환값:** 없음

**사용 예제:**

```cpp
void ABossProjectileOrb::BeginPlay()
{
	Super::BeginPlay();

	// 초기 위치 저장 (필요한 경우)
	InitialLocation = GetActorLocation();

	// 오브 비활성화 (풀링 시스템을 사용하는 경우)
	DeactivateOrb();
}
```

**성능:** `BeginPlay` 함수는 게임 시작 시 한 번만 실행되므로 성능에 큰 영향을 미치지 않습니다.

### 멤버 변수 상세 설명

#### `UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true")) UStaticMeshComponent* OrbMesh;`

**기능:** 오브의 메시 컴포넌트입니다.

**설명:** 오브의 시각적 표현을 담당합니다.

**타입:** `UStaticMeshComponent*`

**접근 지정자:** `VisibleAnywhere`, `BlueprintReadOnly`

**카테고리:** "Projectile"

**메타데이터:** `AllowPrivateAccess = "true"`

#### `UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true")) USphereComponent* CollisionComponent;`

**기능:** 오브의 충돌 컴포넌트입니다.

**설명:** 오브의 충돌을 감지하고 처리합니다.

**타입:** `USphereComponent*`

**접근 지정자:** `VisibleAnywhere`, `BlueprintReadOnly`

**카테고리:** "Projectile"

**메타데이터:** `AllowPrivateAccess = "true"`

#### `UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile") USoundBase* SpawnSound;`

**기능:** 오브가 스폰될 때 재생되는 사운드입니다.

**설명:** 오브가 활성화될 때 재생할 사운드 에셋을 지정합니다.

**타입:** `USoundBase*`

**접근 지정자:** `EditDefaultsOnly`, `BlueprintReadWrite`

**카테고리:** "Projectile"

#### `UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile") USoundBase* ReturnToPoolSound;`

**기능:** 오브가 풀로 반환될 때 재생되는 사운드입니다.

**설명:** 오브가 비활성화되어 오브젝트 풀로 반환될 때 재생할 사운드 에셋을 지정합니다.

**타입:** `USoundBase*`

**접근 지정자:** `EditDefaultsOnly`, `BlueprintReadWrite`

**카테고리:** "Projectile"

### 추가적인 정보

*   **오브젝트 풀링:** `ABossProjectileOrb` 클래스는 오브젝트 풀링 시스템과 함께 사용하는 것이 좋습니다. 오브젝트 풀링을 사용하면 오브젝트 생성 및 파괴 비용을 줄여 성능을 향상시킬 수 있습니다.
*   **블루프린트 연동:** `UFUNCTION` 매크로를 사용하여 C++ 코드를 블루프린트에서 호출할 수 있도록 만들 수 있습니다. 이를 통해 게임 디자이너가 오브의 동작을 쉽게 제어할 수 있습니다.
*   **복제:** `bReplicates = true;` 및 `SetReplicatingMovement(true);` 설정을 통해 네트워크 환경에서 오브의 움직임을 복제할 수 있습니다.

### 트러블슈팅

*   **오브가 스폰되지 않는 경우:**
    *   `ProjectileClass` 변수가 올바르게 설정되었는지 확인합니다.
    *   오브젝트 풀이 비어 있는지 확인합니다.
    *   스폰 위치가 유효한지 확인합니다.
*   **오브가 충돌하지 않는 경우:**
    *   충돌 컴포넌트가 활성화되었는지 확인합니다.
    *   충돌 프로파일이 올바르게 설정되었는지 확인합니다.
    *   충돌 채널 및 반응 설정이 올바르게 설정되었는지 확인합니다.
*   **메모리 누수가 발생하는 경우:**
    *   오브젝트 풀링 시스템이 올바르게 구현되었는지 확인합니다.
    *   오브젝트가 풀로 반환되지 않고 파괴되는 경우가 있는지 확인합니다.
*   **성능 문제가 발생하는 경우:**
    *   Tick 함수에서 불필요한 연산을 줄입니다.
    *   이펙트 및 사운드 재생 횟수를 줄입니다.
    *   오브젝트 풀링을 사용하여 오브젝트 생성 및 파괴 비용을 줄입니다.

### 완전한 사용 예제 코드

다음은 `ABossProjectileOrb` 클래스를 사용하는 완전한 예제 코드입니다. 이 예제는 오브젝트 풀링, 블루프린트 연동, 충돌 처리, 사운드 및 이펙트 재생을 포함합니다.

```cpp
// BossProjectileOrb.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "BossProjectileActor.h" // Forward Declaration
#include "BossProjectileOrb.generated.h"

UCLASS()
class MYPROJECT_API ABossProjectileOrb : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABossProjectileOrb();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void SpawnProjectile();

	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void DestroyOrb();

	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void PlaySpawnSound();

	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void PlayReturnToPoolSound();

	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void PlayCollisionSound();

	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void PlaySpawnEffect();

	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void PlayReturnToPoolEffect();

	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void PlayCollisionEffect();

	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void DestroyOrbWithDelay(float Delay);

	UFUNCTION(BlueprintCallable, Category = "BossProjectileOrb")
	void ActivateOrb();

protected:
	virtual void BeginPlay() override;

private:
	/** 오브 메시 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* OrbMesh;

	/** 충돌 컴포넌트 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

	/** 스폰 사운드 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	USoundBase* SpawnSound;

	/** 풀로 반환 사운드 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	USoundBase* ReturnToPoolSound;

	/** 충돌 사운드 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	USoundBase* CollisionSound;

	/** 스폰 이펙트 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	UParticleSystem* SpawnEffect;

	/** 풀로 반환 이펙트 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	UParticleSystem* ReturnToPoolEffect;

	/** 충돌 이펙트 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	UParticleSystem* CollisionEffect;

	/** 이동 속도 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float MovementSpeed = 100.0f;

	/** 이동 방향 */
	FVector MovementDirection = FVector::ForwardVector;

	/** 데미지 양 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	float DamageAmount = 10.0f;

	/** 데미지 타입 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<UDamageType> DamageType;

	/** 투사체 클래스 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<ABossProjectileActor> ProjectileClass;

	/** 초기 위치 */
	FVector InitialLocation;

	void DeactivateOrb()
	{
		// 메시 컴포넌트 비활성화
		if (OrbMesh)
		{
			OrbMesh->SetVisibility(false);
			OrbMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

		// 충돌 컴포넌트 비활성화
		if (CollisionComponent)
		{
			CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
};
```

```cpp
// BossProjectileOrb.cpp
#include "BossProjectileOrb.h"

ABossProjectileOrb::ABossProjectileOrb()
{
	// 컴포넌트 초기화
	OrbMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OrbMesh"));
	RootComponent = OrbMesh;

	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetSphereRadius(50.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile"));

	// 이벤트 바인딩
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABossProjectileOrb::OnOverlap);

	// 변수 초기화
	bReplicates = true;
	SetReplicatingMovement(true);
}

void ABossProjectileOrb::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 오브의 움직임 로직 (예: 특정 방향으로 이동)
	FVector NewLocation = GetActorLocation() + (MovementDirection * MovementSpeed * DeltaTime);
	SetActorLocation(NewLocation);
}

void ABossProjectileOrb::SpawnProjectile()
{
	// 투사체 스폰 로직
	if (ProjectileClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// 월드에 투사체 스폰
		ABossProjectileActor* Projectile = GetWorld()->SpawnActor<ABossProjectileActor>(ProjectileClass, GetActorLocation(), GetActorRotation(), SpawnParams);

		if (Projectile)
		{
			// 투사체 발사 (예: 특정 방향으로)
			Projectile->FireProjectile(GetActorForwardVector());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ProjectileClass is not set!"));
	}
}

void ABossProjectileOrb::DestroyOrb()
{
	// 파괴 이펙트 재생
	PlayCollisionEffect();

	// 파괴 사운드 재생
	PlayCollisionSound();

	// 오브젝트 풀로 반환
	PlayReturnToPoolSound();

	// 오브젝트 비활성화 및 풀로 반환
	DeactivateOrb();
}

void ABossProjectileOrb::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 플레이어와 충돌했을 때
	if (OtherActor != this && OtherActor->IsA(ACharacter::StaticClass()))
	{
		// 데미지 적용
		UGameplayStatics::ApplyPointDamage(OtherActor, DamageAmount, (SweepResult.TraceStart - SweepResult.TraceEnd), SweepResult, GetInstigatorController(), this, DamageType);

		// 충돌 이펙트 재생
		PlayCollisionEffect();

		// 오브 파괴
		DestroyOrb();
	}
}

void ABossProjectileOrb::PlaySpawnSound()
{
	// 스폰 사운드 재생
	if (SpawnSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SpawnSound, GetActorLocation());
	}
}

void ABossProjectileOrb::PlayReturnToPoolSound()
{
	// 풀 반환 사운드 재생
	if (ReturnToPoolSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ReturnToPoolSound, GetActorLocation());
	}
}

void ABossProjectileOrb::PlayCollisionSound()
{
	// (충돌 사운드 에셋 변수가 있는 경우) 충돌 사운드 재생
	if (CollisionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, CollisionSound, GetActorLocation());
	}
}

void ABossProjectileOrb::PlaySpawnEffect()
{
	// (스폰 이펙트 템플릿 변수가 있는 경우) 스폰 이펙트 재생
	if (SpawnEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SpawnEffect, GetActorTransform());
	}
}

void ABossProjectileOrb::PlayReturnToPoolEffect()
{
	// (풀 반환 이펙트 템플릿 변수가 있는 경우) 풀 반환 이펙트 재생
	if (ReturnToPoolEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ReturnToPoolEffect, GetActorTransform());
	}
}

void ABossProjectileOrb::PlayCollisionEffect()
{
	// (충돌 이펙트 템플릿 변수가 있는 경우) 충돌 이펙트 재생
	if (CollisionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), CollisionEffect, GetActorTransform());
	}
}

void ABossProjectileOrb::DestroyOrbWithDelay(float Delay)
{
	// 지정된 시간 후에 오브 파괴
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ABossProjectileOrb::DestroyOrb, Delay, false);
}

void ABossProjectileOrb::ActivateOrb()
{
	// 메시 컴포넌트 활성화
	if (OrbMesh)
	{
		OrbMesh->SetVisibility(true);
		OrbMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

	// 충돌 컴포넌트 활성화
	if (CollisionComponent)
	{
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	// 스폰 사운드 재생
	PlaySpawnSound();

	// 스폰 이펙트 재생
	PlaySpawnEffect();

	// 초기 위치 및 방향 설정 (필요한 경우)
	SetActorLocation(InitialLocation);
	MovementDirection = FVector::ForwardVector;
}

void ABossProjectileOrb::BeginPlay()
{
	Super::BeginPlay();

	// 초기 위치 저장 (필요한 경우)
	InitialLocation = GetActorLocation();

	// 오브 비활성화 (풀링 시스템을 사용하는 경우)
	DeactivateOrb();
}
```

이 문서는 `ABossProjectileOrb` 클래스에 대한 상세한 API 레퍼런스를 제공하며, 개발자가 이 클래스를 효과적으로 사용하고 문제를 해결하는 데 도움이 될 것입니다. 다음으로는 `ACBoss` 클래스에 대한 상세 API 문서를 작성하겠습니다.


## ACBoss 상세 API 문서

### 1. 클래스 개요

`ACBoss` 클래스는 게임 내 보스 캐릭터의 핵심 로직을 담당하는 액터 클래스입니다. 보스의 행동, 데미지 처리, UI 업데이트, 배경 음악 제어 등 다양한 기능을 포함합니다. `ACharacter` 클래스를 상속받아 기본적인 캐릭터 기능을 제공하며, 보스 특유의 행동 패턴 및 상태 관리를 위한 기능을 추가적으로 구현합니다.

**목적**:

*   보스 캐릭터의 행동 제어 및 상태 관리
*   플레이어의 공격에 대한 데미지 처리
*   보스 UI 업데이트 (HP, 상태 등)
*   보스 배경 음악 재생 및 제어

**사용 시나리오**:

*   보스 전투 구현
*   보스 AI 제어
*   보스 사망 처리 및 연출
*   보스 UI 표시 및 업데이트

**상속 관계**:

```cpp
UObject
    AActor
        ACharacter
            ACBoss
```

**주의사항**:

*   `ACBoss` 클래스는 게임 내에 단일 인스턴스로 존재하도록 설계하는 것이 일반적입니다. 여러 보스가 필요한 경우, `ACBoss` 클래스를 상속받아 새로운 보스 클래스를 생성하는 것을 권장합니다.
*   보스의 AI는 `ACBossAIC` 클래스를 통해 제어됩니다.

**성능 고려사항**:

*   `Tick` 함수 내에서 과도한 연산을 수행하지 않도록 주의해야 합니다. 복잡한 연산은 비동기 처리하거나, 프레임 간 분산하여 처리하는 것을 고려해야 합니다.
*   메모리 누수를 방지하기 위해, 동적으로 생성된 오브젝트는 적절히 해제해야 합니다.

### 2. 함수 상세 설명

#### 2.1. `ACBoss::ACBoss()`

```cpp
ACBoss::ACBoss();
```

**기능**: `ACBoss` 클래스의 생성자입니다.

**매개변수**: 없음

**반환값**: 없음

**설명**:

*   클래스의 멤버 변수를 초기화합니다.
*   필요한 컴포넌트를 생성하고 설정합니다.

**사용 예제**:

```cpp
ACBoss::ACBoss()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
    GetCharacterMovement()->JumpZVelocity = 600.f;
    GetCharacterMovement()->AirControl = 0.2f;

    // Create a camera boom (pulls in towards the player if there is collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller rotation
    FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
    // are set in the derived blueprint asset.
}
```

**성능 특성**: 생성자이므로 게임 시작 시 한 번만 호출됩니다.

#### 2.2. `ACBoss::Tick(float DeltaTime)`

```cpp
void ACBoss::Tick(float DeltaTime);
```

**기능**: 매 프레임마다 호출되는 함수입니다.

**매개변수**:

*   `DeltaTime`: 프레임 간 시간 간격 (초)

**반환값**: 없음

**설명**:

*   보스의 상태를 업데이트합니다.
*   AI 로직을 실행합니다.
*   애니메이션을 업데이트합니다.

**사용 예제**:

```cpp
void ACBoss::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // AI 로직 실행
    if (BossAIController)
    {
        // Example: Update the AI target based on player location
        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
        if (PlayerPawn)
        {
            BossAIController->SetFocus(PlayerPawn);
        }
    }

    // 애니메이션 업데이트
    UpdateAnimation(DeltaTime);
}
```

**성능 특성**: 매 프레임마다 호출되므로, 성능에 미치는 영향이 큽니다. 불필요한 연산을 최소화해야 합니다.

#### 2.3. `ACBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)`

```cpp
float ACBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);
```

**기능**: 데미지를 처리하는 함수입니다.

**매개변수**:

*   `DamageAmount`: 데미지 양
*   `DamageEvent`: 데미지 이벤트 정보
*   `EventInstigator`: 데미지를 가한 컨트롤러
*   `DamageCauser`: 데미지를 가한 액터

**반환값**: 실제로 적용된 데미지 양

**설명**:

*   데미지를 계산하고, 보스의 HP를 감소시킵니다.
*   데미지 이펙트를 재생합니다.
*   보스의 상태를 업데이트합니다 (예: 그로기 상태).

**사용 예제**:

```cpp
float ACBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    if (ActualDamage > 0.f)
    {
        CurrentHealth -= ActualDamage;
        HPUpdate(); // Update the HP bar

        // Play hit reaction animation
        PlayHitMotion();

        if (CurrentHealth <= 0.f)
        {
            // Handle death
            Die();
        }
    }

    return ActualDamage;
}
```

**성능 특성**: 데미지를 받을 때만 호출되므로, 성능에 미치는 영향은 비교적 적습니다.

#### 2.4. `ACBoss::BeginPlay()`

```cpp
void ACBoss::BeginPlay();
```

**기능**: 게임 시작 시 호출되는 함수입니다.

**매개변수**: 없음

**반환값**: 없음

**설명**:

*   보스의 초기 상태를 설정합니다.
*   AI 컨트롤러를 초기화합니다.
*   UI를 초기화합니다.
*   초기 위치를 저장합니다.

**사용 예제**:

```cpp
void ACBoss::BeginPlay()
{
    Super::BeginPlay();

    // Store the initial location
    InitialLocation = GetActorLocation();

    // Get the AI Controller
    BossAIController = Cast<ACBossAIC>(GetController());

    // Show the Boss Status Widget
    ShowBossStatusWidget();

    // Play the Boss BGM
    PlayBossBGM();
}
```

**성능 특성**: 게임 시작 시 한 번만 호출됩니다.

#### 2.5. `ACBoss::PlayHitMotion()`

```cpp
void ACBoss::PlayHitMotion();
```

**기능**: 피격 모션을 재생하는 함수입니다.

**매개변수**: 없음

**반환값**: 없음

**설명**:

*   피격 애니메이션 몽타주를 재생합니다.

**사용 예제**:

```cpp
void ACBoss::PlayHitMotion()
{
    // Example: Play a hit reaction montage
    if (HitReactionMontage)
    {
        UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
        if (AnimInstance)
        {
            AnimInstance->Montage_Play(HitReactionMontage, 1.0f);
        }
    }
}
```

**성능 특성**: 피격 시에만 호출되므로, 성능에 미치는 영향은 적습니다.

#### 2.6. `ACBoss::ShowBossStatusWidget()`

```cpp
void ACBoss::ShowBossStatusWidget();
```

**기능**: 보스 상태 위젯을 표시하는 함수입니다.

**매개변수**: 없음

**반환값**: 없음

**설명**:

*   보스 상태 위젯을 생성하고, 뷰포트에 추가합니다.

**사용 예제**:

```cpp
void ACBoss::ShowBossStatusWidget()
{
    if (BossStatusWidgetClass)
    {
        BossStatusWidget = CreateWidget<UBossStatusWidget>(GetWorld(), BossStatusWidgetClass);
        if (BossStatusWidget)
        {
            BossStatusWidget->AddToViewport();
            BossStatusWidget->UpdateBossHP(CurrentHealth, MaxHealth); // Initial HP update
        }
    }
}
```

**성능 특성**: 게임 시작 시 또는 보스 등장 시에만 호출되므로, 성능에 미치는 영향은 적습니다.

#### 2.7. `ACBoss::HPUpdate()`

```cpp
void ACBoss::HPUpdate();
```

**기능**: 보스 HP를 업데이트하는 함수입니다.

**매개변수**: 없음

**반환값**: 없음

**설명**:

*   보스 상태 위젯의 HP 바를 업데이트합니다.

**사용 예제**:

```cpp
void ACBoss::HPUpdate()
{
    if (BossStatusWidget)
    {
        BossStatusWidget->UpdateBossHP(CurrentHealth, MaxHealth);
    }
}
```

**성능 특성**: 데미지를 받거나 HP가 변경될 때 호출되므로, 성능에 미치는 영향은 비교적 적습니다.

#### 2.8. `ACBoss::RestartUI()`

```cpp
void ACBoss::RestartUI();
```

**기능**: UI를 재시작하는 함수입니다.

**매개변수**: 없음

**반환값**: 없음

**설명**:

*   UI를 초기 상태로 되돌립니다.

**사용 예제**:

```cpp
void ACBoss::RestartUI()
{
    if (BossStatusWidget)
    {
        BossStatusWidget->RemoveFromViewport();
        BossStatusWidget = nullptr;
    }

    ShowBossStatusWidget(); // Recreate and show the widget
}
```

**성능 특성**: UI 재시작 시에만 호출되므로, 성능에 미치는 영향은 적습니다.

#### 2.9. `ACBoss::PlayBossBGM()`

```cpp
void ACBoss::PlayBossBGM();
```

**기능**: 보스 배경 음악을 재생하는 함수입니다.

**매개변수**: 없음

**반환값**: 없음

**설명**:

*   보스 배경 음악을 재생합니다.

**사용 예제**:

```cpp
void ACBoss::PlayBossBGM()
{
    if (BossBGM)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), BossBGM);
    }
}
```

**성능 특성**: 보스 등장 시에만 호출되므로, 성능에 미치는 영향은 적습니다.

#### 2.10. `ACBoss::StopBossBGM()`

```cpp
void ACBoss::StopBossBGM();
```

**기능**: 보스 배경 음악을 정지하는 함수입니다.

**매개변수**: 없음

**반환값**: 없음

**설명**:

*   보스 배경 음악을 정지합니다.

**사용 예제**:

```cpp
void ACBoss::StopBossBGM()
{
    // Example: Stop the currently playing BGM
    UGameplayStatics::StopAllSounds(GetWorld());
}
```

**성능 특성**: 보스 사망 시 또는 특정 이벤트 발생 시에만 호출되므로, 성능에 미치는 영향은 적습니다.

#### 2.11. `ACBoss::LowerBossBGMVolume()`

```cpp
void ACBoss::LowerBossBGMVolume();
```

**기능**: 보스 배경 음악 볼륨을 감소시키는 함수입니다. (주로 죽음 시 사용)

**매개변수**: 없음

**반환값**: 없음

**설명**:

*   보스 배경 음악의 볼륨을 점진적으로 감소시킵니다.

**사용 예제**:

```cpp
void ACBoss::LowerBossBGMVolume()
{
    // Example: Lower the BGM volume over time
    // This would require a more complex implementation involving audio components and fading
    // For simplicity, we'll just stop the BGM for now
    StopBossBGM();
}
```

**성능 특성**: 보스 사망 시에만 호출되므로, 성능에 미치는 영향은 적습니다.

### 3. 사용 예제

다음은 `ACBoss` 클래스를 사용하는 간단한 예제입니다.

```cpp
// ACBoss.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Components/WidgetComponent.h"
#include "BossStatusWidget.h"
#include "ACBoss.generated.h"

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

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Take Damage event
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// 피격 모션 재생
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void PlayHitMotion();

	// 보스 상태 위젯 표시
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void ShowBossStatusWidget();

	// HP 업데이트
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void HPUpdate();

	// UI 재시작
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void RestartUI();

	// 보스 배경 음악 재생
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void PlayBossBGM();

	// 보스 배경 음악 정지
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void StopBossBGM();

	// 보스 배경 음악 볼륨 감소 (죽음 시)
	UFUNCTION(BlueprintCallable, Category = "Boss")
	void LowerBossBGMVolume();

private:
	// 초기 위치
	FVector InitialLocation;

	// AI 컨트롤러
	class ACBossAIC* BossAIController;

	// 보스 상태 위젯
	class UBossStatusWidget* BossStatusWidget;

	// 최대 HP
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
	float MaxHealth = 100.0f;

	// 현재 HP
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Boss")
	float CurrentHealth = 100.0f;

	// 피격 애니메이션 몽타주
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
	class UAnimMontage* HitReactionMontage;

	// 보스 상태 위젯 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
	TSubclassOf<class UBossStatusWidget> BossStatusWidgetClass;

	// 보스 배경 음악
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Boss")
	class USoundBase* BossBGM;

	// 사망 처리 함수 (예시)
	UFUNCTION()
	void Die();

	// 애니메이션 업데이트 함수 (예시)
	UFUNCTION()
	void UpdateAnimation(float DeltaTime);

	// 카메라 붐
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// 따라가는 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
};

// ACBoss.cpp
#include "ACBoss.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BossStatusWidget.h"
#include "ACBossAIC.h"

ACBoss::ACBoss()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller rotation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
	// are set in the derived blueprint asset.
}

void ACBoss::BeginPlay()
{
	Super::BeginPlay();

	// Store the initial location
	InitialLocation = GetActorLocation();

	// Get the AI Controller
	BossAIController = Cast<ACBossAIC>(GetController());

	// Show the Boss Status Widget
	ShowBossStatusWidget();

	// Play the Boss BGM
	PlayBossBGM();
}

void ACBoss::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// AI 로직 실행
	if (BossAIController)
	{
		// Example: Update the AI target based on player location
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			BossAIController->SetFocus(PlayerPawn);
		}
	}

	// 애니메이션 업데이트
	UpdateAnimation(DeltaTime);
}

void ACBoss::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACBoss::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACBoss::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACBoss::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACBoss::LookUpAtRate);
}

float ACBoss::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		CurrentHealth -= ActualDamage;
		HPUpdate(); // Update the HP bar

		// Play hit reaction animation
		PlayHitMotion();

		if (CurrentHealth <= 0.f)
		{
			// Handle death
			Die();
		}
	}

	return ActualDamage;
}

void ACBoss::PlayHitMotion()
{
	// Example: Play a hit reaction montage
	if (HitReactionMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(HitReactionMontage, 1.0f);
		}
	}
}

void ACBoss::ShowBossStatusWidget()
{
	if (BossStatusWidgetClass)
	{
		BossStatusWidget = CreateWidget<UBossStatusWidget>(GetWorld(), BossStatusWidgetClass);
		if (BossStatusWidget)
		{
			BossStatusWidget->AddToViewport();
			BossStatusWidget->UpdateBossHP(CurrentHealth, MaxHealth); // Initial HP update
		}
	}
}

void ACBoss::HPUpdate()
{
	if (BossStatusWidget)
	{
		BossStatusWidget->UpdateBossHP(CurrentHealth, MaxHealth);
	}
}

void ACBoss::RestartUI()
{
	if (BossStatusWidget)
	{
		BossStatusWidget->RemoveFromViewport();
		BossStatusWidget = nullptr;
	}

	ShowBossStatusWidget(); // Recreate and show the widget
}

void ACBoss::PlayBossBGM()
{
	if (BossBGM)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), BossBGM);
	}
}

void ACBoss::StopBossBGM()
{
	// Example: Stop the currently playing BGM
	UGameplayStatics::StopAllSounds(GetWorld());
}

void ACBoss::LowerBossBGMVolume()
{
	// Example: Lower the BGM volume over time
	// This would require a more complex implementation involving audio components and fading
	// For simplicity, we'll just stop the BGM for now
	StopBossBGM();
}

void ACBoss::Die()
{
	// Example: Handle death logic
	StopBossBGM();
	// Play death animation, etc.
	Destroy(); // Or any other death handling logic
}

void ACBoss::UpdateAnimation(float DeltaTime)
{
	// Example: Update animation based on movement
	// This is a placeholder, replace with your actual animation logic
	float ForwardSpeed = GetVelocity().Size(); // Get the speed of the boss
	// Update the animation based on the speed
}

void ACBoss::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACBoss::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ACBoss::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACBoss::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
```

**설명**:

*   `ACBoss` 클래스는 `ACharacter` 클래스를 상속받아 기본적인 캐릭터 기능을 제공합니다.
*   `BeginPlay` 함수에서 초기 상태를 설정하고, AI 컨트롤러를 초기화하며, UI를 초기화합니다.
*   `Tick` 함수에서 AI 로직을 실행하고, 애니메이션을 업데이트합니다.
*   `TakeDamage` 함수에서 데미지를 처리하고, HP를 감소시키며, 피격 모션을 재생합니다.
*   `ShowBossStatusWidget` 함수에서 보스 상태 위젯을 표시합니다.
*   `HPUpdate` 함수에서 보스 상태 위젯의 HP 바를 업데이트합니다.
*   `PlayBossBGM` 함수에서 보스 배경 음악을 재생합니다.
*   `StopBossBGM` 함수에서 보스 배경 음악을 정지합니다.
*   `LowerBossBGMVolume` 함수에서 보스 배경 음악의 볼륨을 감소시킵니다.

### 4. 성능 고려사항

*   `Tick` 함수 내에서 과도한 연산을 수행하지 않도록 주의해야 합니다. 복잡한 연산은 비동기 처리하거나, 프레임 간 분산하여 처리하는 것을 고려해야 합니다.
*   메모리 누수를 방지하기 위해, 동적으로 생성된 오브젝트는 적절히 해제해야 합니다.
*   애니메이션 몽타주 재생 시, 불필요한 애니메이션 이벤트를 최소화해야 합니다.

### 5. 호환성 정보

*   Unreal Engine 버전: 4.26 이상
*   플랫폼: Windows, Linux, Mac, iOS, Android
*   컴파일러: Visual Studio 2019 이상 (Windows), Xcode (Mac), Clang (Linux)
*   의존성: 없음

### 6. 트러블슈팅

*   **보스가 움직이지 않는 경우**: AI 컨트롤러가 제대로 초기화되었는지 확인해야 합니다.
*   **UI가 표시되지 않는 경우**: 위젯 클래스가 제대로 설정되었는지, 위젯이 뷰포트에 추가되었는지 확인해야 합니다.
*   **데미지가 제대로 적용되지 않는 경우**: `TakeDamage` 함수가 제대로 호출되었는지, 데미지 계산 로직에 오류가 없는지 확인해야 합니다.
*   **배경 음악이 재생되지 않는 경우**: 사운드 에셋이 제대로 설정되었는지, `PlayBossBGM` 함수가 제대로 호출되었는지 확인해야 합니다.

### 7. 다음 단계

다음으로는 `ACBossAIC` 클래스에 대한 상세 API 문서를 작성하겠습니다. `ACBossAIC` 클래스는 `ACBoss` 클래스의 AI를 제어하는 데 사용되는 AI 컨트롤러 클래스입니다.


## ACBossAIC 상세 API 문서

`ACBossAIC` 클래스는 `ACBoss` 클래스의 AI를 제어하는 데 사용되는 AI 컨트롤러 클래스입니다. StateTree를 사용하여 보스의 행동을 정의하고 관리합니다. 이 클래스는 보스의 행동 패턴, 공격 방식, 이동 전략 등을 StateTree 에셋을 통해 설정하고, 런타임에 보스가 이러한 행동들을 수행하도록 제어하는 역할을 합니다.

### 1. 클래스 개요

*   **목적**: `ACBoss` 클래스의 AI 제어 및 StateTree 기반 행동 관리
*   **역할**: 보스의 행동 패턴 정의, 공격 방식 설정, 이동 전략 관리, StateTree 에셋 연결 및 실행
*   **상속 관계**: `AAIController`
*   **사용 시나리오**: 보스 캐릭터의 AI를 구현하고, StateTree를 사용하여 다양한 행동 패턴을 설계할 때 사용됩니다. 예를 들어, 특정 거리에서 공격, 특정 HP 이하에서 페이즈 변경, 특정 패턴 반복 등의 복잡한 행동을 구현할 수 있습니다.

### 2. 클래스 멤버 상세 설명

#### 2.1 생성자

```c++
ACBossAIC();
```

*   **설명**: `ACBossAIC` 클래스의 생성자입니다.
*   **매개변수**: 없음
*   **반환값**: 없음
*   **예외**: 없음
*   **사용 예제**:

```c++
ACBossAIC* BossAIController = NewObject<ACBossAIC>();
```

*   **성능**: 생성자는 기본적인 초기화만 수행하므로 성능에 미치는 영향은 미미합니다.

#### 2.2 `OnPossess` 함수

```c++
virtual void OnPossess(APawn* InPawn) override;
```

*   **설명**: Pawn을 소유했을 때 호출되는 함수입니다. 이 함수는 AI 컨트롤러가 특정 Pawn을 제어하기 시작할 때 호출됩니다. 여기서는 주로 StateTree를 시작하고 보스의 AI를 초기화하는 데 사용됩니다.
*   **매개변수**:
    *   `InPawn`: 소유하게 된 Pawn 객체입니다. `ACBoss` 클래스의 인스턴스여야 합니다.
*   **반환값**: 없음
*   **예외**: `InPawn`이 유효하지 않은 경우 예외가 발생할 수 있습니다.
*   **사용 예제**:

```c++
void ACBossAIC::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    ACBoss* Boss = Cast<ACBoss>(InPawn);
    if (Boss)
    {
        // StateTree 시작 로직 구현
        UBehaviorTreeComponent* BehaviorComp = Cast<UBehaviorTreeComponent>(GetComponentByClass(UBehaviorTreeComponent::StaticClass()));
        if (BehaviorComp)
        {
            BehaviorComp->StartTree(Boss->GetBehaviorTree()); // 보스의 BehaviorTree 에셋 가져와서 시작
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("BehaviorTreeComponent not found!"));
        }

        // 초기화 로직 (필요한 경우)
        // ...
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ACBossAIC::OnPossess - Invalid Pawn!"));
    }
}
```

*   **성능**: `OnPossess` 함수는 게임 시작 시 또는 보스가 스폰될 때 한 번만 호출되므로, 성능에 큰 영향을 미치지 않습니다. 하지만 StateTree 시작과 관련된 연산은 최적화가 필요할 수 있습니다.

### 3. 사용 예제 (완전한 코드)

다음은 `ACBossAIC` 클래스를 사용하는 완전한 예제 코드입니다. 이 예제는 보스 AI 컨트롤러를 생성하고, StateTree를 시작하여 보스의 행동을 제어하는 방법을 보여줍니다.

```c++
// CBossAIC.h
#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CBossAIC.generated.h"

UCLASS()
class DDTPROJECT_API ACBossAIC : public AAIController
{
	GENERATED_BODY()

public:
	// 생성자
	ACBossAIC();

protected:
	// Pawn 소유 시 호출되는 함수
	virtual void OnPossess(APawn* InPawn) override;
};

// CBossAIC.cpp
#include "CBossAIC.h"
#include "CBoss.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Kismet/GameplayStatics.h"

ACBossAIC::ACBossAIC()
{
    // BehaviorTreeComponent 생성 및 설정
    UBehaviorTreeComponent* BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
    if (BehaviorComp)
    {
        // 컴포넌트 등록 (필요한 경우)
    }
}

void ACBossAIC::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    ACBoss* Boss = Cast<ACBoss>(InPawn);
    if (Boss)
    {
        // StateTree 시작 로직 구현
        UBehaviorTreeComponent* BehaviorComp = Cast<UBehaviorTreeComponent>(GetComponentByClass(UBehaviorTreeComponent::StaticClass()));
        if (BehaviorComp)
        {
            UBehaviorTree* BehaviorTree = Boss->GetBehaviorTree();
            if (BehaviorTree)
            {
                BehaviorComp->StartTree(*BehaviorTree); // 보스의 BehaviorTree 에셋 가져와서 시작
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("BehaviorTree is null!"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("BehaviorTreeComponent not found!"));
        }

        // 초기화 로직 (필요한 경우)
        // ...
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("ACBossAIC::OnPossess - Invalid Pawn!"));
    }
}
```

**설명:**

1.  **헤더 파일 (`CBossAIC.h`):**
    *   `ACBossAIC` 클래스를 선언합니다.
    *   `AAIController`를 상속받습니다.
    *   `OnPossess` 함수를 오버라이드합니다.
2.  **소스 파일 (`CBossAIC.cpp`):**
    *   `ACBossAIC` 생성자에서 `UBehaviorTreeComponent`를 생성하고 설정합니다.
    *   `OnPossess` 함수에서 다음을 수행합니다.
        *   소유된 Pawn이 `ACBoss`인지 확인합니다.
        *   `ACBoss`에서 `BehaviorTree`를 가져옵니다.
        *   `BehaviorTreeComponent`를 사용하여 `BehaviorTree`를 시작합니다.

**사용 방법:**

1.  `ACBoss` 클래스에 `UBehaviorTree* BehaviorTree;` 변수를 추가하고, 에디터에서 Behavior Tree 에셋을 할당할 수 있도록 `UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = AI)` 매크로를 사용합니다.
2.  `ACBoss::BeginPlay()` 함수에서 `AAIController* AIController = UGameplayStatics::GetAIController(this, 0);` 를 사용하여 AIController를 가져옵니다.
3.  `ACBoss` 액터의 AI Controller Class를 `ACBossAIC`로 설정합니다.

### 4. 성능 고려사항

*   `OnPossess` 함수 내에서 StateTree를 시작하는 것은 비교적 비용이 많이 드는 작업일 수 있습니다. StateTree의 복잡도에 따라 성능에 영향을 미칠 수 있으므로, 필요한 경우 비동기 로딩 또는 최적화를 고려해야 합니다.
*   StateTree의 틱 간격과 연산 복잡도를 조절하여 CPU 사용량을 최적화해야 합니다.

### 5. 호환성 정보

*   Unreal Engine 버전: 4.26 이상
*   플랫폼: Windows, Linux, Mac, iOS, Android
*   컴파일러: Visual Studio 2019 이상 (Windows), Xcode (Mac), Clang (Linux)
*   의존성: 없음

### 6. 트러블슈팅

*   **보스가 움직이지 않는 경우**:
    *   `ACBossAIC`가 `ACBoss`의 AI 컨트롤러로 제대로 설정되었는지 확인합니다.
    *   StateTree 에셋이 제대로 할당되었는지 확인합니다.
    *   StateTree의 루트 노드가 제대로 설정되었는지 확인합니다.
*   **StateTree가 시작되지 않는 경우**:
    *   `OnPossess` 함수가 제대로 호출되는지 확인합니다.
    *   `BehaviorTreeComponent`가 제대로 생성되었는지 확인합니다.
    *   `ACBoss`에서 `BehaviorTree`를 가져오는 데 실패했는지 확인합니다.

### 7. 다음 단계

다음으로는 `ACBossWeapon` 클래스에 대한 상세 API 문서를 작성하겠습니다. `ACBossWeapon` 클래스는 `ACBoss` 클래스의 무기를 관리하고, 공격 로직을 처리하는 데 사용됩니다.


## ACBossWeapon 상세 API 문서

### 1. 클래스 개요

`ACBossWeapon` 클래스는 보스 캐릭터(`ACBoss`)가 사용하는 무기를 관리하고, 공격 로직 및 콜리전을 처리하는 핵심 클래스입니다. 이 클래스는 무기의 장착, 해제, 콜리전 활성화/비활성화, 그리고 공격 시의 특수 효과 등을 제어합니다. `ACBossWeapon`은 보스의 공격 패턴과 밀접하게 연결되어 있으며, 애니메이션 노티파이를 통해 특정 시점에 콜리전을 활성화하거나 특수 효과를 발동시키는 방식으로 동작합니다.

**주요 역할:**

*   보스 무기의 장착 및 해제 관리
*   공격 시 콜리전 활성화 및 비활성화 제어
*   애니메이션 노티파이를 통한 공격 로직 트리거
*   무기 관련 특수 효과 및 파티클 시스템 관리

**사용 시나리오:**

*   보스 캐릭터가 특정 공격 애니메이션을 실행할 때, 무기의 콜리전을 활성화하여 플레이어에게 데미지를 입히는 경우
*   보스 캐릭터가 무기를 휘두를 때, 특정 파티클 이펙트나 사운드를 재생하는 경우
*   보스 캐릭터의 무기를 교체하거나, 새로운 무기를 장착하는 경우

**상속 관계:**

`ACBossWeapon`은 `AActor` 클래스를 상속받습니다.

```c++
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ACBossWeapon.generated.h"

UCLASS()
class YOUR_PROJECT_API ACBossWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACBossWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ...
};
```

**주의사항 및 제약사항:**

*   `ACBossWeapon`은 보스 캐릭터에 종속적으로 동작하므로, 단독으로 사용될 수 없습니다.
*   콜리전 처리는 `OnBossComponentBeginOverlap` 및 `OnBossComponentEndOverlap` 함수를 통해 이루어지므로, 해당 함수들을 적절히 구현해야 합니다.
*   애니메이션 노티파이를 통해 콜리전 활성화/비활성화 시점을 정확하게 제어해야, 의도하지 않은 데미지 발생을 방지할 수 있습니다.

**성능 고려사항:**

*   콜리전 컴포넌트의 개수가 많아지면, 성능에 영향을 미칠 수 있습니다. 필요한 콜리전만 활성화하고, 불필요한 콜리전은 비활성화하여 최적화해야 합니다.
*   파티클 이펙트의 품질과 개수를 조절하여, GPU 부하를 줄여야 합니다.

### 2. 함수 상세 설명

#### 2.1 생성자

```c++
ACBossWeapon::ACBossWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 콜리전 컴포넌트 생성 및 설정 (예시)
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->SetGenerateOverlapEvents(false); // 초기에는 콜리전 비활성화
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACBossWeapon::OnBossComponentBeginOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ACBossWeapon::OnBossComponentEndOverlap);

	// 메시 컴포넌트 생성 및 설정 (예시)
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
}
```

*   **역할**: `ACBossWeapon` 클래스의 생성자입니다. 액터의 기본 속성을 초기화하고, 콜리전 컴포넌트 및 메시 컴포넌트를 생성합니다.
*   **매개변수**: 없음
*   **반환값**: 없음
*   **예외 처리**: 없음
*   **사용 예제**:

```c++
ACBossWeapon* MyWeapon = NewObject<ACBossWeapon>(GetWorld());
```

*   **베스트 프랙티스**: 생성자에서는 액터의 기본 속성 및 컴포넌트만 초기화하고, 실제 게임 로직은 `BeginPlay` 함수에서 처리하는 것이 좋습니다.
*   **성능 특성**: 생성자는 액터가 생성될 때 한 번만 호출되므로, 성능에 큰 영향을 미치지 않습니다.

#### 2.2 `OnBossBeginEquip`

```c++
void ACBossWeapon::OnBossBeginEquip()
{
	OnBossBeginEquip_Implementation();
}
```

*   **역할**: 보스가 무기를 장착하기 시작할 때 호출되는 함수입니다. 이 함수는 BlueprintNativeEvent로 선언되어 있어 C++ 코드와 Blueprint에서 모두 구현할 수 있습니다.
*   **매개변수**: 없음
*   **반환값**: 없음
*   **예외 처리**: 없음
*   **사용 예제**:

```c++
// C++ 코드에서 호출
MyWeapon->OnBossBeginEquip();

// Blueprint에서 이벤트 그래프를 통해 구현
```

*   **베스트 프랙티스**: 무기 장착 시 필요한 초기화 작업을 수행하는 데 사용합니다. 예를 들어, 무기의 메시를 보이게 하거나, 특정 파티클 이펙트를 활성화할 수 있습니다.
*   **성능 특성**: 무기 장착 시 한 번만 호출되므로, 성능에 큰 영향을 미치지 않습니다.

#### 2.3 `OnBossBeginEquip_Implementation`

```c++
void ACBossWeapon::OnBossBeginEquip_Implementation()
{
	// C++ 또는 Blueprint에서 오버라이드하여 구현
	UE_LOG(LogTemp, Warning, TEXT("OnBossBeginEquip_Implementation called!"));
}
```

*   **역할**: `OnBossBeginEquip` 함수의 구현체입니다. 이 함수는 C++ 또는 Blueprint에서 오버라이드하여 실제 무기 장착 로직을 구현합니다.
*   **매개변수**: 없음
*   **반환값**: 없음
*   **예외 처리**: 없음
*   **사용 예제**:

```c++
// C++ 코드에서 오버라이드
void AMyBossWeapon::OnBossBeginEquip_Implementation()
{
	Super::OnBossBeginEquip_Implementation();
	// 무기 메시를 보이게 하는 로직
	MeshComponent->SetVisibility(true);
}

// Blueprint에서 이벤트 그래프를 통해 구현
```

*   **베스트 프랙티스**: `Super::OnBossBeginEquip_Implementation();`를 호출하여 부모 클래스의 구현을 먼저 실행하는 것이 좋습니다.
*   **성능 특성**: 무기 장착 시 한 번만 호출되므로, 성능에 큰 영향을 미치지 않습니다.

#### 2.4 `OnBossUnequip`

```c++
void ACBossWeapon::OnBossUnequip()
{
	OnBossUnequip_Implementation();
}
```

*   **역할**: 보스가 무기를 해제하기 시작할 때 호출되는 함수입니다. 이 함수는 BlueprintNativeEvent로 선언되어 있어 C++ 코드와 Blueprint에서 모두 구현할 수 있습니다.
*   **매개변수**: 없음
*   **반환값**: 없음
*   **예외 처리**: 없음
*   **사용 예제**:

```c++
// C++ 코드에서 호출
MyWeapon->OnBossUnequip();

// Blueprint에서 이벤트 그래프를 통해 구현
```

*   **베스트 프랙티스**: 무기 해제 시 필요한 정리 작업을 수행하는 데 사용합니다. 예를 들어, 무기의 메시를 숨기거나, 특정 파티클 이펙트를 비활성화할 수 있습니다.
*   **성능 특성**: 무기 해제 시 한 번만 호출되므로, 성능에 큰 영향을 미치지 않습니다.

#### 2.5 `OnBossUnequip_Implementation`

```c++
void ACBossWeapon::OnBossUnequip_Implementation()
{
	// C++ 또는 Blueprint에서 오버라이드하여 구현
	UE_LOG(LogTemp, Warning, TEXT("OnBossUnequip_Implementation called!"));
}
```

*   **역할**: `OnBossUnequip` 함수의 구현체입니다. 이 함수는 C++ 또는 Blueprint에서 오버라이드하여 실제 무기 해제 로직을 구현합니다.
*   **매개변수**: 없음
*   **반환값**: 없음
*   **예외 처리**: 없음
*   **사용 예제**:

```c++
// C++ 코드에서 오버라이드
void AMyBossWeapon::OnBossUnequip_Implementation()
{
	Super::OnBossUnequip_Implementation();
	// 무기 메시를 숨기는 로직
	MeshComponent->SetVisibility(false);
}

// Blueprint에서 이벤트 그래프를 통해 구현
```

*   **베스트 프랙티스**: `Super::OnBossUnequip_Implementation();`를 호출하여 부모 클래스의 구현을 먼저 실행하는 것이 좋습니다.
*   **성능 특성**: 무기 해제 시 한 번만 호출되므로, 성능에 큰 영향을 미치지 않습니다.

#### 2.6 `OnBossCollisions`

```c++
UFUNCTION(BlueprintCallable, Category = "BossWeapon")
void ACBossWeapon::OnBossCollisions(bool bNewCollision)
{
	if (bNewCollision)
	{
		OnSelectCollision();
	}
	else
	{
		OffBossCollisions();
	}
}
```

*   **역할**:  무기의 콜리전 활성화/비활성화를 제어하는 함수입니다. Blueprint에서 호출 가능하며, `bNewCollision` 매개변수를 통해 활성화 여부를 결정합니다.
*   **매개변수**:
    *   `bNewCollision` (bool): 콜리전을 활성화할지 (true) 비활성화할지 (false)를 결정합니다.
*   **반환값**: 없음
*   **예외 처리**: 없음
*   **사용 예제**:

```c++
// C++ 코드에서 호출
MyWeapon->OnBossCollisions(true); // 콜리전 활성화
MyWeapon->OnBossCollisions(false); // 콜리전 비활성화

// Blueprint에서 함수 호출 노드를 통해 구현
```

*   **베스트 프랙티스**: 애니메이션 노티파이를 통해 이 함수를 호출하여, 공격 애니메이션의 특정 시점에 콜리전을 활성화/비활성화하는 것이 좋습니다.
*   **성능 특성**: 콜리전 활성화/비활성화는 비교적 비용이 많이 드는 작업이므로, 빈번하게 호출하지 않도록 주의해야 합니다.

#### 2.7 `OnSelectCollision`

```c++
UFUNCTION(BlueprintCallable, Category = "BossWeapon")
void ACBossWeapon::OnSelectCollision()
{
	// 콜리전 활성화 로직
	CollisionComponent->SetGenerateOverlapEvents(true);
	UE_LOG(LogTemp, Warning, TEXT("OnSelectCollision called!"));
}
```

*   **역할**: 무기의 콜리전을 활성화하는 함수입니다.
*   **매개변수**: 없음
*   **반환값**: 없음
*   **예외 처리**: 없음
*   **사용 예제**:

```c++
// C++ 코드에서 호출
MyWeapon->OnSelectCollision();
```

*   **베스트 프랙티스**: `OnBossCollisions` 함수를 통해 간접적으로 호출하는 것이 좋습니다.
*   **성능 특성**: 콜리전 활성화는 비교적 비용이 많이 드는 작업이므로, 필요한 시점에만 호출해야 합니다.

#### 2.8 `OffBossCollisions`

```c++
UFUNCTION(BlueprintCallable, Category = "BossWeapon")
void ACBossWeapon::OffBossCollisions()
{
	// 콜리전 비활성화 로직
	CollisionComponent->SetGenerateOverlapEvents(false);
	UE_LOG(LogTemp, Warning, TEXT("OffBossCollisions called!"));
}
```

*   **역할**: 무기의 콜리전을 비활성화하는 함수입니다.
*   **매개변수**: 없음
*   **반환값**: 없음
*   **예외 처리**: 없음
*   **사용 예제**:

```c++
// C++ 코드에서 호출
MyWeapon->OffBossCollisions();
```

*   **베스트 프랙티스**: `OnBossCollisions` 함수를 통해 간접적으로 호출하는 것이 좋습니다.
*   **성능 특성**: 콜리전 비활성화는 비교적 비용이 많이 드는 작업이므로, 필요한 시점에만 호출해야 합니다.

#### 2.9 `BossAttachToCollision`

```c++
UFUNCTION(BlueprintCallable, Category = "BossWeapon")
void ACBossWeapon::BossAttachToCollision(USceneComponent* InParent, FName InSocketName)
{
	if(InParent)
	{
		// 콜리전 컴포넌트를 지정된 소켓에 부착
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		CollisionComponent->AttachToComponent(InParent, AttachmentRules, InSocketName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InParent is nullptr in BossAttachToCollision!"));
	}
}
```

*   **역할**: 무기의 콜리전 컴포넌트를 지정된 부모 컴포넌트의 소켓에 부착하는 함수입니다.
*   **매개변수**:
    *   `InParent` (USceneComponent\*): 콜리전 컴포넌트를 부착할 부모 컴포넌트입니다.
    *   `InSocketName` (FName): 콜리전 컴포넌트를 부착할 소켓의 이름입니다.
*   **반환값**: 없음
*   **예외 처리**: `InParent`가 nullptr인 경우, 에러 로그를 출력합니다.
*   **사용 예제**:

```c++
// C++ 코드에서 호출
MyWeapon->BossAttachToCollision(BossMeshComponent, FName("WeaponSocket"));
```

*   **베스트 프랙티스**: 무기를 보스 캐릭터의 메시 컴포넌트에 부착할 때 사용합니다.
*   **성능 특성**: 컴포넌트 부착은 비교적 비용이 많이 드는 작업이므로, 필요한 시점에만 호출해야 합니다.

#### 2.10 `StartCollisionAtSocket`

```c++
UFUNCTION(BlueprintCallable, Category = "BossWeapon")
void ACBossWeapon::StartCollisionAtSocket(USceneComponent* InParent, FName InSocketName)
{
	if (InParent)
	{
		// 메시 컴포넌트를 지정된 소켓에 부착
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		MeshComponent->AttachToComponent(InParent, AttachmentRules, InSocketName);

		// 콜리전 활성화
		OnSelectCollision();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InParent is nullptr in StartCollisionAtSocket!"));
	}
}
```

*   **역할**: 무기의 메시 컴포넌트를 지정된 부모 컴포넌트의 소켓에 부착하고, 콜리전을 활성화하는 함수입니다.
*   **매개변수**:
    *   `InParent` (USceneComponent\*): 메시 컴포넌트를 부착할 부모 컴포넌트입니다.
    *   `InSocketName` (FName): 메시 컴포넌트를 부착할 소켓의 이름입니다.
*   **반환값**: 없음
*   **예외 처리**: `InParent`가 nullptr인 경우, 에러 로그를 출력합니다.
*   **사용 예제**:

```c++
// C++ 코드에서 호출
MyWeapon->StartCollisionAtSocket(BossMeshComponent, FName("WeaponSocket"));
```

*   **베스트 프랙티스**: 공격 애니메이션 시작 시, 무기를 특정 소켓에 부착하고 콜리전을 활성화하는 데 사용합니다.
*   **성능 특성**: 컴포넌트 부착 및 콜리전 활성화는 비교적 비용이 많이 드는 작업이므로, 필요한 시점에만 호출해야 합니다.

#### 2.11 `EndCollisionToOwner`

```c++
UFUNCTION(BlueprintCallable, Category = "BossWeapon")
void ACBossWeapon::EndCollisionToOwner(USceneComponent* InParent, FName InSocketName)
{
	if (InParent)
	{
		// 메시 컴포넌트를 지정된 소켓에 부착
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		MeshComponent->AttachToComponent(InParent, AttachmentRules, InSocketName);

		// 콜리전 비활성화
		OffBossCollisions();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InParent is nullptr in EndCollisionToOwner!"));
	}
}
```

*   **역할**: 무기의 메시 컴포넌트를 지정된 부모 컴포넌트의 소켓에 부착하고, 콜리전을 비활성화하는 함수입니다.
*   **매개변수**:
    *   `InParent` (USceneComponent\*): 메시 컴포넌트를 부착할 부모 컴포넌트입니다.
    *   `InSocketName` (FName): 메시 컴포넌트를 부착할 소켓의 이름입니다.
*   **반환값**: 없음
*   **예외 처리**: `InParent`가 nullptr인 경우, 에러 로그를 출력합니다.
*   **사용 예제**:

```c++
// C++ 코드에서 호출
MyWeapon->EndCollisionToOwner(BossMeshComponent, FName("WeaponSocket"));
```

*   **베스트 프랙티스**: 공격 애니메이션 종료 시, 무기를 특정 소켓에 부착하고 콜리전을 비활성화하는 데 사용합니다.
*   **성능 특성**: 컴포넌트 부착 및 콜리전 비활성화는 비교적 비용이 많이 드는 작업이므로, 필요한 시점에만 호출해야 합니다.

#### 2.12 `BeginPlay`

```c++
void ACBossWeapon::BeginPlay()
{
	Super::BeginPlay();

}
```

*   **역할**: 액터가 게임에 배치된 후, 게임 시작 시점에 호출되는 함수입니다.
*   **매개변수**: 없음
*   **반환값**: 없음
*   **예외 처리**: 없음
*   **사용 예제**:

```c++
// C++ 코드에서 오버라이드
void AMyBossWeapon::BeginPlay()
{
	Super::BeginPlay();
	// 초기 상태 설정
	MeshComponent->SetVisibility(false); // 초기에는 무기 숨김
}
```

*   **베스트 프랙티스**: 액터의 초기 상태를 설정하는 데 사용합니다. 예를 들어, 무기의 메시를 숨기거나, 특정 변수를 초기화할 수 있습니다.
*   **성능 특성**: 게임 시작 시 한 번만 호출되므로, 성능에 큰 영향을 미치지 않습니다.

#### 2.13 `Tick`

```c++
void ACBossWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
```

*   **역할**: 매 프레임마다 호출되는 함수입니다.
*   **매개변수**:
    *   `DeltaTime` (float): 이전 프레임과 현재 프레임 사이의 시간 간격입니다.
*   **반환값**: 없음
*   **예외 처리**: 없음
*   **사용 예제**:

```c++
// C++ 코드에서 오버라이드
void AMyBossWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// 무기 회전 로직 (예시)
	FRotator NewRotation = MeshComponent->GetComponentRotation();
	NewRotation.Yaw += 10.0f * DeltaTime;
	MeshComponent->SetWorldRotation(NewRotation);
}
```

*   **베스트 프랙티스**: 액터의 상태를 업데이트하거나, 특정 로직을 주기적으로 실행하는 데 사용합니다. 하지만, `Tick` 함수는 매 프레임마다 호출되므로, 성능에 영향을 미칠 수 있는 복잡한 로직은 피하는 것이 좋습니다.
*   **성능 특성**: `Tick` 함수는 매 프레임마다 호출되므로, 성능에 큰 영향을 미칠 수 있습니다. 필요한 경우에만 사용하고, 최적화에 신경 써야 합니다.

#### 2.14 `BossAttachTo`

```c++
UFUNCTION(BlueprintCallable, Category = "BossWeapon")
void ACBossWeapon::BossAttachTo(USceneComponent* InParent, FName InSocketName)
{
	if (InParent)
	{
		// 메시 컴포넌트를 지정된 소켓에 부착
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		MeshComponent->AttachToComponent(InParent, AttachmentRules, InSocketName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InParent is nullptr in BossAttachTo!"));
	}
}
```

*   **역할**: 무기의 메시 컴포넌트를 지정된 부모 컴포넌트의 소켓에 부착하는 함수입니다.
*   **매개변수**:
    *   `InParent` (USceneComponent\*): 메시 컴포넌트를 부착할 부모 컴포넌트입니다.
    *   `InSocketName` (FName): 메시 컴포넌트를 부착할 소켓의 이름입니다.
*   **반환값**: 없음
*   **예외 처리**: `InParent`가 nullptr인 경우, 에러 로그를 출력합니다.
*   **사용 예제**:

```c++
// C++ 코드에서 호출
MyWeapon->BossAttachTo(BossMeshComponent, FName("WeaponSocket"));
```

*   **베스트 프랙티스**: 무기를 보스 캐릭터의 메시 컴포넌트에 부착할 때 사용합니다.
*   **성능 특성**: 컴포넌트 부착은 비교적 비용이 많이 드는 작업이므로, 필요한 시점에만 호출해야 합니다.

#### 2.15 `OnBossComponentBeginOverlap`

```c++
UFUNCTION()
void ACBossWeapon::OnBossComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 콜리전 시작 시 로직
	UE_LOG(LogTemp, Warning, TEXT("OnBossComponentBeginOverlap called!"));
	// Example: Apply damage to the overlapped actor
	if (OtherActor && OtherActor != this && OtherComp)
	{
		// 플레이어에게 데미지 적용 (예시)
		UGameplayStatics::ApplyPointDamage(OtherActor, 20.0f, (OtherActor->GetActorLocation() - GetActorLocation()), SweepResult, nullptr, this, UDamageType::StaticClass());
	}
}
```

*   **역할**: 콜리전 컴포넌트가 다른 액터와 겹치기 시작할 때 호출되는 함수입니다.
*   **매개변수**:
    *   `OverlappedComponent` (UPrimitiveComponent\*): 겹쳐진 컴포넌트입니다.
    *   `OtherActor` (AActor\*): 겹쳐진 다른 액터입니다.
    *   `OtherComp` (UPrimitiveComponent\*): 겹쳐진 다른 액터의 컴포넌트입니다.
    *   `OtherBodyIndex` (int32): 겹쳐진 다른 액터의 바디 인덱스입니다.
    *   `bFromSweep` (bool): 스윕(Sweep)에 의한 겹침인지 여부입니다.
    *   `SweepResult` (const FHitResult&): 스윕 결과입니다.
*   **반환값**: 없음
*   **예외 처리**: 없음
*   **사용 예제**:

```c++
// C++ 코드에서 구현
void AMyBossWeapon::OnBossComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBossComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	// 플레이어에게 데미지 적용 (예시)
	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag("Player"))
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, 20.0f, (OtherActor->GetActorLocation() - GetActorLocation()), SweepResult, nullptr, this, UDamageType::StaticClass());
	}
}
```

*   **베스트 프랙티스**: 콜리전이 발생했을 때, 데미지를 적용하거나, 특정 이펙트를 재생하는 데 사용합니다.
*   **성능 특성**: 콜리전 이벤트는 성능에 영향을 미칠 수 있으므로, 필요한 경우에만 사용하고, 최적화에 신경 써야 합니다.

#### 2.16 `OnBossComponentEndOverlap`

```c++
UFUNCTION()
void ACBossWeapon::OnBossComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// 콜리전 종료 시 로직
	UE_LOG(LogTemp, Warning, TEXT("OnBossComponentEndOverlap called!"));
}
```

*   **역할**: 콜리전 컴포넌트가 다른 액터와의 겹침이 종료될 때 호출되는 함수입니다.
*   **매개변수**:
    *   `OverlappedComponent` (UPrimitiveComponent\*): 겹쳐진 컴포넌트입니다.
    *   `OtherActor` (AActor\*): 겹쳐진 다른 액터입니다.
    *   `OtherComp` (UPrimitiveComponent\*): 겹쳐진 다른 액터의 컴포넌트입니다.
    *   `OtherBodyIndex` (int32): 겹쳐진 다른 액터의 바디 인덱스입니다.
*   **반환값**: 없음
*   **예외 처리**: 없음
*   **사용 예제**:

```c++
// C++ 코드에서 구현
void AMyBossWeapon::OnBossComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnBossComponentEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	// 콜리전 종료 시 로직 (예시)
	UE_LOG(LogTemp, Log, TEXT("Overlap Ended with: %s"), *GetNameSafe(OtherActor));
}
```

*   **베스트 프랙티스**: 콜리전이 종료되었을 때, 특정 상태를 초기화하거나, 이펙트를 중단하는 데 사용합니다.
*   **성능 특성**: 콜리전 이벤트는 성능에 영향을 미칠 수 있으므로, 필요한 경우에만 사용하고, 최적화에 신경 써야 합니다.

### 3. 사용 예제 (완전한 코드)

다음은 `ACBossWeapon` 클래스를 사용하는 완전한 코드 예제입니다. 이 예제에서는 보스 무기를 장착하고, 공격 애니메이션에 따라 콜리전을 활성화/비활성화하며, 플레이어에게 데미지를 입히는 로직을 구현합니다.

```c++
// MyBossWeapon.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "ACBossWeapon.h"
#include "MyBoss.h" // 가정: 보스 캐릭터 클래스 이름이 MyBoss임
#include "MyBossWeapon.generated.h"

UCLASS()
class YOUR_PROJECT_API AMyBossWeapon : public ACBossWeapon
{
	GENERATED_BODY()

public:
	AMyBossWeapon();

protected:
	virtual void BeginPlay() override;

	// 콜리전 시작 시 호출되는 함수
	UFUNCTION()
	void OnBossComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// 콜리전 종료 시 호출되는 함수
	UFUNCTION()
	void OnBossComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	// 무기 장착 시작 시 호출되는 함수
	virtual void OnBossBeginEquip_Implementation() override;

	// 무기 해제 시작 시 호출되는 함수
	virtual void OnBossUnequip_Implementation() override;

public:
	// 무기 메시 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* MeshComponent;

	// 콜리전 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionComponent;

	// 데미지 양
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float DamageAmount;
};

// MyBossWeapon.cpp
#include "MyBossWeapon.h"

AMyBossWeapon::AMyBossWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	// 메시 컴포넌트 생성 및 설정
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
	MeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	// 콜리전 컴포넌트 생성 및 설정
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	CollisionComponent->SetGenerateOverlapEvents(false); // 초기에는 콜리전 비활성화
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyBossWeapon::OnBossComponentBeginOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AMyBossWeapon::OnBossComponentEndOverlap);

	// 데미지 양 초기화
	DamageAmount = 20.0f;
}

void AMyBossWeapon::BeginPlay()
{
	Super::BeginPlay();
	MeshComponent->SetVisibility(false); // 초기에는 무기 숨김
}

void AMyBossWeapon::OnBossComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnBossComponentBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	// 플레이어에게 데미지 적용
	if (OtherActor && OtherActor != this && OtherActor->ActorHasTag("Player"))
	{
		UGameplayStatics::ApplyPointDamage(OtherActor, DamageAmount, (OtherActor->GetActorLocation() - GetActorLocation()), SweepResult, nullptr, this, UDamageType::StaticClass());
	}
}

void AMyBossWeapon::OnBossComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnBossComponentEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AMyBossWeapon::OnBossBeginEquip_Implementation()
{
	Super::OnBossBeginEquip_Implementation();
	MeshComponent->SetVisibility(true); // 무기 보이게 하기
}

void AMyBossWeapon::OnBossUnequip_Implementation()
{
	Super::OnBossUnequip_Implementation();
	MeshComponent->SetVisibility(false); // 무기 숨기기
}
```

**사용 방법:**

1.  `MyBossWeapon` 클래스를 생성하고, 에디터에서 메시 컴포넌트에 사용할 스태틱 메시를 할당합니다.
2.  콜리전 컴포넌트의 크기를 적절하게 조절합니다.
3.  보스 캐릭터(`MyBoss`)에 `MyBossWeapon` 액터를 추가하고, 소켓에 부착합니다.
4.  공격 애니메이션에 애니메이션 노티파이를 추가하여, `OnBossCollisions` 함수를 호출하여 콜리전을 활성화/비활성화합니다.

### 4. 성능 고려사항

*   **콜리전 처리**: 콜리전 이벤트는 성능에 영향을 미칠 수 있으므로, 필요한 경우에만 사용하고, 콜리전 컴포넌트의 크기를 최소화하여 불필요한 콜리전 검사를 줄여야 합니다.
*   **파티클 이펙트**: 파티클 이펙트의 품질과 개수를 조절하여, GPU 부하를 줄여야 합니다.
*   **메모리 사용량**: 무기의 메시 및 텍스처는 메모리를 많이 사용할 수 있으므로, LOD (Level of Detail)를 사용하여 메모리 사용량을 최적화해야 합니다.

### 5. 호환성 정보

*   Unreal Engine 버전: 4.26 이상
*   플랫폼: Windows, Linux, Mac, iOS, Android
*   컴파일러: Visual Studio 2019 이상 (Windows), Xcode (Mac), Clang (Linux)
*   의존성: 없음

### 6. 트러블슈팅

*   **콜리전이 제대로 작동하지 않는 경우**:
    *   콜리전 컴포넌트의 콜리전 프로파일이 올바르게 설정되었는지 확인합니다.
    *   애니메이션 노티파이가 올바른 시점에 `OnBossCollisions` 함수를 호출하는지 확인합니다.
    *   플레이어 캐릭터의 콜리전 설정이 올바른지 확인합니다.
*   **무기가 제대로 장착/해제되지 않는 경우**:
    *   `OnBossBeginEquip` 및 `OnBossUnequip` 함수가 제대로 호출되는지 확인합니다.
    *   무기의 메시가 올바른 소켓에 부착되었는지 확인합니다.
*   **데미지가 제대로 적용되지 않는 경우**:
    *   `OnBossComponentBeginOverlap` 함수가 제대로 호출되는지 확인합니다.
    *   `UGameplayStatics::ApplyPointDamage` 함수의 매개변수가 올바르게 설정되었는지 확인합니다.

### 7. 다음 단계

다음으로는 `AFlySpline` 클래스에 대한 상세 API 문서를 작성하겠습니다. `AFlySpline` 클래스는 보스 캐릭터의 비행 경로를 정의하는 데 사용됩니다.


## AFlySpline 상세 API 문서

`AFlySpline` 클래스는 보스 캐릭터의 비행 경로를 정의하는 액터 클래스입니다. 이 클래스를 사용하면 언리얼 엔진 에디터에서 스플라인을 직접 편집하여 보스의 복잡한 비행 패턴을 쉽게 만들 수 있습니다.

### 1. 클래스 개요

*   **클래스 이름**: `AFlySpline`
*   **상속 관계**: `AActor`
*   **클래스 목적**: 보스 캐릭터의 비행 경로를 정의하고 관리합니다.
*   **주요 기능**:
    *   언리얼 엔진 에디터에서 스플라인 편집을 지원합니다.
    *   스플라인을 따라 비행 경로를 생성하고 관리합니다.
    *   비행 경로를 구성하는 실린더 및 림 메시를 생성합니다.
    *   스플라인의 특정 인덱스에 접근하는 기능을 제공합니다.

### 2. 사용 시나리오

`AFlySpline` 클래스는 다음과 같은 시나리오에서 유용하게 사용될 수 있습니다.

*   **보스 전투 디자인**: 보스의 복잡하고 예측 불가능한 비행 패턴을 디자인합니다.
*   **컷신 연출**: 컷신에서 캐릭터의 비행 경로를 제어합니다.
*   **게임 플레이 요소**: 플레이어가 따라갈 수 있는 비행 경로를 생성합니다.

**예제 시나리오**:

1.  언리얼 엔진 에디터에서 `AFlySpline` 액터를 레벨에 배치합니다.
2.  스플라인 컴포넌트를 사용하여 원하는 비행 경로를 만듭니다.
3.  보스 캐릭터의 AI 컨트롤러에서 `AFlySpline` 액터를 참조합니다.
4.  보스 캐릭터가 스플라인을 따라 비행하도록 AI 로직을 구현합니다.

### 3. 클래스 멤버 상세 설명

#### 3.1. 생성자

```c++
AFlySpline();
```

*   **설명**: `AFlySpline` 클래스의 생성자입니다.
*   **기능**: 클래스의 기본 속성을 초기화합니다.

#### 3.2. `OnConstruction` 함수

```c++
virtual void OnConstruction(const FTransform& Transform) override;
```

*   **설명**: 액터가 에디터에서 생성되거나 수정될 때 호출되는 함수입니다.
*   **매개변수**:
    *   `Transform`: 액터의 트랜스폼 정보입니다.
*   **기능**: 스플라인을 기반으로 실린더 및 림 메시를 생성합니다. 이 함수는 에디터에서 스플라인이 변경될 때마다 자동으로 호출되어 비행 경로를 시각적으로 업데이트합니다.
*   **사용 예제**:

```c++
void AFlySpline::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);

    // 스플라인을 기반으로 실린더 및 림 메시를 생성합니다.
    BuildCylinderAndRims();
}
```

#### 3.3. `BuildCylinderAndRims` 함수

```c++
void BuildCylinderAndRims();
```

*   **설명**: 스플라인을 따라 실린더 및 림 메시를 생성하는 함수입니다.
*   **기능**: 스플라인 포인트를 기반으로 실린더 메시와 림 메시를 생성하여 비행 경로를 시각적으로 표현합니다.
*   **사용 예제**:

```c++
void AFlySpline::BuildCylinderAndRims()
{
    // 기존 메시 컴포넌트를 제거합니다.
    for (auto& Component : CylinderComponents)
    {
        Component->DestroyComponent();
    }
    CylinderComponents.Empty();

    for (auto& Component : RimComponents)
    {
        Component->DestroyComponent();
    }
    RimComponents.Empty();

    // 스플라인 포인트를 기반으로 실린더 및 림 메시를 생성합니다.
    const int32 NumSplinePoints = SplineComponent->GetNumberOfSplinePoints();
    for (int32 i = 0; i < NumSplinePoints - 1; ++i)
    {
        // 실린더 메시 생성
        UStaticMeshComponent* Cylinder = NewObject<UStaticMeshComponent>(this);
        Cylinder->RegisterComponent();
        Cylinder->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        Cylinder->SetStaticMesh(CylinderMesh);

        FVector StartLocation = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::World);
        FVector EndLocation = SplineComponent->GetLocationAtSplinePoint(i + 1, ESplineCoordinateSpace::World);
        FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
        float Distance = FVector::Distance(StartLocation, EndLocation);
        FRotator Rotation = Direction.Rotation();

        Cylinder->SetWorldLocation(StartLocation + Direction * Distance * 0.5f);
        Cylinder->SetWorldRotation(Rotation);
        Cylinder->SetWorldScale3D(FVector(CylinderRadius, Distance * 0.5f, CylinderRadius));

        CylinderComponents.Add(Cylinder);

        // 림 메시 생성
        UStaticMeshComponent* RimStart = NewObject<UStaticMeshComponent>(this);
        RimStart->RegisterComponent();
        RimStart->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        RimStart->SetStaticMesh(RimMesh);
        RimStart->SetWorldLocation(StartLocation);
        RimStart->SetWorldScale3D(FVector(RimScale));

        FRotator RimStartRotation = FRotator(0.0f, Rotation.Yaw + 90.0f, 0.0f);
        RimStart->SetWorldRotation(RimStartRotation);
        RimComponents.Add(RimStart);

        UStaticMeshComponent* RimEnd = NewObject<UStaticMeshComponent>(this);
        RimEnd->RegisterComponent();
        RimEnd->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        RimEnd->SetStaticMesh(RimMesh);
        RimEnd->SetWorldLocation(EndLocation);
        RimEnd->SetWorldScale3D(FVector(RimScale));

        FRotator RimEndRotation = FRotator(0.0f, Rotation.Yaw + 90.0f, 0.0f);
        RimEnd->SetWorldRotation(RimEndRotation);
        RimComponents.Add(RimEnd);
    }
}
```

#### 3.4. `GetHorizontalSplines` 함수

```c++
TArray<USplineComponent*> GetHorizontalSplines();
```

*   **설명**: 수평 스플라인 컴포넌트 배열을 반환하는 함수입니다.
*   **반환값**: `TArray<USplineComponent*>` - 수평 스플라인 컴포넌트 배열입니다.
*   **기능**: 수평 스플라인 컴포넌트 배열을 반환합니다.

#### 3.5. `GetSplineAtIndex` 함수

```c++
USplineComponent* GetSplineAtIndex(int32 Index);
```

*   **설명**: 지정된 인덱스의 스플라인 컴포넌트를 반환하는 함수입니다.
*   **매개변수**:
    *   `Index`: 스플라인 컴포넌트의 인덱스입니다.
*   **반환값**: `USplineComponent*` - 지정된 인덱스의 스플라인 컴포넌트입니다.
*   **기능**: 지정된 인덱스의 스플라인 컴포넌트를 반환합니다.

#### 3.6. `BuildRim` 함수

```c++
void BuildRim();
```

*   **설명**: 림 메시를 생성하는 함수입니다.
*   **기능**: 스플라인 포인트를 기반으로 림 메시를 생성하여 비행 경로의 끝점을 시각적으로 표현합니다.

#### 3.7. `CreateHorizontalSplines` 함수

```c++
void CreateHorizontalSplines();
```

*   **설명**: 수평 스플라인 컴포넌트를 생성하는 함수입니다.
*   **기능**: 수평 스플라인 컴포넌트를 생성하고 초기화합니다.

### 4. 성능 고려사항

*   **메시 복잡도**: 실린더 및 림 메시의 복잡도가 높을수록 렌더링 부하가 증가할 수 있습니다. 메시의 LOD (Level of Detail)를 사용하여 성능을 최적화하십시오.
*   **스플라인 포인트 수**: 스플라인 포인트 수가 많을수록 메시 생성 및 업데이트에 더 많은 시간이 소요될 수 있습니다. 필요한 최소한의 스플라인 포인트를 사용하여 성능을 유지하십시오.
*   **업데이트 빈도**: `OnConstruction` 함수는 에디터에서 스플라인이 변경될 때마다 호출됩니다. 스플라인을 자주 변경하면 성능에 영향을 미칠 수 있습니다.

### 5. 호환성 정보

*   **Unreal Engine 버전**: 4.26 이상
*   **플랫폼**: Windows, Linux, Mac, iOS, Android
*   **컴파일러**: Visual Studio 2019 이상 (Windows), Xcode (Mac), Clang (Linux)
*   **의존성**: 없음

### 6. 트러블슈팅

*   **스플라인 메시가 제대로 표시되지 않는 경우**:
    *   `CylinderMesh` 및 `RimMesh` 변수가 올바르게 설정되었는지 확인합니다.
    *   메시의 스케일 및 회전이 올바른지 확인합니다.
    *   `BuildCylinderAndRims` 함수가 제대로 호출되는지 확인합니다.
*   **스플라인 편집 시 성능 문제가 발생하는 경우**:
    *   메시 복잡도를 줄입니다.
    *   스플라인 포인트 수를 줄입니다.
    *   `OnConstruction` 함수의 호출 빈도를 줄입니다.

### 7. 다음 단계

다음으로는 `AGateOfBabylon` 클래스에 대한 상세 API 문서를 작성하겠습니다. `AGateOfBabylon` 클래스는 보스 캐릭터의 특수 공격인 '바빌론의 문'을 구현하는 데 사용됩니다.


## AGateOfBabylon 상세 API 문서

### 1. 클래스 개요

`AGateOfBabylon` 클래스는 보스 캐릭터의 특수 공격인 '바빌론의 문'을 구현하는 데 사용되는 액터 클래스입니다. 이 클래스는 지정된 위치에 여러 개의 투사체를 생성하고 발사하여 플레이어에게 위협적인 공격을 수행합니다. 투사체는 오브젝트 풀링을 사용하여 관리되어 성능을 최적화합니다.

**목적 및 역할:**

*   바빌론의 문 공격 패턴 구현
*   투사체 오브젝트 풀링 관리
*   투사체 생성 및 발사 제어
*   플레이어를 향한 투사체 방향 설정

**사용 시나리오:**

*   보스 전투 중 특정 페이즈에서 '바빌론의 문' 공격 활성화
*   플레이어가 특정 영역에 진입했을 때 공격 발동
*   특정 애니메이션 노티파이에서 공격 발동

**상속 관계:**

*   `AActor`

**인터페이스:**

*   `ActivateGate()`: 바빌론의 문 공격을 활성화합니다.
*   `DeactivateGate()`: 바빌론의 문 공격을 비활성화합니다.
*   `SpawnProjectile()`: 오브젝트 풀에서 투사체를 가져와 발사합니다.

**주의사항 및 제약사항:**

*   투사체 풀의 크기가 적절하게 설정되어야 합니다. 풀 크기가 너무 작으면 투사체 생성에 실패할 수 있습니다.
*   투사체의 데미지, 속도, 수명 등의 속성은 적절하게 조정되어야 합니다.
*   `ActivateGate()`와 `DeactivateGate()` 함수를 적절한 시점에 호출하여 공격을 제어해야 합니다.

**성능 고려사항:**

*   투사체 개수가 많을수록 렌더링 부하가 증가할 수 있습니다. 투사체 개수를 최적화하거나 LOD를 사용하여 성능을 개선하십시오.
*   투사체 이동 로직이 복잡할수록 CPU 부하가 증가할 수 있습니다. 이동 로직을 최적화하거나 단순화하십시오.
*   오브젝트 풀링을 사용하여 투사체 생성 및 소멸 비용을 줄이십시오.

### 2. 함수 상세 설명

#### 2.1 `AGateOfBabylon()`

```c++
AGateOfBabylon();
```

**설명:** `AGateOfBabylon` 클래스의 생성자입니다. 멤버 변수를 초기화하고 오브젝트 풀을 설정합니다.

**매개변수:** 없음

**반환값:** 없음

**예외 처리:** 없음

**사용 예제:**

```c++
AGateOfBabylon::AGateOfBabylon()
{
    // Tick 함수 활성화
    PrimaryActorTick.bCanEverTick = true;

    // 투사체 메시 컴포넌트 생성 및 설정
    ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
    RootComponent = ProjectileMesh;
    ProjectileMesh->SetCollisionProfileName(TEXT("NoCollision"));

    // 투사체 발사 위치 컴포넌트 생성
    ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
    ProjectileSpawnPoint->SetupAttachment(RootComponent);

    // 초기 투사체 풀 크기 설정
    InitialPoolSize = 20;

    // 투사체 클래스 설정 (에디터에서 설정 가능)
    ProjectileClass = AGateOfBabyonProjectile::StaticClass();
}
```

**성능 특성:** 생성자는 초기화 작업만 수행하므로 성능에 미치는 영향은 미미합니다.

#### 2.2 `Tick(float DeltaTime)`

```c++
virtual void Tick(float DeltaTime) override;
```

**설명:** 매 프레임마다 호출되는 함수입니다. 투사체의 위치를 업데이트하고 필요한 로직을 수행합니다.

**매개변수:**

*   `DeltaTime` (float): 이전 프레임 이후 경과된 시간 (초)

**반환값:** 없음

**예외 처리:** 없음

**사용 예제:**

```c++
void AGateOfBabylon::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 플레이어를 바라보도록 회전
    UpdateLookAtPlayer();
}
```

**성능 특성:** `Tick` 함수는 매 프레임마다 호출되므로 성능에 미치는 영향이 큽니다. 따라서 불필요한 연산을 최소화해야 합니다.

#### 2.3 `ActivateGate()`

```c++
UFUNCTION(BlueprintCallable, Category = "GateOfBabylon")
void ActivateGate();
```

**설명:** 바빌론의 문 공격을 활성화합니다. 투사체 발사를 시작하고 필요한 초기 설정을 수행합니다.

**매개변수:** 없음

**반환값:** 없음

**예외 처리:** 투사체 풀이 비어있는 경우 예외를 처리해야 합니다.

**사용 예제:**

```c++
void AGateOfBabylon::ActivateGate()
{
    // 이미 활성화되어 있다면 중복 실행 방지
    if (bIsActive) return;

    bIsActive = true;

    // 타이머를 사용하여 투사체 지속적으로 발사
    GetWorldTimerManager().SetTimer(ProjectileSpawnTimerHandle, this, &AGateOfBabylon::SpawnProjectile, ProjectileSpawnInterval, true, 0.0f);
}
```

**성능 특성:** `ActivateGate` 함수는 초기 설정 작업만 수행하므로 성능에 미치는 영향은 미미합니다. 하지만 타이머를 사용하여 투사체를 지속적으로 발사하는 경우, `SpawnProjectile` 함수의 성능이 중요합니다.

#### 2.4 `DeactivateGate()`

```c++
UFUNCTION(BlueprintCallable, Category = "GateOfBabylon")
void DeactivateGate();
```

**설명:** 바빌론의 문 공격을 비활성화합니다. 투사체 발사를 중단하고 활성화된 투사체를 회수합니다.

**매개변수:** 없음

**반환값:** 없음

**예외 처리:** 없음

**사용 예제:**

```c++
void AGateOfBabylon::DeactivateGate()
{
    // 이미 비활성화되어 있다면 중복 실행 방지
    if (!bIsActive) return;

    bIsActive = false;

    // 타이머 중지
    GetWorldTimerManager().ClearTimer(ProjectileSpawnTimerHandle);

    // 활성화된 모든 투사체 회수 (구현 필요)
    // 예: 모든 투사체에 대해 DeactivateProjectile() 호출
}
```

**성능 특성:** `DeactivateGate` 함수는 타이머를 중단하고 투사체를 회수하는 작업을 수행하므로 성능에 미치는 영향은 미미합니다. 하지만 활성화된 투사체가 많을 경우 회수 작업에 시간이 오래 걸릴 수 있습니다.

#### 2.5 `BeginPlay()`

```c++
virtual void BeginPlay() override;
```

**설명:** 액터가 스폰될 때 호출되는 함수입니다. 투사체 풀을 초기화합니다.

**매개변수:** 없음

**반환값:** 없음

**예외 처리:** 없음

**사용 예제:**

```c++
void AGateOfBabylon::BeginPlay()
{
    Super::BeginPlay();

    // 투사체 풀 초기화
    InitializeProjectilePool();
}
```

**성능 특성:** `BeginPlay` 함수는 초기화 작업만 수행하므로 성능에 미치는 영향은 미미합니다.

#### 2.6 `InitializeProjectilePool()`

```c++
UFUNCTION(BlueprintCallable, Category = "GateOfBabylon")
void InitializeProjectilePool();
```

**설명:** 투사체 오브젝트 풀을 초기화합니다. 미리 지정된 개수의 투사체를 생성하여 풀에 저장합니다.

**매개변수:** 없음

**반환값:** 없음

**예외 처리:** 투사체 클래스가 유효하지 않은 경우 예외를 처리해야 합니다.

**사용 예제:**

```c++
void AGateOfBabylon::InitializeProjectilePool()
{
    // 투사체 풀 초기화
    for (int32 i = 0; i < InitialPoolSize; ++i)
    {
        AGateOfBabyonProjectile* Projectile = GetWorld()->SpawnActor<AGateOfBabyonProjectile>(ProjectileClass);
        if (Projectile)
        {
            Projectile->DeactivateProjectile(); // 초기 상태는 비활성화
            ProjectilePool.Add(Projectile);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn projectile for pool initialization!"));
        }
    }
}
```

**성능 특성:** `InitializeProjectilePool` 함수는 초기화 작업이므로 게임 시작 시 한 번만 호출됩니다. 풀 크기가 클수록 초기화 시간이 오래 걸릴 수 있습니다.

#### 2.7 `GetProjectileFromPool()`

```c++
UFUNCTION(BlueprintCallable, Category = "GateOfBabylon")
AGateOfBabyonProjectile* GetProjectileFromPool();
```

**설명:** 오브젝트 풀에서 사용 가능한 투사체를 가져옵니다. 풀에 사용 가능한 투사체가 없으면 `nullptr`을 반환합니다.

**매개변수:** 없음

**반환값:**

*   `AGateOfBabyonProjectile*`: 사용 가능한 투사체 액터 포인터. 풀에 사용 가능한 투사체가 없으면 `nullptr` 반환.

**예외 처리:** 없음

**사용 예제:**

```c++
AGateOfBabyonProjectile* AGateOfBabylon::GetProjectileFromPool()
{
    // 풀에서 사용 가능한 투사체 검색
    for (AGateOfBabyonProjectile* Projectile : ProjectilePool)
    {
        if (!Projectile->IsActive())
        {
            return Projectile;
        }
    }

    // 풀에 사용 가능한 투사체가 없으면 nullptr 반환
    return nullptr;
}
```

**성능 특성:** `GetProjectileFromPool` 함수는 풀을 순회하여 사용 가능한 투사체를 검색하므로 풀 크기가 클수록 검색 시간이 오래 걸릴 수 있습니다.

#### 2.8 `SpawnProjectile()`

```c++
UFUNCTION(BlueprintCallable, Category = "GateOfBabylon")
void SpawnProjectile();
```

**설명:** 오브젝트 풀에서 투사체를 가져와 발사합니다. 투사체의 위치, 회전, 속도 등을 설정하고 활성화합니다.

**매개변수:** 없음

**반환값:** 없음

**예외 처리:** 풀에서 투사체를 가져오는 데 실패하거나 투사체 속성 설정에 실패하는 경우 예외를 처리해야 합니다.

**사용 예제:**

```c++
void AGateOfBabylon::SpawnProjectile()
{
    // 풀에서 투사체 가져오기
    AGateOfBabyonProjectile* Projectile = GetProjectileFromPool();

    if (Projectile)
    {
        // 투사체 위치 및 회전 설정
        FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
        FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

        Projectile->SetActorLocation(SpawnLocation);
        Projectile->SetActorRotation(SpawnRotation);

        // 투사체 활성화 및 발사
        Projectile->ActivateProjectile();
        Projectile->MoveToRandomLocationAroundPlayer(); // 플레이어 주변 랜덤 위치로 이동 로직
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Projectile pool is empty! Cannot spawn projectile."));
    }
}
```

**성능 특성:** `SpawnProjectile` 함수는 투사체 속성을 설정하고 활성화하는 작업을 수행하므로 성능에 미치는 영향이 비교적 큽니다. 특히 투사체 이동 로직이 복잡할수록 CPU 부하가 증가할 수 있습니다.

#### 2.9 `UpdateLookAtPlayer()`

```c++
UFUNCTION(BlueprintCallable, Category = "GateOfBabylon")
void UpdateLookAtPlayer();
```

**설명:** 바빌론의 문 액터가 플레이어를 바라보도록 회전을 업데이트합니다.

**매개변수:** 없음

**반환값:** 없음

**예외 처리:** 플레이어를 찾을 수 없는 경우 예외를 처리해야 합니다.

**사용 예제:**

```c++
void AGateOfBabylon::UpdateLookAtPlayer()
{
    // 플레이어 찾기
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (PlayerPawn)
    {
        // 플레이어 위치 계산
        FVector PlayerLocation = PlayerPawn->GetActorLocation();
        FVector BabylonLocation = GetActorLocation();

        // 플레이어를 바라보는 회전 계산
        FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(BabylonLocation, PlayerLocation);

        // 액터 회전 설정
        SetActorRotation(NewRotation);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Player not found! Cannot update look at rotation."));
    }
}
```

**성능 특성:** `UpdateLookAtPlayer` 함수는 플레이어 위치를 계산하고 회전을 설정하는 작업을 수행하므로 성능에 미치는 영향은 비교적 적습니다. 하지만 매 프레임마다 호출되므로 최적화가 필요할 수 있습니다.

### 3. 사용 예제 (완전한 코드)

다음은 `AGateOfBabylon` 클래스를 사용하는 완전한 코드 예제입니다.

```c++
// GateOfBabylon.h
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GateOfBabyonProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "GateOfBabylon.generated.h"

UCLASS()
class MYPROJECT_API AGateOfBabylon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGateOfBabylon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 바빌론의 문 공격 활성화
	UFUNCTION(BlueprintCallable, Category = "GateOfBabylon")
	void ActivateGate();

	// 바빌론의 문 공격 비활성화
	UFUNCTION(BlueprintCallable, Category = "GateOfBabylon")
	void DeactivateGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// 투사체 메시 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* ProjectileMesh;

	// 투사체 발사 위치 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* ProjectileSpawnPoint;

	// 투사체 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGateOfBabyonProjectile> ProjectileClass;

	// 투사체 풀
	UPROPERTY()
	TArray<AGateOfBabyonProjectile*> ProjectilePool;

	// 초기 투사체 풀 크기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	int32 InitialPoolSize;

	// 투사체 발사 간격 (초)
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	float ProjectileSpawnInterval = 0.2f;

	// 투사체 발사 타이머 핸들
	FTimerHandle ProjectileSpawnTimerHandle;

	// 공격 활성화 여부
	bool bIsActive = false;

	// 투사체 풀 초기화
	UFUNCTION(BlueprintCallable, Category = "GateOfBabylon")
	void InitializeProjectilePool();

	// 풀에서 투사체 가져오기
	UFUNCTION(BlueprintCallable, Category = "GateOfBabylon")
	AGateOfBabyonProjectile* GetProjectileFromPool();

	// 투사체 발사
	UFUNCTION(BlueprintCallable, Category = "GateOfBabylon")
	void SpawnProjectile();

	// 플레이어를 바라보도록 회전
	UFUNCTION(BlueprintCallable, Category = "GateOfBabylon")
	void UpdateLookAtPlayer();
};

// GateOfBabylon.cpp
#include "GateOfBabylon.h"

// Sets default values
AGateOfBabylon::AGateOfBabylon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 투사체 메시 컴포넌트 생성 및 설정
	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));
	RootComponent = ProjectileMesh;
	ProjectileMesh->SetCollisionProfileName(TEXT("NoCollision"));

	// 투사체 발사 위치 컴포넌트 생성
	ProjectileSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(RootComponent);

	// 초기 투사체 풀 크기 설정
	InitialPoolSize = 20;

	// 투사체 클래스 설정 (에디터에서 설정 가능)
	ProjectileClass = AGateOfBabyonProjectile::StaticClass();
}

// Called when the game starts or when spawned
void AGateOfBabylon::BeginPlay()
{
	Super::BeginPlay();

	// 투사체 풀 초기화
	InitializeProjectilePool();
}

// Called every frame
void AGateOfBabylon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 플레이어를 바라보도록 회전
	UpdateLookAtPlayer();
}

void AGateOfBabylon::ActivateGate()
{
    // 이미 활성화되어 있다면 중복 실행 방지
    if (bIsActive) return;

    bIsActive = true;

    // 타이머를 사용하여 투사체 지속적으로 발사
    GetWorldTimerManager().SetTimer(ProjectileSpawnTimerHandle, this, &AGateOfBabylon::SpawnProjectile, ProjectileSpawnInterval, true, 0.0f);
}

void AGateOfBabylon::DeactivateGate()
{
    // 이미 비활성화되어 있다면 중복 실행 방지
    if (!bIsActive) return;

    bIsActive = false;

    // 타이머 중지
    GetWorldTimerManager().ClearTimer(ProjectileSpawnTimerHandle);

    // 활성화된 모든 투사체 회수 (구현 필요)
    // 예: 모든 투사체에 대해 DeactivateProjectile() 호출
}

void AGateOfBabylon::InitializeProjectilePool()
{
    // 투사체 풀 초기화
    for (int32 i = 0; i < InitialPoolSize; ++i)
    {
        AGateOfBabyonProjectile* Projectile = GetWorld()->SpawnActor<AGateOfBabyonProjectile>(ProjectileClass);
        if (Projectile)
        {
            Projectile->DeactivateProjectile(); // 초기 상태는 비활성화
            ProjectilePool.Add(Projectile);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to spawn projectile for pool initialization!"));
        }
    }
}

AGateOfBabyonProjectile* AGateOfBabylon::GetProjectileFromPool()
{
    // 풀에서 사용 가능한 투사체 검색
    for (AGateOfBabyonProjectile* Projectile : ProjectilePool)
    {
        if (!Projectile->IsActive())
        {
            return Projectile;
        }
    }

    // 풀에 사용 가능한 투사체가 없으면 nullptr 반환
    return nullptr;
}

void AGateOfBabylon::SpawnProjectile()
{
    // 풀에서 투사체 가져오기
    AGateOfBabyonProjectile* Projectile = GetProjectileFromPool();

    if (Projectile)
    {
        // 투사체 위치 및 회전 설정
        FVector SpawnLocation = ProjectileSpawnPoint->GetComponentLocation();
        FRotator SpawnRotation = ProjectileSpawnPoint->GetComponentRotation();

        Projectile->SetActorLocation(SpawnLocation);
        Projectile->SetActorRotation(SpawnRotation);

        // 투사체 활성화 및 발사
        Projectile->ActivateProjectile();
        Projectile->MoveToRandomLocationAroundPlayer(); // 플레이어 주변 랜덤 위치로 이동 로직
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Projectile pool is empty! Cannot spawn projectile."));
    }
}

void AGateOfBabylon::UpdateLookAtPlayer()
{
    // 플레이어 찾기
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    if (PlayerPawn)
    {
        // 플레이어 위치 계산
        FVector PlayerLocation = PlayerPawn->GetActorLocation();
        FVector BabylonLocation = GetActorLocation();

        // 플레이어를 바라보는 회전 계산
        FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(BabylonLocation, PlayerLocation);

        // 액터 회전 설정
        SetActorRotation(NewRotation);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Player not found! Cannot update look at rotation."));
    }
}
```

이 예제 코드는 `AGateOfBabylon` 클래스의 기본적인 기능을 구현합니다. 필요에 따라 투사체 속성, 발사 패턴, 오브젝트 풀링 전략 등을 수정하여 다양한 '바빌론의 문' 공격 패턴을 만들 수 있습니다.

### 4. 성능 고려사항 (상세)

*   **메모리 사용량 및 할당 패턴:**
    *   **오브젝트 풀:** `ProjectilePool`은 `AGateOfBabyonProjectile` 액터의 포인터를 저장하므로, 풀 크기에 따라 메모리 사용량이 결정됩니다. 풀 크기가 클수록 더 많은 메모리를 사용합니다.
    *   **액터 생성/소멸:** 오브젝트 풀을 사용하면 액터 생성/소멸 빈도를 줄여 가비지 컬렉션 부하를 줄일 수 있습니다. 하지만 초기 풀 생성 시 메모리 할당이 발생합니다.
*   **실행 시간 및 복잡도:**
    *   **`GetProjectileFromPool()`:** 이 함수는 풀을 순회하여 사용 가능한 투사체를 검색하므로 O(N)의 시간 복잡도를 가집니다. 풀 크기가 클수록 검색 시간이 오래 걸릴 수 있습니다. 해시 테이블 또는 큐를 사용하여 검색 시간을 O(1)로 줄일 수 있습니다.
    *   **`SpawnProjectile()`:** 이 함수는 투사체의 위치, 회전, 속도 등을 설정하므로 비교적 많은 연산을 수행합니다. 투사체 이동 로직이 복잡할수록 CPU 부하가 증가합니다.
    *   **`UpdateLookAtPlayer()`:** 이 함수는 매 프레임마다 호출되므로 최적화가 필요합니다. 플레이어 위치를 캐싱하거나, 회전 변화가 미미한 경우 업데이트를 생략하는 방법을 고려할 수 있습니다.
*   **최적화 기법 및 팁:**
    *   **오브젝트 풀 크기 최적화:** 게임 플레이에 필요한 최소한의 투사체 개수를 파악하여 풀 크기를 설정하십시오. 풀 크기가 너무 크면 메모리 낭비가 발생하고, 너무 작으면 투사체 생성에 실패할 수 있습니다.
    *   **투사체 이동 로직 최적화:** 투사체 이동 로직을 단순화하거나, Look-Up Table (LUT)을 사용하여 연산량을 줄이십시오.
    *   **`UpdateLookAtPlayer()` 최적화:** 플레이어 위치를 캐싱하거나, 회전 변화가 미미한 경우 업데이트를 생략하십시오.
    *   **멀티스레딩:** 투사체 이동 로직이나 충돌 검사와 같이 CPU 부하가 큰 작업을 별도의 스레드에서 실행하여 메인 스레드의 부하를 줄일 수 있습니다.
*   **프로파일링 가이드:**
    *   **Unreal Insights:** Unreal Insights를 사용하여 CPU, GPU, 메모리 사용량을 프로파일링하고 성능 병목 지점을 식별하십시오.
    *   **Stat 명령:** `stat unitgraph`, `stat gpu`, `stat memory` 등의 Stat 명령을 사용하여 실시간 성능 정보를 확인하십시오.

### 5. 호환성 정보

*   **Unreal Engine 버전 요구사항:** 4.26 이상
*   **플랫폼별 지원 정보:** Windows, Linux, Mac, iOS, Android
*   **컴파일러 요구사항:** Visual Studio 2019 이상 (Windows), Xcode (Mac), Clang (Linux)
*   **의존성 정보:** 없음 (Unreal Engine 기본 클래스만 사용)

### 6. 트러블슈팅

*   **투사체가 생성되지 않는 경우:**
    *   `ProjectileClass` 변수가 올바르게 설정되었는지 확인합니다.
    *   `InitialPoolSize`가 0보다 큰 값으로 설정되었는지 확인합니다.
    *   투사체 클래스 (`AGateOfBabyonProjectile`)에 문제가 없는지 확인합니다.
    *   로그 메시지를 확인하여 오류가 발생했는지 확인합니다.
*   **투사체가 플레이어를 향해 발사되지 않는 경우:**
    *   `UpdateLookAtPlayer()` 함수가 제대로 호출되는지 확인합니다.
    *   플레이어 Pawn을 제대로 찾는지 확인합니다.
    *   `FindLookAtRotation()` 함수의 입력 값이 올바른지 확인합니다.
*   **성능 문제가 발생하는 경우:**
    *   투사체 개수를 줄입니다.
    *   투사체 이동 로직을 최적화합니다.
    *   오브젝트 풀 크기를 최적화합니다.
    *   Unreal Insights 또는 Stat 명령을 사용하여 성능 병목 지점을 식별하고 해결합니다.
*   **메모리 누수가 발생하는 경우:**
    *   오브젝트 풀에서 회수되지 않은 투사체가 있는지 확인합니다.
    *   `ActivateGate()`와 `DeactivateGate()` 함수가 제대로 호출되어 투사체 발사를 제어하는지 확인합니다.
    *   Unreal Engine 메모리 디버거를 사용하여 메모리 누수 지점을 식별하고 해결합니다.

### 7. 다음 단계

다음으로는 `AGateOfBabyonProjectile` 클래스에 대한 상세 API 문서를 작성하여, 투사체의 동작, 데미지 처리, 충돌 처리 등을 자세히 설명하겠습니다. 또한, 보스 AI와 연동하여 `AGateOfBabylon` 공격을 발동하는 방법에 대한 예제를 제공할 예정입니다.


```cpp
// GateOfBabyonProjectile.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "GateOfBabyonProjectile.generated.h"

/**
 * @brief AGateOfBabyonProjectile 클래스는 바빌론의 문에서 발사되는 투사체를 나타냅니다.
 *
 * 이 클래스는 투사체의 이동, 충돌, 활성화/비활성화 로직을 담당합니다.
 * 오브젝트 풀링 시스템과 함께 사용되어 성능을 최적화합니다.
 */
UCLASS()
class DDTPROJECT_API AGateOfBabyonProjectile : public AActor
{
	GENERATED_BODY()

public:
	/**
	 * @brief AGateOfBabyonProjectile 생성자입니다.
	 *
	 * 투사체의 기본 속성을 초기화합니다.
	 */
	AGateOfBabyonProjectile();

	/**
	 * @brief 매 프레임마다 호출되는 함수입니다.
	 *
	 * 투사체의 이동 로직을 업데이트합니다.
	 *
	 * @param DeltaTime 게임 월드의 델타 시간입니다.
	 */
	virtual void Tick(float DeltaTime) override;

	/**
	 * @brief 투사체를 활성화합니다.
	 *
	 * 투사체의 위치, 회전, 속도를 설정하고 콜리전을 활성화합니다.
	 *
	 * @param StartLocation 투사체의 시작 위치입니다.
	 * @param StartRotation 투사체의 시작 회전입니다.
	 * @param LaunchVelocity 투사체의 발사 속도입니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "GateOfBabylonProjectile")
	void ActivateProjectile(FVector StartLocation, FRotator StartRotation, FVector LaunchVelocity);

	/**
	 * @brief 투사체를 비활성화합니다.
	 *
	 * 투사체의 콜리전을 비활성화하고 숨깁니다. 오브젝트 풀로 반환될 준비를 합니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "GateOfBabylonProjectile")
	void DeactivateProjectile();

protected:
	/**
	 * @brief 액터가 스폰되었을 때 호출되는 함수입니다.
	 *
	 * 컴포넌트를 초기화하고 콜리전 이벤트를 바인딩합니다.
	 * 오브젝트 풀링 시스템을 위해 초기 활성화는 하지 않습니다.
	 */
	virtual void BeginPlay() override;

	/**
	 * @brief 콜리전 컴포넌트입니다.
	 *
	 * 투사체의 충돌을 감지합니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USphereComponent* CollisionComponent;

	/**
	 * @brief 투사체 이동 컴포넌트입니다.
	 *
	 * 투사체의 이동을 제어합니다.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	/**
	 * @brief 투사체 데미지 타입입니다.
	 *
	 * 투사체가 가하는 데미지 타입을 정의합니다.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageTypeClass;

	/**
	 * @brief 투사체가 가하는 데미지 양입니다.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage", meta = (AllowPrivateAccess = "true"))
	float DamageAmount;

private:

	/**
	 * @brief 오버랩 시작 시 호출되는 함수입니다.
	 *
	 * 투사체가 다른 액터와 충돌했을 때 데미지를 가하고 투사체를 비활성화합니다.
	 *
	 * @param OverlappedComponent 충돌된 컴포넌트입니다.
	 * @param OtherActor 충돌한 액터입니다.
	 * @param OtherComponent 충돌한 액터의 컴포넌트입니다.
	 * @param OtherBodyIndex 충돌한 바디의 인덱스입니다.
	 * @param bFromSweep 스윕에 의한 충돌인지 여부입니다.
	 * @param SweepResult 스윕 결과입니다.
	 */
	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**
	 * @brief 플레이어 주변의 랜덤한 위치로 이동합니다.
	 *
	 * 투사체가 플레이어 주변의 예측 불가능한 위치로 이동하도록 합니다.
	 * 주로 특수한 공격 패턴에 사용됩니다.
	 */
	UFUNCTION(BlueprintCallable, Category = "GateOfBabylonProjectile")
	void MoveToRandomLocationAroundPlayer();

	/**
	 * @brief 투사체가 활성화되었는지 여부입니다.
	 */
	bool bIsActive = false;

public:
	/**
	 * @brief 투사체가 활성화되었는지 여부를 반환합니다.
	 * @return 투사체의 활성화 여부
	 */
	UFUNCTION(BlueprintPure, Category = "GateOfBabylonProjectile")
	bool IsActive() const { return bIsActive; }
};
```

```cpp
// GateOfBabyonProjectile.cpp

#include "GateOfBabyonProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/DamageType.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetMathLibrary.h"

AGateOfBabyonProjectile::AGateOfBabyonProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	// 콜리전 컴포넌트 생성 및 설정
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;
	CollisionComponent->InitSphereRadius(10.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("Projectile")); // 적절한 콜리전 프로파일 설정

	// 투사체 이동 컴포넌트 생성 및 설정
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 3000.0f;
	ProjectileMovement->MaxSpeed = 3000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->ProjectileGravityScale = 0.0f; // 중력 영향 없음

	// 초기에는 비활성화
	bIsActive = false;
	SetActorHiddenInGame(true);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AGateOfBabyonProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGateOfBabyonProjectile::ActivateProjectile(FVector StartLocation, FRotator StartRotation, FVector LaunchVelocity)
{
	SetActorLocation(StartLocation);
	SetActorRotation(StartRotation);
	ProjectileMovement->Velocity = LaunchVelocity;

	SetActorHiddenInGame(false);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly); // 충돌 활성화
	bIsActive = true;
}

void AGateOfBabyonProjectile::DeactivateProjectile()
{
	SetActorHiddenInGame(true);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 충돌 비활성화
	bIsActive = false;
}

void AGateOfBabyonProjectile::BeginPlay()
{
	Super::BeginPlay();

	// 오버랩 이벤트 바인딩
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AGateOfBabyonProjectile::OnBeginOverlap);
}

void AGateOfBabyonProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this && bIsActive)
	{
		// 데미지 적용
		UGameplayStatics::ApplyPointDamage(OtherActor, DamageAmount, (SweepResult.Location - GetActorLocation()).GetSafeNormal(), SweepResult, GetInstigatorController(), this, DamageTypeClass);

		// 투사체 비활성화 (오브젝트 풀로 반환)
		DeactivateProjectile();
	}
}

void AGateOfBabyonProjectile::MoveToRandomLocationAroundPlayer()
{
	// 플레이어 찾기
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (!PlayerPawn) return;

	// 플레이어 위치
	FVector PlayerLocation = PlayerPawn->GetActorLocation();

	// 랜덤 위치 생성 (플레이어 주변 반경 500)
	FVector RandomOffset = FVector(FMath::FRandRange(-500.0f, 500.0f), FMath::FRandRange(-500.0f, 500.0f), 0.0f);
	FVector NewLocation = PlayerLocation + RandomOffset;

	// 새 위치로 이동
	SetActorLocation(NewLocation);

	// 방향 설정 (플레이어 방향)
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerLocation);
	SetActorRotation(NewRotation);
}

```

### 상세 API 문서: `AGateOfBabyonProjectile`

#### 1. 클래스 개요

`AGateOfBabyonProjectile` 클래스는 언리얼 엔진에서 바빌론의 문에서 발사되는 투사체를 구현하는 데 사용됩니다. 이 클래스는 투사체의 이동, 충돌 감지, 데미지 처리, 그리고 오브젝트 풀링 시스템과의 통합을 관리합니다. 이 클래스를 사용하면 다양한 종류의 투사체를 생성하고, 각각 다른 속성(속도, 데미지, 효과)을 부여할 수 있습니다.

**목적:**

*   바빌론의 문에서 발사되는 투사체 구현
*   투사체의 이동, 충돌, 데미지 처리 관리
*   오브젝트 풀링 시스템을 통한 성능 최적화

**사용 시나리오:**

*   보스 몬스터의 공격 패턴 구현
*   특정 스킬 또는 마법 효과 구현
*   다수의 투사체를 동시에 발사하는 공격 구현

**상속 관계:**

*   `AActor`

**주요 특징:**

*   오브젝트 풀링을 지원하여 투사체 생성 및 소멸 비용을 최소화합니다.
*   데미지 타입 및 데미지 양을 설정할 수 있습니다.
*   플레이어 주변의 랜덤 위치로 이동하는 기능 (특수 공격 패턴 구현)
*   활성화/비활성화 기능을 통해 투사체를 효율적으로 관리합니다.

**주의사항:**

*   `DamageTypeClass`는 반드시 유효한 `UDamageType` 클래스를 지정해야 합니다.
*   오브젝트 풀링 시스템을 사용하려면 `ActivateProjectile` 및 `DeactivateProjectile` 함수를 적절히 호출해야 합니다.
*   콜리전 프로파일은 프로젝트 설정에 맞게 조정해야 합니다.

**성능 고려사항:**

*   투사체 개수를 적절히 조절하여 성능을 최적화합니다.
*   투사체의 이동 로직을 간결하게 유지하여 CPU 사용량을 줄입니다.
*   오브젝트 풀 크기를 적절히 설정하여 메모리 사용량을 관리합니다.

#### 2. 함수 상세 설명

##### 2.1 `AGateOfBabyonProjectile()`

```cpp
AGateOfBabyonProjectile();
```

**설명:**

*   `AGateOfBabyonProjectile` 클래스의 생성자입니다.
*   투사체의 기본 속성(콜리전, 이동 컴포넌트, 초기 상태)을 초기화합니다.

**매개변수:**

*   없음

**반환값:**

*   없음

**예외 처리:**

*   없음

**사용 예제:**

```cpp
AGateOfBabyonProjectile::AGateOfBabyonProjectile()
{
    PrimaryActorTick.bCanEverTick = true;

    // 콜리전 컴포넌트 생성 및 설정
    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
    RootComponent = CollisionComponent;
    CollisionComponent->InitSphereRadius(10.0f);
    CollisionComponent->SetCollisionProfileName(TEXT("Projectile")); // 적절한 콜리전 프로파일 설정

    // 투사체 이동 컴포넌트 생성 및 설정
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->UpdatedComponent = CollisionComponent;
    ProjectileMovement->InitialSpeed = 3000.0f;
    ProjectileMovement->MaxSpeed = 3000.0f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.0f; // 중력 영향 없음

    // 초기에는 비활성화
    bIsActive = false;
    SetActorHiddenInGame(true);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
```

**성능 특성:**

*   생성자이므로 게임 시작 시 또는 오브젝트 풀 확장 시 호출될 수 있습니다.
*   컴포넌트 생성 비용이 발생할 수 있으므로, 오브젝트 풀을 사용하여 호출 빈도를 줄이는 것이 좋습니다.

##### 2.2 `virtual void Tick(float DeltaTime) override`

```cpp
virtual void Tick(float DeltaTime) override;
```

**설명:**

*   매 프레임마다 호출되는 함수입니다.
*   투사체의 이동 로직을 업데이트합니다. 현재는 비어 있지만, 필요한 경우 투사체의 추가적인 움직임 또는 효과를 구현할 수 있습니다.

**매개변수:**

*   `DeltaTime`: 게임 월드의 델타 시간 (초 단위)

**반환값:**

*   없음

**예외 처리:**

*   없음

**사용 예제:**

```cpp
void AGateOfBabyonProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // 추가적인 이동 로직 또는 효과 구현 가능
    // 예: 특정 방향으로 추가 속도 적용
    // ProjectileMovement->AddForce(FVector(0.0f, 0.0f, 100.0f));
}
```

**성능 특성:**

*   매 프레임마다 호출되므로, 함수 내부 로직을 최적화하여 CPU 사용량을 최소화해야 합니다.

##### 2.3 `void ActivateProjectile(FVector StartLocation, FRotator StartRotation, FVector LaunchVelocity)`

```cpp
UFUNCTION(BlueprintCallable, Category = "GateOfBabylonProjectile")
void ActivateProjectile(FVector StartLocation, FRotator StartRotation, FVector LaunchVelocity);
```

**설명:**

*   투사체를 활성화합니다.
*   투사체의 위치, 회전, 속도를 설정하고 콜리전을 활성화합니다.
*   오브젝트 풀에서 가져온 투사체를 재사용할 때 호출됩니다.

**매개변수:**

*   `StartLocation`: 투사체의 시작 위치 (FVector)
*   `StartRotation`: 투사체의 시작 회전 (FRotator)
*   `LaunchVelocity`: 투사체의 발사 속도 (FVector)

**반환값:**

*   없음

**예외 처리:**

*   없음

**사용 예제:**

```cpp
// 바빌론의 문 액터에서 투사체를 발사하는 예제
void AGateOfBabylon::SpawnProjectile()
{
    AGateOfBabyonProjectile* Projectile = GetProjectileFromPool();
    if (Projectile)
    {
        FVector StartLocation = GetActorLocation(); // 발사 위치
        FRotator StartRotation = GetActorRotation(); // 발사 방향
        FVector LaunchVelocity = StartRotation.Vector() * ProjectileLaunchSpeed;

        Projectile->ActivateProjectile(StartLocation, StartRotation, LaunchVelocity);
    }
}
```

**성능 특성:**

*   투사체를 활성화하는 데 필요한 최소한의 연산만 수행하도록 최적화합니다.
*   컴포넌트 활성화/비활성화 비용을 고려하여, 필요한 경우에만 콜리전을 활성화합니다.

##### 2.4 `void DeactivateProjectile()`

```cpp
UFUNCTION(BlueprintCallable, Category = "GateOfBabylonProjectile")
void DeactivateProjectile();
```

**설명:**

*   투사체를 비활성화합니다.
*   투사체의 콜리전을 비활성화하고 숨깁니다. 오브젝트 풀로 반환될 준비를 합니다.
*   투사체가 목표에 도달하거나 특정 조건이 만족되었을 때 호출됩니다.

**매개변수:**

*   없음

**반환값:**

*   없음

**예외 처리:**

*   없음

**사용 예제:**

```cpp
// 투사체가 충돌했을 때 비활성화하는 예제
void AGateOfBabyonProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && bIsActive)
    {
        // 데미지 적용
        UGameplayStatics::ApplyPointDamage(OtherActor, DamageAmount, (SweepResult.Location - GetActorLocation()).GetSafeNormal(), SweepResult, GetInstigatorController(), this, DamageTypeClass);

        // 투사체 비활성화 (오브젝트 풀로 반환)
        DeactivateProjectile();
    }
}
```

**성능 특성:**

*   투사체를 비활성화하는 데 필요한 최소한의 연산만 수행하도록 최적화합니다.
*   컴포넌트 활성화/비활성화 비용을 고려하여, 필요한 경우에만 콜리전을 비활성화합니다.

##### 2.5 `virtual void BeginPlay() override`

```cpp
virtual void BeginPlay() override;
```

**설명:**

*   액터가 스폰되었을 때 호출되는 함수입니다.
*   컴포넌트를 초기화하고 콜리전 이벤트를 바인딩합니다.
*   오브젝트 풀링 시스템을 위해 초기 활성화는 하지 않습니다.

**매개변수:**

*   없음

**반환값:**

*   없음

**예외 처리:**

*   없음

**사용 예제:**

```cpp
void AGateOfBabyonProjectile::BeginPlay()
{
    Super::BeginPlay();

    // 오버랩 이벤트 바인딩
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AGateOfBabyonProjectile::OnBeginOverlap);
}
```

**성능 특성:**

*   게임 시작 시 또는 액터 스폰 시 호출됩니다.
*   이벤트 바인딩 비용을 고려하여, 필요한 이벤트만 바인딩합니다.

##### 2.6 `void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)`

```cpp
UFUNCTION()
void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
```

**설명:**

*   오버랩 시작 시 호출되는 함수입니다.
*   투사체가 다른 액터와 충돌했을 때 데미지를 가하고 투사체를 비활성화합니다.

**매개변수:**

*   `OverlappedComponent`: 충돌된 컴포넌트 (UPrimitiveComponent\*)
*   `OtherActor`: 충돌한 액터 (AActor\*)
*   `OtherComp`: 충돌한 액터의 컴포넌트 (UPrimitiveComponent\*)
*   `OtherBodyIndex`: 충돌한 바디의 인덱스 (int32)
*   `bFromSweep`: 스윕에 의한 충돌인지 여부 (bool)
*   `SweepResult`: 스윕 결과 (const FHitResult&)

**반환값:**

*   없음

**예외 처리:**

*   `OtherActor`가 유효하지 않거나 자기 자신인 경우, 함수를 종료합니다.

**사용 예제:**

```cpp
void AGateOfBabyonProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && bIsActive)
    {
        // 데미지 적용
        UGameplayStatics::ApplyPointDamage(OtherActor, DamageAmount, (SweepResult.Location - GetActorLocation()).GetSafeNormal(), SweepResult, GetInstigatorController(), this, DamageTypeClass);

        // 투사체 비활성화 (오브젝트 풀로 반환)
        DeactivateProjectile();
    }
}
```

**성능 특성:**

*   충돌 이벤트가 발생할 때마다 호출되므로, 함수 내부 로직을 최적화하여 CPU 사용량을 최소화해야 합니다.
*   `UGameplayStatics::ApplyPointDamage` 함수는 데미지 처리에 필요한 연산을 수행하므로, 데미지 양과 데미지 타입을 적절히 설정하여 성능을 최적화합니다.

##### 2.7 `void MoveToRandomLocationAroundPlayer()`

```cpp
UFUNCTION(BlueprintCallable, Category = "GateOfBabylonProjectile")
void MoveToRandomLocationAroundPlayer();
```

**설명:**

*   플레이어 주변의 랜덤한 위치로 이동합니다.
*   투사체가 플레이어 주변의 예측 불가능한 위치로 이동하도록 합니다.
*   주로 특수한 공격 패턴에 사용됩니다.

**매개변수:**

*   없음

**반환값:**

*   없음

**예외 처리:**

*   플레이어를 찾을 수 없는 경우, 함수를 종료합니다.

**사용 예제:**

```cpp
void AGateOfBabyonProjectile::MoveToRandomLocationAroundPlayer()
{
    // 플레이어 찾기
    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    if (!PlayerPawn) return;

    // 플레이어 위치
    FVector PlayerLocation = PlayerPawn->GetActorLocation();

    // 랜덤 위치 생성 (플레이어 주변 반경 500)
    FVector RandomOffset = FVector(FMath::FRandRange(-500.0f, 500.0f), FMath::FRandRange(-500.0f, 500.0f), 0.0f);
    FVector NewLocation = PlayerLocation + RandomOffset;

    // 새 위치로 이동
    SetActorLocation(NewLocation);

    // 방향 설정 (플레이어 방향)
    FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerLocation);
    SetActorRotation(NewRotation);
}
```

**성능 특성:**

*   랜덤 위치 생성 및 이동 연산을 수행하므로, 호출 빈도를 적절히 조절하여 CPU 사용량을 관리합니다.
*   플레이어 찾기 연산을 최적화하여 성능을 향상시킬 수 있습니다.

##### 2.8 `bool IsActive() const`

```cpp
UFUNCTION(BlueprintPure, Category = "GateOfBabylonProjectile")
bool IsActive() const { return bIsActive; }
```

**설명:**

*   투사체가 활성화되었는지 여부를 반환합니다.

**매개변수:**

*   없음

**반환값:**

*   `bIsActive`: 투사체의 활성화 여부 (bool)

**예외 처리:**

*   없음

**사용 예제:**

```cpp
// 투사체가 활성화되었는지 확인하는 예제
if (Projectile->IsActive())
{
    // 투사체가 활성화된 경우 수행할 작업
    UE_LOG(LogTemp, Warning, TEXT("Projectile is active!"));
}
else
{
    // 투사체가 비활성화된 경우 수행할 작업
    UE_LOG(LogTemp, Warning, TEXT("Projectile is inactive!"));
}
```

**성능 특성:**

*   단순한 변수 반환이므로 성능에 미치는 영향은 미미합니다.

#### 3. 사용 예제 (완전한 코드)

```cpp
// GateOfBabylon.h (바빌론의 문 액터)
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GateOfBabyonProjectile.h"
#include "GateOfBabylon.generated.h"

UCLASS()
class DDTPROJECT_API AGateOfBabylon : public AActor
{
	GENERATED_BODY()

public:
	AGateOfBabylon();

protected:
	virtual void BeginPlay() override;

private:
	// 투사체 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AGateOfBabyonProjectile> ProjectileClass;

	// 오브젝트 풀 크기
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	int32 InitialPoolSize = 10;

	// 투사체 발사 속도
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Projectile", meta = (AllowPrivateAccess = "true"))
	float ProjectileLaunchSpeed = 2000.0f;

	// 오브젝트 풀
	TArray<AGateOfBabyonProjectile*> ProjectilePool;

	// 플레이어 찾기 간격
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI", meta = (AllowPrivateAccess = "true"))
	float PlayerSearchInterval = 1.0f;

	// 플레이어 찾기 타이머 핸들
	FTimerHandle PlayerSearchTimerHandle;

	// 플레이어
	AActor* PlayerActor = nullptr;

public:
	// 투사체 발사 함수
	UFUNCTION(BlueprintCallable, Category = "GateOfBabylon")
	void SpawnProjectile();

	// 오브젝트 풀에서 투사체 가져오기
	AGateOfBabyonProjectile* GetProjectileFromPool();

	// 오브젝트 풀 초기화
	void InitializeProjectilePool();

	// 플레이어 찾기
	void UpdateLookAtPlayer();

};

// GateOfBabylon.cpp (바빌론의 문 액터)
#include "GateOfBabylon.h"
#include "Kismet/GameplayStatics.h"

AGateOfBabylon::AGateOfBabylon()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AGateOfBabylon::BeginPlay()
{
	Super::BeginPlay();

	// 오브젝트 풀 초기화
	InitializeProjectilePool();

	// 플레이어 찾기 시작
	GetWorldTimerManager().SetTimer(PlayerSearchTimerHandle, this, &AGateOfBabylon::UpdateLookAtPlayer, PlayerSearchInterval, true);
}

void AGateOfBabylon::SpawnProjectile()
{
	AGateOfBabyonProjectile* Projectile = GetProjectileFromPool();
	if (Projectile)
	{
		FVector StartLocation = GetActorLocation(); // 발사 위치
		FRotator StartRotation = GetActorRotation(); // 발사 방향
		FVector LaunchVelocity = StartRotation.Vector() * ProjectileLaunchSpeed;

		Projectile->ActivateProjectile(StartLocation, StartRotation, LaunchVelocity);
	}
}

AGateOfBabyonProjectile* AGateOfBabylon::GetProjectileFromPool()
{
	for (AGateOfBabyonProjectile* Projectile : ProjectilePool)
	{
		if (!Projectile->IsActive())
		{
			return Projectile;
		}
	}

	// 풀에 사용 가능한 투사체가 없으면 nullptr 반환
	return nullptr;
}

void AGateOfBabylon::InitializeProjectilePool()
{
	if (ProjectileClass)
	{
		for (int32 i = 0; i < InitialPoolSize; ++i)
		{
			AGateOfBabyonProjectile* Projectile = GetWorld()->SpawnActor<AGateOfBabyonProjectile>(ProjectileClass, FVector::ZeroVector, FRotator::ZeroRotator);
			if (Projectile)
			{
				Projectile->DeactivateProjectile(); // 초기에는 비활성화
				ProjectilePool.Add(Projectile);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ProjectileClass가 설정되지 않았습니다!"));
	}
}

void AGateOfBabylon::UpdateLookAtPlayer()
{
	if (!PlayerActor)
	{
		// 플레이어 찾기
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			PlayerActor = PlayerPawn;
		}
		else
		{
			return; // 플레이어를 찾을 수 없으면 종료
		}
	}

	// 플레이어 방향으로 회전
	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerActor->GetActorLocation());
	SetActorRotation(NewRotation);
}

```

**설명:**

*   `GateOfBabylon.h` 및 `GateOfBabylon.cpp` 파일은 바빌론의 문 액터를 정의합니다.
*   `InitializeProjectilePool` 함수는 오브젝트 풀을 초기화합니다.
*   `GetProjectileFromPool` 함수는 오브젝트 풀에서 사용 가능한 투사체를 가져옵니다.
*   `SpawnProjectile` 함수는 투사체를 발사합니다.
*   `UpdateLookAtPlayer` 함수는 플레이어를 향해 회전합니다.

**사용 방법:**

1.  `ProjectileClass` 변수에 사용할 `AGateOfBabyonProjectile` 클래스를 지정합니다.
2.  `InitialPoolSize` 변수에 오브젝트 풀 크기를 설정합니다.
3.  `ProjectileLaunchSpeed` 변수에 투사체 발사 속도를 설정합니다.
4.  `SpawnProjectile` 함수를 호출하여 투사체를 발사합니다.

#### 4. 성능 고려사항

*   **메모리 사용량:** 오브젝트 풀 크기를 적절히 설정하여 메모리 사용량을 관리합니다.
*   **실행 시간:** 투사체의 이동 로직을 간결하게 유지하여 CPU 사용량을 줄입니다.
*   **최적화 기법:** 오브젝트 풀링을 통해 투사체 생성 및 소멸 비용을 최소화합니다.

#### 5. 호환성 정보

*   **Unreal Engine 버전:** 4.26 이상
*   **플랫폼:** Windows, Linux, Mac, iOS, Android
*   **컴파일러:** Visual Studio 2019 이상 (Windows), Clang (Linux, Mac, iOS, Android)
*   **의존성:** Unreal Engine 기본 클래스만 사용

#### 6. 트러블슈팅

*   **투사체가 생성되지 않는 경우:**
    *   `ProjectileClass` 변수가 올바르게 설정되었는지 확인합니다.
    *   `InitialPoolSize`가 0보다 큰 값으로 설정되었는지 확인합니다.
    *   투사체 클래스 (`AGateOfBabyonProjectile`)에 문제가 없는지 확인합니다.
    *   로그 메시지를 확인하여 오류가 발생했는지 확인합니다.
*   **투사체가 플레이어를 향해 발사되지 않는 경우:**
    *   `UpdateLookAtPlayer()` 함수가 제대로 호출되는지 확인합니다.
    *   플레이어 Pawn을 제대로 찾는지 확인합니다.
    *   `FindLookAtRotation()` 함수의 입력 값이 올바른지 확인합니다.
*   **성능 문제가 발생하는 경우:**
    *   투사체 개수를 줄입니다.
    *   투사체 이동 로직을 최적화합니다.
    *   오브젝트 풀 크기를 최적화합니다.
    *   Unreal Insights 또는 Stat 명령을 사용하여 성능 병목 지점을 식별하고 해결합니다.
*   **메모리 누수가 발생하는 경우:**
    *   오브젝트 풀에서 회수되지 않은 투사체가 있는지 확인합니다.
    *   `ActivateGate()`와 `DeactivateGate()` 함수가 제대로 호출되어 투사체 발사를 제어하는지 확인합니다.
    *   Unreal Engine 메모리 디버거를 사용하여 메모리 누수 지점을 식별하고 해결합니다.

#### 7. 다음 단계

다음으로는 보스 AI와 연동하여 `AGateOfBabylon` 공격을 발동하는 방법에 대한 예제를 제공할 예정입니다. 또한, 다양한 투사체 패턴(예: 유도탄, 곡사)을 구현하는 방법과, 투사체에 특수 효과(예: 폭발, 디버프)를 추가하는 방법에 대한 가이드를 제공할 예정입니다.


