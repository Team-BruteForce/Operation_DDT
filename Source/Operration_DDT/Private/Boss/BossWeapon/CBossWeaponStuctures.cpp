/**
 * @file CBossWeaponStuctures.cpp
 * @brief 보스 웨폰 구조체 구현 파일
 * 
 * @details
 * 보스의 웨폰 관련 구조체들을 구현하는 파일입니다.
 * 보스의 공격 패턴과 몽타주를 관리합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#include "Boss/BossWeapon/CBossWeaponStuctures.h"
#include "Global.h"
#include "Boss/Component/CBossMovementComponent.h"
#include "GameFramework/Character.h"

void FBossMeleeLightAttack::PlayPaseOneMontage ( class ACharacter* InOwner )
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
