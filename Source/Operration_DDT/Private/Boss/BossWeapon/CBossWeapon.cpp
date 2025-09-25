/**
 * @file CBossWeapon.cpp
 * @brief 보스 웨폰 구현 파일
 * 
 * @details
 * 보스의 웨폰 시스템을 구현하는 파일입니다.
 * 웨폰의 콜리전과 공격 로직을 처리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/BossWeapon/CBossWeapon.h"
#include "Global.h"
#include "Components/ShapeComponent.h"
#include "GameFramework/Character.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Boss/Actor/BossCollision.h"
#include "Components/BoxComponent.h"

// Sets default values
ACBossWeapon::ACBossWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CHelpers::CreateComponent<USkeletalMeshComponent>(this, &SkeletalMesh, "SkeletalMesh");
	CHelpers::CreateComponent(this, &Root, "Root");
	SetRootComponent(Root);
	SkeletalMesh->SetupAttachment(Root);
    
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkeletalMesh->SetVisibility(false);


}

// Called when the game starts or when spawned
void ACBossWeapon::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	TArray<USceneComponent*> children;
	Root->GetChildrenComponents(true, children);
	for (USceneComponent* child : children)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(child);
		CLog::Log("ACBossWeapon"+child->GetName());
		if (!!shape)
		{
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACBossWeapon::OnBossComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACBossWeapon::OnBossComponentEndOverlap);

			Collisions.Add(shape);
		}
	}
	OffBossCollisions();

	Super::BeginPlay();
	if (BossSkillCollisionClass){
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner=this;
	BossSkillCollision=GetWorld()->SpawnActor<ABossCollision>(
	   BossSkillCollisionClass,
	   FVector(1500),
	   FRotator::ZeroRotator,
	   SpawnParams
   );
	}
}

void ACBossWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACBossWeapon::OnBossCollisions()
{
	if (OnBossAttachmentBeginCollision.IsBound())
		OnBossAttachmentBeginCollision.Broadcast();

	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACBossWeapon::OnSelectCollision(FName CollisionName)
{
	if (OnBossAttachmentBeginCollision.IsBound())
		OnBossAttachmentBeginCollision.Broadcast();

	for (UShapeComponent* shape : Collisions)
	{
		if (shape->GetName()==CollisionName)
		{
			CheckNull(shape);
			shape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
}

void ACBossWeapon::OffBossCollisions()
{
	if (OnBossAttachmentEndCollision.IsBound())
		OnBossAttachmentEndCollision.Broadcast();

	for (UShapeComponent* shape : Collisions)
		shape->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ACBossWeapon::OnBossComponentBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());
	CheckNull(Cast<ACharacter>(OtherActor));

	if (OnBossAttachmentBeginOverlap.IsBound())
		OnBossAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(OtherActor));
	OffBossCollisions();
}
void ACBossWeapon::OnBossComponentEndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	CheckTrue(OwnerCharacter == OtherActor);
	CheckTrue(OwnerCharacter->GetClass() == OtherActor->GetClass());
	CheckNull(Cast<ACharacter>(OtherActor));
	
	
	if (OnBossAttachmentEndOverlap.IsBound())
		OnBossAttachmentEndOverlap.Broadcast(OwnerCharacter, Cast<ACharacter>(OtherActor));
}

void ACBossWeapon::BossAttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);
}

void ACBossWeapon::BossAttachToCollision(FName InCollisionName, FName InSocketName)
{
	for (UShapeComponent* collision : Collisions)
	{
		if (collision->GetName() == InCollisionName.ToString())
		{
			collision->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), InSocketName);

			return;
		}
	}
}

void ACBossWeapon::StartCollisionAtSocket(FName InSocketName,bool IsMove)
{
	CheckNull(OwnerCharacter);
	CheckNull(BossSkillCollision);
	CheckNull(OwnerCharacter->GetMesh());

	FVector SocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(InSocketName);
	FRotator SocketRotation = OwnerCharacter->GetMesh()->GetSocketRotation(InSocketName);

	if (IsMove){
		BossSkillCollision->SetActorLocation(SocketLocation);
		BossSkillCollision->SetActorRotation(SocketRotation);
	}
	BossSkillCollision->BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ACBossWeapon::EndCollisionToOwner(bool IsEndMove)
{
	CheckNull(OwnerCharacter);
	CheckNull(OwnerCharacter->GetMesh());

	if (IsEndMove)
		BossSkillCollision->SetActorLocation(FVector(1500));
	BossSkillCollision->BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
}