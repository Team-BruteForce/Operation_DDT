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
#include "Boss/BossManager.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/Components/CCameraActionComponent.h"
#include "Player/Components/CWeaponComponent.h"
#include "Player/Components/CFireComponent.h"
#include "Player/Components/CStatusComponent.h"
#include "Player/Components/CRespawnComponent.h"
#include "Player/Components/CMagazineComponent.h"
#include "Player/Components/CStaminaComponent.h"
#include "Player/Components/CBulletObjectPoolComponent.h"
#include "Player/Components/CUIComponent.h"
#include "Player/Components/CDamageUIManageComponent.h"
#include "InputMappingContext.h"
#include "Player/Widget/CPauseWidget.h"
#include "Sound/SoundCue.h"

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
#pragma region Components
	CHelpers::CreateActorComponent<UCMontageComponent>(this, &Montages, "Montage");
	CHelpers::CreateActorComponent<UCMovementComponent>(this, &Movement, "Movement");
	CHelpers::CreateActorComponent<UCStateComponent>(this, &State, "State");
	CHelpers::CreateActorComponent<UCWeaponComponent>(this, &WeaponComp, "WeaponComp");
	CHelpers::CreateActorComponent<UCCameraActionComponent>(this, &CameraActionComp, "CameraActionComp");
	CHelpers::CreateActorComponent<UCFireComponent>(this, &FireComp, "FireComp");
	CHelpers::CreateActorComponent<UCStatusComponent>(this, &Status, "Status");
	CHelpers::CreateActorComponent<UCRespawnComponent>(this, &RespawnComp, "RespawnComp");
	CHelpers::CreateActorComponent<UCMagazineComponent>(this, &MagazineComp, "MagazineComp");
	CHelpers::CreateActorComponent<UCStaminaComponent>(this, &StaminaComp, "StaminaComp");
	CHelpers::CreateActorComponent<UCBulletObjectPoolComponent>(this, &BulletPool, "BulletPool");
	CHelpers::CreateActorComponent<UCUIComponent>(this, &UIComp, "UIComp");
	CHelpers::CreateActorComponent<UCDamageUIManageComponent>(this, &DamageUIManager, "DamageUIManager");
	
#pragma endregion
	
	
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

	// 이동 방향이 아닌 컨트롤러 방향을 따르도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// 컨트롤러의 Desired Rotation을 사용 (필요 시, 기본적으로 true일 수 있음)
	GetCharacterMovement()->bUseControllerDesiredRotation = true;

	CHelpers::CreateComponent <UStaticMeshComponent>(this, &SwordHolster, "SwordHolster",GetMesh(), FName(TEXT("Holster_Sword")));
	UStaticMesh* holster;
	CHelpers::GetAsset (&holster, AssetPaths::SWORD_HOLSTER);
	SwordHolster->SetStaticMesh(holster);
	
}

// Called when the game starts or when spawned
void ADDTPlayer::BeginPlay()
{
	Super::BeginPlay();

	if(Movement)
	{
		UE_LOG(LogTemp, Warning, TEXT("Movement is %s"), *Movement->GetName());
		Movement->Move();
		Movement->OnRun();
		Movement->EnableControlRotation ();

	}
	if (State)
	{
		UE_LOG(LogTemp, Warning, TEXT("State is %s"), *State->GetName());
		State->OnStateTypeChanged.AddDynamic(this, &ADDTPlayer::OnStateTypeChanged);
	}
	if (CameraActionComp && UIComp)
	{
		CameraActionComp->SetIdlePosition();

	}

	/*if (StaminaComp)
	{
		StaminaComp->OnStaminaChanged.AddDynamic(this, &ADDTPlayer::OnStaminaChanged);
		OnStaminaChanged(StaminaComp->GetNowStamina(), StaminaComp->GetMaxStamina());
			
	}*/
	
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ADDTPlayer::OnPlayerOverlap);
	
	APlayerController* pc = Cast<APlayerController>(GetController());
	if (pc)
	{
		//UE_LOG(LogTemp, Log, TEXT("ADDTPlayer) PC Loaded"));
		UEnhancedInputLocalPlayerSubsystem* subsys = ULocalPlayer::GetSubsystem <UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsys)
		{
			//subsys->AddMappingContext(IMC_Player, 0);
			//UE_LOG(LogTemp, Log, TEXT("ADDTPlayer) Mapping Context Loaded"));
			if (IMC_Player)
            {
                subsys->AddMappingContext(IMC_Player, 0);
                //UE_LOG(LogTemp, Log, TEXT("ADDTPlayer) Mapping Context Loaded: %s, Priority: %d"), *IMC_Player->GetName(), 0);
                for (const FEnhancedActionKeyMapping& Mapping : IMC_Player->GetMappings())
                {
                    //E_LOG(LogTemp, Log, TEXT("Mapped Action: %s, Key: %s"), *Mapping.GetDisplayName().ToString(), *Mapping.Key.ToString());
                }
            }
		}
		else
		{
			//UE_LOG(LogTemp, Error, TEXT("ADDTPlayer) Mapping Context Load Error"));
		}
	}
	else
	{
		//UE_LOG (LogTemp, Error, TEXT("ADDTPlayer) PC Load Error"));
	}
	
}

void ADDTPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//CLog::Print(TEXT("ADDTPlayer) Type : " + State->GetTypeString()));
}


void ADDTPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (input)
	{
		//UE_LOG(LogTemp, Log, TEXT("ADDTPlayer) Input Loaded"));
		//Movement->SetupInputBinding (input);
		input->BindAction(IA_Sword, ETriggerEvent::Started, WeaponComp, &UCWeaponComponent::SetSwordMode);
		input->BindAction(IA_Rifle, ETriggerEvent::Started, WeaponComp, &UCWeaponComponent::SetRifleMode);
		input->BindAction(IA_Attack, ETriggerEvent::Started, WeaponComp, &UCWeaponComponent::DoAction);
		input->BindAction(IA_AimRifle, ETriggerEvent::Started,CameraActionComp, &UCCameraActionComponent::SetAimPosition );
		input->BindAction(IA_AimRifle, ETriggerEvent::Completed, CameraActionComp, &UCCameraActionComponent::SetIdlePosition );
		input->BindAction(IA_Roll, ETriggerEvent::Started, this, &ADDTPlayer::OnAvoid);
		input->BindAction(IA_Heal, ETriggerEvent::Started, Status, &UCStatusComponent::CalculateHealing);
		//input->BindAction(IA_Reload, ETriggerEvent::Started, MagazineComp, &UCMagazineComponent::StartReloadSequence);
		input->BindAction(IA_Reload, ETriggerEvent::Started, State, &UCStateComponent::SetReloadMode);

		input->BindAction(IA_Move, ETriggerEvent::Triggered, Movement, &UCMovementComponent::OnMove);
		input->BindAction(IA_Move, ETriggerEvent::Completed, Movement, &UCMovementComponent::SetFalseOnMovePressing);
		input->BindAction(IA_TurnHor, ETriggerEvent::Triggered, Movement, &UCMovementComponent::OnHorizontalLook);
		input->BindAction(IA_TurnVer, ETriggerEvent::Triggered, Movement, &UCMovementComponent::OnVerticalLook);
		input->BindAction(IA_Sprint, ETriggerEvent::Started, Movement, &UCMovementComponent::SprintStart);
		input->BindAction(IA_Sprint, ETriggerEvent::Completed, Movement, &UCMovementComponent::SprintEnd);
		input->BindAction(IA_Pause, ETriggerEvent::Started, this, &ADDTPlayer::Pause);
	}
	else
	{
		//UE_LOG(LogTemp, Error, TEXT("ADDTPlayer) Input Loaded Failed"));
	}

}

void ADDTPlayer::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ABossManager>(OtherActor))
	{
		RespawnComp->SetRespawnLocation(RespawnComp->BossDoorLocation);
		//CLog::Log("ADDTPlayer) 리스폰 지역 변경");
	}
}


float ADDTPlayer::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                             class AController* EventInstigator, AActor* DamageCauser)
{
	if (State->IsCanDodge())
	{
		CLog::Print("Dodge!");
		return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	}
	Status->GetDamage(DamageAmount);
	if (Status->GetNowHp() <= 0)
	{
		State->SetDeadMode();
		UGameplayStatics::PlaySound2D(GetWorld(), DeathSound);
		//Dead();
	}
	else
	{
		State->SetHittedMode();
		//Hitted();
	}
	CLog::Log("Player Take Damage" + FString::SanitizeFloat(DamageAmount));
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void ADDTPlayer::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{
		case EStateType::Rolling:
		{
			// Roll() 호출 제거 - 이미 OnAvoid()에서 PlayRollingMode(InputDirection)로 처리됨
			// 방향별 구르기가 기본 구르기로 덮어써지는 것을 방지
			break;
		}
		case EStateType::Hitted:
		{
			Hitted();
			break;
		}
		case EStateType::Dead:
		{
			Dead();	
			break;
		}
		case EStateType::Healing:
		{
			Heal();
			break;
		}
		case EStateType::RifleReload:
		{
				Reload();
			break;
		}
		
	}
}

