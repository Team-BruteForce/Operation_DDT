// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/CBoss.h"
#include "Global.h"
#include "Enemy/CBossAIC.h"
/**
 * @brief 보스 캐릭터 생성자
 * 
 * 보스 캐릭터의 기본 설정을 초기화합니다.
 * - 스켈레탈 메시 에셋 로드 및 설정
 * - 애니메이션 인스턴스 클래스 설정
 * - AI 컨트롤러 클래스 설정
 */
ACBoss::ACBoss()
{
	// 스켈레탈 메시 에셋 로드 및 설정
	USkeletalMesh* mesh;
	CHelpers::GetAsset<USkeletalMesh>(&mesh, AssetPaths::Boss_MESH);
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	
	// 애님 인스턴스 클래스 에셋 로드 및 설정
	TSubclassOf<UAnimInstance> Anim;
	CHelpers::GetClass<UAnimInstance>(&Anim, AssetPaths::Boss_ANIM);
	GetMesh()->SetAnimInstanceClass(Anim);

	// AI 컨트롤러 클래스 에셋 로드 및 설정
	TSubclassOf<ACBossAIC> AIC;
	CHelpers::GetClass<ACBossAIC>(&AIC, AssetPaths::Boss_AIC);
	AIControllerClass = AIC;
}

/**
 * @brief 공격 애니메이션을 재생하는 테스트 함수
 * 
 * @param StateTag 재생할 공격 애니메이션을 식별하는 게임플레이 태그
 * 
 * 지정된 StateTag에 해당하는 공격 애니메이션 몽타주를 재생합니다.
 * 현재는 테스트 목적으로 사용되며, 향후 실제 공격 시스템으로 확장될 예정입니다.
 */
void ACBoss::AttackTest(FGameplayTag StateTag)
{
	// CLog::Log(StateTag.ToString());
	GetMesh()->GetAnimInstance()->Montage_Play(AttackOptions[StateTag]);
}

/**
 * @brief 게임 시작 또는 스폰 시 호출되는 함수
 * 
 * 캐릭터가 게임에 스폰될 때 초기화 작업을 수행합니다.
 */
void ACBoss::BeginPlay()
{
	Super::BeginPlay();
	AttackOptions.GetKeys(AttackTags);
}