// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Components/CFireComponent.h"
#include "Global.h"
#include "KismetTraceUtils.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Player/CPlayerBullet.h"
#include "Player/DDTPlayer.h"
#include "Weapons/CAttachment.h"
#include "Player/Components/CMagazineComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Player/Components/CBulletObjectPoolComponent.h"

// Sets default values for this component's properties
UCFireComponent::UCFireComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCFireComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ADDTPlayer>(GetOwner());
	FName HandSocketName = FName("Hand_Rifle");
	RifleSocketName = FName("MuzzlePos");
	Rifle = Cast<ACAttachment>(GetActorAttachedToSocket(HandSocketName));
	MagazineComponent = CHelpers::GetComponent<UCMagazineComponent>(OwnerCharacter);
	BulletPool = CHelpers::GetComponent<UCBulletObjectPoolComponent>(OwnerCharacter);
	
}


// Called every frame
void UCFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//CLog::Print("MagazinePool : " +FString::FromInt( MagazinePool.Num()));
	
	// ...
}

AActor* UCFireComponent::GetActorAttachedToSocket(const FName& SocketName)
{
	if (!OwnerCharacter)
	{
		CLog::Log("OwnerCharacter is null");
		return nullptr;
	}

	USkeletalMeshComponent* MeshComponent = OwnerCharacter->GetMesh();
	if (!MeshComponent)
	{
		CLog::Log("MeshComponent is null");
		return nullptr;
	}

	TArray<USceneComponent*> AttachedComponents;
	MeshComponent->GetChildrenComponents(true, AttachedComponents);

	for (USceneComponent* Comp : AttachedComponents)
	{
		if (Comp->GetAttachSocketName() == SocketName)
		{
			AActor* AttachedActor = Comp->GetOwner();
			if (AttachedActor)
			{
				CLog::Log("Found Actor : "+ AttachedActor->GetName());
				return AttachedActor;
			}
		}
	}
	
	// 소켓에 붙어있는 액터를 찾지 못한 경우
	CLog::Log("No actor found attached to socket: " + SocketName.ToString());
	return nullptr;
}

void UCFireComponent::SetMuzzleVector(const FName& SocketName)
{
	if (!Rifle)
	{
		CLog::Log("Rifle is null");
		return;
	}
	USkeletalMeshComponent* MeshComponent = Rifle->GetMeshComp();
	if (!MeshComponent)
	{
		CLog::Log("MeshComponent is null");
		return;
	}
	if (MeshComponent->DoesSocketExist(SocketName))
	{
		FTransform SocketTransform = MeshComponent->GetSocketTransform(SocketName);
		MuzzleVector = SocketTransform.GetLocation();
		MuzzleRotator = SocketTransform.GetRotation().Rotator();
		MuzzleForwardDirection = SocketTransform.GetRotation().GetForwardVector();
		CLog::Log(MuzzleVector);
		
	}
	else
	{
		CLog::Log("Socket not found: " + SocketName.ToString());
	}
}

