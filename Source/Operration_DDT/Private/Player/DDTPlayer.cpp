// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DDTPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Global.h"
#include "MaterialStatsCommon.h"
#include "Camera/CameraComponent.h"
#include "Player/Components/CMovementComponent.h"
#include "Player/Components/CStateComponent.h"
#include "Player/Components/CMontageComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"
#include "Components/StaticMeshComponent.h"
#include "DataWrappers/ChaosVDParticleDataWrapper.h"
#include "Player/Components/CCameraActionComponent.h"
#include "Player/Components/CWeaponComponent.h"
#include "Player/Components/CFireComponent.h"

// Sets default values
ADDTPlayer::ADDTPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, AssetPaths::PLAYER_MESH);
	GetMesh()->SetSkeletalMesh (mesh);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));

	TSubclassOf<UAnimInstance> AnimInstanceClass;
	CHelpers::GetClass<UAnimInstance>(&AnimInstanceClass, AssetPaths::PLAYER_ANIM);
	GetMesh()->SetAnimClass(AnimInstanceClass);

	GetCharacterMovement ()->RotationRate = FRotator(0.f, 720.f, 0.f);

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	// AddOn Components
	CHelpers::CreateActorComponent<UCMontageComponent>(this, &Montages, "Montage");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &WeaponComp, "WeaponComp");
	CHelpers::CreateActorComponent<UCCameraActionComponent>(this, &CameraActionComp, "CameraActionComp");
	CHelpers::CreateActorComponent<UCFireComponent>(this, &FireComp, "FireComp");

	SpringArm->SetRelativeLocation(FVector(-60.f, 0.f, 180.f));
	SpringArm->SetRelativeRotation(FRotator(0.f, 90.f, 0.f));
	SpringArm->TargetArmLength = 200;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;

	Camera->bUsePawnControlRotation = false;

	CHelpers::CreateComponent <UStaticMeshComponent>(this, &SwordHolster, "SwordHolster",GetMesh(), FName(TEXT("Holster_Sword")));
	UStaticMesh* holster;
	CHelpers::GetAsset (&holster, AssetPaths::SWORD_HOLSTER);
	SwordHolster->SetStaticMesh(holster);
	
}

// Called when the game starts or when spawned
void ADDTPlayer::BeginPlay()
{
	Super::BeginPlay();

	Movement->OnRun();
	Movement->EnableControlRotation ();

	State->OnStateTypeChanged.AddDynamic(this, &ADDTPlayer::OnStateTypeChanged);
	CameraActionComp->SetIdlePosition();

	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc)
	{
		auto* subsys = ULocalPlayer::GetSubsystem <UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsys)
		{
			subsys->AddMappingContext(IMC_Player, 1);
		}
	}
	
}

void ADDTPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//CLog::Print(*(State->GetTypeString()));
}


void ADDTPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (input)
	{
		Movement->SetupInputBinding (input);
		input->BindAction(IA_Sword, ETriggerEvent::Started, WeaponComp, &UCWeaponComponent::SetSwordMode);
		input->BindAction(IA_Rifle, ETriggerEvent::Started, WeaponComp, &UCWeaponComponent::SetRifleMode);
		input->BindAction(IA_Attack, ETriggerEvent::Started, WeaponComp, &UCWeaponComponent::DoAction);
		input->BindAction(IA_AimRifle, ETriggerEvent::Started,CameraActionComp, &UCCameraActionComponent::SetAimPosition );
		input->BindAction(IA_AimRifle, ETriggerEvent::Completed, CameraActionComp, &UCCameraActionComponent::SetIdlePosition );
	}

}

void ADDTPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	/*switch (InNewType)
	{
		break;
	}*/
}

