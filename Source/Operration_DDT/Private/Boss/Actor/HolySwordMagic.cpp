/**
 * @file HolySwordMagic.cpp
 * @brief 성검 마법 액터 구현 파일
 * 
 * @details
 * 보스의 성검 마법 스킬을 구현하는 액터입니다.
 * 박스 컴포넌트와 나이아가라 이펙트를 사용합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/Actor/HolySwordMagic.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "Global.h"
#include "Boss/Component/BossProjectileComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AHolySwordMagic::AHolySwordMagic()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 콜리전 박스 생성
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;
	CollisionBox->SetBoxExtent(FVector(100.0f, 100.0f, 200.0f));
	


	// 첫 번째 나이아가라 컴포넌트 생성
	FirstNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("FirstNiagaraComponent"));
	FirstNiagaraComponent->SetupAttachment(RootComponent);
	FirstNiagaraComponent->SetAutoActivate(false);

	// 두 번째 나이아가라 컴포넌트 생성
	SecondNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SecondNiagaraComponent"));
	SecondNiagaraComponent->SetupAttachment(RootComponent);
	SecondNiagaraComponent->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AHolySwordMagic::BeginPlay()
{
	Super::BeginPlay();
	// 오버랩 이벤트 바인딩 (중복 바인딩 방지)
	if (!CollisionBox->OnComponentBeginOverlap.IsBound())
	{
		CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AHolySwordMagic::OnOverlapBegin);
	}
	// 첫 번째 나이아가라 바로 시작
	StartFirstNiagara();
	
	// 두 번째 나이아가라 0.2초 후 시작 타이머 설정
	SecondNiagaraDelayTimer = SecondNiagaraDelayTime;
	
	// 5초 후 풀로 반환하는 타이머 시작 (오브젝트 풀 사용 시)
	if (bUseObjectPool)
	{
		FTimerHandle ReturnTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(ReturnTimerHandle, [this]()
		{
			// 오브젝트가 여전히 유효한지 확인
			if (!IsValid(this) || !IsValid(GetOwner()))
			{
				return;
			}
			
			class UBossProjectileComponent* ProjectileComp = CHelpers::GetComponent<class UBossProjectileComponent>(GetOwner());
			if (ProjectileComp)
			{
				ProjectileComp->ReturnHolySwordMagicToPool(this);
			}
		}, 5.0f, false);
	}
}

// Called every frame
void AHolySwordMagic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 두 번째 나이아가라 딜레이 타이머 처리
	if (SecondNiagaraDelayTimer > 0.0f)
	{
		SecondNiagaraDelayTimer -= DeltaTime;
		if (SecondNiagaraDelayTimer <= 0.0f)
		{
			StartSecondNiagara();
		}
	}
	
	// 콜리전 딜레이 타이머 처리 (활성화)
	if (CollisionDelayTimer > 0.0f)
	{
		CollisionDelayTimer -= DeltaTime;
		if (CollisionDelayTimer <= 0.0f)
		{
			EnableCollision();
		}
	}
	
	// 콜리전 비활성화 타이머 처리
	if (CollisionDisableTimer > 0.0f)
	{
		CollisionDisableTimer -= DeltaTime;
		if (CollisionDisableTimer <= 0.0f)
		{
			DisableCollision();
		}
	}
	
	// 나이아가라 완료 체크 (주석 처리 - 5초 고정 타이머 사용)
	// CheckNiagaraCompletion();
}

void AHolySwordMagic::StartFirstNiagara()
{
	if (FirstNiagaraSystem && FirstNiagaraComponent)
	{
		FirstNiagaraComponent->SetAsset(FirstNiagaraSystem);
		FirstNiagaraComponent->Activate();
		bFirstNiagaraPlaying = true;
		
		// 첫 번째 나이아가라 시작 시 사운드 재생
		if (FirstSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				FirstSound,
				GetActorLocation(),
				GetActorRotation(),
				1.0f,
				1.0f,
				0.0f,
				SoundAttenuation,
				nullptr
			);
		}
	}
}

void AHolySwordMagic::StartSecondNiagara()
{
	if (SecondNiagaraSystem && SecondNiagaraComponent)
	{
		SecondNiagaraComponent->SetAsset(SecondNiagaraSystem);
		SecondNiagaraComponent->Activate();
		bSecondNiagaraPlaying = true;
		
		// 두 번째 나이아가라 시작 후 0.1초 뒤 콜리전 활성화 타이머 시작
		CollisionDelayTimer = CollisionDelayTime;
		
		// 콜리전 활성화 후 0.5초 뒤 비활성화 (총 0.6초 후)
		CollisionDisableTimer = CollisionDelayTime + CollisionDisableTime;
	}
}

void AHolySwordMagic::EnableCollision()
{
	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		bCollisionEnabled = true;
		
		// 콜리전 활성화 시 사운드 재생
		if (SecondSound)
		{
			UGameplayStatics::PlaySoundAtLocation(
				GetWorld(),
				SecondSound,
				GetActorLocation(),
				GetActorRotation(),
				1.0f,
				1.0f,
				0.0f,
				SoundAttenuation,
				nullptr
			);
		}
	}
}

void AHolySwordMagic::DisableCollision()
{
	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		bCollisionEnabled = false;
	}
}

void AHolySwordMagic::CheckNiagaraCompletion()
{
	// 나이아가라 상태 체크만 수행 (반환은 5초 고정 타이머로 처리)
	
	// 첫 번째 나이아가라 완료 체크
	if (bFirstNiagaraPlaying && FirstNiagaraComponent)
	{
		if (!FirstNiagaraComponent->IsActive())
		{
			bFirstNiagaraPlaying = false;
		}
	}
	
	// 두 번째 나이아가라 완료 체크
	if (bSecondNiagaraPlaying && SecondNiagaraComponent)
	{
		if (!SecondNiagaraComponent->IsActive())
		{
			bSecondNiagaraPlaying = false;
		}
	}
}

void AHolySwordMagic::ResetForPool()
{
	// 나이아가라 컴포넌트들 비활성화
	if (FirstNiagaraComponent)
	{
		FirstNiagaraComponent->Deactivate();
	}
	
	if (SecondNiagaraComponent)
	{
		SecondNiagaraComponent->Deactivate();
	}
	
	// 콜리전 비활성화
	DisableCollision();
	
	// 상태 변수 초기화
	bFirstNiagaraPlaying = false;
	bSecondNiagaraPlaying = false;
	bCollisionEnabled = false;
	bHasHitPlayer = false;
	
	// 타이머 초기화
	SecondNiagaraDelayTimer = 0.0f;
	CollisionDelayTimer = 0.0f;
	CollisionDisableTimer = 0.0f;
	
	// 모든 타이머 클리어
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
}

void AHolySwordMagic::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 이미 플레이어를 맞춘 경우 무시
	if (bHasHitPlayer)
	{
		return;
	}
	
	// 플레이어와 충돌 시 콜리전만 비활성화 (다단히트 방지)
	ACharacter* Player = Cast<ACharacter>(OtherActor);
	if (Player && OtherActor->GetName() != GetOwner()->GetName())
	{
		UGameplayStatics::ApplyDamage(Player, 40, Player->GetController(), Player, nullptr);
		
		// 다단히트 방지 플래그 설정
		bHasHitPlayer = true;

		// 콜리전만 비활성화
		DisableCollision();
	}
}