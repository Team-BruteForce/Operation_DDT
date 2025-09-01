// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "CEnemyMeleeAttackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMeleeAttackHit, AActor*, HitActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class OPERRATION_DDT_API UCEnemyMeleeAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCEnemyMeleeAttackComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 근접 공격 콜리전
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* MeleeAttackCollision;

	// 공격 설정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	bool bIsAttackActive = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AttackCooldown = 1.0f;

	// 공격 활성화/비활성화
	UFUNCTION(BlueprintCallable, Category = "Attack")
	void ActivateMeleeAttack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	void DeactivateMeleeAttack();

	UFUNCTION(BlueprintCallable, Category = "Attack")
	bool IsAttackActive() const { return bIsAttackActive; }

	// 데미지 계산 (StatusComponent의 공격력 사용)
	UFUNCTION(BlueprintCallable, Category = "Attack")
	float GetMeleeDamage() const;

	// 이벤트
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnMeleeAttackHit OnMeleeAttackHit;

private:
	// 콜리전 오버랩 이벤트
	UFUNCTION()
	void OnMeleeAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// 공격 쿨다운 관리
	float LastAttackTime;
};