void ADDTPlayer::OnAvoid()
{
	/*CheckFalse(State->IsIdleMode());
	
	CheckTrue(State->IsDeadMode());
	CheckTrue(State->IsHittedMode());
	CheckTrue(State->IsEquipMode());*/

	if (!(State->GetType() == EStateType::Idle ||
		State->GetType() == EStateType::RifleReload ||
		State->GetType() == EStateType::Action
		))
	{
		CLog::Log("ADDTPlayer) Not Idle or Reload" );
		return;
	}
	
	//CheckTrue(State->IsRollingMode());
	//CheckTrue(State->IsReloadMode());
	CheckFalse(Movement->CanMove());

	if (StaminaComp->GetNowStamina() <= 0)
	{
		CLog::Log("Not Enough Stamina to roll");
		return;
	}
	State->SetRollingMode();
	
	// 현재 입력 방향 가져오기
	FVector InputDirection = GetCharacterMovement()->GetLastInputVector();
	InputDirection.Z = 0.f;
	//FVector InputDirection = Movement->GetCachedDirection();
	CLog::Log(InputDirection);
	
	// 입력 방향이 있으면 방향별 구르기, 없으면 기본 구르기
	if (!InputDirection.IsNearlyZero())
	{
		Montages->PlayRollingMode(InputDirection);
	}
	else
	{
		CLog::Log("Just Forward Roll");
		Montages->PlayRollingMode(); // 기존 방식
	}
	StaminaComp->ConsumeStamina(StaminaComp->RollingStamina);

}

void ADDTPlayer::Roll()
{
	Montages->PlayRollingMode();
}

void ADDTPlayer::Hitted()
{
	CLog::Log("ADDTPlayer Hitted");
	Montages->PlayHittedMode();
	CameraActionComp->ShakeCameraByHit();
}

void ADDTPlayer::Dead()
{
	Movement->ResetDirection();
	Montages->PlayDeadMode();
	Movement->DisableControlRotation();
}

void ADDTPlayer::Reload()
{
	//CheckTrue(State->IsRollingMode());
	if (State->IsRollingMode())
	{
		State->SetIdleMode();
		return;
	}
	MagazineComp->Reloading();
}


void ADDTPlayer::Heal()
{
	//CLog::Log("ADDTPlayer) " + State->GetTypeString() );
	
	if (Status->GetHealItemCount() > 0)
	{
		Montages->PlayHealingMode();
		//Status->GetHeal(70.f);
	}
}

void ADDTPlayer::End_Rolling()
{
	State->SetIdleMode();
}

void ADDTPlayer::End_Hitted()
{
	State->SetIdleMode();
}

void ADDTPlayer::End_Healing()
{
	AActor* Weapon = FireComp->GetActorAttachedToSocket(FName("Reload_Rifle"));
	CheckNull(Weapon);
	Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	Weapon->AttachToComponent (GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName("Hand_Rifle"));
	State->SetIdleMode();
}

void ADDTPlayer::End_Reload()
{
	/*AActor* Weapon = FireComp->GetActorAttachedToSocket(FName("Reload_Rifle"));
	CheckNull(Weapon);
	Weapon->DetachFromActor(FDetachmentTransformRules::KeepRelativeTransform);
	Weapon->AttachToComponent (GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), FName("Hand_Rifle"));*/
	State->SetIdleMode();
}

void ADDTPlayer::Pause()
{
	APlayerController* pc = Cast<APlayerController>(GetController());
	pc->SetPause(true);
	UIComp->pauseWidget->AddToViewport();
	
}


FVector ADDTPlayer::GetCurrentInputDirection()
{
	// Movement 컴포넌트에서 현재 입력 방향 가져오기
	return Movement->GetCurrentDirection();
}

