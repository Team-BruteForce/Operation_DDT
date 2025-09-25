#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CItemPoolManager.generated.h"

USTRUCT(BlueprintType)
struct FItemPrewarmEntry
{
    GENERATED_BODY()

    // 미리 스폰해 둘 아이템 클래스
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ItemClass;

    // 해당 클래스를 몇 개 생성할지
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Count = 0;
};

USTRUCT()
struct FItemPoolBucket
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<AActor*> InactiveItems;

    UPROPERTY()
    TArray<AActor*> ActiveItems;
};

/**
 * 드랍 아이템 오브젝트 풀 매니저
 * - 클래스별 풀을 보관하고 총 개수를 최대치로 제한
 */
UCLASS()
class OPERRATION_DDT_API AItemPoolManager : public AActor
{
    GENERATED_BODY()

public:
    AItemPoolManager();

protected:
    virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category="ItemPool")
    AActor* AcquireItem(TSubclassOf<AActor> ItemClass, const FVector& SpawnLocation, const FRotator& SpawnRotation);

    UFUNCTION(BlueprintCallable, Category="ItemPool")
    void ReleaseItem(AActor* ItemActor);

    UFUNCTION(BlueprintCallable, BlueprintPure, Category="ItemPool")
    int32 GetTotalPooledCount() const { return TotalCount; }

    /**
     * @brief 모든 활성화된 아이템들을 풀로 반환하는 초기화 함수
     * 플레이어 리스폰 시 호출하여 레벨에 남아있는 아이템들을 정리
     */
    UFUNCTION(BlueprintCallable, Category = "ItemPool")
    void ResetAllActiveItems();
    
    /**
     * @brief 특정 클래스의 모든 활성 아이템들을 풀로 반환
     * @param ItemClass 초기화할 아이템 클래스 (nullptr이면 모든 클래스)
     */
    UFUNCTION(BlueprintCallable, Category = "ItemPool")
    void ResetActiveItemsOfClass(TSubclassOf<AActor> ItemClass = nullptr);

public:
    // 풀에서 보유 가능한 총 아이템 수(활성+비활성 합산)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemPool")
    int32 MaxTotalCount = 15;

    // 시작 시 미리 스폰할 아이템 목록(클래스별 개수)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="ItemPool")
    TArray<FItemPrewarmEntry> PrewarmEntries;

private:
    UPROPERTY()
    TMap<TSubclassOf<AActor>, FItemPoolBucket> ClassToBucket;

    UPROPERTY()
    TMap<AActor*, TSubclassOf<AActor>> ItemToClass;

    int32 TotalCount = 0;

private:
    void SetItemActive(AActor* ItemActor, bool bActive);
};


