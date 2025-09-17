#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ODH/Item/IPooledItem.h"
#include "DropHPPotionItem.generated.h"

class USphereComponent;
class UNiagaraComponent;

UCLASS()
class OPERRATION_DDT_API ADropHPPotionItem : public AActor, public IPooledItem
{
    GENERATED_BODY()

public:
    ADropHPPotionItem();

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
                        bool bFromSweep, const FHitResult& SweepResult);

    void ReturnToPool();

private:
    UPROPERTY(VisibleAnywhere, Category="Components")
    USphereComponent* Collision;

    // 낙하 연출 상태
    bool bFalling = false;
    FVector StartLocation;
    float Elapsed = 0.0f;
    float RiseHeight = 50.0f;
    float RiseTime = 0.15f;
    float Gravity = 980.0f;
    float VerticalVelocity = 0.0f;

    // 아이템 상태
    bool bCanBePickedUp = false;

public:
    // 자동 반환 타이머
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop|Timer")
    float AutoReturnTime = 10.0f; // 10초 후 자동 반환

private:
    UPROPERTY()
    FTimerHandle AutoReturnTimerHandle;

public:
    // 나이아가라 이펙트
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop|Effect")
    class UNiagaraSystem* PickupEffect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop|Effect")
    float EffectSpawnHeight = 10.0f; // 지면에서 얼마나 위에 이펙트를 스폰할지
    
    // 이펙트 위치 보정 오프셋 (XYZ)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop|Effect")
    FVector EffectOffset = FVector(0.f, 0.f, 0.f);
    
    // 현재 활성화된 이펙트 컴포넌트 (풀링용)
    UPROPERTY()
    UNiagaraComponent* ActiveEffectComponent;

public:
    // 지면 스냅(지연 적용) 설정
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop|GroundSnap")
    bool bUseGroundSnap = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop|GroundSnap")
    float GroundTraceDistance = 800.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop|GroundSnap")
    float GroundOffset = 25.0f;

public:
    virtual void Tick(float DeltaSeconds) override;
    virtual void OnPooledActivated(const FVector& DesiredLocation, const FRotator& DesiredRotation) override;
    virtual void OnPooledDeactivated() override;

private:
    void EnablePickup();

private:
    // (지연 스냅 제거)
};


