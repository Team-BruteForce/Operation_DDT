// Fill out your copyright notice in the Description page of Project Settings.

#include "Boss/Actor/GateOfBabyonProjectile.h"
#include "Global.h"
#include "Boss/Component/CBossTargetingComponent.h"
#include "TimerManager.h"
#include "NiagaraComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGateOfBabyonProjectile::AGateOfBabyonProjectile()
{
 	PrimaryActorTick.bCanEverTick = true;
	
	// Root 컴포넌트 생성
	CHelpers::CreateComponent<USceneComponent>(this, &Root, "Root");
	SetRootComponent(Root);
	
	// 충돌 컴포넌트 생성 (Root에 연결)
	CHelpers::CreateComponent<UCapsuleComponent>(this, &CollisionComponent, "CollisionComponent", Root);

	// 나이아가라 컴포넌트 생성 (Root에 연결)
	CHelpers::CreateComponent<UNiagaraComponent>(this, &ProjectileNiagaraComponent, "ProjectileNiagaraComponent", Root);

	// 프로젝타일 이동 컴포넌트 생성
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->UpdatedComponent = Root;
	ProjectileMovementComponent->InitialSpeed = 6000.0f;
	ProjectileMovementComponent->MaxSpeed = 6000.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->bShouldBounce = false;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	
}

// Called when the game starts or when spawned
void AGateOfBabyonProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	
	// 오브젝트 풀 사용 시 초기에는 비활성화 상태로 시작
	if (bUseObjectPool)
	{
		DeactivateProjectile();
	}
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AGateOfBabyonProjectile::OnBeginOverlap);

}

// Called every frame
void AGateOfBabyonProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGateOfBabyonProjectile::MoveToRandomLocationAroundPlayer(AActor* Player)
{
	if (!Player || !ProjectileMovementComponent) return;
	
	// 플레이어 위치 가져오기
	FVector PlayerLocation = Player->GetActorLocation();
	
	// 플레이어 주변 반지름 범위 내에서 랜덤 위치 계산
	float RandomAngle = FMath::RandRange(0.0f, 2.0f * PI);
	float RandomDistance = FMath::RandRange(0.0f, RandomRadius);
	
	FVector RandomOffset = FVector(
		FMath::Cos(RandomAngle) * RandomDistance,
		FMath::Sin(RandomAngle) * RandomDistance,
		0.0f
	);
	
	TargetLocation = PlayerLocation + RandomOffset;
	
	// 프로젝타일 컴포넌트로 목표 위치로 발사
	FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
	ProjectileMovementComponent->Velocity = Direction * ProjectileMovementComponent->InitialSpeed;
}

void AGateOfBabyonProjectile::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile OnBeginOverlap called!"));
	
	if (bIsCollided) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile already collided, ignoring"));
		return;
	}
	
	bIsCollided = true;
	UE_LOG(LogTemp, Warning, TEXT("Projectile overlap detected"));
	
	// 프로젝타일 정지
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->Velocity = FVector::ZeroVector;
	}
	
	// 플레이어와 충돌했는지 확인
	bool bIsPlayerCollision = false;
	if (OtherActor)
	{
		// 플레이어인지 확인 (Pawn 타입이거나 특정 태그를 가진 경우)
		if (OtherActor->IsA<APawn>() || OtherActor->ActorHasTag(TEXT("Player")))
		{
			bIsPlayerCollision = true;
			UE_LOG(LogTemp, Warning, TEXT("Player collision detected!"));
		}
	}
	
	if (bIsPlayerCollision)
	{
		// 플레이어 충돌: 프로젝타일 나이아가라 즉시 끄고 임팩트 재생
		if (ProjectileNiagaraComponent)
		{
			ProjectileNiagaraComponent->Deactivate();
			UE_LOG(LogTemp, Warning, TEXT("Projectile Niagara deactivated immediately"));
		}
		
		// 임팩트 나이아가라 재생
		if (ImpactSystem)
		{

			FVector ImpactLocation = GetActorLocation();
			UE_LOG(LogTemp, Warning, TEXT("Spawning ImpactSystem at projectile location: %s"), *ImpactLocation.ToString());
			
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(),
				ImpactSystem,
				ImpactLocation,
				FRotator::ZeroRotator
			);
		}
		
		// 임팩트 사운드 재생 (50% 확률로만 재생)
		if (DestroySound && FMath::RandRange(0.0f, 1.0f) < 0.5f)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				DestroySound,
				GetActorLocation(),
				GetActorRotation(),
				0.9f, // 90% 볼륨
				1.0f,
				0.0f,
				SoundAttenuation,
				nullptr
			);
		}
		ACharacter* Player = Cast<ACharacter>(OtherActor);
		if(Player)
		{
			UGameplayStatics::ApplyDamage(Player,20,Player->GetController(),Player,nullptr);
		}
		
		// 즉시 비활성화
		GetWorld()->GetTimerManager().SetTimer(
			DeactivateTimerHandle,
			[this]()
			{
				UE_LOG(LogTemp, Warning, TEXT("Player collision - calling DeactivateProjectile immediately"));
				DeactivateProjectile();
			},
			0.1f, // 0.1초 후 즉시 비활성화
			false
		);
	}
	else
	{
		// 다른 오브젝트 충돌: 1초 대기 후 프로젝타일 나이아가라 끄고 임팩트 재생
		UE_LOG(LogTemp, Warning, TEXT("Non-player collision - waiting 1 second"));
		
		GetWorld()->GetTimerManager().SetTimer(
			DeactivateTimerHandle,
			[this]()
			{
				UE_LOG(LogTemp, Warning, TEXT("1 second passed - deactivating projectile Niagara and spawning impact"));
				
				// 프로젝타일 나이아가라 끄기
				if (ProjectileNiagaraComponent)
				{
					ProjectileNiagaraComponent->Deactivate();
					UE_LOG(LogTemp, Warning, TEXT("Projectile Niagara deactivated after 1 second"));
				}
				
				// 임팩트 나이아가라 재생
				if (ImpactSystem)
				{
					FVector ImpactLocation = GetActorLocation();
					UE_LOG(LogTemp, Warning, TEXT("Spawning ImpactSystem at projectile location: %s"), *ImpactLocation.ToString());
					
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(
						GetWorld(),
						ImpactSystem,
						ImpactLocation,
						FRotator::ZeroRotator
					);
				}
				
				// 임팩트 사운드 재생 (50% 확률로만 재생)
				if (DestroySound && FMath::RandRange(0.0f, 1.0f) < 0.5f)
				{
					UGameplayStatics::PlaySoundAtLocation(
						GetWorld(),
						DestroySound,
						GetActorLocation(),
						GetActorRotation(),
						0.9f, // 90% 볼륨
						1.0f,
						0.0f,
						SoundAttenuation,
						nullptr
					);
				}
				
				// 2초 후 비활성화
				GetWorld()->GetTimerManager().SetTimer(
					DeactivateTimerHandle,
					[this]()
					{
						UE_LOG(LogTemp, Warning, TEXT("Non-player collision - calling DeactivateProjectile after impact"));
						DeactivateProjectile();
					},
					2.0f,
					false
				);
			},
			0.5f, // 1초 대기
			false
		);
	}
}

