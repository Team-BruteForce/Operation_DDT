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

	// 오브젝트 풀 초기화
	for (int32 i = 0; i < MaxMagazinePool; i++)
	{
		ACPlayerBullet* bullet = CreateBulletForPool();
		MagazinePool.Add(bullet);
	}
	CurrentPoolIndex = 0;
	
}


// Called every frame
void UCFireComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
	
	/*// 디버그 라인 그리기
	FColor DebugColor = bHit ? FColor::Red : FColor::Green;
	float DebugThickness = 2.0f;
	float DebugDuration = 1.0f; // 1초간 표시
	
	DrawDebugLine(
		GetWorld(),
		TraceStart,
		TraceEnd,
		DebugColor,
		false, // bPersistentLines
		DebugDuration,
		0, // DepthPriority
		DebugThickness
	);
	
	// 충돌 지점에 구체 그리기 (충돌했을 때만)
	if (bHit)
	{
		DrawDebugSphere(
			GetWorld(),
			HitResult.ImpactPoint,
			10.0f, // 반지름
			12, // 세그먼트 수
			FColor::Yellow,
			false, // bPersistentLines
			DebugDuration
		);
	}
	
	// 시작점과 끝점에 작은 구체 그리기
	DrawDebugSphere(
		GetWorld(),
		TraceStart,
		5.0f,
		8,
		FColor::Blue,
		false,
		DebugDuration
	);
	
	DrawDebugSphere(
		GetWorld(),
		TraceEnd,
		5.0f,
		8,
		FColor::Cyan,
		false,
		DebugDuration
	);
	
	// 디버그 정보 출력
	CLog::Log("=== TRACE DEBUG INFO ===");
	CLog::Log("Camera Location: " + CameraLocation.ToString());
	CLog::Log("Camera Forward: " + CameraForwardVector.ToString());
	CLog::Log("Trace Start: " + TraceStart.ToString());
	CLog::Log("Trace End: " + TraceEnd.ToString());
	CLog::Log("Hit Result: " + FString(bHit ? TEXT("TRUE") : TEXT("FALSE")));
	if (bHit)
	{
		CLog::Log("Hit Actor: " + (HitResult.GetActor() ? HitResult.GetActor()->GetName() : FString(TEXT("NULL"))));
		CLog::Log("Hit Component: " + (HitResult.GetComponent() ? HitResult.GetComponent()->GetName() : FString(TEXT("NULL"))));
	}*/
	
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
	
	// 5. 총구 정보 가져오기
	SetMuzzleVector(RifleSocketName);
	FVector MuzzleLocation = MuzzleVector;
	
	// 6. 총구에서 충돌지점으로의 방향 벡터 계산
	FVector FireDirection = (TargetPoint - MuzzleLocation).GetSafeNormal();
	
	// 7. 오브젝트 풀에서 비활성화된 총알 가져오기
	ACPlayerBullet* Bullet = GetInactiveBullet();
	if (!Bullet)
	{
		CLog::Log("No available bullets in pool");
		return;
	}
	
	// 8. 총알 재활성화 및 위치/방향 설정
	Bullet->SetActive(true);
	Bullet->SetActorLocation(MuzzleLocation);
	Bullet->SetActorRotation(FireDirection.Rotation());
	Bullet->SetVelocity(FireDirection);
	Bullet->StartLifeTimer();  // 수명 타이머 시작
	
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



ACPlayerBullet* UCFireComponent::CreateBulletForPool()
{
	// 총알 생성 (기존과 동일한 방식)
	FActorSpawnParameters params;
	params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	params.bNoFail = true;
	params.Owner = OwnerCharacter;
	
	ACPlayerBullet* bullet = GetWorld()->SpawnActor<ACPlayerBullet>(PlayerBulletClass, params);
	
	// 생성 즉시 비활성화
	bullet->SetActive(false);
	
	// 풀로 돌아갈 때 사용할 콜백 등록
	bullet->OnReturnToPool.AddDynamic(this, &UCFireComponent::ReturnBulletToPool);
	
	return bullet;
}

ACPlayerBullet* UCFireComponent::GetInactiveBullet()
{
	// 순환 방식으로 비활성화된 총알 찾기
	for (int32 i = 0; i < MagazinePool.Num(); i++)
	{
		int32 index = (CurrentPoolIndex + i) % MagazinePool.Num();
		if (!MagazinePool[index]->IsActive())
		{
			CurrentPoolIndex = (index + 1) % MagazinePool.Num();
			return MagazinePool[index];
		}
	}
	return nullptr; // 모든 총알이 활성화된 상태
}

void UCFireComponent::ReturnBulletToPool(ACPlayerBullet* bullet)
{
	// 총알을 풀로 돌려보내기
	if (bullet)
	{
		bullet->SetActive(false);
		bullet->SetActorLocation(FVector::ZeroVector); // 안전한 위치로 이동
		bullet->StopLifeTimer();
	}
}

void UCFireComponent::FindInactiveBullet()
{
	bool findResult = false;

	for (int32 i = 0; i < MagazinePool.Num(); i++)
	{
		if (!MagazinePool[i]->IsActive())
		{
			findResult = true;
		}
	}
}

