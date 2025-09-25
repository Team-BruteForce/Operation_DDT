/**
 * @file CBoss.h
 * @brief 보스 캐릭터 메인 클래스 헤더 파일
 * 
 * @details
 * 게임의 보스 캐릭터를 정의하는 핵심 클래스입니다.
 * 보스의 기본 구조, 컴포넌트, 그리고 주요 기능들을 선언합니다.
 * 
 * @author [개발자명]
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "Actor/BossDissolve.h"
#include "GameFramework/Character.h"
#include "CBoss.generated.h"

/**
 * @brief 보스 캐릭터 클래스
 * 
 * 보스 캐릭터의 메인 클래스입니다.
 * 컴포넌트 기반 설계로 모듈화된 기능을 제공합니다.
 */
UCLASS()
class OPERRATION_DDT_API ACBoss : public ACharacter
{
	GENERATED_BODY()

public:
	
	/**
	 * @brief 보스 캐릭터 생성자
	 */
	ACBoss();

	virtual void Tick(float DeltaSeconds) override;
	/**
	 *@brief 데미지 함수
	 */
	UFUNCTION()
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
							 AController* EventInstigator, AActor* DamageCauser) override;
	/**
	 * @brief 보스 초기 위치 (BeginPlay에서 저장)
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss Location")
	FVector InitialLocation;

	/**
	 * @brief 보스 죽음 상태 플래그 (한 번만 죽음 처리)
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss State")
	bool bIsDead = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<ABossDissolve>BossDissolveClass;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class ABossDissolve* BossDissolve; 

	/**
	 * @brief 듀얼 레이어 디졸브 시스템
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve Materials")
	TArray<class UMaterialInterface*> MetalDissolveMaterials;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dissolve Materials") 
	TArray<class UMaterialInterface*> GoldenDissolveMaterials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dissolve")
	TArray<class UMaterialInstanceDynamic*> DynamicMaterials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dissolve")
	TArray<class UMaterialInterface*> OriginalMaterials;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dissolve")
	bool bIsTransforming = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Dissolve")
	float DissolveProgress = 0.0f;

	UPROPERTY()
	FTimerHandle DissolveTimerHandle;

	UPROPERTY()
	TArray<bool> bGoldenApplied = {false, false, false, false};

	/**
	 * @brief 보스 상태 컴포넌트
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	class UBossStateComponent* BossStateComponent;

	/**
	 * @brief 보스 이동 컴포넌트
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	class UCBossMovementComponent* BossMovementComponent;

	/**
	 * @brief 보스 무기 컴포넌트
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	class UCBossWeaponComponent* BossWeaponComponent;

	/**
	 * @brief 보스 스탯 컴포넌트
	 */
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "State")
	class UCBossStatusComponent* BossStatusComponent;
	
	/**
	 * @brief 보스 모션 워핑 컴포넌트
	 */
	UPROPERTY(EditAnywhere)
	class UMotionWarpingComponent* BossMotionWarping;

	/**
	 *@brief 보스 프로젝타일(투사체) 컴포넌트
	 */
	UPROPERTY(EditAnywhere)
	class UBossProjectileComponent* ProjectileComp;

	/**
 *@brief 보스 타겟 연산 관련 컴포넌트
 */
	UPROPERTY(EditAnywhere)
	class UCBossTargetingComponent* TargetingComp;

	/**
 *@brief 보스 디버그 컴포넌트
 */
	UPROPERTY(VisibleAnywhere)
	class UBossDebugComponent* DebugComp;

	/**
*@brief 보스 디버그 컴포넌트
*/
	UPROPERTY(EditAnywhere)
	class UBossEffectComponent* EffectComponent;

	/**
*@brief 보스 비행 컴포넌트
*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UFlyingComponent* FlyingComponent;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TSubclassOf<UUserWidget> BossWidgetClass;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UBossStatusWidget* BossWidget;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsPaseChange=false;

	UPROPERTY()
	bool flag=false;

	/**
	 * @brief 보스 배경음악
	 */
	UPROPERTY(EditAnywhere)
	class USoundBase* BossBGM;

	/**
	 * @brief 배경음악 재생용 사운드 컴포넌트
	 */
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	class UAudioComponent* BossBGMComponent;

	/**
	 * @brief 배경음악 재생 중인지 확인하는 플래그
	 */
	UPROPERTY()
	bool bIsBGMPlaying = false;

	/**
	 * @brief 볼륨 페이드아웃 타이머
	 */
	UPROPERTY()
	FTimerHandle VolumeFadeTimer;

	/**
	 * @brief 게임 시작 시 호출되는 함수
	 */
	virtual void BeginPlay() override;
	
public:
	UFUNCTION()
	void PlayHitMotion(FName BoneName);
	UFUNCTION()
	void ShowBossStatusWidget();

	UFUNCTION()
	void HPUpdate();

	UFUNCTION()
	void RestartUI();

	/**
	 * @brief 보스 배경음악 재생
	 */
	UFUNCTION()
	void PlayBossBGM();

	/**
	 * @brief 보스 배경음악 정지
	 */
	UFUNCTION()
	void StopBossBGM();

	/**
	 * @brief 보스 배경음악 볼륨 감소 (죽음 시)
	 */
	UFUNCTION()
	void LowerBossBGMVolume();
	
};