void AGateOfBabyonProjectile::ActivateProjectile()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	
	// Tick 강제 활성화
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	SetActorTickEnabled(true);
	
	// 충돌 컴포넌트 활성화
	if (CollisionComponent)
	{
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
		// 오브젝트 풀에서 재사용할 때 충돌 바인딩 다시 설정
		CollisionComponent->OnComponentBeginOverlap.RemoveDynamic(this, &AGateOfBabyonProjectile::OnBeginOverlap);
		CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AGateOfBabyonProjectile::OnBeginOverlap);
	}
	
	bIsCollided = false;
	
	// 나이아가라 시스템 활성화
	if (ProjectileSystem && ProjectileNiagaraComponent)
	{
		// 완전히 재설정
		ProjectileNiagaraComponent->Deactivate();
		ProjectileNiagaraComponent->SetAsset(nullptr);
		ProjectileNiagaraComponent->SetAsset(ProjectileSystem);
		ProjectileNiagaraComponent->Activate(true); // 강제 재시작
	}
	
	// 발사 사운드 재생 (30% 확률로만 재생)
	if (ShotSound && FMath::RandRange(0.0f, 1.0f) < 0.3f)
	{
		UGameplayStatics::PlaySoundAtLocation(
			GetWorld(),
			ShotSound,
			GetActorLocation(),
			GetActorRotation(),
			0.8f, // 80% 볼륨
			1.0f,
			0.0f,
			SoundAttenuation,
			nullptr
		);
	}
	
	// 플레이어 위치로 이동 시작
	UCBossTargetingComponent* TargetingComp = CHelpers::GetComponent<UCBossTargetingComponent>(GetOwner());
	if (TargetingComp && TargetingComp->FindPlayer())
	{
		MoveToRandomLocationAroundPlayer(TargetingComp->FindPlayer());
	}
}

void AGateOfBabyonProjectile::DeactivateProjectile()
{
	UE_LOG(LogTemp, Warning, TEXT("DeactivateProjectile called"));
	
	// 타이머 정리
	GetWorld()->GetTimerManager().ClearTimer(DeactivateTimerHandle);
	
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	
	// 충돌 컴포넌트 비활성화
	if (CollisionComponent)
	{
		CollisionComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		UE_LOG(LogTemp, Warning, TEXT("CollisionComponent disabled"));
	}
	
	// 나이아가라 비활성화
	if (ProjectileNiagaraComponent)
	{
		ProjectileNiagaraComponent->Deactivate();
	}
	
	// 프로젝타일 정지
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->Velocity = FVector::ZeroVector;
	}
	
	// 오브젝트 풀 사용 시 Destroy 대신 비활성화만
	if (bUseObjectPool)
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile returned to object pool"));
		// 오브젝트 풀에 반환 (추후 구현)
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Projectile destroyed"));
		Destroy();
	}
}

