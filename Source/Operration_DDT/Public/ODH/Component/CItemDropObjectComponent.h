#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Engine.h"
#include "ODH/Component/CItemDropComponent.h"
#include "CItemDropObjectComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemObjectDestroyed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnItemObjectRespawned);

/**
 * 아이템 드랍 오브젝트를 관리하는 컴포넌트
 * - 체력 시스템으로 오브젝트 파괴 관리
 * - 체력이 0이 되면 Mesh 숨김 및 아이템 드랍
 * - 일정 시간 후 자동으로 리스폰
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OPERRATION_DDT_API UCItemDropObjectComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCItemDropObjectComponent();

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
    // 데미지를 받는 함수
    UFUNCTION(BlueprintCallable, Category = "Item Drop Object")
    void TakeDamage(float DamageAmount);

    // 오브젝트 리스폰 함수
    UFUNCTION(BlueprintCallable, Category = "Item Drop Object")
    void RespawnObject();

    // 현재 체력 확인
    UFUNCTION(BlueprintCallable, Category = "Item Drop Object")
    float GetCurrentHealth() const { return CurrentHealth; }

    // 최대 체력 확인
    UFUNCTION(BlueprintCallable, Category = "Item Drop Object")
    float GetMaxHealth() const { return MaxHealth; }

    // 오브젝트가 파괴된 상태인지 확인
    UFUNCTION(BlueprintCallable, Category = "Item Drop Object")
    bool IsDestroyed() const { return bIsDestroyed; }

    // 테스트용: 즉시 데미지를 받는 함수
    UFUNCTION(BlueprintCallable, Category = "Item Drop Object")
    void TestTakeDamage();

    // 이벤트 델리게이트
    UPROPERTY(BlueprintAssignable, Category = "Item Drop Object")
    FOnItemObjectDestroyed OnObjectDestroyed;

    UPROPERTY(BlueprintAssignable, Category = "Item Drop Object")
    FOnItemObjectRespawned OnObjectRespawned;

protected:
    // 오브젝트 파괴 처리
    UFUNCTION()
    void HandleObjectDestroyed();

    // 리스폰 타이머 업데이트
    void UpdateRespawnTimer(float DeltaTime);

    // Mesh 가시성 설정
    void SetMeshVisibility(bool bVisible);

    // 아이템 드랍 실행
    void ExecuteItemDrop();

    // 지연 파괴 함수
    UFUNCTION()
    void DelayedDestroy();

    // 충돌 이벤트 핸들러
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:
    // 체력 관련
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health", meta = (AllowPrivateAccess = "true"))
    float MaxHealth = 100.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", meta = (AllowPrivateAccess = "true"))
    float CurrentHealth;

    // 리스폰 관련
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Respawn", meta = (AllowPrivateAccess = "true"))
    float RespawnTime = 30.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Respawn", meta = (AllowPrivateAccess = "true"))
    float CurrentRespawnTimer;

    // 상태 플래그
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State", meta = (AllowPrivateAccess = "true"))
    bool bIsDestroyed = false;

    // 컴포넌트 참조
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UCItemDropComponent* ItemDropComponent;

    // Mesh 컴포넌트 참조
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* MeshComponent;

    // 충돌 감지 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    bool bEnableCollisionDamage = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    float CollisionDamageAmount = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    float CollisionCooldownTime = 1.0f;

    // 마지막 충돌 시간 (쿨다운 관리용)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision", meta = (AllowPrivateAccess = "true"))
    float LastCollisionTime = 0.0f;

    // 파괴 지연 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Destroy", meta = (AllowPrivateAccess = "true"))
    float DestroyDelay = 0.05f;

    // 파괴 타이머 핸들
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Destroy", meta = (AllowPrivateAccess = "true"))
    FTimerHandle DestroyTimerHandle;
};
