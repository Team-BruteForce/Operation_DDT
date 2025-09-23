// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPlayerBullet.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/DDTPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "Boss/CBoss.h"
#include "Player/DDTGameMode.h"
#include "Player/Components/CBulletObjectPoolComponent.h"
#include "Player/Components/CDamageUIManageComponent.h"
#include "Player/Widget/CDamageWidget_Normal.h"
#include "Player/Widget/CNormalDamageUIActor.h"

// Sets default values
ACPlayerBullet::ACPlayerBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	RootComponent = CapsuleComp;
		
	
	// 충돌 설정
	CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->ProjectileGravityScale = 0.0f;
	Movement->UpdatedComponent = RootComponent;

}

// Called when the game starts or when spawned
void ACPlayerBullet::BeginPlay()
{
	Super::BeginPlay();
	// SetLifeSpan 제거 - 오브젝트 풀에서 관리
	OwnerCharacter = Cast<ADDTPlayer>(GetOwner());
	BulletPool = CHelpers::GetComponent<UCBulletObjectPoolComponent>(OwnerCharacter);
	DamageUIManager = CHelpers::GetComponent<UCDamageUIManageComponent>(OwnerCharacter);
	bIsActive = false;  // 초기에는 비활성화
	
	// 랜덤 데미지 설정
	SetRandomDamage();
	
	// 충돌 이벤트 바인딩
	if (CapsuleComp)
	{
		CapsuleComp->OnComponentHit.AddDynamic(this, &ACPlayerBullet::OnBulletHit);
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
	//CLog::Log("Bullet Speed:" + FString:: SanitizeFloat(this->GetVelocity().Size()));

	//if (bCanMove)
	//{
	//	FVector point = GetActorLocation() + TargetPoint * Speed * DeltaTime;
	//	SetActorLocation(point);
	//}

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
		//BulletTrailComp->SetVectorParameter("Direction", value);
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

void ACPlayerBullet::OnBulletHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// 자기 자신과의 충돌 무시
	if (OtherActor == this || OtherActor == OwnerCharacter)
		return;

	if (!CanApplyDamage)
	{
		//Destroy();
		ReturnToPool();
		return;
	}
	
	// 충돌한 액터 정보 로그 출력
	FString ActorName = OtherActor ? OtherActor->GetName() : TEXT("NULL");
	FString ActorClass = OtherActor ? OtherActor->GetClass()->GetName() : TEXT("NULL");
	CLog::Log("Bullet Hit with Actor: " + ActorName + " (Class: " + ActorClass + ")");
	
	// 충돌한 본 정보 가져오기
	FName HitBoneName = Hit.BoneName;
	CLog::Log("Hit Bone: " + HitBoneName.ToString());
	
	// 본별 데미지 배율 계산
	float DamageMultiplier = GetDamageMultiplierForBone(HitBoneName);
	float FinalDamage = BulletDamage;
	
	// 본별 로그 출력
	if (DamageMultiplier > 1.0f)
	{
		CLog::Log("HEADSHOT! Damage: " + FString::SanitizeFloat(FinalDamage));
	}
	else
	{
		CLog::Log("BODYSHOT! Damage: " + FString::SanitizeFloat(FinalDamage));
	}
	
	/*// 충돌 지점에 본별 색상 디버그 구체 그리기
	FColor HitColor = (DamageMultiplier > 1.0f) ? FColor::Red : FColor::Green;  // 헤드샷: 빨간색, 바디샷: 녹색
	DrawDebugSphere(
		GetWorld(),
		Hit.ImpactPoint,
		15.0f,  // 구체 반지름
		12,     // 구체 세그먼트 수
		HitColor,
		false,  // bPersistentLines
		30.0f,  // LifeTime (초)
		0,      // DepthPriority
		2.0f    // Thickness
	);*/
	
	// FPointDamageEvent 생성 및 설정
	FPointDamageEvent PointDamageEvent;
	PointDamageEvent.Damage = FinalDamage;
	PointDamageEvent.HitInfo = Hit;  // Hit 정보 사용
	PointDamageEvent.ShotDirection = Movement->Velocity.GetSafeNormal();  // 총알 방향
	PointDamageEvent.DamageTypeClass = UDamageType::StaticClass();
	
	// UGameplayStatics::ApplyPointDamage로 데미지 적용
	UGameplayStatics::ApplyPointDamage(OtherActor, FinalDamage, PointDamageEvent.ShotDirection, PointDamageEvent.HitInfo, OwnerCharacter->GetInstigatorController(), this, PointDamageEvent.DamageTypeClass);

	ACBoss* Boss = Cast<ACBoss>(OtherActor);
	if (Boss)
	{
		/*FVector DamageLocation = Boss->GetMesh()->GetComponentLocation();
		DamageLocation.Z += 325.f;

		ACNormalDamageUIActor* DamageUIActor = BulletPool->GetInactiveDamageUI();
		DamageUIActor->SetActorLocation(DamageLocation);
		DamageUIActor->SetActive(true);*/

		/*UCDamageWidget_Normal* DamageUI = DamageUIManager->GetInactiveUI();
		DamageUI->SetActive(true);*/
		/*ADDTGameMode* GM = GetWorld()->GetAuthGameMode<ADDTGameMode>();
		if (GM)
		{
			UCDamageWidget_Normal* DamageUI = GM->DamageUI;			
		}*/

	}
	
	// 충돌 후 Destroy() 대신 풀로 돌아가기
	ReturnToPool();
	
}

void ACPlayerBullet::SetActive(bool bValue)
{
	bIsActive = bValue;
	bIsInUse = bValue;  // 사용 중 플래그도 함께 설정
	bCanMove = bValue;
	MeshComp->SetVisibility(bValue);


	if (bValue)
	{
		CapsuleComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

		Movement->bSimulationEnabled = true;
		Movement->UpdateComponentVelocity();
		Movement->Activate(true);
		Movement->SetUpdatedComponent(RootComponent);
		
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
		Movement->Deactivate();
		ResetBulletState();
		
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

float ACPlayerBullet::GetDamageMultiplierForBone(const FName& BoneName) const
{
	// 헤드 본들에 대한 데미지 배율 (2배)
	if (BoneName == "head" || BoneName == "Head" || 
		BoneName == "head_01" || BoneName == "Head_01" ||
		BoneName == "skull" || BoneName == "Skull")
	{
		return 2.0f;  // 헤드샷 데미지 2배
	}
	
	// 기본 데미지 (1배)
	return 1.0f;
}

bool ACPlayerBullet::IsLifeTimerActive() const
{
	return GetWorld()->GetTimerManager().IsTimerActive(LifeTimerHandle);
}

void ACPlayerBullet::ResetBulletState()
{
	// ProjectileMovementComponent 상태 완전 초기화
	if (Movement)
	{
		//Movement->Velocity = FVector::ZeroVector;
		//Movement->StopMovementImmediately();
		//Movement->ResetMovementMode();
	}
	
	//// 위치 초기화
	//SetActorLocation(FVector::ZeroVector);
	//SetActorRotation(FRotator::ZeroRotator);
}

void ACPlayerBullet::ReturnToPool()
{
	// 풀로 돌아가기
	if (bIsActive)
	{
		// 타이머 먼저 정지
		StopLifeTimer();
		
		// 상태 초기화
		SetActive(false);
		
		// 나이아가라 이펙트가 남아있다면 강제로 제거
		if (BulletTrailComp && BulletTrailComp->IsValidLowLevel())
		{
			BulletTrailComp->DestroyComponent();
			BulletTrailComp = nullptr;
		}

	}
}

