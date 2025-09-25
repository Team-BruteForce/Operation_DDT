#include "ODH/Component/CItemPoolManager.h"
#include "Engine/World.h"
#include "ODH/Item/IPooledItem.h"

AItemPoolManager::AItemPoolManager()
{
    PrimaryActorTick.bCanEverTick = false;
    SetReplicates(true);
}

void AItemPoolManager::BeginPlay()
{
    Super::BeginPlay();

    // 프리워밍: 클래스별 Count 만큼 미리 스폰(숨김/충돌OFF)하여 Inactive로 적재
    for (const FItemPrewarmEntry& Entry : PrewarmEntries)
    {
        if (!GetWorld() || !*Entry.ItemClass) continue;

        FItemPoolBucket& Bucket = ClassToBucket.FindOrAdd(Entry.ItemClass);

        const int32 NumToSpawn = FMath::Max(0, Entry.Count);
        for (int32 i = 0; i < NumToSpawn && TotalCount < MaxTotalCount; ++i)
        {
            FActorSpawnParameters Params;
            Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
            Params.Owner = this;

            // 초기 위치는 매니저 위치(사용 시 위치 재설정됨)
            AActor* NewItem = GetWorld()->SpawnActor<AActor>(Entry.ItemClass, GetActorLocation(), GetActorRotation(), Params);
            if (!NewItem) continue;

            TotalCount++;
            ItemToClass.Add(NewItem, Entry.ItemClass);
            Bucket.InactiveItems.Add(NewItem);

            // 비활성화 상태로 대기
            SetItemActive(NewItem, false);
        }
    }
}

static void ToggleActorBasics(AActor* Actor, bool bActive)
{
    if (!Actor) return;
    Actor->SetActorHiddenInGame(!bActive);
    Actor->SetActorEnableCollision(bActive);
    Actor->SetActorTickEnabled(bActive);
}

void AItemPoolManager::SetItemActive(AActor* ItemActor, bool bActive)
{
    ToggleActorBasics(ItemActor, bActive);
}

AActor* AItemPoolManager::AcquireItem(TSubclassOf<AActor> ItemClass, const FVector& SpawnLocation, const FRotator& SpawnRotation)
{
    if (!GetWorld() || !*ItemClass) return nullptr;

    FItemPoolBucket& Bucket = ClassToBucket.FindOrAdd(ItemClass);

    // 비활성 아이템이 있으면 재사용
    if (Bucket.InactiveItems.Num() > 0)
    {
        AActor* Item = Bucket.InactiveItems.Pop(false);
        Bucket.ActiveItems.Add(Item);
        Item->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
        SetItemActive(Item, true);
        if (IPooledItem* Hook = Cast<IPooledItem>(Item))
        {
            Hook->OnPooledActivated(SpawnLocation, SpawnRotation);
        }
        return Item;
    }

    // 총 개수가 한도 미만이면 새로 스폰
    if (TotalCount < MaxTotalCount)
    {
        FActorSpawnParameters Params;
        Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        Params.Owner = this;
        AActor* NewItem = GetWorld()->SpawnActor<AActor>(ItemClass, SpawnLocation, SpawnRotation, Params);
        if (NewItem)
        {
            TotalCount++;
            Bucket.ActiveItems.Add(NewItem);
            ItemToClass.Add(NewItem, ItemClass);
            SetItemActive(NewItem, true);
            if (IPooledItem* Hook = Cast<IPooledItem>(NewItem))
            {
                Hook->OnPooledActivated(SpawnLocation, SpawnRotation);
            }
            return NewItem;
        }
    }

    // 풀 가득 → 획득 실패(드랍 스킵)
    return nullptr;
}

void AItemPoolManager::ReleaseItem(AActor* ItemActor)
{
    if (!ItemActor) return;
    TSubclassOf<AActor>* FoundClass = ItemToClass.Find(ItemActor);
    if (!FoundClass || !*FoundClass)
    {
        // 미등록: 안전하게 숨김만 처리
        SetItemActive(ItemActor, false);
        return;
    }

    FItemPoolBucket* Bucket = ClassToBucket.Find(*FoundClass);
    if (!Bucket) { SetItemActive(ItemActor, false); return; }

    // Active에서 제거 후 Inactive로 이동
    Bucket->ActiveItems.Remove(ItemActor);
    Bucket->InactiveItems.Add(ItemActor);
    SetItemActive(ItemActor, false);
    if (IPooledItem* Hook = Cast<IPooledItem>(ItemActor))
    {
        Hook->OnPooledDeactivated();
    }
}

void AItemPoolManager::ResetAllActiveItems()
{
    int32 ResetCount = 0;
    
    // 모든 클래스 버킷을 순회
    for (auto& Pair : ClassToBucket)
    {
        TSubclassOf<AActor> ItemClass = Pair.Key;
        FItemPoolBucket& Bucket = Pair.Value;
        
        // 해당 클래스의 활성 아이템들을 모두 풀로 반환
        TArray<AActor*> ActiveItemsCopy = Bucket.ActiveItems;
        for (AActor* Item : ActiveItemsCopy)
        {
            if (IsValid(Item))
            {
                // ReleaseItem 함수를 사용하여 안전하게 반환
                ReleaseItem(Item);
                ResetCount++;
            }
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("ItemPoolManager: Reset %d active items to pool"), ResetCount);
}

void AItemPoolManager::ResetActiveItemsOfClass(TSubclassOf<AActor> ItemClass)
{
    if (!ItemClass)
    {
        // ItemClass가 nullptr이면 모든 아이템 초기화
        ResetAllActiveItems();
        return;
    }
    
    FItemPoolBucket* Bucket = ClassToBucket.Find(ItemClass);
    if (!Bucket)
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemPoolManager: No bucket found for class %s"), 
               *ItemClass->GetName());
        return;
    }
    
    int32 ResetCount = 0;
    TArray<AActor*> ActiveItemsCopy = Bucket->ActiveItems;
    
    for (AActor* Item : ActiveItemsCopy)
    {
        if (IsValid(Item))
        {
            ReleaseItem(Item);
            ResetCount++;
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("ItemPoolManager: Reset %d %s items to pool"), 
           ResetCount, *ItemClass->GetName());
}


