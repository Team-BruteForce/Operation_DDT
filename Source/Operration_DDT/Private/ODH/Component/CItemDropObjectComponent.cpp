#include "ODH/Component/CItemDropObjectComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Player/CPlayerBullet.h"

UCItemDropObjectComponent::UCItemDropObjectComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    
    // 초기 체력 설정
    CurrentHealth = MaxHealth;
    CurrentRespawnTimer = 0.0f;
    bIsDestroyed = false;
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
        MeshComponent = Owner->FindComponentByClass<UStaticMeshComponent>();
        
        // ItemDropComponent가 없으면 경고
        if (!ItemDropComponent)
        {
            UE_LOG(LogTemp, Warning, TEXT("UCItemDropObjectComponent: ItemDropComponent not found on owner %s"), 
                   *Owner->GetName());
        }
        
        // MeshComponent가 없으면 경고
        if (!MeshComponent)
        {
            UE_LOG(LogTemp, Warning, TEXT("UCItemDropObjectComponent: MeshComponent not found on owner %s"), 
                   *Owner->GetName());
        }
        else
        {
            // 충돌 이벤트 바인딩
            if (bEnableCollisionDamage)
            {
                MeshComponent->OnComponentHit.AddDynamic(this, &UCItemDropObjectComponent::OnHit);
            }
        }
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
    
    // 이미 파괴된 상태이거나 데미지가 0 이하면 무시
    if (bIsDestroyed || DamageAmount <= 0.0f)
    {
        UE_LOG(LogTemp, Warning, TEXT("Damage ignored - bIsDestroyed: %s, DamageAmount: %f"), 
               bIsDestroyed ? TEXT("true") : TEXT("false"), DamageAmount);
        return;
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
    // 체력을 최대치로 복구
    CurrentHealth = MaxHealth;
    
    // 파괴 상태 해제
    bIsDestroyed = false;
    
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
    
    // 파괴 지연시간 후에 Mesh 숨기고 리스폰 타이머 시작
    if (GetWorld())
    {
        GetWorld()->GetTimerManager().SetTimer(
            DestroyTimerHandle,
            this,
            &UCItemDropObjectComponent::DelayedDestroy,
            DestroyDelay,
            false
        );
    }
}

void UCItemDropObjectComponent::UpdateRespawnTimer(float DeltaTime)
{
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
    if (MeshComponent)
    {
        MeshComponent->SetVisibility(bVisible);
        
        // 충돌도 함께 설정 (보이지 않을 때는 충돌 비활성화)
        MeshComponent->SetCollisionEnabled(bVisible ? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
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
    
    // 리스폰 타이머 시작
    CurrentRespawnTimer = RespawnTime;

    // 파괴 상태로 설정
    bIsDestroyed = true;

    // 아이템 드랍 실행
    ExecuteItemDrop();

    // 파괴 이벤트 브로드캐스트
    OnObjectDestroyed.Broadcast();
    
    UE_LOG(LogTemp, Log, TEXT("UCItemDropObjectComponent: Mesh hidden, respawn timer started (%f seconds) - %s"), 
           RespawnTime, *GetOwner()->GetName());
}

void UCItemDropObjectComponent::TestTakeDamage()
{
    UE_LOG(LogTemp, Warning, TEXT("UCItemDropObjectComponent: TestTakeDamage called"));
    TakeDamage(50.0f);
}
