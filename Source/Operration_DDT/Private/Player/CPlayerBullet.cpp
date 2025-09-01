// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/CPlayerBullet.h"
#include "Global.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Player/DDTPlayer.h"
#include "Kismet/GameplayStatics.h"

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
	SetLifeSpan (LifeTime);
	OwnerCharacter = Cast<ADDTPlayer>(GetOwner());
	
	// 충돌 이벤트 바인딩
	if (CapsuleComp)
	{
		CapsuleComp->OnComponentBeginOverlap.AddDynamic(this, &ACPlayerBullet::OnBulletOverlap);
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
}

void ACPlayerBullet::OnBulletOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 자기 자신과의 충돌 무시
	if (OtherActor == this || OtherActor == OwnerCharacter)
		return;
	
	CLog::Log("Overlap Occured");
	UGameplayStatics::ApplyDamage(OtherActor, Damage, OwnerCharacter->GetInstigatorController(), this, UDamageType::StaticClass());
	
	// 충돌 후 총알 제거
	Destroy();
}

