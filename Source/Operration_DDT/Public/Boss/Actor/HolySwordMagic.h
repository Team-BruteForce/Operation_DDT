// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "HolySwordMagic.generated.h"

UCLASS()
class OPERRATION_DDT_API AHolySwordMagic : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHolySwordMagic();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// 나이아가라 컴포넌트들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UNiagaraComponent* FirstNiagaraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UNiagaraComponent* SecondNiagaraComponent;
	
	// 콜리전 박스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* CollisionBox;
	
	// 나이아가라 시스템 에셋들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara Systems")
	class UNiagaraSystem* FirstNiagaraSystem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Niagara Systems")
	class UNiagaraSystem* SecondNiagaraSystem;
public:
	// 상태 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bFirstNiagaraPlaying = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bSecondNiagaraPlaying = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bCollisionEnabled = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bHasHitPlayer = false;
	
	// 타이머 변수들
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timers")
	float SecondNiagaraDelayTimer = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timers")
	float CollisionDelayTimer = 0.0f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Timers")
	float CollisionDisableTimer = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float SecondNiagaraDelayTime = 1.3f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float CollisionDelayTime = 0.1f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timers")
	float CollisionDisableTime = 0.5f;
	
	// 함수들
	void StartFirstNiagara();
	void StartSecondNiagara();
	void EnableCollision();
	void DisableCollision();
	void CheckNiagaraCompletion();
	void ResetForPool();
	
	// 오버랩 이벤트
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
public:
	// 오브젝트 풀 관련
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object Pool", meta = (AllowPrivateAccess = "true"))
	bool bUseObjectPool = true;

};
