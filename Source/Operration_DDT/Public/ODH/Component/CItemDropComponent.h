#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CItemDropComponent.generated.h"

/**
 * 에너미 사망 시 아이템 드랍을 담당하는 컴포넌트
 * - 사망 시 70% 확률로 드랍
 * - 드랍 시 70% 확률로 총알, 30% 확률로 체력 포션
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OPERRATION_DDT_API UCItemDropComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UCItemDropComponent();

protected:
    virtual void BeginPlay() override;

private:
    UFUNCTION()
    void HandleOwnerDeath();

    void TryDropItem();

    void SpawnItem(TSubclassOf<AActor> ItemClass);

public:
    // 사망 시 드랍할 확률 (0~1)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop")
    float DropChance = 0.7f;

    // 드랍이 발생했을 때 총알이 나올 확률 (0~1)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop")
    float BulletWeight = 0.7f; // 포션은 0.3

    // 드랍할 아이템 클래스들 (액터)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop")
    TSubclassOf<AActor> BulletItemActors;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Drop")
    TSubclassOf<AActor> HealthPotionItemActors;
};


