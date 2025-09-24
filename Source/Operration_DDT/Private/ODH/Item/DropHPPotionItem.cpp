#include "ODH/Item/DropHPPotionItem.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"
#include "ODH/Component/CItemPoolManager.h"
#include "Player/DDTPlayer.h"
#include "Player/Components/CStatusComponent.h"
#include "DrawDebugHelpers.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "ODH/Item/CDropItemEffectPoolManager.h"

ADropHPPotionItem::ADropHPPotionItem()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    SetRootComponent(Collision);
    Collision->InitSphereRadius(40.f);
    Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 처음엔 비활성화
    Collision->SetCollisionResponseToAllChannels(ECR_Overlap);
}
void ADropHPPotionItem::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (!bFalling) return;

    Elapsed += DeltaSeconds;

    if (Elapsed <= RiseTime)
    {
        const float Alpha = Elapsed / RiseTime;
        const float Height = FMath::InterpEaseOut(0.f, RiseHeight, Alpha, 2.f);
        SetActorLocation(StartLocation + FVector(0,0,Height));
        return;
    }

    const float After = Elapsed - RiseTime;
    VerticalVelocity -= Gravity * DeltaSeconds;
    FVector Loc = GetActorLocation();
    Loc.Z += VerticalVelocity * DeltaSeconds;
    SetActorLocation(Loc);

    if (Loc.Z <= StartLocation.Z)
    {
        FVector Snap = StartLocation;
        SetActorLocation(Snap);
        bFalling = false;
        
        // 낙하 완료 후 획득 가능하게 만들고 이펙트 스폰
        EnablePickup();
    }
}

void ADropHPPotionItem::OnPooledActivated(const FVector& DesiredLocation, const FRotator& DesiredRotation)
{
    StartLocation = DesiredLocation;
    if (bUseGroundSnap)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            const float Up = FMath::Max(GroundTraceDistance * 0.5f, 500.0f);
            const float Down = FMath::Max(GroundTraceDistance, 2000.0f);
            const FVector Start = DesiredLocation + FVector(0,0,Up);
            const FVector End = DesiredLocation - FVector(0,0,Down);

            FHitResult Hit;
            FCollisionQueryParams Params(SCENE_QUERY_STAT(DropPotion_GroundSnap), false, this);
            Params.AddIgnoredActor(this);
            FCollisionObjectQueryParams ObjParams;
            ObjParams.AddObjectTypesToQuery(ECC_WorldStatic);
            if (World->LineTraceSingleByObjectType(Hit, Start, End, ObjParams, Params))
            {
                StartLocation = FVector(DesiredLocation.X, DesiredLocation.Y, Hit.ImpactPoint.Z + GroundOffset);
            }
        }
    }
    Elapsed = 0.0f;
    VerticalVelocity = 300.0f;
    bFalling = true;
}



void ADropHPPotionItem::OnPooledDeactivated()
{
    bFalling = false;
    bCanBePickedUp = false;
    Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    
    // 활성화된 이펙트를 풀에 반환
    if (ActiveEffectComponent)
    {
        UWorld* World = GetWorld();
        if (World)
        {
            if (ACDropItemEffectPoolManager* EffectPool = Cast<ACDropItemEffectPoolManager>(UGameplayStatics::GetActorOfClass(World, ACDropItemEffectPoolManager::StaticClass())))
            {
                EffectPool->ReleaseDropItemEffect(ActiveEffectComponent);
            }
        }
        ActiveEffectComponent = nullptr;
    }
}

void ADropHPPotionItem::EnablePickup()
{
    bCanBePickedUp = true;
    Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    
    // 나이아가라 이펙트 풀에서 가져와서 재생
    UWorld* World = GetWorld();
    if (World)
    {
        if (ACDropItemEffectPoolManager* EffectPool = Cast<ACDropItemEffectPoolManager>(UGameplayStatics::GetActorOfClass(World, ACDropItemEffectPoolManager::StaticClass())))
        {
            UE_LOG(LogTemp, Log, TEXT("DropHPPotionItem: Found effect pool manager"));
            ActiveEffectComponent = EffectPool->AcquireDropItemEffect();
            if (ActiveEffectComponent)
            {
                UE_LOG(LogTemp, Log, TEXT("DropHPPotionItem: Acquired effect component"));
                const FVector EffectLocation = GetActorLocation() + FVector(0, 0, EffectSpawnHeight) + EffectOffset;
                EffectPool->PlayDropItemEffectAtLocation(ActiveEffectComponent, EffectLocation, GetActorRotation());
                UE_LOG(LogTemp, Log, TEXT("DropHPPotionItem: Playing effect at location: %s"), *EffectLocation.ToString());
            }
            else
            {
                UE_LOG(LogTemp, Warning, TEXT("DropHPPotionItem: Failed to acquire effect component"));
            }
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("DropHPPotionItem: Effect pool manager not found"));
        }
    }

}

void ADropHPPotionItem::EnablePickupWithoutEffect()
{
    bCanBePickedUp = true;
    Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    
    // 이펙트 없이 상호작용만 활성화 (레벨 배치된 아이템용)
    // 자동 반환 타이머는 시작하지 않음 (레벨 배치된 아이템은 영구적)
}

void ADropHPPotionItem::BeginPlay()
{
    Super::BeginPlay();
    if (Collision)
    {
        Collision->OnComponentBeginOverlap.AddDynamic(this, &ADropHPPotionItem::OnOverlapBegin);
    }
    
    // 레벨에 직접 배치된 아이템의 경우 즉시 상호작용 가능하게 설정 (이펙트 없이)
    if (!bFalling && !bCanBePickedUp)
    {
        StartLocation = GetActorLocation();
        EnablePickupWithoutEffect();
    }
}

void ADropHPPotionItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!OtherActor || !bCanBePickedUp) return;

    if (ADDTPlayer* Player = Cast<ADDTPlayer>(OtherActor))
    {
        // 플레이어의 체력 회복 아이템 개수 증가
        if (UCStatusComponent* StatusComp = Player->GetComponentByClass<UCStatusComponent>())
        {
            StatusComp->GainHealItem();
        }
        
        ReturnToPool();
    }
}

void ADropHPPotionItem::ReturnToPool()
{
    UWorld* World = GetWorld();
    if (!World) { SetActorHiddenInGame(true); SetActorEnableCollision(false); return; }

    if (AItemPoolManager* Pool = Cast<AItemPoolManager>(UGameplayStatics::GetActorOfClass(World, AItemPoolManager::StaticClass())))
    {
        Pool->ReleaseItem(this);
    }
    else
    {
        // 레벨에 직접 배치된 아이템의 경우 단순히 비활성화만 처리
        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
        SetActorTickEnabled(false);
    }
}


