/**
 * @file BossProjectileComponent.cpp
 * @brief 보스 프로젝타일 컴포넌트 구현 파일
 * 
 * @details
 * 보스 캐릭터의 프로젝타일 발사 기능을 관리하는 컴포넌트입니다.
 * 다양한 타입의 프로젝타일과 오브 생성 기능을 제공합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Component/BossProjectileComponent.h"
#include "Global.h"
#include "Boss/Component/CBossTargetingComponent.h"
#include "Boss/Projectile/BossProjectileActor.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UBossProjectileComponent::UBossProjectileComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	CHelpers::GetClass<ABossProjectileActor>(&ProjectileClass,AssetPaths::Boss_Projectile);
	CHelpers::GetClass<ABossProjectileOrb>(&ProjectileOrbClass,AssetPaths::Boss_Orb);
}

void UBossProjectileComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// 오브젝트 풀 초기화
	InitializeObjectPool();
}

// 기존 단일 발사 함수
void UBossProjectileComponent::ShotProjectile()
{
	UCBossTargetingComponent* TargetingComp = CHelpers::GetComponent<UCBossTargetingComponent>(GetOwner());
	if (TargetingComp && TargetingComp->FindPlayer())
	{
		ACharacter* Boss = Cast<ACharacter>(GetOwner());
		if (Boss)
		{
			// 플레이어 방향으로 회전 계산 (창이 세워서 날아가도록)
			FVector PlayerLocation = TargetingComp->FindPlayer()->GetActorLocation();
			FVector BossLocation = Boss->GetActorLocation();
			FVector Direction = (PlayerLocation - BossLocation).GetSafeNormal();
			FRotator TargetRotation = Direction.Rotation();
			
			// 보스 소켓 위치에서 스폰 (플레이어 방향으로 회전)
			FVector SpawnLocation = Boss->GetMesh()->GetSocketLocation(FName("LightningSpear"));
			
			// 오브젝트 풀에서 투사체 가져오기
			AProjectile_LightSpear* Projectile = GetProjectileFromPool();
			if (Projectile)
			{
				// 상태 리셋
				Projectile->CurrentTime = 0.0f;
				
				// 기존 타이머 클리어
				if (Projectile->LifeTimeTimerHandle.IsValid())
				{
					GetWorld()->GetTimerManager().ClearTimer(Projectile->LifeTimeTimerHandle);
				}
				
				// 위치와 회전 설정 후 활성화
				Projectile->SetActorLocation(SpawnLocation);
				Projectile->SetActorRotation(TargetRotation);
				Projectile->SetActorHiddenInGame(false);
				Projectile->SetActorEnableCollision(true);
				Projectile->SetActorTickEnabled(true);
				
				// Shape 콜리전 직접 활성화 (오브젝트 풀 재사용 시 필요)
				if (Projectile->Shape)
				{
					Projectile->Shape->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
				}
				
				// 오너 설정
				Projectile->SetOwner(GetOwner());
				
				// 발사 함수 호출
				Projectile->FireProjectile();
				
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("ShotProjectile - 투사체 풀에서 사용 가능한 투사체가 없음"));
			}
		}
	}
}

void UBossProjectileComponent::SpawnOrb()
{
	// 오브 1개 소환
	ACharacter* Boss = Cast<ACharacter>(GetOwner());
	
	// 오브 소켓 위치 사용
	FVector SocketLocation = Boss->GetMesh()->GetSocketLocation(FName("OrbSocket_1"));
	FRotator SocketRotation = Boss->GetMesh()->GetSocketRotation(FName("OrbSocket_1"));
	
	// 오브젝트 풀에서 오브 가져오기
	ABossProjectileOrb* Orb = GetOrbFromPool();
	if (Orb)
	{
		// 상태 리셋
		Orb->CurrentTime = 0.0f;
		
		// 위치와 회전 설정 후 활성화
		Orb->SetActorLocation(SocketLocation);
		Orb->SetActorRotation(SocketRotation);
		Orb->SetActorHiddenInGame(false);
		Orb->SetActorEnableCollision(true);
		Orb->SetActorTickEnabled(true);
		
		// 오브 활성화 시 소리 및 이펙트 재생
		Orb->ActivateOrb();
		
		// 오브 생성 성공 시에만 카운트와 상태 설정
		OrbSpawnCount = 1;
		ExitOrb = true;  // 오브가 존재하므로 true
		UE_LOG(LogTemp, Warning, TEXT("SpawnOrb - 오브 1개 소환 완료"));
	}
	else
	{
		// 오브 생성 실패 시 상태 유지
		OrbSpawnCount = 0;
		ExitOrb = false;  // 오브가 없으므로 false
		UE_LOG(LogTemp, Error, TEXT("SpawnOrb - 오브 풀에서 사용 가능한 오브가 없음"));
	}
}

void UBossProjectileComponent::SpawnSingleOrb()
{
	ACharacter* Boss = Cast<ACharacter>(GetOwner());
	
	// 보스 왼손 소켓 위치 사용
	FVector SocketLocation = Boss->GetMesh()->GetSocketLocation(FName("LeftHand"));
	
	// 오브젝트 풀에서 오브 가져오기
	ABossProjectileOrb* Orb = GetOrbFromPool();
	if (Orb)
	{
		// 상태 리셋
		Orb->CurrentTime = 0.0f;
		
		// 위치와 회전 설정 후 활성화
		Orb->SetActorLocation(SocketLocation);
		Orb->SetActorRotation(FRotator::ZeroRotator);
		Orb->SetActorHiddenInGame(false);
		Orb->SetActorEnableCollision(true);
		Orb->SetActorTickEnabled(true);
		
		// 오브 활성화 시 소리 및 이펙트 재생
		Orb->ActivateOrb();
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SpawnSingleOrb - 오브 풀에서 사용 가능한 오브가 없음"));
	}
}

void UBossProjectileComponent::SpawnOrbContinuously(float IntervalTime)
{
	// 이미 무한 반복 스폰 중이면 기존 타이머 정리
	if (bOrbContinuousSpawning)
	{
		CancelOrbContinuousSpawning();
	}
	
	// 무한 반복 스폰 시작
	bOrbContinuousSpawning = true;
	
	// 첫 번째 즉시 실행
	SpawnSingleOrb();
	
	// 지정된 간격으로 무한 반복
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(OrbContinuousTimerHandle, [this]()
		{
			if (bOrbContinuousSpawning)
			{
				SpawnSingleOrb();
			}
		}, IntervalTime, true);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("오브 무한 반복 스폰 시작 - 간격: %.2f초 (왼손 소켓)"), IntervalTime);
}

void UBossProjectileComponent::CancelOrbContinuousSpawning()
{
	if (bOrbContinuousSpawning)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().ClearTimer(OrbContinuousTimerHandle);
		}
		
		bOrbContinuousSpawning = false;
		
		UE_LOG(LogTemp, Warning, TEXT("오브 무한 반복 스폰 취소됨"));
	}
}

void UBossProjectileComponent::SpawnSingleProjectile()
{
	ACharacter* Boss = Cast<ACharacter>(GetOwner());
	
	// 보스 위치에서 투사체 스폰
	FVector SpawnLocation = Boss->GetMesh()->GetBoneLocation("head")+FVector(-100,0,100);
	FRotator SpawnRotation = Boss->GetActorRotation();
	
	// 투사체 스폰 시 소유자 설정
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = GetOwner();
	
	ABossProjectileActor* Projectile = GetWorld()->SpawnActor<ABossProjectileActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	if (Projectile)
	{
		UCBossTargetingComponent* TargetingComp=CHelpers::GetComponent<UCBossTargetingComponent>(GetOwner());
		// 투사체를 플레이어에게 발사
		Projectile->FireProjectile(TargetingComp->FindPlayer());
	}
}

void UBossProjectileComponent::SpawnProjectileContinuously(float IntervalTime)
{
	// 이미 무한 반복 스폰 중이면 기존 타이머 정리
	if (bProjectileContinuousSpawning)
	{
		CancelProjectileContinuousSpawning();
	}
	
	// 무한 반복 스폰 시작
	bProjectileContinuousSpawning = true;
	
	// 첫 번째 즉시 실행
	SpawnSingleProjectile();
	
	// 지정된 간격으로 무한 반복
	UWorld* World = GetWorld();
	if (World)
	{
		World->GetTimerManager().SetTimer(ProjectileContinuousTimerHandle, [this]()
		{
			if (bProjectileContinuousSpawning)
			{
				SpawnSingleProjectile();
			}
		}, IntervalTime, true);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("투사체 무한 반복 스폰 시작 - 간격: %.2f초"), IntervalTime);
}

void UBossProjectileComponent::CancelProjectileContinuousSpawning()
{
	if (bProjectileContinuousSpawning)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().ClearTimer(ProjectileContinuousTimerHandle);
		}
		
		bProjectileContinuousSpawning = false;
		
		UE_LOG(LogTemp, Warning, TEXT("투사체 무한 반복 스폰 취소됨"));
	}
}

// 새로운 기능: 타겟 액터를 받아서 발사 직전에 타겟의 위치로 이동
void UBossProjectileComponent::ShotProjectileToLocation(AActor* Target, float WaitTime)
{
	if (!Target)
	{
		UE_LOG(LogTemp, Error, TEXT("ShotProjectileToLocation - 타겟이 null입니다"));
		return;
	}
	ACharacter* Player = Cast<ACharacter>(Target);
	
	// 발사 직전에 타겟의 현재 위치 가져오기
	FVector TargetLocation = Player->GetMesh()->GetBoneLocation(FName("root"));
	
	// 투사체 스폰
	ABossProjectileActor* Projectile = GetWorld()->SpawnActor<ABossProjectileActor>(ProjectileClass, GetOwner()->GetActorLocation(), FRotator(0));
	if (Projectile)
	{
		UE_LOG(LogTemp, Warning, TEXT("ShotProjectileToLocation - 투사체 스폰 성공"));
		// 새로운 기능 호출 (타겟의 현재 위치로 이동)
		Projectile->FireProjectileToLocation(TargetLocation, WaitTime);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ShotProjectileToLocation - 투사체 스폰 실패"));
	}
}

void UBossProjectileComponent::DestroyOrb()
{
	OrbSpawnCount--;
	CLog::Log(FString::Printf(TEXT("DestroyOrb - Remaining Orbs: %d"), OrbSpawnCount));
	
	if (OrbSpawnCount > 0)
	{
		ExitOrb = true;   // 아직 오브가 남아있음
		CLog::Log("DestroyOrb - ExitOrb = true (Still has orbs)");
	}
	else if (OrbSpawnCount <= 0)
	{
		ExitOrb = false;  // 모든 오브가 파괴됨
		OrbSpawnCount = 0;  // 음수 방지
		CLog::Log("DestroyOrb - ExitOrb = false (All orbs destroyed)");
	}
}

// 내부적으로 사용하는 HolySwordMagic 스폰 함수
void UBossProjectileComponent::SpawnHolySwordMagicAtLocation(const FVector& TargetLocation)
{
	if (!HolySwordMagicClass)
	{
		HolySwordMagicClass = AHolySwordMagic::StaticClass();
	}
	
	// 오브젝트 풀에서 성검 마법 가져오기
	AHolySwordMagic* SpawnedHolySwordMagic = GetHolySwordMagicFromPool();
	if (SpawnedHolySwordMagic)
	{
		// 상태 리셋
		SpawnedHolySwordMagic->bFirstNiagaraPlaying = false;
		SpawnedHolySwordMagic->bSecondNiagaraPlaying = false;
		SpawnedHolySwordMagic->bCollisionEnabled = false;
		SpawnedHolySwordMagic->SecondNiagaraDelayTimer = 0.0f;
		SpawnedHolySwordMagic->CollisionDelayTimer = 0.0f;
		SpawnedHolySwordMagic->CollisionDisableTimer = 0.0f;
		
		// 위치와 회전 설정 후 활성화
		SpawnedHolySwordMagic->SetActorLocation(TargetLocation);
		SpawnedHolySwordMagic->SetActorRotation(FQuat::Identity);
		SpawnedHolySwordMagic->SetActorHiddenInGame(false);
		SpawnedHolySwordMagic->SetActorEnableCollision(true);
		SpawnedHolySwordMagic->SetActorTickEnabled(true);
		
		// BeginPlay 다시 호출하여 나이아가라 시작
		SpawnedHolySwordMagic->BeginPlay();
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("HolySwordMagic 풀에서 사용 가능한 성검 마법이 없음 at location: %s"), *TargetLocation.ToString());
	}
}

void UBossProjectileComponent::SpawnHolySwordMagicAtCurrentPlayerLocation()
{
	UCBossTargetingComponent* TargetComp = CHelpers::GetComponent<UCBossTargetingComponent>(GetOwner());
	CheckNull(TargetComp);
	ACharacter* Player = Cast<ACharacter>(TargetComp->FindPlayer());
	CheckNull(Player);
	FVector PlayerLocation = Player->GetMesh()->GetBoneLocation(FName("root"));
	SpawnHolySwordMagicAtLocation(PlayerLocation);
}

void UBossProjectileComponent::SpawnHolySwordMagicRepeatedly(float IntervalTime, int32 ExecuteCount)
{
	// 이미 스폰 중이면 기존 타이머 정리
	if (bHolySwordMagicSpawning)
	{
		CancelHolySwordMagicSpawning();
	}
	
	// 매개변수 저장
	HolySwordMagicExecuteCount = ExecuteCount;
	HolySwordMagicCurrentCount = 0;
	bHolySwordMagicSpawning = true;
	
	// 첫 번째 즉시 실행
	SpawnHolySwordMagicAtCurrentPlayerLocation();
	HolySwordMagicCurrentCount++;
	
	// 남은 횟수가 있으면 타이머 시작
	if (HolySwordMagicCurrentCount < HolySwordMagicExecuteCount)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().SetTimer(HolySwordMagicTimerHandle, [this]()
			{
				if (bHolySwordMagicSpawning && HolySwordMagicCurrentCount < HolySwordMagicExecuteCount)
				{
					SpawnHolySwordMagicAtCurrentPlayerLocation();
					HolySwordMagicCurrentCount++;
					
					// 모든 횟수 완료 시 타이머 정리
					if (HolySwordMagicCurrentCount >= HolySwordMagicExecuteCount)
					{
						bHolySwordMagicSpawning = false;
						UWorld* World = GetWorld();
						if (World)
						{
							World->GetTimerManager().ClearTimer(HolySwordMagicTimerHandle);
						}
					}
				}
			}, IntervalTime, true);
		}
	}
	else
	{
		// 한 번만 실행하는 경우 즉시 완료
		bHolySwordMagicSpawning = false;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("HolySwordMagic 반복 스폰 시작 - 간격: %.2f초, 횟수: %d"), IntervalTime, ExecuteCount);
}

void UBossProjectileComponent::CancelHolySwordMagicSpawning()
{
	if (bHolySwordMagicSpawning)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->GetTimerManager().ClearTimer(HolySwordMagicTimerHandle);
		}
		
		bHolySwordMagicSpawning = false;
		HolySwordMagicCurrentCount = 0;
		HolySwordMagicExecuteCount = 0;
		
		UE_LOG(LogTemp, Warning, TEXT("HolySwordMagic 반복 스폰 취소됨"));
	}
}

void UBossProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBossProjectileComponent::SetRectangleRange(bool bShow, float Width, float Height, FColor Color)
{
	bShowRectangleRange = bShow;
	RectangleWidth = Width;
	RectangleHeight = Height;
	RectangleColor = Color;
	
	UE_LOG(LogTemp, Warning, TEXT("RECTANGLE RANGE: Set - Show: %s, Size: %.1f x %.1f"), 
		bShow ? TEXT("ON") : TEXT("OFF"), Width, Height);
}

void UBossProjectileComponent::ToggleRectangleRange()
{
	bShowRectangleRange = !bShowRectangleRange;
	UE_LOG(LogTemp, Warning, TEXT("RECTANGLE RANGE: Toggle - Current: %s"), 
		bShowRectangleRange ? TEXT("ON") : TEXT("OFF"));
}

void UBossProjectileComponent::TestRectangleRange()
{
	if (GetOwner())
	{
		FVector OwnerLocation = GetOwner()->GetActorLocation();
	}
	
	// 강제로 사각형 그리기
	DrawRectangleRangeDebug();
	
}

void UBossProjectileComponent::DrawRectangleRangeDebug()
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	
	// 플레이어 방향 계산 (게이트 스폰과 동일하게)
	UCBossTargetingComponent* TargetingComp = CHelpers::GetComponent<UCBossTargetingComponent>(GetOwner());
	FVector ForwardDirection;
	if (TargetingComp && TargetingComp->FindPlayer())
	{
		FVector PlayerLocation = TargetingComp->FindPlayer()->GetActorLocation();
		ForwardDirection = (PlayerLocation - OwnerLocation).GetSafeNormal();
	}
	else
	{
		// 플레이어를 찾지 못했을 때는 기존 방식 사용
		FRotator OwnerRotation = GetOwner()->GetActorRotation();
		ForwardDirection = OwnerRotation.Vector();
	}
	
	// 플레이어 방향을 기준으로 오른쪽과 위쪽 방향 계산
	FVector RightDirection = FVector::CrossProduct(ForwardDirection, FVector::UpVector).GetSafeNormal();
	FVector UpDirection = FVector::UpVector;
	
	// 2차원 사각형 면 (보스 앞쪽에 위치)
	float HalfWidth = RectangleWidth * 0.5f;  // 좌우 폭
	float HalfHeight = RectangleHeight * 0.5f; // 위아래 높이
	
	// 보스 앞쪽에 2차원 사각형 면 그리기 (사각형 아래쪽이 보스 root 위치와 맞춤)
	FVector TopRight = OwnerLocation + ForwardDirection * 100.0f + RightDirection * HalfWidth + UpDirection * RectangleHeight;
	FVector TopLeft = OwnerLocation + ForwardDirection * 100.0f - RightDirection * HalfWidth + UpDirection * RectangleHeight;
	FVector BottomRight = OwnerLocation + ForwardDirection * 100.0f + RightDirection * HalfWidth; // 보스 root 위치
	FVector BottomLeft = OwnerLocation + ForwardDirection * 100.0f - RightDirection * HalfWidth; // 보스 root 위치
	
	// 2차원 사각형 그리기
	DrawDebugLine(World, TopRight, TopLeft, FColor::Red, false, 0.1f, 0, 5.0f);
	DrawDebugLine(World, TopLeft, BottomLeft, FColor::Red, false, 0.1f, 0, 5.0f);
	DrawDebugLine(World, BottomLeft, BottomRight, FColor::Red, false, 0.1f, 0, 5.0f);
	DrawDebugLine(World, BottomRight, TopRight, FColor::Red, false, 0.1f, 0, 5.0f);
	
	// 대각선 그리기 (시각적 효과)
	DrawDebugLine(World, TopRight, BottomLeft, FColor::Red, false, 0.1f, 0, 3.0f);
	DrawDebugLine(World, TopLeft, BottomRight, FColor::Red, false, 0.1f, 0, 3.0f);
	
	// 사각형 안에 50개의 원을 겹치지 않게 배치
	static bool bInitialized = false;
	static TArray<FVector> RandomPositions;
	
	if (!bInitialized)
	{
		// 40개의 겹치지 않는 위치 생성
		RandomPositions.Empty();
		int32 MaxAttempts = 1000; // 최대 시도 횟수
		int32 CircleRadius = 100;
		float MinDistance = CircleRadius * 2.1f; // 원들 사이 최소 거리 (겹치지 않게)
		
		for (int32 i = 0; i < 40; i++)
		{
			bool bValidPosition = false;
			int32 Attempts = 0;
			
			while (!bValidPosition && Attempts < MaxAttempts)
			{
				// 사각형 내부의 랜덤 위치 계산
				float RandomX = FMath::RandRange(-RectangleWidth * 0.5f + CircleRadius, RectangleWidth * 0.5f - CircleRadius);
				float RandomY = FMath::RandRange((float)CircleRadius, RectangleHeight - (float)CircleRadius);
				
				FVector RandomPos = OwnerLocation + ForwardDirection * 100.0f + RightDirection * RandomX + UpDirection * RandomY;
				
				// 기존 원들과의 거리 확인
				bValidPosition = true;
				for (const FVector& ExistingPos : RandomPositions)
				{
					float Distance = FVector::Dist(RandomPos, ExistingPos);
					if (Distance < MinDistance)
					{
						bValidPosition = false;
						break;
					}
				}
				
				if (bValidPosition)
				{
					RandomPositions.Add(RandomPos);
				}
				
				Attempts++;
			}
		}
		bInitialized = true;
	}
	
	// 겹치지 않는 위치에 원 그리기
	for (const FVector& RandomPos : RandomPositions)
	{
		DrawDebugCircle(World, RandomPos, 100.0f, 32, FColor::Blue, false, 0.1f, 0, 5.0f);
	}
	
	// 중심점
	DrawDebugSphere(World, OwnerLocation, 50.0f, 8, FColor::Red, false, 0.1f, 0, 5.0f);
}

void UBossProjectileComponent::SpawnMagicCirclesAtCirclePositions()
{
	if (!MagicCircle)
	{
		return;
	}
	
	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}
	
	// 랜덤 위치 배열 생성
	TArray<FVector> CirclePositions;
	
	FVector OwnerLocation = GetOwner()->GetActorLocation();
	
	// 플레이어 방향 계산
	UCBossTargetingComponent* TargetingComp = CHelpers::GetComponent<UCBossTargetingComponent>(GetOwner());
	FVector ForwardDirection;
	if (TargetingComp && TargetingComp->FindPlayer())
	{
		FVector PlayerLocation = TargetingComp->FindPlayer()->GetActorLocation();
		ForwardDirection = (PlayerLocation - OwnerLocation).GetSafeNormal();
	}
	else
	{
		// 플레이어를 찾지 못했을 때는 기존 방식 사용
		FRotator OwnerRotation = GetOwner()->GetActorRotation();
		ForwardDirection = OwnerRotation.Vector();
	}
	
	// 플레이어 방향을 기준으로 오른쪽과 위쪽 방향 계산
	FVector RightDirection = FVector::CrossProduct(ForwardDirection, FVector::UpVector).GetSafeNormal();
	FVector UpDirection = FVector::UpVector;
	
	int32 CircleRadius = 100;
	float MinDistance = CircleRadius * 2.1f;
	
	for (int32 i = 0; i < 40; i++)
	{
		bool bValidPosition = false;
		int32 MaxAttempts = 1000;
		int32 Attempts = 0;
		
		while (!bValidPosition && Attempts < MaxAttempts)
		{
			float RandomX = FMath::RandRange(-RectangleWidth * 0.5f + CircleRadius, RectangleWidth * 0.5f - CircleRadius);
			float RandomY = FMath::RandRange((float)CircleRadius, RectangleHeight - (float)CircleRadius);
			
			FVector RandomPos = OwnerLocation + ForwardDirection * 100.0f + RightDirection * RandomX + UpDirection * RandomY;
			
			bValidPosition = true;
			for (const FVector& ExistingPos : CirclePositions)
			{
				float Distance = FVector::Dist(RandomPos, ExistingPos);
				if (Distance < MinDistance)
				{
					bValidPosition = false;
					break;
				}
			}
			
			if (bValidPosition)
			{
				CirclePositions.Add(RandomPos);
			}
			
			Attempts++;
		}
	}
	
	// 오브젝트 풀에서 게이트들을 가져와서 활성화
	int32 CurrentIndex = 0;
	World->GetTimerManager().SetTimer(MagicCircleSpawnTimerHandle, [this, World, CirclePositions, CurrentIndex]() mutable
	{
		if (CurrentIndex < CirclePositions.Num())
		{
			// 오브젝트 풀에서 게이트 가져오기
			AGateOfBabylon* Gate = GetGateFromPool();
			if (Gate)
			{
				// 위치 설정
				Gate->SetActorLocation(CirclePositions[CurrentIndex]);
				
				// 플레이어 방향으로 회전 설정
				UCBossTargetingComponent* TargetingComp = CHelpers::GetComponent<UCBossTargetingComponent>(GetOwner());
				if (TargetingComp && TargetingComp->FindPlayer())
				{
					FVector GateLocation = CirclePositions[CurrentIndex];
					FVector PlayerLocation = TargetingComp->FindPlayer()->GetActorLocation();
					FVector Direction = (PlayerLocation - GateLocation).GetSafeNormal();
					FRotator TargetRotation = Direction.Rotation();
					
					// 아래를 내려다보는 각도로 설정 (공중 패턴)
					TargetRotation.Pitch = 90.0f;  // 30도 아래로 기울임
					
					Gate->SetActorRotation(TargetRotation);
				}
				else
				{
					// 플레이어를 찾지 못했을 때 기본 방향 (아래를 내려다보게)
					Gate->SetActorRotation(FRotator(90, 0, 0));
				}
				
				Gate->ActivateGate();
			}
			
			CurrentIndex++;
			
			// 40개 모두 스폰 완료 시 타이머 정리
			if (CurrentIndex >= CirclePositions.Num())
			{
				World->GetTimerManager().ClearTimer(MagicCircleSpawnTimerHandle);
			}
		}
	}, 0.05f, true, 0.0f);
}

void UBossProjectileComponent::InitializeObjectPool()
{
	if (bObjectPoolInitialized) return;
	
	UWorld* World = GetWorld();
	if (!World) return;
	
	// GateOfBabylon 40개 미리 생성
	for (int32 i = 0; i < 40; i++)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = GetOwner();
		
		AGateOfBabylon* Gate = World->SpawnActor<AGateOfBabylon>(
			MagicCircle,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);
		
		if (Gate)
		{
			Gate->bUseObjectPool = true;
			Gate->SetActorHiddenInGame(true);
			Gate->SetActorEnableCollision(false);
			Gate->SetActorTickEnabled(false);
			GatePool.Add(Gate);
		}
	}
	
	// 오브 5개 미리 생성
	for (int32 i = 0; i < 5; i++)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = GetOwner();
		
		ABossProjectileOrb* Orb = World->SpawnActor<ABossProjectileOrb>(
			ProjectileOrbClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);
		
		if (Orb)
		{
			Orb->bUseObjectPool = true;
			Orb->SetActorHiddenInGame(true);
			Orb->SetActorEnableCollision(false);
			Orb->SetActorTickEnabled(false);
			OrbPool.Add(Orb);
		}
	}
	
	// 창 투사체 3개 미리 생성
	for (int32 i = 0; i < 3; i++)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = GetOwner();
		
		AProjectile_LightSpear* Projectile = World->SpawnActor<AProjectile_LightSpear>(
			ProjectileSpearClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);
		
		if (Projectile)
		{
			Projectile->bUseObjectPool = true;
			Projectile->SetActorHiddenInGame(true);
			Projectile->SetActorEnableCollision(false);
			Projectile->SetActorTickEnabled(false);
			ProjectilePool.Add(Projectile);
		}
	}
	
	// 성검 마법 10개 미리 생성
	for (int32 i = 0; i < 10; i++)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = GetOwner();
		
		AHolySwordMagic* HolySwordMagic = World->SpawnActor<AHolySwordMagic>(
			HolySwordMagicClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);
		
		if (HolySwordMagic)
		{
			HolySwordMagic->bUseObjectPool = true;
			HolySwordMagic->SetActorHiddenInGame(true);
			HolySwordMagic->SetActorEnableCollision(false);
			HolySwordMagic->SetActorTickEnabled(false);
			HolySwordMagicPool.Add(HolySwordMagic);
		}
	}
	
	// BossProjectileActor 15개 미리 생성
	for (int32 i = 0; i < 15; i++)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = GetOwner();
		
		ABossProjectileActor* BossProjectile = World->SpawnActor<ABossProjectileActor>(
			ProjectileClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			SpawnParams
		);
		
		if (BossProjectile)
		{
			BossProjectile->bUseObjectPool = true;
			BossProjectile->SetActorHiddenInGame(true);
			BossProjectile->SetActorEnableCollision(false);
			BossProjectile->SetActorTickEnabled(false);
			BossProjectilePool.Add(BossProjectile);
		}
		else
		{
		}
	}
	
	bObjectPoolInitialized = true;
}

AGateOfBabylon* UBossProjectileComponent::GetGateFromPool()
{
	for (AGateOfBabylon* Gate : GatePool)
	{
		if (Gate && Gate->IsHidden())
		{
			return Gate;
		}
	}
	return nullptr;
}

ABossProjectileOrb* UBossProjectileComponent::GetOrbFromPool()
{
	for (ABossProjectileOrb* Orb : OrbPool)
	{
		if (Orb && Orb->IsHidden())
		{
			return Orb;
		}
	}
	return nullptr;
}

AProjectile_LightSpear* UBossProjectileComponent::GetProjectileFromPool()
{
	for (AProjectile_LightSpear* Projectile : ProjectilePool)
	{
		if (Projectile && Projectile->IsHidden())
		{
			return Projectile;
		}
	}
	return nullptr;
}

AHolySwordMagic* UBossProjectileComponent::GetHolySwordMagicFromPool()
{
	for (AHolySwordMagic* HolySwordMagic : HolySwordMagicPool)
	{
		if (HolySwordMagic && HolySwordMagic->IsHidden())
		{
			return HolySwordMagic;
		}
	}
	return nullptr;
}

// 게이트를 풀로 반환하는 함수
void UBossProjectileComponent::ReturnGateToPool(AGateOfBabylon* Gate)
{
	if (Gate)
	{
		Gate->SetActorHiddenInGame(true);
		Gate->SetActorEnableCollision(false);
		Gate->SetActorTickEnabled(false);
	}
}

void UBossProjectileComponent::ReturnOrbToPool(ABossProjectileOrb* Orb)
{
	if (Orb)
	{
		Orb->SetActorHiddenInGame(true);
		Orb->SetActorEnableCollision(false);
		Orb->SetActorTickEnabled(false);
	}
}

void UBossProjectileComponent::ReturnProjectileToPool(AProjectile_LightSpear* Projectile)
{
	if (Projectile)
	{
		// 타이머 클리어 (안전 장치)
		if (Projectile->LifeTimeTimerHandle.IsValid())
		{
			GetWorld()->GetTimerManager().ClearTimer(Projectile->LifeTimeTimerHandle);
			Projectile->LifeTimeTimerHandle.Invalidate();
		}
		
		Projectile->SetActorHiddenInGame(true);
		Projectile->SetActorEnableCollision(false);
		Projectile->SetActorTickEnabled(false);
		
	}
}

void UBossProjectileComponent::ReturnHolySwordMagicToPool(AHolySwordMagic* HolySwordMagic)
{
	if (HolySwordMagic)
	{
		// 나이아가라와 콜리전 정리
		HolySwordMagic->ResetForPool();
		
		HolySwordMagic->SetActorHiddenInGame(true);
		HolySwordMagic->SetActorEnableCollision(false);
		HolySwordMagic->SetActorTickEnabled(false);
	}
}

ABossProjectileActor* UBossProjectileComponent::GetBossProjectileFromPool()
{
	for (ABossProjectileActor* BossProjectile : BossProjectilePool)
	{
		if (BossProjectile && BossProjectile->IsHidden())
		{
			return BossProjectile;
		}
	}
	return nullptr;
}

void UBossProjectileComponent::ReturnBossProjectileToPool(ABossProjectileActor* BossProjectile)
{
	if (BossProjectile)
	{
		BossProjectile->SetActorHiddenInGame(true);
		BossProjectile->SetActorEnableCollision(false);
		BossProjectile->SetActorTickEnabled(false);
		
	}
}

/**
 * @brief 보스 투사체 시스템 완전 초기화 (매니저용)
 * 
 * 모든 활성화된 투사체들을 비활성화하고 타이머들을 정리합니다.
 * 보스 매니저에서 보스 리셋 시 사용됩니다.
 */
