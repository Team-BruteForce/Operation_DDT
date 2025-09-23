// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CNormalDamageUIActor.generated.h"

UCLASS()
class OPERRATION_DDT_API ACNormalDamageUIActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACNormalDamageUIActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 오브젝트 풀 관련 함수들
	UFUNCTION()
	void StartLifeTimer();
	
	UFUNCTION()
	void StopLifeTimer();
	
	UFUNCTION()
	void OnLifeTimeExpired();
	
	void ReturnToPool();
	
	void SetActive(bool bValue);
	bool IsActive() const;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "Default")
	float LifeTime = 1.f;
	
private:
	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* SceneComp;
	
	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* WidgetComp;

	UPROPERTY(EditDefaultsOnly, Category = "UIWidget")
	TSubclassOf<class UCDamageWidget_Normal> UIWidgetClass;

	class UCDamageWidget_Normal* DamageUI;
	
	UPROPERTY(EditDefaultsOnly)
	class ADDTPlayer* Player;

	class UCameraComponent* CameraComp;

private:
	bool bIsActive = false;
	bool bIsInUse = false;  // 사용 중 플래그 추가
	FTimerHandle LifeTimerHandle;

};
