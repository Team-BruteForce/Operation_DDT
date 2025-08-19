// Fill out your copyright notice in the Description page of Project Settings.

/**
 * @file CBoss.cpp
 * @brief 보스 캐릭터의 핵심 로직을 구현하는 파일
 * 
 * @date 2024-12-19
 * @author [이효원]
 * @version 1.0
 * 
 * @section overview 개요
 * 이 파일은 보스 캐릭터의 생성, 초기화, 공격 애니메이션 재생 등의 기능을 담당합니다.
 * StateTree와 연동하여 보스의 AI 행동을 제어하는 기반이 되는 클래스입니다.
 * 
 * @section purpose 개발 목적
 * - 기획자가 데이터를 변경할 때 즉시 게임에 반영되도록 실시간 데이터 동기화 시스템 구축
 * - 보스 캐릭터의 AI 행동 패턴을 StateTree 기반으로 체계적으로 관리
 * - 애니메이션 시스템과 연동하여 자연스러운 보스 전투 구현
 * 
 * @section features 주요 기능
 * - 보스 캐릭터의 기본 설정 (메시, 애니메이션, AI 컨트롤러)
 * - 공격 애니메이션 몽타주 재생
 * - 게임 시작 시 초기화 작업
 * - StateTree 기반 AI 행동 제어 시스템
 * 
 * @section technical_details 기술적 세부사항
 * - UObject 기반 컴포넌트 시스템 활용
 * - 애니메이션 몽타주를 통한 공격 패턴 관리
 * - AI 컨트롤러와의 연동을 통한 자동화된 행동 제어
 * 
 * @section portfolio_highlight 포트폴리오 하이라이트
 * 이 시스템은 게임 개발에서 중요한 실시간 데이터 동기화와 AI 행동 제어를 
 * 체계적으로 구현한 사례로, 대규모 게임 프로젝트에서 요구되는 
 * 유지보수성과 확장성을 보여줍니다.
 */

#include "Boss/CBoss.h"
#include "Global.h"
#include "Boss/CBossAIC.h"
#include "Boss/Component/CBossWeaponComponent.h"
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

	CHelpers::CreateActorComponent<UBossStateComponent>(this,&BossStateComponent,"StateComp");
	CHelpers::CreateActorComponent<UCBossMovementComponent>(this,&BossMovementComponent,"MovementComp");
	CHelpers::CreateActorComponent<UCBossWeaponComponent>(this,&BossWeaponComponent,"WeaponComp");
	
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