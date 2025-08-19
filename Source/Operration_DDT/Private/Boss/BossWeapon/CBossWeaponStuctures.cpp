// Fill out your copyright notice in the Description page of Project Settings.


#include "Boss/BossWeapon/CBossWeaponStuctures.h"
#include "Global.h"
#include "Boss/Component/CBossMovementComponent.h"
#include "GameFramework/Character.h"

FString FBossDoActionData::DoAction ( class ACharacter* InOwner )
{
	UCBossMovementComponent* movement = CHelpers::GetComponent<UCBossMovementComponent> ( InOwner );

	if ( !!movement ) {
		// if ( bFixedCamera )
			// movement->EnableFixedCamera ( );

		// if ( bCanMove == false )
			// movement->Stop ( );
	}
	if ( !!Montage )
		InOwner->PlayAnimMontage ( Montage , PlayRate );
	return TEXT ( "Left" );
}
void FBossHitData::SendDamage ( class ACharacter* InAttacker , AActor* InAttackCauser , class ACharacter* InOther )
{
	FActionDamageEvent e;
	e.HitData = this;
	InOther->TakeDamage ( Power , e, InAttacker->GetController ( ) , InAttackCauser );
}

void FBossHitData::PlayMontage ( class ACharacter* InOwner )
{
	if ( !!Montage )
		InOwner->PlayAnimMontage ( Montage , PlayRate );
}

void FBossHitData::PlayHitStop ( UWorld* InWorld )
{
	CheckTrue ( FMath::IsNearlyZero ( StopTime ) );

	TArray<APawn*>pawns;
	for ( AActor* actor : InWorld->GetCurrentLevel()->Actors ) {
		APawn* pawn = Cast<ACharacter> ( actor );

		if ( !!pawn ) {
			pawn->CustomTimeDilation = 1e-3f;
			pawns.Add ( pawn );
		}
	}
	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda ( [=]( ) {
		for ( APawn* pawn : pawns )
			pawn->CustomTimeDilation = 1;
	} );
	FTimerHandle hanlde;
	InWorld->GetTimerManager ( ).SetTimer ( hanlde , timerDelegate , StopTime , false );
}
void FBossHitData::PlaySoundWave ( ACharacter* InOwner )
{
	CheckNull ( Sound );

	UWorld* world = InOwner->GetWorld ( );
	FVector location = InOwner->GetActorLocation ( );

	UGameplayStatics::SpawnSoundAtLocation ( world , Sound , location );
}

void FBossHitData::PlayEffect ( UWorld* InWorld , const FVector& InLocation )
{
	CheckNull ( Effect );

	FTransform transform;
	transform.SetLocation ( EffectLocation );
	transform.SetScale3D ( EffectScale );
	transform.AddToTranslation ( InLocation );

	CHelpers::PlayEffect ( InWorld , Effect , transform );
}

void FBossHitData::PlayEffect ( UWorld* InWorld , const FVector& InLocation , const FRotator& InRotation )
{
	CheckNull ( Effect );

	FTransform transform;
	transform.SetLocation ( InLocation + InRotation.RotateVector ( EffectLocation ) );
	transform.SetScale3D ( EffectScale );

	CHelpers::PlayEffect ( InWorld , Effect , transform );
}
