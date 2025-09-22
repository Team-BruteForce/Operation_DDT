// Fill out your copyright notice in the Description page of Project Settings.

#include "ODH/ODH_Enemy/Projectile/CPoisonArea.h"
#include "ODH/ODH_Enemy/Projectile/CPoisonAreaPoolManager.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Player/DDTPlayer.h"
#include "Engine/DamageEvents.h"

// Sets default values
ACPoisonArea::ACPoisonArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 루트 컴포넌트 생성
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// 콜리전 컴포넌트 생성
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(RootComponent);
	CollisionComponent->SetSphereRadius(AreaRadius);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	// 나이아가라 이펙트 컴포넌트 생성
	AreaEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("AreaEffect"));
	AreaEffect->SetupAttachment(RootComponent);
	AreaEffect->SetAutoActivate(false); // 수동으로 활성화

	// 초기에는 비활성화 상태
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	// 초기값 설정
	bIsActive = false;
	RemainingDuration = 0.0f;
	DamageTimer = 0.0f;
	PoolManager = nullptr;
}

// Called when the game starts or when spawned
void ACPoisonArea::BeginPlay()
{
	Super::BeginPlay();
	
	// 충돌 이벤트 바인딩
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ACPoisonArea::OnPoisonAreaBeginOverlap);
	CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ACPoisonArea::OnPoisonAreaEndOverlap);
}

// Called every frame
void ACPoisonArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsActive)
	{
		// 지속 시간 감소
		RemainingDuration -= DeltaTime;
		
		// 독성 데미지 타이머 업데이트
		DamageTimer += DeltaTime;
		
		// 데미지 간격마다 데미지 적용
		if (DamageTimer >= DamageInterval)
		{
			ApplyPoisonDamage();
			DamageTimer = 0.0f;
		}
		
		// 디버그 그리기
		if (bShowDebugArea)
		{
			DrawDebugArea();
		}
		
		// 지속 시간 만료 시 비활성화
		if (RemainingDuration <= 0.0f)
		{
			DeactivatePoisonArea();
		}
	}
}

void ACPoisonArea::ActivatePoisonArea(float Duration)
{
	// 독성 영역 활성화
	bIsActive = true;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	// 나이아가라 이펙트 활성화
	if (AreaEffect)
	{
		if (AreaEffectAsset)
		{
			AreaEffect->SetAsset(AreaEffectAsset);
		}
		AreaEffect->Activate();
	}

	// 지속 시간 설정
	RemainingDuration = Duration > 0.0f ? Duration : DefaultDuration;
	DamageTimer = 0.0f;

	// 영역 내 플레이어 목록 초기화
	PlayersInArea.Empty();

	// 콜리전 반경 업데이트
	CollisionComponent->SetSphereRadius(AreaRadius);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, 
			FString::Printf(TEXT("PoisonArea Activated for %.1f seconds"), RemainingDuration));
	}
}

void ACPoisonArea::DeactivatePoisonArea()
{
	// 독성 영역 비활성화
	bIsActive = false;
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);

	// 나이아가라 이펙트 비활성화
	if (AreaEffect)
	{
		AreaEffect->Deactivate();
	}

	// 영역 내 플레이어 목록 초기화
	PlayersInArea.Empty();

	// 변수 초기화
	RemainingDuration = 0.0f;
	DamageTimer = 0.0f;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("PoisonArea Deactivated"));
	}

	// 풀 매니저로 반환
	ReturnToPool();
}

void ACPoisonArea::ReturnToPool()
{
	if (PoolManager)
	{
		PoolManager->ReleasePoisonArea(this);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("PoisonArea Returned to Pool"));
		}
	}
	else
	{
		// 풀 매니저가 없으면 직접 제거
		Destroy();
	}
}

void ACPoisonArea::SetPoolManager(ACPoisonAreaPoolManager* Manager)
{
	PoolManager = Manager;
}

void ACPoisonArea::OnPoisonAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsActive || !OtherActor)
		return;

	// 플레이어인지 확인
	if (ADDTPlayer* Player = Cast<ADDTPlayer>(OtherActor))
	{
		// 이미 목록에 없는 경우에만 추가
		if (!PlayersInArea.Contains(OtherActor))
		{
			PlayersInArea.Add(OtherActor);
			
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, 
					FString::Printf(TEXT("Player entered Poison Area: %s"), *OtherActor->GetName()));
			}
		}
	}
}

void ACPoisonArea::OnPoisonAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!bIsActive || !OtherActor)
		return;

	// 플레이어인지 확인
	if (ADDTPlayer* Player = Cast<ADDTPlayer>(OtherActor))
	{
		// 목록에서 제거
		PlayersInArea.Remove(OtherActor);
		
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, 
				FString::Printf(TEXT("Player left Poison Area: %s"), *OtherActor->GetName()));
		}
	}
}

void ACPoisonArea::ApplyPoisonDamage()
{
	if (!bIsActive)
		return;

	// 영역 내 모든 플레이어에게 데미지 적용
	for (AActor* Player : PlayersInArea)
	{
		if (IsValid(Player))
		{
			ApplyDamageToPlayer(Player);
		}
	}
}

void ACPoisonArea::ApplyDamageToPlayer(AActor* Player)
{
	if (!IsValid(Player))
		return;

	// 플레이어에게 데미지 적용
	FPointDamageEvent DamageEvent;
	DamageEvent.Damage = PoisonDamage;
	DamageEvent.ShotDirection = FVector::UpVector;
	DamageEvent.DamageTypeClass = nullptr;

	Player->TakeDamage(PoisonDamage, DamageEvent, nullptr, this);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, 
			FString::Printf(TEXT("Poison Damage Applied: %.1f to %s"), PoisonDamage, *Player->GetName()));
	}
}

void ACPoisonArea::DrawDebugArea()
{
	if (!bIsActive)
		return;

	// 독성 영역 시각화
	DrawDebugSphere(
		GetWorld(),
		GetActorLocation(),
		AreaRadius,
		16,
		FColor::Green,
		false,
		0.1f,
		0,
		2.0f
	);

	// 남은 지속 시간 표시
	FString TimeText = FString::Printf(TEXT("Poison Area: %.1fs"), RemainingDuration);
	DrawDebugString(
		GetWorld(),
		GetActorLocation() + FVector(0, 0, 100),
		TimeText,
		nullptr,
		FColor::Green,
		0.1f
	);
}
