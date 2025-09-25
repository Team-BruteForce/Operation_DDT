/**
 * @file BossProjectileOrb.h
 * @brief 보스 프로젝타일 오브 헤더 파일
 * 
 * @details
 * 보스의 오브 프로젝타일을 구현하는 액터입니다.
 * 타겟팅 컴포넌트와 연동하여 플레이어를 추적합니다.
 * 
 * @author 이효원
 * @date 2024-12-19
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossProjectileActor.h"
#include "BossProjectileOrb.generated.h"

/**
 * @brief 보스 프로젝타일 오브
 * 
 * 보스의 오브 프로젝타일을 구현하는 액터입니다.
 * 타겟팅 컴포넌트와 연동하여 플레이어를 추적합니다.
 */
UCLASS()
class OPERRATION_DDT_API ABossProjectileOrb : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossProjectileOrb();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	

public:
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile();
	
	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void DestroyOrb();

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void OnOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	// ===== 사운드 재생 함수들 =====
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlaySpawnSound();
	
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayReturnToPoolSound();
	
	UFUNCTION(BlueprintCallable, Category = "Audio")
	void PlayCollisionSound();
	
	// ===== 이펙트 재생 함수들 =====
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void PlaySpawnEffect();
	
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void PlayReturnToPoolEffect();
	
	UFUNCTION(BlueprintCallable, Category = "Effects")
	void PlayCollisionEffect();
	
	// ===== 딜레이 파괴 함수 =====
	UFUNCTION(BlueprintCallable, Category = "Timing")
	void DestroyOrbWithDelay();
	
	// 오브젝트 풀 활성화 함수
	UFUNCTION(BlueprintCallable, Category = "Object Pool")
	void ActivateOrb();

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class USphereComponent* SphereComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<ABossProjectileActor> ProjectileClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Time")
	float CurrentTime;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time")
	float MaxTime=5.0f;
	
	// 오브젝트 풀 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pool")
	bool bUseObjectPool;
	
	// 사운드/이펙트 재생 딜레이
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timing", meta=(ToolTip="사운드/이펙트 재생 후 파괴까지 대기 시간"))
	float DestroyDelay = 1.0f;
	
	// 타이머 핸들 (딜레이 파괴용)
	FTimerHandle DestroyTimerHandle;

public:
	// 사운드 시스템 (SoundBase 사용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta=(ToolTip="오브 소환 사운드"))
	class USoundBase* SpawnSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta=(ToolTip="오브 풀 반환 사운드"))
	class USoundBase* ReturnToPoolSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta=(ToolTip="오브 충돌 사운드"))
	class USoundBase* CollisionSound;
	
	// 사운드 어테뉴에이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio", meta=(ToolTip="사운드 거리 감쇠 설정"))
	class USoundAttenuation* SoundAttenuation;
	
	// 이펙트 시스템 (나이아가라)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta=(ToolTip="오브 소환 이펙트"))
	class UNiagaraSystem* SpawnEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta=(ToolTip="오브 풀 반환 이펙트"))
	class UNiagaraSystem* ReturnToPoolEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effects", meta=(ToolTip="오브 충돌 이펙트"))
	class UNiagaraSystem* CollisionEffect;
	
	// 오디오 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	class UAudioComponent* AudioComponent;

};