void UCFireComponent::Fire()
{
	// 1. 기본 설정 및 검증
	if (!OwnerCharacter || !Rifle)
	{
		CLog::Log("OwnerCharacter or Rifle is null");
		return;
	}
	
	// 2. 카메라 정보 가져오기
	UCameraComponent* Camera = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);
	if (!Camera)
	{
		CLog::Log("Camera is null");
		return;
	}
	
	FVector CameraLocation = Camera->GetComponentLocation();
	FVector CameraForwardVector = Camera->GetForwardVector();
	
	// 3. 카메라 전방벡터를 기준으로 라인트레이스 실행
	FHitResult HitResult;
	FVector TraceStart = CameraLocation;
	FVector TraceEnd = CameraLocation + (CameraForwardVector * MaxTraceDistance);
	
	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		TraceStart,
		TraceEnd,
		ECollisionChannel::ECC_GameTraceChannel9, // "Rifle" trace channel
		FCollisionQueryParams::DefaultQueryParam
	);
	
	// 5. 총구 정보 가져오기
	SetMuzzleVector(RifleSocketName);
	FVector MuzzleLocation = MuzzleVector;
	
	// 4. 충돌지점 결정
	FVector TargetPoint;
	if (bHit)
	{
		// 적이나 오브젝트와 충돌한 경우
		TargetPoint = HitResult.ImpactPoint;
		CLog::Log("Hit target at: " + TargetPoint.ToString());
		
	}
	else
	{
		// 충돌하지 않은 경우 카메라 전방의 먼 지점
		TargetPoint = TraceEnd;
		CLog::Log("No hit, using far point: " + TargetPoint.ToString());
	}
	
	/*
	// 카메라에서 발사하는 라인트레이스 그리기 (파란색)
	DrawDebugLine(
		GetWorld(),
		TraceStart,
		TraceEnd,
		FColor::Blue,
		false,  // bPersistentLines
		30.0f,   // LifeTime (초)
		0,      // DepthPriority
		1.0f    // Thickness
	);
	
	// 총구에서 발사하는 라인트레이스 그리기 (녹색)
	FVector MuzzleTraceEnd = MuzzleLocation + ((TargetPoint - MuzzleLocation).GetSafeNormal() * MaxTraceDistance);
	DrawDebugLine(
		GetWorld(),
		MuzzleLocation,
		MuzzleTraceEnd,
		FColor::White,
		false,  // bPersistentLines
		30.0f,   // LifeTime (초)
		0,      // DepthPriority
		1.0f    // Thickness
	);
	*/
	
	
	// 6. 총구에서 충돌지점으로의 방향 벡터 계산
	FVector FireDirection = (TargetPoint - MuzzleLocation).GetSafeNormal();

	CLog::Log("Fire Direction: " + FireDirection.ToString());
	
	// 7. 오브젝트 풀에서 비활성화된 총알 가져오기
	/*ACPlayerBullet* Bullet = GetInactiveBullet();
	if (!Bullet)
	{
		CLog::Log("No available bullets in pool");
		return;
	}*/
	//ACPlayerBullet* vfx = BulletPool->CreateBulletVFXForPool();
	ACPlayerBullet* vfx = BulletPool->GetInactiveVFX();
	
	vfx->SetActorLocation(MuzzleLocation);
	vfx->SetActorRotation(FireDirection.Rotation());
	vfx->SetActive(true);
	vfx->SetVelocity(FireDirection);
	//vfx->SetTargetPoint(FireDirection);

	CLog::Log("Pool)   Bullet Vfx Velocity: " + vfx->GetVelocity().ToString());
	vfx->StartLifeTimer();

	//ACPlayerBullet* bullet = BulletPool->CreateBulletForPool();
	ACPlayerBullet* bullet = BulletPool->GetInactiveBullet();
	
	bullet->SetActorLocation(CameraLocation);
	bullet->SetActorRotation(CameraForwardVector.Rotation());
	bullet->SetActive(true);
	bullet->SetVelocity(CameraForwardVector);
	//bullet->SetTargetPoint(CameraForwardVector);
	bullet->StartLifeTimer();
	
	CLog::Log("Fired bullet towards target: " + TargetPoint.ToString());
	
	MagazineComponent->FireBullet();
	
	// 9. 총구 화염 이펙트 부착
	if (MuzzleFireSystem && Rifle)
	{
		USkeletalMeshComponent* RifleMesh = Rifle->GetMeshComp();
		if (RifleMesh)
		{
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				MuzzleFireSystem,
				RifleMesh,           // 총의 메시 컴포넌트
				RifleSocketName,     // "MuzzlePos" 소켓
				FVector::ZeroVector, // 소켓 기준 오프셋 (0,0,0)
				FRotator::ZeroRotator, // 소켓 기준 회전 오프셋
				EAttachLocation::SnapToTarget,
				true  // bAutoDestroy
			);
		}
	}
}
