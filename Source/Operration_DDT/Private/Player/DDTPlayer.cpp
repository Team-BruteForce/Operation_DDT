// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/DDTPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Global.h"
#include "Camera/CameraComponent.h"
#include "Player/Components/CMovementComponent.h"
#include "Player/Components/CStateComponent.h"
#include "Player/Components/CMontageComponent.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h"

// Sets default values
ADDTPlayer::ADDTPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, AssetPaths::PLAYER_MESH);
	GetMesh()->SetSkeletalMesh (mesh);
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -90.f), FRotator(0.f, -90.f, 0.f));

	GetCharacterMovement ()->RotationRate = FRotator(0.f, 720.f, 0.f);

	CHelpers::CreateComponent<USpringArmComponent>(this, &SpringArm, "SpringArm", GetMesh());
	CHelpers::CreateComponent<UCameraComponent>(this, &Camera, "Camera", SpringArm);

	CHelpers::CreateActorComponent<UCMontageComponent>(this, &Montages, "Montage");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");

	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->TargetArmLength = 200;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;

	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	bUseControllerRotationPitch = false;

	Camera->bUsePawnControlRotation = false;


}

// Called when the game starts or when spawned
void ADDTPlayer::BeginPlay()
{
	Super::BeginPlay();

	Movement->OnRun();					//Movement의 기본을 Run으로 설정
	Movement->DisableControlRotation();	//Movement의 기본을 DisableControlRotation으로 설정

	State->OnStateTypeChanged.AddDynamic(this, &ADDTPlayer::OnStateTypeChanged);

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


void ADDTPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (input)
	{
		Movement->SetupInputBinding (input);
	}

}

void ADDTPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{

}

