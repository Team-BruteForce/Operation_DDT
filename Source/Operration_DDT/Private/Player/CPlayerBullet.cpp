// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPlayerBullet.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/DDTPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

// Sets default values
ACPlayerBullet::ACPlayerBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//CapsuleComp = CreateDefaultSubobject <UCapsuleComponent>(TEXT("CapsuleComp"));
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	SetRootComponent(Root);
	CHelpers::CreateComponent<UCapsuleComponent>(this, &CapsuleComp, "CapsuleComp", Root);

	//CapsuleComp->SetCapsuleHalfHeight (4.f);
	//CapsuleComp->SetCapsuleRadius (2.f);
	
	// 충돌 설정
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CapsuleComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CapsuleComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	
	CHelpers::CreateComponent<UStaticMeshComponent>(this, &MeshComp, "MeshComp", Root);

	//ProjectileMovement 부착
	CHelpers::CreateActorComponent<UProjectileMovementComponent>(this, &Movement, "Movement");
	Movement->ProjectileGravityScale = 0.0f;
	Movement->UpdatedComponent = RootComponent;

}

// Called when the game starts or when spawned
void ACPlayerBullet::BeginPlay()
{
	Super::BeginPlay();
	// SetLifeSpan 제거 - 오브젝트 풀에서 관리
	OwnerCharacter = Cast<ADDTPlayer>(GetOwner());
	bIsActive = false;  // 초기에는 비활성화
	
	// 랜덤 데미지 설정
	SetRandomDamage();
	
	// 충돌 이벤트 바인딩
	if (CapsuleComp)
	{
		CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ACPlayerBullet::OnBulletOverlap);
	}

	if (BulletTrailSystem)
	{
		BulletTrailComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			BulletTrailSystem,
			CapsuleComp,
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::SnapToTarget,
			false  // bAutoDestroy
		);
	}
	
}

// Called every frame
void ACPlayerBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACPlayerBullet::SetVelocity(FVector value)
{
	Movement->Velocity = value * Speed;
	
	// 이펙트 방향을 총알 진행 방향에 맞춰 조정 (이펙트가 존재할 때만)
	if (BulletTrailComp && BulletTrailComp->IsValidLowLevel())
	{
		// 속도 벡터를 회전으로 변환
		FRotator EffectRotation = value.Rotation();
		BulletTrailComp->SetWorldRotation(EffectRotation);
		
		// 나이아가라 파라미터로 방향 전달 (선택사항)
		BulletTrailComp->SetVectorParameter("Direction", value);
	}
}

void ACPlayerBullet::SetRandomDamage()
{
	// 1. 랜덤 시드 초기화 (현재 시간 기반)
	FMath::RandInit(FDateTime::Now().GetTicks());
	
	// 2. 160~165 범위의 정수 랜덤 값 생성
	int32 RandomInt = FMath::RandRange(160, 165);
	
	// 3. BulletDamage에 정수 랜덤 값 설정
	BulletDamage = static_cast<float>(RandomInt);
	
	// 4. 로그 출력 (디버깅용)
	CLog::Log("Random Integer Bullet Damage: " + FString::FromInt(RandomInt));
}

void ACPlayerBullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 자기 자신과의 충돌 무시
	if (OtherActor == this || OtherActor == OwnerCharacter)
		return;
	
	CLog::Log("Overlap Occured");
	
	// FPointDamageEvent 생성 및 설정
	FPointDamageEvent PointDamageEvent;
	PointDamageEvent.Damage = BulletDamage;
	PointDamageEvent.HitInfo = SweepResult;  // 충돌 정보 사용
	PointDamageEvent.ShotDirection = Movement->Velocity.GetSafeNormal();  // 총알 방향
	PointDamageEvent.DamageTypeClass = UDamageType::StaticClass();
	
	// UGameplayStatics::ApplyPointDamage로 데미지 적용
	UGameplayStatics::ApplyPointDamage(OtherActor, BulletDamage, PointDamageEvent.ShotDirection, PointDamageEvent.HitInfo, OwnerCharacter->GetInstigatorController(), this, PointDamageEvent.DamageTypeClass);
	
	// 충돌 후 Destroy() 대신 풀로 돌아가기
	ReturnToPool();
}

void ACPlayerBullet::SetActive(bool bValue)
{
	bIsActive = bValue;
	MeshComp->SetVisibility(bValue);

	if (bValue)
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
		// 활성화 시 나이아가라 이펙트 생성
		if (BulletTrailSystem && !BulletTrailComp)
		{
			BulletTrailComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
				BulletTrailSystem,
				CapsuleComp,
				NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::SnapToTarget,
				true  // bAutoDestroy
			);
		}
	}
	else
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 비활성화 시 속도 초기화
		Movement->Velocity = FVector::ZeroVector;
		
		// 비활성화 시 나이아가라 이펙트 제거
		if (BulletTrailComp)
		{
			BulletTrailComp->DestroyComponent();
			BulletTrailComp = nullptr;
		}
	}
}

bool ACPlayerBullet::IsActive() const
{
	return bIsActive;
}

void ACPlayerBullet::StartLifeTimer()
{
	// 수명 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(
		LifeTimerHandle,
		this,
		&ACPlayerBullet::OnLifeTimeExpired,
		LifeTime,
		false  // 한 번만 실행
	);
}

void ACPlayerBullet::StopLifeTimer()
{
	// 수명 타이머 중지
	GetWorld()->GetTimerManager().ClearTimer(LifeTimerHandle);
}

void ACPlayerBullet::OnLifeTimeExpired()
{
	// 수명이 만료되면 풀로 돌아가기
	ReturnToPool();
}

void ACPlayerBullet::ReturnToPool()
{
	// 풀로 돌아가기
	if (bIsActive)
	{
		SetActive(false);
		
		// 나이아가라 이펙트가 남아있다면 강제로 제거
		if (BulletTrailComp && BulletTrailComp->IsValidLowLevel())
		{
			BulletTrailComp->DestroyComponent();
			BulletTrailComp = nullptr;
		}
		
		OnReturnToPool.Broadcast(this);
	}
}