void UBossProjectileComponent::ResetProjectileSystem()
{
	UWorld* World = GetWorld();
	if (!World) return;

	// 1. 모든 타이머 정리
	World->GetTimerManager().ClearTimer(OrbTimerHandle);
	World->GetTimerManager().ClearTimer(OrbContinuousTimerHandle);
	World->GetTimerManager().ClearTimer(ProjectileContinuousTimerHandle);
	World->GetTimerManager().ClearTimer(HolySwordMagicTimerHandle);
	World->GetTimerManager().ClearTimer(MagicCircleSpawnTimerHandle);

	// 2. 연속 스폰 상태 초기화
	bOrbContinuousSpawning = false;
	bProjectileContinuousSpawning = false;
	bHolySwordMagicSpawning = false;

	// 3. 카운터 및 상태 변수 초기화
	OrbSpawnCount = 0;
	HolySwordMagicExecuteCount = 0;
	HolySwordMagicCurrentCount = 0;
	ExitOrb = false;

	// 4. 모든 투사체 풀의 객체들을 풀로 반환 (비활성화)
	for (AProjectile_LightSpear* Projectile : ProjectilePool)
	{
		if (Projectile)
		{
			// 타이머 클리어
			if (Projectile->LifeTimeTimerHandle.IsValid())
			{
				World->GetTimerManager().ClearTimer(Projectile->LifeTimeTimerHandle);
			}
			
			// 풀로 반환 (비활성화)
			ReturnProjectileToPool(Projectile);
		}
	}

	// 5. 모든 오브 풀의 객체들을 풀로 반환 (비활성화)
	for (ABossProjectileOrb* Orb : OrbPool)
	{
		if (Orb)
		{
			ReturnOrbToPool(Orb);
		}
	}

	// 6. 보스 투사체 풀의 객체들을 풀로 반환 (비활성화)
	for (ABossProjectileActor* BossProjectile : BossProjectilePool)
	{
		if (BossProjectile)
		{
			ReturnBossProjectileToPool(BossProjectile);
		}
	}

	// 7. HolySwordMagic 풀의 모든 객체들을 풀로 반환 (비활성화)
	for (AHolySwordMagic* HolySwordMagic : HolySwordMagicPool)
	{
		if (HolySwordMagic)
		{
			ReturnHolySwordMagicToPool(HolySwordMagic);
		}
	}

	// 8. GateOfBabylon 풀의 모든 객체들을 풀로 반환 (비활성화)
	for (AGateOfBabylon* Gate : GatePool)
	{
		if (Gate)
		{
			ReturnGateToPool(Gate);
		}
	}

}