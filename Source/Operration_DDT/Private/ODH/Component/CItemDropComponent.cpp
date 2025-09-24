#include "ODH/Component/CItemDropComponent.h"
#include "ODH/ODH_Enemy/Component/CEnemyStatusComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "ODH/Component/CItemPoolManager.h"
#include "GameFramework/Actor.h"

UCItemDropComponent::UCItemDropComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UCItemDropComponent::BeginPlay()
{
    Super::BeginPlay();

    // 소유자에서 상태 컴포넌트 찾아 사망 이벤트 바인딩
    if (AActor* Owner = GetOwner())
    {
        if (UCEnemyStatusComponent* Status = Owner->FindComponentByClass<UCEnemyStatusComponent>())
        {
            Status->OnDeath.AddDynamic(this, &UCItemDropComponent::HandleOwnerDeath);
        }
    }
}

void UCItemDropComponent::HandleOwnerDeath()
{
    TryDropItem();
}

void UCItemDropComponent::TryDropItem()
{
    if (!GetWorld()) return;

    const float Roll = FMath::FRand();
    if (Roll > DropChance)
    {
        return; // 드랍 안 함
    }

    // IsItemDropper가 true이면 두 아이템을 모두 드랍
    if (IsItemDropper)
    {
        AActor* Owner = GetOwner();
        if (!Owner) return;
        
        const FVector BaseLocation = Owner->GetActorLocation();
        // 원형 반경 내에서 두 점을 각도 분리값만큼 벌려서 배치(기본 20도)
        const float Angle = FMath::FRandRange(0.0f, 2.0f * PI);
        const float Delta = FMath::DegreesToRadians(PairAngleSeparationDegrees);
        const FVector RandOffsetR = GenerateRandomXYOffsetCircular(DropRadiusMinXY, DropRadiusMaxXY, Angle);
        const FVector RandOffsetL = GenerateRandomXYOffsetCircular(DropRadiusMinXY, DropRadiusMaxXY, Angle + Delta);
        
        // 총알 드랍 (왼쪽으로 약간 이동)
        if (*BulletItemActors)
        {
            SpawnItemAtLocation(BulletItemActors, 
                BaseLocation + FVector(RandOffsetL.X, RandOffsetL.Y, 0.0f), 
                FRotator::ZeroRotator);
        }
        
        // 체력 포션 드랍 (오른쪽으로 약간 이동)
        if (*HealthPotionItemActors)
        {
            SpawnItemAtLocation(HealthPotionItemActors, 
                BaseLocation + FVector(RandOffsetR.X, RandOffsetR.Y, 0.0f), 
                FRotator::ZeroRotator);
        }
        
        return;
    }

    // 기존 로직: 어떤 아이템을 드랍할지 결정
    const float TypeRoll = FMath::FRand();
    const bool bDropBullet = (TypeRoll <= BulletWeight);

    TSubclassOf<AActor> DropClass = bDropBullet ? BulletItemActors : HealthPotionItemActors;
    if (!*DropClass)
    {
        // 폴백: 다른 것이 설정되어 있으면 그걸 시도
        DropClass = bDropBullet ? HealthPotionItemActors : BulletItemActors;
        if (!*DropClass) return;
    }

    SpawnItem(DropClass);
}

void UCItemDropComponent::SpawnItem(TSubclassOf<AActor> ItemClass)
{
    if (!GetWorld() || !*ItemClass) return;
    AActor* Owner = GetOwner();
    if (!Owner) return;

    const FVector BaseLocation = Owner->GetActorLocation();
    // 단일 드랍도 원형 반경 내에서 스폰
    const FVector RandOffset = GenerateRandomXYOffsetCircular(DropRadiusMinXY, DropRadiusMaxXY);
    const FVector SpawnLocation = BaseLocation + FVector(RandOffset.X, RandOffset.Y, 0.0f);
    const FRotator SpawnRotation = FRotator::ZeroRotator;

    // 풀 매니저 찾기
    AItemPoolManager* Pool = Cast<AItemPoolManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AItemPoolManager::StaticClass()));
    if (!Pool)
    {
        // 풀이 없으면 드랍 생략(정책상 스폰하지 않음)
        return;
    }

    // 풀에서 아이템 획득(풀 가득이면 nullptr 반환 → 드랍 생략)
    Pool->AcquireItem(ItemClass, SpawnLocation, SpawnRotation);
}

void UCItemDropComponent::ForceDropItem()
{
    TryDropItem();
}

void UCItemDropComponent::SpawnItemAtLocation(TSubclassOf<AActor> ItemClass, const FVector& Location, const FRotator& Rotation)
{
    if (!GetWorld() || !*ItemClass) return;

    // 풀 매니저 찾기
    AItemPoolManager* Pool = Cast<AItemPoolManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AItemPoolManager::StaticClass()));
    if (!Pool)
    {
        // 풀이 없으면 직접 스폰
        GetWorld()->SpawnActor(ItemClass, &Location, &Rotation);
        return;
    }

    // 풀에서 아이템 획득
    Pool->AcquireItem(ItemClass, Location, Rotation);
}

FVector UCItemDropComponent::GenerateRandomXYOffset(float MinAbs, float MaxAbs) const
{
    const float ClampedMin = FMath::Max(0.0f, MinAbs);
    const float ClampedMax = FMath::Max(ClampedMin, MaxAbs);

    const float AbsX = FMath::FRandRange(ClampedMin, ClampedMax);
    const float AbsY = FMath::FRandRange(ClampedMin, ClampedMax);

    const float SignX = (FMath::RandBool() ? 1.0f : -1.0f);
    const float SignY = (FMath::RandBool() ? 1.0f : -1.0f);

    return FVector(AbsX * SignX, AbsY * SignY, 0.0f);
}

FVector UCItemDropComponent::GenerateRandomXYOffsetCircular(float MinRadius, float MaxRadius, float AngleRadians) const
{
    const float ClampedMin = FMath::Max(0.0f, MinRadius);
    const float ClampedMax = FMath::Max(ClampedMin, MaxRadius);

    const float Radius = FMath::FRandRange(ClampedMin, ClampedMax);
    const float Theta = (AngleRadians >= 0.0f) ? AngleRadians : FMath::FRandRange(0.0f, 2.0f * PI);

    const float X = Radius * FMath::Cos(Theta);
    const float Y = Radius * FMath::Sin(Theta);
    return FVector(X, Y, 0.0f);
}


