#pragma once

#include "Enemy/AEnemy.h"
#include "AIController.h"
#include "Enemy/Component/CEnemyMovement.h"
#include "Global.h"

/**
 * @brief AAEnemy의 생성자입니다.
 * @details 캐릭터의 외형(메시), 애니메이션, AI 컨트롤러 등 필요한 핵심 에셋들을 로드하고 컴포넌트를 생성하여 부착합니다.
 */
AAEnemy::AAEnemy()
{
	// 스켈레탈 메시 에셋 로드 및 설정
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, AssetPaths::ENEMY_MESH);
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	
	// 애님 인스턴스 클래스 에셋 로드 및 설정
	TSubclassOf<UAnimInstance> Anim;
	CHelpers::GetClass<UAnimInstance>(&Anim, AssetPaths::ENEMY_ANIM);
	GetMesh()->SetAnimInstanceClass(Anim);

	// AI 컨트롤러 클래스 에셋 로드 및 설정
	TSubclassOf<AAIController> AIC;
	CHelpers::GetClass<AAIController>(&AIC, AssetPaths::ENEMY_AIC);
	AIControllerClass = AIC;

	// 이동 컴포넌트 생성
	CHelpers::CreateActorComponent<UCEnemyMovement>(this, &EnemyMovementComponent, "MovementComponent");
}

AAPatrolPath* AAEnemy::GetPatrolPath()
{
	return PatrolPath;
}

void AAEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AAEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}