/**
 * @file GateOfBabylon.cpp
 * @brief 바빌론의 문 액터 구현 파일
 * 
 * @details
 * 보스의 바빌론의 문 스킬을 구현하는 액터입니다.
 * 나이아가라 이펙트와 프로젝타일 발사를 처리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Actor/GateOfBabylon.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Global.h"
#include "Boss/Component/BossProjectileComponent.h"
#include "Boss/Component/CBossTargetingComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGateOfBabylon::AGateOfBabylon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 나이아가라 컴포넌트 생성
	MagicCircleComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MagicCircleComponent"));
	RootComponent = MagicCircleComponent;
	
	ProjectileNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ProjectileNiagaraComponent"));
	ProjectileNiagaraComponent->SetupAttachment(RootComponent);
	ProjectileNiagaraComponent->SetAutoActivate(false); // 처음에는 비활성화
}

// Called when the game starts or when spawned
void AGateOfBabylon::BeginPlay()
{
	Super::BeginPlay();
	
	// 오브젝트 풀 사용 시 초기에는 비활성화 상태로 시작
	if (bUseObjectPool)
	{
		DeactivateGate();
		// 프로젝타일도 미리 스폰해서 오브젝트 풀에 추가
		InitializeProjectilePool();
	}
}

// Called every frame
void AGateOfBabylon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 활성화되어 있고 오브젝트 풀 사용 중일 때만 플레이어 추적
	if (!IsHidden() && bUseObjectPool)
	{
		UpdateLookAtPlayer();
	}
}

void AGateOfBabylon::ActivateGate()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
	
	// 프로젝타일 오브젝트 풀은 이미 BeginPlay()에서 초기화됨
	
	// 1. 마법진 나이아가라 재생
	if (MagicCircleSystem && MagicCircleComponent)
	{
		MagicCircleComponent->SetAsset(MagicCircleSystem);
		MagicCircleComponent->Activate();
		
		// 나이아가라 시작과 동시에 사운드 재생 (30% 확률로만 재생)
		if (SpawnSound && FMath::RandRange(0.0f, 1.0f) < 0.3f)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				SpawnSound,
				GetActorLocation(),
				GetActorRotation(),
				0.8f, // 80% 볼륨
				1.0f,
				0.0f,
				SoundAttenuation,
				nullptr
			);
		}
	}
	
	// 2. n초 후 투사체 나이아가라 재생 (타이머 방식)
	if (ProjectileSystem && ProjectileNiagaraComponent)
	{
		ProjectileNiagaraComponent->SetAsset(ProjectileSystem);
		
		// 타이머로 지연 실행
		GetWorld()->GetTimerManager().SetTimer(
			ProjectileDelayTimerHandle,
			[this]()
			{
				if (ProjectileNiagaraComponent)
				{
					ProjectileNiagaraComponent->Activate();
					
					// 두 번째 나이아가라 활성화 후 2초 뒤에 프로젝타일 스폰
					GetWorld()->GetTimerManager().SetTimer(
						ProjectileSpawnTimerHandle,
						[this]()
						{
							SpawnProjectile();
						},
						ProjectileSpawnDelayTime,
						false
					);
				}
			},
			ProjectileDelayTime,
			false
		);
	}
	
	// 3. 10초 후 자동 비활성화 (오브젝트 풀 반환)
	GetWorld()->GetTimerManager().SetTimer(
		AutoDeactivateTimerHandle,
		[this]()
		{
			DeactivateGate();
		},
		10.0f, // 10초 후 자동 비활성화
		false
	);
}

void AGateOfBabylon::DeactivateGate()
{
	CheckNull(this);
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	
	// 모든 타이머 정리
	GetWorld()->GetTimerManager().ClearTimer(ProjectileDelayTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(ProjectileSpawnTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(AutoDeactivateTimerHandle);
	
	// 나이아가라 비활성화
	if (MagicCircleComponent)
	{
		MagicCircleComponent->Deactivate();
	}
	if (ProjectileNiagaraComponent)
	{
		ProjectileNiagaraComponent->Deactivate();
	}
	
	// 오브젝트 풀 사용 시 Destroy 대신 비활성화만
	if (bUseObjectPool)
	{
		// 오브젝트 풀에 반환
		UBossProjectileComponent* ProjectileComp = CHelpers::GetComponent<UBossProjectileComponent>(GetOwner());
		if (ProjectileComp)
		{
			ProjectileComp->ReturnGateToPool(this);
		}
	}
	else
	{
		Destroy();
	}
}

void AGateOfBabylon::InitializeProjectilePool()
{
	if (bProjectilePoolInitialized) return;
	
	UWorld* World = GetWorld();
	if (!World || !ProjectileClass) return;
	
	// 프로젝타일 1개만 미리 생성 (마법진 하나당 하나씩)
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Owner = GetOwner();
	
	AGateOfBabyonProjectile* Projectile = World->SpawnActor<AGateOfBabyonProjectile>(
		ProjectileClass,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		SpawnParams
	);
	
	if (Projectile)
	{
		// 오브젝트 풀 사용하도록 설정 (기본값 유지)
		// BeginPlay()에서 이미 DeactivateProjectile()이 호출되었으므로
		// 추가로 DeactivateProjectile()을 호출할 필요 없음
		ProjectilePool.Add(Projectile);
	}
	
	bProjectilePoolInitialized = true;
}

AGateOfBabyonProjectile* AGateOfBabylon::GetProjectileFromPool()
{
	for (AGateOfBabyonProjectile* Projectile : ProjectilePool)
	{
		if (Projectile && Projectile->IsHidden())
		{
			return Projectile;
		}
	}
	return nullptr;
}

void AGateOfBabylon::SpawnProjectile()
{
	if (!ProjectileClass)
	{
		return;
	}
	
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	// 두 번째 나이아가라 끄기
	if (ProjectileNiagaraComponent)
	{
		ProjectileNiagaraComponent->Deactivate();
	}
	
	// 자체 오브젝트 풀에서 프로젝타일 가져오기
	AGateOfBabyonProjectile* Projectile = GetProjectileFromPool();
	
	if (Projectile)
	{
		// 위치 설정 후 활성화
		Projectile->SetActorLocation(GetActorLocation());
		Projectile->SetActorRotation(GetActorRotation());
		Projectile->ActivateProjectile();
	}
	else
	{
		// 풀이 비어있으면 경고 로그 (이론적으로는 발생하지 않아야 함)
		
		// 비상용: 새로 스폰
		FVector SpawnLocation = GetActorLocation();
		FRotator SpawnRotation = GetActorRotation();
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = GetOwner();
		
		AGateOfBabyonProjectile* NewProjectile = World->SpawnActor<AGateOfBabyonProjectile>(
			ProjectileClass,
			SpawnLocation,
			SpawnRotation,
			SpawnParams
		);
		
		if (NewProjectile && NewProjectile->bUseObjectPool)
		{
			NewProjectile->ActivateProjectile();
		}
	}
}

void AGateOfBabylon::UpdateLookAtPlayer()
{
	// 보스 타겟팅 컴포넌트에서 플레이어 찾기
	if (!GetOwner()) return;
	
	UCBossTargetingComponent* TargetingComp = CHelpers::GetComponent<UCBossTargetingComponent>(GetOwner());
	if (TargetingComp && TargetingComp->FindPlayer())
	{
		FVector GateLocation = GetActorLocation();
		FVector PlayerLocation = TargetingComp->FindPlayer()->GetActorLocation();
		FVector Direction = (PlayerLocation - GateLocation).GetSafeNormal();
		FRotator TargetRotation = Direction.Rotation();
		
		// 수직으로 서있게 (90도)
		TargetRotation.Pitch = 90.0f;
		
		SetActorRotation(TargetRotation);
	}
}


