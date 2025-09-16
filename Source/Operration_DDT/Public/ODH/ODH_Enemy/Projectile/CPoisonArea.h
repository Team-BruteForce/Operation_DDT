// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"
#include "Engine/Engine.h"
#include "CPoisonArea.generated.h"

UCLASS()
class OPERRATION_DDT_API ACPoisonArea : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPoisonArea();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 독성 영역 활성화 함수
	UFUNCTION(BlueprintCallable, Category = "Poison Area")
	void ActivatePoisonArea(float Duration = 10.0f);

	// 독성 영역 비활성화 함수
	UFUNCTION(BlueprintCallable, Category = "Poison Area")
	void DeactivatePoisonArea();

	// 풀 매니저로 반환하는 함수
	UFUNCTION(BlueprintCallable, Category = "Poison Area")
	void ReturnToPool();

	// 풀 매니저 설정 함수
	void SetPoolManager(class ACPoisonAreaPoolManager* Manager);

	// 독성 데미지 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Area|Damage")
	float PoisonDamage = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Area|Damage")
	float DamageInterval = 1.0f; // 데미지 간격 (초)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Area|Damage")
	float AreaRadius = 200.0f; // 독성 영역 반경

	// 독성 영역 지속 시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Area|Duration")
	float DefaultDuration = 10.0f; // 기본 지속 시간

	// 콜리전 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* CollisionComponent;

	// 나이아가라 이펙트 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* AreaEffect;

	// 나이아가라 이펙트 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Area|Effect")
	UNiagaraSystem* AreaEffectAsset;

	// 독성 영역 활성화 상태
	UPROPERTY(BlueprintReadOnly, Category = "Poison Area")
	bool bIsActive = false;

	// 디버그 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Poison Area|Debug")
	bool bShowDebugArea = false;

private:
	// 충돌 이벤트 핸들러
	UFUNCTION()
	void OnPoisonAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnPoisonAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 독성 데미지 적용
	void ApplyPoisonDamage();

	// 플레이어에게 독성 데미지 적용
	void ApplyDamageToPlayer(AActor* Player);

	// 독성 영역 관련 변수들
	float RemainingDuration = 0.0f;
	float DamageTimer = 0.0f;
	
	// 영역 내 플레이어 목록
	TArray<AActor*> PlayersInArea;

	// 독성 영역이 활성화되었는지 확인
	bool IsPoisonAreaActive() const { return bIsActive; }

	// 디버그 그리기 함수
	void DrawDebugArea();

	// 풀 매니저 참조
	class ACPoisonAreaPoolManager* PoolManager;
};
