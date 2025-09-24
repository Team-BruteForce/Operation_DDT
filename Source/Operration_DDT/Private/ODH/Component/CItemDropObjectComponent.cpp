#include "ODH/Component/CItemDropObjectComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Player/CPlayerBullet.h"
#include "ODH/Component/ShakeAndSinkComponent.h"

UCItemDropObjectComponent::UCItemDropObjectComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // 초기 체력 설정
    CurrentHealth = MaxHealth;
    CurrentRespawnTimer = 0.0f;
    bIsDestroyed = false;
    bItemDropped = false;
}

void UCItemDropObjectComponent::BeginPlay()
{
    Super::BeginPlay();
    
    // 소유자에서 필요한 컴포넌트들 찾기
    AActor* Owner = GetOwner();
    if (Owner)
    {
        // ItemDropComponent 찾기
        ItemDropComponent = Owner->FindComponentByClass<UCItemDropComponent>();
        
        // MeshComponent 찾기 (StaticMeshComponent 또는 SkeletalMeshComponent)
        /*MeshComponent = Owner->FindComponentByClass<UStaticMeshComponent>();*/
        
        // ItemDropComponent가 없으면 경고
        if (!ItemDropComponent)
        {
            UE_LOG(LogTemp, Warning, TEXT("UCItemDropObjectComponent: ItemDropComponent not found on owner %s"), 
                   *Owner->GetName());
        }
        
        UE_LOG(LogTemp, Log, TEXT("UCItemDropObjectComponent: BeginPlay completed - %s"), 
               *Owner->GetName());
    }
    
    // 초기 체력을 최대 체력으로 설정
    CurrentHealth = MaxHealth;
    
    // 타이머 핸들 초기화
    DestroyTimerHandle.Invalidate();
}

void UCItemDropObjectComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
    // 파괴된 상태일 때만 리스폰 타이머 업데이트
    if (bIsDestroyed)
    {
        UpdateRespawnTimer(DeltaTime);
    }
}

void UCItemDropObjectComponent::TakeDamage(float DamageAmount)
{
    UE_LOG(LogTemp, Warning, TEXT("UCItemDropObjectComponent::TakeDamage called with damage: %f"), DamageAmount);
    
    // 하강 연출 시작 (지연 0, 떨림 0) - 에디터 설정값 반영
    if (AActor* Owner = GetOwner())
    {
        UShakeAndSinkComponent* EffectComp = Owner->FindComponentByClass<UShakeAndSinkComponent>();
        if (!EffectComp)
        {
            EffectComp = NewObject<UShakeAndSinkComponent>(Owner);
            if (EffectComp)
            {
                EffectComp->RegisterComponent();
            }
        }

        if (EffectComp)
        {
            EffectComp->StartEffect(0.0f, SinkDistanceOnDestroy, SinkDurationOnDestroy, 0.0f, 0.0f);
        }
    }
    
    // 이미 파괴된 상태이거나 데미지가 0 이하면 무시
    if (bIsDestroyed || DamageAmount <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Damage ignored - bIsDestroyed: %s, DamageAmount: %f"), 
               bIsDestroyed ? TEXT("true") : TEXT("false"), DamageAmount);
        return;
    }

    // 타격 횟수 증가 후 홀수 타격일 때만 아이템 드랍
    HitCount++;
    if ((HitCount % 2) == 1)
    {
        ExecuteItemDrop();
    }

    UE_LOG(LogTemp, Warning, TEXT("Taking damage: %f, Current health before: %f"), DamageAmount, CurrentHealth);
    
    // 체력 감소
    CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);
    
    UE_LOG(LogTemp, Warning, TEXT("Current health after damage: %f"), CurrentHealth);
    
    // 체력이 0 이하가 되면 파괴 처리
    if (CurrentHealth <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Health reached 0, destroying object"));
        HandleObjectDestroyed();
    }
}


void UCItemDropObjectComponent::RespawnObject()
{
    // IsRespawn이 false이면 리스폰하지 않음
    if (!IsRespawn)
    {
        UE_LOG(LogTemp, Log, TEXT("UCItemDropObjectComponent: Respawn disabled, not respawning - %s"), 
               *GetOwner()->GetName());
        return;
    }
    
    // 체력을 최대치로 복구
    CurrentHealth = MaxHealth;
    
    // 파괴 상태 해제
    bIsDestroyed = false;
    bItemDropped = false;
    HitCount = 0;
    
    // 리스폰 타이머 리셋
    CurrentRespawnTimer = 0.0f;
    
    // Mesh 다시 보이게 설정
    SetMeshVisibility(true);
    
    // 리스폰 이벤트 브로드캐스트
    OnObjectRespawned.Broadcast();
    
    UE_LOG(LogTemp, Log, TEXT("UCItemDropObjectComponent: Object respawned successfully - %s"), 
           *GetOwner()->GetName());
}

void UCItemDropObjectComponent::HandleObjectDestroyed()
{
    // 이미 파괴된 상태면 무시
    if (bIsDestroyed)
    {
        return;
    }
    
    UE_LOG(LogTemp, Log, TEXT("UCItemDropObjectComponent: Object destroyed - %s"), 
           *GetOwner()->GetName());

    

    // 하강 중 6초 후 메쉬 숨김이 적용되므로, 그 시점에 파괴 처리 콜백 실행
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            DestroyTimerHandle,
            this,
            &UCItemDropObjectComponent::DelayedDestroy,
            6.0f,
            false
        );
    }
}

void UCItemDropObjectComponent::UpdateRespawnTimer(float DeltaTime)
{
    // IsRespawn이 false이면 리스폰 타이머 업데이트 안함
    if (!IsRespawn)
    {
        return;
    }
    
    // 리스폰 타이머 감소
    CurrentRespawnTimer -= DeltaTime;
    
    // 타이머가 0 이하가 되면 리스폰
    if (CurrentRespawnTimer <= 0.0f)
    {
        UE_LOG(LogTemp, Log, TEXT("UCItemDropObjectComponent: Respawn time reached, respawning object - %s"), 
               *GetOwner()->GetName());
        RespawnObject();
    }
}

void UCItemDropObjectComponent::SetMeshVisibility(bool bVisible)
{
    // HitMeshComponent1 가시성 설정
    if (HitMeshComponent1)
    {
        HitMeshComponent1->SetVisibility(bVisible);
        
        // 충돌도 함께 설정 (보이지 않을 때는 충돌 비활성화)
        HitMeshComponent1->SetCollisionEnabled(bVisible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
    }

    // HitMeshComponent2 가시성 설정
    if (HitMeshComponent2)
    {
        HitMeshComponent2->SetVisibility(bVisible);
        
        // 충돌도 함께 설정 (보이지 않을 때는 충돌 비활성화)
        HitMeshComponent2->SetCollisionEnabled(bVisible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
    }

    // MeshComponent 가시성 설정 (충돌 없음)
    if (MeshComponent)
    {
        MeshComponent->SetVisibility(bVisible);
    }
}

void UCItemDropObjectComponent::ExecuteItemDrop()
{
    // ItemDropComponent가 있으면 아이템 드랍 시도
    if (ItemDropComponent)
    {
        // ItemDropComponent의 ForceDropItem 함수를 사용하여 아이템 드랍
        ItemDropComponent->ForceDropItem();
    }
}

void UCItemDropObjectComponent::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!bEnableCollisionDamage || bIsDestroyed || !OtherActor)
    {
        return;
    }

    // 쿨다운 체크
    if (GetWorld())
    {
        float CurrentTime = GetWorld()->GetTimeSeconds();
        if (CurrentTime - LastCollisionTime < CollisionCooldownTime)
        {
            return;
        }
        LastCollisionTime = CurrentTime;
    }

    UE_LOG(LogTemp, Warning, TEXT("UCItemDropObjectComponent: Hit by %s"), *OtherActor->GetName());
    
    // 플레이어 총알인지 확인하고 ReturnToPool 호출
    if (ACPlayerBullet* PlayerBullet = Cast<ACPlayerBullet>(OtherActor))
    {
        UE_LOG(LogTemp, Warning, TEXT("UCItemDropObjectComponent: Player bullet hit, returning to pool"));
        PlayerBullet->ReturnToPool();

        // 충돌 데미지 적용
        TakeDamage(CollisionDamageAmount);
    }
}


void UCItemDropObjectComponent::DelayedDestroy()
{
    // Mesh 숨기기
    SetMeshVisibility(false);
    
    // 파괴 상태로 설정
    bIsDestroyed = true;

    // 아이템 드랍 실행
    ExecuteItemDrop();

    // 파괴 이벤트 브로드캐스트
    OnObjectDestroyed.Broadcast();
    
    // IsRespawn이 true일 때만 리스폰 타이머 시작
    if (IsRespawn)
    {
        CurrentRespawnTimer = RespawnTime;
        UE_LOG(LogTemp, Log, TEXT("UCItemDropObjectComponent: Mesh hidden, respawn timer started (%f seconds) - %s"), 
               RespawnTime, *GetOwner()->GetName());
    }
    else
    {
        CurrentRespawnTimer = 0.0f; // 리스폰 안함
        UE_LOG(LogTemp, Log, TEXT("UCItemDropObjectComponent: Mesh hidden, respawn disabled - %s"), 
               *GetOwner()->GetName());
    }
}

void UCItemDropObjectComponent::SetHitMeshComponent1(UStaticMeshComponent* MeshComp)
{
    HitMeshComponent1 = MeshComp;
    HitMeshComponent1->OnComponentHit.AddDynamic(this, &UCItemDropObjectComponent::OnHit);
}

void UCItemDropObjectComponent::SetHitMeshComponent2(UStaticMeshComponent* MeshComp)
{
    HitMeshComponent2 = MeshComp;
    HitMeshComponent2->OnComponentHit.AddDynamic(this, &UCItemDropObjectComponent::OnHit);
}

void UCItemDropObjectComponent::SetMeshComponent(UStaticMeshComponent* MeshComp)
{
    MeshComponent = MeshComp;
}

void UCItemDropObjectComponent::TestTakeDamage()
{
    UE_LOG(LogTemp, Warning, TEXT("UCItemDropObjectComponent: TestTakeDamage called"));
    TakeDamage(1.0f);
}
